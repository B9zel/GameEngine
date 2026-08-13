import configparser
import os
import pathlib as pl

from clang import cindex

from FieldTypes import ArrayType, ClassField, ETypePrimitive, MacrosData, VariableField
from GeneralFile import ParseArrayConfig, SerchAllFiles
from ParsClasses import ParseClassesOfFile


SCRIPT_DIRECTORY = pl.Path(__file__).parent
PROJECT_DIRECTORY = SCRIPT_DIRECTORY.parent

CONFIG = configparser.ConfigParser()
CONFIG.read(SCRIPT_DIRECTORY / "BuildGenConfig.ini")
SOURCE_PATHS = CONFIG["DEFAULT"]["Path"]
CLASS_MACRO = CONFIG["Macros"]["Class"]
PROPERTY_MACRO = CONFIG["Macros"]["Property"]
OUTPUT_FILES = CONFIG["DEFAULT"]["OutputGenFiles"]

# Keep parser flags in one place because they define how project headers are seen by libclang.
PARSER_ARGUMENTS = [
    "-x",
    "c++",
    "-std=c++17",
    "-nostdinc",
    "-I.",
    f"-I{PROJECT_DIRECTORY / 'CoreEngine' / 'Core'}",
    "-DRPROPERTY(x)=",
]


def GetNamespace(cursor):
    namespace_parts = []
    parent = cursor.semantic_parent
    while parent and parent.kind != cindex.CursorKind.TRANSLATION_UNIT:
        if parent.spelling and parent.kind == cindex.CursorKind.NAMESPACE:
            namespace_parts.append(parent.spelling)
        parent = parent.semantic_parent
    return "::".join(reversed(namespace_parts))


def FindedClassMacros(translation_unit: cindex.TranslationUnit):
    """Collect configured class macros and their source locations."""
    result = []
    for token in translation_unit.get_tokens(extent=translation_unit.cursor.extent):
        try:
            if token.spelling == CLASS_MACRO:
                macro = MacrosData()
                macro.Name = token.spelling
                macro.Location = token.location.line
                macro.Params = CollectPropertyConfig(
                    list(translation_unit.get_tokens(extent=token.cursor.extent)),
                    SearchClassCheck,
                )
                result.append(macro)
        except UnicodeDecodeError:
            continue
    return result


def CollectPropertyFields(translation_unit, cursor):
    """Match property macros to fields in a reflected class."""
    unmatched_property_macros = []
    for token in translation_unit.get_tokens(extent=cursor.extent):
        try:
            if token.spelling == PROPERTY_MACRO:
                macro = MacrosData()
                macro.Name = token.spelling
                macro.Location = token.location.line
                macro.Params = CollectPropertyConfig(
                    list(translation_unit.get_tokens(extent=token.cursor.extent)),
                    SearchVariableCheck,
                )
                unmatched_property_macros.append(macro)
        except UnicodeDecodeError:
            continue

    property_fields = []
    for node in cursor.get_children():
        if not unmatched_property_macros:
            break
        if node.kind != cindex.CursorKind.FIELD_DECL:
            continue

        matching_macros = [
            macro for macro in unmatched_property_macros if macro.Location <= node.location.line
        ]
        if not matching_macros:
            continue

        type_name, primitive_type, inner_type = CollectFullTypeName(translation_unit, node)
        if primitive_type == ETypePrimitive.ARRAY:
            property_field = ArrayType()
            property_field.InnerType = inner_type
            property_field.IsPointer = "*" in inner_type
        else:
            property_field = VariableField()
            property_field.IsPointer = (
                False
                if primitive_type == ETypePrimitive.CUSTOM_PRIMITIVE
                else node.type.kind == cindex.TypeKind.POINTER
            )

        matched_macro = matching_macros[0]
        property_field.NameVar = node.spelling
        property_field.Type = type_name
        property_field.TypePrimitive = primitive_type
        property_field.Params = matched_macro.Params
        property_fields.append(property_field)
        unmatched_property_macros.remove(matched_macro)

    return property_fields


def GetDeclarationFromType(clang_type):
    if clang_type.kind in (
        cindex.TypeKind.POINTER,
        cindex.TypeKind.LVALUEREFERENCE,
        cindex.TypeKind.RVALUEREFERENCE,
    ):
        clang_type = clang_type.get_pointee()

    declaration = clang_type.get_declaration()
    if declaration and declaration.kind != cindex.CursorKind.NO_DECL_FOUND:
        return declaration

    canonical_type = clang_type.get_canonical()
    if canonical_type and canonical_type != clang_type:
        declaration = canonical_type.get_declaration()
        if declaration and declaration.kind != cindex.CursorKind.NO_DECL_FOUND:
            return declaration
    return None


def SearchVariableCheck(indexed_token):
    return indexed_token[1].spelling == "RPROPERTY"


def SearchClassCheck(indexed_token):
    return indexed_token[1].spelling == "RCLASS"


def CollectPropertyConfig(tokens: list, search_predicate):
    """Return macro arguments, excluding punctuation."""
    if not tokens:
        return []

    matching_positions = list(filter(search_predicate, enumerate(tokens)))
    if not matching_positions:
        return []

    result = []
    for index in range(matching_positions[0][0] + 1, len(tokens)):
        spelling = tokens[index].spelling
        if spelling == ")":
            break
        if spelling not in ("(", ",", ";"):
            result.append(spelling)
    return result


def CollectNamespaceOfProperty(_cursor):
    """Keep property namespaces empty to preserve the current generated output."""
    return ""


def CollectFullTypeName(translation_unit, cursor) -> tuple[str, ETypePrimitive, str]:
    tokens = list(translation_unit.get_tokens(extent=cursor.extent))
    if not tokens:
        # Fall back to nearby tokens when libclang gives the field an empty extent.
        tokens = [
            token
            for token in translation_unit.get_tokens(extent=translation_unit.cursor.extent)
            if token.location.line
            in (cursor.location.line - 1, cursor.location.line, cursor.location.line + 1)
            and token.location.file
            and cursor.location.file
            and token.location.file.name == cursor.location.file.name
        ]
    if not tokens:
        return cursor.type.spelling, ETypePrimitive.PRIMITIVE, ""

    template_type = ExtractTemplateInnder("".join(token.spelling for token in tokens), "DArray")
    if template_type[0]:
        namespace = GetNamespace(cursor)
        qualified_inner_type = f"{namespace}::{template_type[1]}"
        return (
            f"DArray<{qualified_inner_type}>",
            ETypePrimitive.ARRAY,
            qualified_inner_type,
        )

    custom_primitives = {
        "FVector",
        "FTransform",
        "String",
        "int8",
        "int16",
        "int32",
        "int64",
        "uint8",
        "uint16",
        "uint32",
        "uint64",
        "LinearColor",
    }
    if tokens[0].spelling in custom_primitives:
        return tokens[0].spelling, ETypePrimitive.CUSTOM_PRIMITIVE, ""

    declaration = GetDeclarationFromType(cursor.type)
    if declaration:
        namespace = CollectNamespaceOfProperty(declaration)
        return f"{namespace}::{declaration.type.spelling}", ETypePrimitive.PRIMITIVE, ""

    spelling = "".join(
        token.spelling for token in translation_unit.get_tokens(extent=cursor.extent)
    )
    if "*" in spelling:
        return spelling[:spelling.find("*")], ETypePrimitive.PRIMITIVE, ""

    spelling = cursor.type.spelling
    if "::" in spelling:
        return spelling[:spelling.rfind("::")], ETypePrimitive.PRIMITIVE, ""
    return spelling, ETypePrimitive.PRIMITIVE, ""


def ExtractTemplateInnder(type_name: str, search_template: str) -> tuple[bool, str]:
    """Extract the inner type from a possibly nested template expression."""
    template_start = type_name.find(search_template)
    if template_start < 0:
        return False, ""

    inner_type_start = template_start + len(search_template)
    opening_bracket = type_name.find("<", inner_type_start)
    if opening_bracket < 0:
        return False, ""

    depth = 0
    closing_bracket = -1
    for index in range(opening_bracket, len(type_name)):
        character = type_name[index]
        if character == "<":
            depth += 1
        elif character == ">":
            depth -= 1
            if depth == 0:
                closing_bracket = index
    if closing_bracket < 0:
        return False, ""
    return True, type_name[opening_bracket + 1:closing_bracket]


def CollectGeneratedBody(translation_unit, cursor):
    for token in translation_unit.get_tokens(extent=cursor.extent):
        if token.spelling == "GENERATED_BODY":
            generated_body = MacrosData()
            generated_body.Name = "GENERATED_BODY"
            generated_body.Location = token.location.line
            return generated_body


def GetParent(translation_unit, cursor):
    collect_parent = False
    parent_name = ""
    for token in translation_unit.get_tokens(extent=cursor.extent):
        if token.spelling == "{":
            break
        if token.spelling == ":":
            collect_parent = True
            continue
        if collect_parent and token.spelling not in ("public", "protected", "private"):
            parent_name += token.spelling
            break
    return parent_name


def GetParentWithNamepsace(namespace_above: str, parent_full_name: str):
    if not parent_full_name:
        return ""
    if "::" in parent_full_name and namespace_above in parent_full_name:
        return parent_full_name
    return namespace_above + "::" + parent_full_name


def ParseFile(translation_unit, class_macros: list) -> list:
    if not class_macros:
        return []

    classes = []
    for node in walk(translation_unit.cursor):
        if node.kind != cindex.CursorKind.CLASS_DECL:
            continue

        macro_line = node.location.line - 1
        matching_macros = [macro for macro in class_macros if macro.Location == macro_line]
        if not matching_macros:
            continue

        generated_body = CollectGeneratedBody(translation_unit, node)
        if generated_body is None:
            continue

        class_field = ClassField()
        class_field.Name = node.spelling
        class_field.LineGenBody = generated_body
        class_field.ParamsClass = matching_macros[0]
        class_field.Parent = GetParentWithNamepsace(
            class_field.Namespace,
            GetParent(translation_unit, node),
        )
        if not class_field.IsValidGeneretedBody():
            continue
        class_field.Variable = CollectPropertyFields(translation_unit, node)
        classes.append(class_field)
    return classes


def walk(cursor):
    for child in cursor.get_children():
        yield child
        yield from walk(child)


def main():
    """Parse reflected headers and populate the prepared generated files."""
    source_paths, modules = ParseArrayConfig(SOURCE_PATHS)
    files_by_module = SerchAllFiles(source_paths, modules, ".h")
    index = cindex.Index.create()

    for module, files in files_by_module.items():
        for file in files:
            translation_unit = index.parse(
                os.path.abspath(file),
                args=PARSER_ARGUMENTS,
                options=cindex.TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD,
            )
            classes = ParseFile(
                translation_unit,
                FindedClassMacros(translation_unit),
            )
            ParseClassesOfFile(classes, file, f"{OUTPUT_FILES}/{module}")


if __name__ == "__main__":
    main()
