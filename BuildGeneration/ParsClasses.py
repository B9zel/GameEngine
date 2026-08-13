import pathlib as pl

from FieldTypes import ETypePrimitive
from GeneralFile import GetNameFilesWithoutExtenshion


SCRIPT_DIRECTORY = pl.Path(__file__).parent


def _generated_file_path(output_directory, source_name, suffix):
    file_stem = GetNameFilesWithoutExtenshion(source_name)
    return (SCRIPT_DIRECTORY / output_directory).absolute() / f"{file_stem}{suffix}"


def _append_if_missing(path, content_parts):
    """Append only parts that are not already present, preserving legacy behavior."""
    with open(path, "r+") as output_file:
        current_content = output_file.read()
        new_content = "".join(
            part for part in content_parts if part not in current_content
        )
        if new_content and new_content not in current_content:
            output_file.write(new_content)


def _join_unique(parts):
    result = ""
    for part in parts:
        if part not in result:
            result += part
    return result


def _replace_if_changed(path, content):
    with open(path, "r+") as output_file:
        current_content = output_file.read()
        if content == current_content:
            return
        output_file.seek(0)
        output_file.truncate(0)
        output_file.write(content)


def GenerateHeader(
    ClassNameLine,
    NameOpenFile,
    PathToOpenedFile,
    DirectoryOuputFile,
    FieldClass: list,
):
    """Build the generated header text for all reflected classes in one file."""
    del ClassNameLine, PathToOpenedFile  # Required by the shared generation interface.

    output_path = _generated_file_path(
        DirectoryOuputFile,
        NameOpenFile,
        ".generated.h",
    )
    if not output_path.exists():
        return False, ""

    pre_generated_classes = (
        f"#ifdef File{NameOpenFile} \n"
        f"#error \"{output_path.name} already included, missing '#pragma once'\" \n"
        f"#endif \n"
        f"#define File{NameOpenFile} \n"
        f"#include <ReflectionSystem/Include/ReflectionMacros.h> \n"
    )
    generated_text = pre_generated_classes
    current_file_id = f"{NameOpenFile}_{FieldClass[0].Name}"

    for field in FieldClass:
        if field.Namespace:
            generated_text += (
                f"namespace {field.Namespace} "
                "{\n"
                f"class {field.Name}; \n"
                "}\n"
            )
        else:
            generated_text += f"class {field.Name}; \n"

        generated_text += f"struct Construct_{field.Name}_Statics; \n"
        generated_text += (
            f"DeclareNewClass({field.Name}Generated) \n"
            f"#define {current_file_id}_{field.LineGenBody.Location}_GENERATED_BODY \\\n"
            "public: \\\n"
            "          static CoreEngine::Reflection::ClassField* GetStaticClass(); \\\n"
            f"          friend struct Construct_{field.Name}_Statics; \\\n"
            "private:\n"
        )

    generated_text += f"GenetateHeaderRegistryClass({field.Name}, {field.Namespace})\n"
    generated_text += (
        "#undef CURRENT_FILE_ID \n"
        f"#define CURRENT_FILE_ID {current_file_id}"
    )

    _append_if_missing(output_path, [generated_text])
    return generated_text, pre_generated_classes


def _format_property_params(variable):
    if not variable.Params:
        return "CoreEngine::Reflection::EPropertyFieldParams()"
    return "|".join(
        f"CoreEngine::Reflection::EPropertyFieldParams::{param}"
        for param in variable.Params
    )


def _generate_property_line(field, variable):
    params = _format_property_params(variable)
    owner = f"{field.Namespace}::{field.Name}"
    offset = f"offsetof({owner}, {variable.NameVar})"
    is_pointer = "true" if variable.IsPointer else "false"

    if variable.TypePrimitive in (
        ETypePrimitive.PRIMITIVE,
        ETypePrimitive.CUSTOM_PRIMITIVE,
    ):
        if variable.IsPointer:
            return (
                f"\tGenerateClassPropertyFiled({variable.NameVar}, {variable.Type}, "
                f"{offset}, {params})\n"
            )
        return (
            f"\tGeneratePropertyFiled({variable.NameVar}, {variable.Type}, "
            f"{offset}, {is_pointer}, {params})\n"
        )

    if variable.TypePrimitive == ETypePrimitive.ARRAY:
        default_params = "CoreEngine::Reflection::EPropertyFieldParams()"
        if variable.IsPointer:
            pointer_position = variable.InnerType.find("*")
            inner_type = variable.InnerType[:pointer_position]
            return (
                f"\tGenerateClassArrayPropertyFiled({variable.NameVar}, {variable.Type}, "
                f"{inner_type}, {offset}, {is_pointer}, {default_params})\n"
            )
        return (
            f"\tGenerateArrayPropertyFiled({variable.NameVar}, {variable.Type}, "
            f"{offset}, {is_pointer}, {default_params})\n"
        )
    return ""


def _generate_property_array(field, property_names):
    declaration = (
        "\tstatic DArray<UniquePtr<CoreEngine::Reflection::PropertyField>>& "
        "GetPropertyFieldArray() { \n"
        "\t\tstatic bool HasInit = false;\n"
        f"\t\tstatic DArray<UniquePtr<CoreEngine::Reflection::PropertyField>> "
        f"{field.Name}Generated_Fields; \n"
        "\t\tif (!HasInit) {\n"
    )
    for property_name in property_names:
        declaration += (
            f"\t\t{field.Name}Generated_Fields.emplace_back("
            f"MakeUniquePtr<Construct_{field.Name}_Statics::{property_name}>());\n"
        )
    declaration += "\t\t\tHasInit = true;"
    declaration += "\n\t\t}\n"
    declaration += f"\t\treturn {field.Name}Generated_Fields;\n"
    declaration += "\t} \n"
    return declaration


def GenerateSource(
    ClassNameLine,
    NameOpenFile,
    PathToOpenedFile,
    DirectoryOuputFile,
    FieldClass,
):
    """Build the generated C++ implementation for all reflected classes."""
    output_path = _generated_file_path(
        DirectoryOuputFile,
        NameOpenFile,
        ".gen.cpp",
    )
    if not output_path.exists():
        return False, ""

    pre_generated_implementation = f"#include <{PathToOpenedFile}> \n\n"
    implementation = ""

    for field in FieldClass:
        # Property descriptors belong to the current class, not the whole header.
        generated_property_names = []
        implementation += (
            f"struct Construct_{field.Name}_Statics \n"
            "{\n"
            f"Construct_{field.Name}_Statics() {{}}\n"
        )

        for variable in field.Variable:
            implementation += _generate_property_line(field, variable)
            generated_property_names.append(f"Field_{variable.NameVar}")

        implementation += _generate_property_array(field, generated_property_names)
        implementation += "\n};\n\n"

        parent = f"{field.Parent}::GetStaticClass()" if field.Parent else "nullptr"
        class_params = "|".join(field.ParamsClass.Params) or "EClassFieldParams::NONE"
        implementation += (
            f"ImplementNewClass({field.Name}Generated, {field.Name},{field.Namespace}, "
            f"{class_params},sizeof({field.Namespace}::{field.Name}), "
            f"Construct_{field.Name}_Statics::GetPropertyFieldArray(), {parent})\n"
            f"ImplementStaticClass({field.Namespace}::{field.Name}, "
            f"{ClassNameLine}Generated,\"{field.Name}\")\n"
        )
        implementation += (
            f"GenetateSourceRegistryClass({field.Name}, {field.Namespace})"
        )

    _append_if_missing(
        output_path,
        [pre_generated_implementation, implementation],
    )
    return pre_generated_implementation, implementation


def GenerateCodeClass(
    ClassNameLine,
    NameOpenFile,
    PathToOpenedFile,
    DirectoryOuputFile,
    FieldClass,
):
    if not ClassNameLine:
        return False

    header = GenerateHeader(
        ClassNameLine,
        NameOpenFile,
        PathToOpenedFile,
        DirectoryOuputFile,
        FieldClass,
    )
    source = GenerateSource(
        ClassNameLine,
        NameOpenFile,
        PathToOpenedFile,
        DirectoryOuputFile,
        FieldClass,
    )
    if not header[0] or not source[0]:
        return False, header, source
    return True, header, source


def ParseClassesOfFile(Classes, file, OutputFiles):
    """Generate and replace the output content for one reflected header."""
    if not Classes:
        return

    source_name = GetNameFilesWithoutExtenshion(file.name)
    generation_result = GenerateCodeClass(
        Classes[0].Name,
        source_name,
        file,
        OutputFiles,
        Classes,
    )
    header_content = _join_unique(generation_result[1])
    source_content = _join_unique(generation_result[2])

    header_path = _generated_file_path(OutputFiles, source_name, ".generated.h")
    source_path = _generated_file_path(OutputFiles, source_name, ".gen.cpp")
    _replace_if_changed(header_path, header_content)
    _replace_if_changed(source_path, source_content)
