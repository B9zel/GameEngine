import configparser
import pathlib as pl

from argparse import Namespace
from pkgutil import extend_path

from clang import cindex
from ParsClasses import ParseClassesOfFile
from FieldTypes import *
from GeneralFile import SerchAllFiles, ParseArrayConfig,GetNameFilesWithoutExtenshion
import os


OpenConfig = configparser.ConfigParser()
OpenConfig.read(f"{pl.Path(__file__).parent}/BuildGenConfig.ini")
Path = OpenConfig["DEFAULT"]["Path"]
ClassKeyWord = OpenConfig["Macros"]["Class"]
PropertyKeyWord = OpenConfig["Macros"]["Property"]
OutputFiles = OpenConfig["DEFAULT"]["OutputGenFiles"]

files = SerchAllFiles(ParseArrayConfig(Path), ".h")
# files += SerchAllFiles(ParseArrayConfig(Path), ".cpp")



def GetNamespaceWithClass(cursor):
    partsOfNamespace = []
    buffCur = cursor.semantic_parent
    while buffCur and buffCur.kind != cindex.CursorKind.TRANSLATION_UNIT:
        if buffCur.spelling and buffCur.kind in (cindex.CursorKind.NAMESPACE,
            cindex.CursorKind.CLASS_DECL,
            cindex.CursorKind.STRUCT_DECL):
            partsOfNamespace.append(buffCur.spelling)
        buffCur = buffCur.semantic_parent
    return "::".join(reversed(partsOfNamespace))

def GetNamespace(cursor):
    partsOfNamespace = []
    buffCur = cursor.semantic_parent
    while buffCur and buffCur.kind != cindex.CursorKind.TRANSLATION_UNIT:
        if buffCur.spelling and buffCur.kind == cindex.CursorKind.NAMESPACE:
            partsOfNamespace.append(buffCur.spelling)
        buffCur = buffCur.semantic_parent
    return "::".join(reversed(partsOfNamespace))

def FindedClassMacros(pr:cindex.TranslationUnit):
    res:MacrosData = []
    for token in pr.get_tokens(extent=pr.cursor.extent):
        print(token.location.line, token.location.column)
        if token.location.line == 180 and token.location.column == 1:
            print()
        try:
            if token.spelling == ClassKeyWord:
                NewMacros = MacrosData()
                NewMacros.Name = token.spelling
                NewMacros.Location = token.location.line
                res.append(NewMacros)
        except UnicodeDecodeError:
            continue
    return res

def GarbagePropertyFields(pr, cursor):
    garbageMacrosProperty = []
    for token in pr.get_tokens(extent=cursor.extent):
        try:
            if token.spelling == PropertyKeyWord:
                NewMacros = MacrosData()
                NewMacros.Name = token.spelling
                NewMacros.Location = token.location.line
                NewMacros.Params = CollectPropertyConfig(pr, list(pr.get_tokens(extent=token.cursor.extent)))
                garbageMacrosProperty.append(NewMacros)
        except UnicodeDecodeError:
            continue
    PropertyFields = []
    for node in cursor.get_children():
        if not garbageMacrosProperty:
            break
        if node.kind == cindex.CursorKind.FIELD_DECL:
            PosMacros = node.location.line
            FileName = node.location.file.name
            FindedMacros = list(filter(lambda el: el.Location <= PosMacros, garbageMacrosProperty))
            if not FindedMacros:
                continue

            TypeCollectRes = CollectFullTypeName(pr, node)
            if TypeCollectRes[1] == ETypePrimitive.ARRAY:
                NewProperty = ArrayType()
                NewProperty.InnerType = TypeCollectRes[2]
                NewProperty.IsPointer = "*" in NewProperty.InnerType
            elif TypeCollectRes[1] == ETypePrimitive.CUSTOM_PRIMITIVE:
                NewProperty = VariableField()
                NewProperty.IsPointer = False
            else:
                NewProperty = VariableField()
                NewProperty.IsPointer = node.type.kind == cindex.TypeKind.POINTER
            NewProperty.NameVar = node.spelling
            NewProperty.Type = TypeCollectRes[0]
            NewProperty.TypePrimitive = TypeCollectRes[1]
            NewProperty.Params = FindedMacros[0].Params

            PropertyFields.append(NewProperty)
            garbageMacrosProperty.remove(*FindedMacros)
    return PropertyFields

def GetDeclarationFromType(type):
    if type.kind in (cindex.TypeKind.POINTER, cindex.TypeKind.LVALUEREFERENCE, cindex.TypeKind.RVALUEREFERENCE):
        type = type.get_pointee()

    declaration = type.get_declaration()
    if declaration and declaration.kind != cindex.CursorKind.NO_DECL_FOUND:
        return declaration
    canonic = type.get_canonical()
    if canonic and canonic != type:
        declaration = canonic.get_declaration()
        if declaration and declaration.type != cindex.CursorKind.NO_DECL_FOUND:
            return declaration
    return None

def CollectPropertyConfig(tu, token:list):
    if not token:
        return []
    ResCollect = []

    def SearchCheck(j):
        return j[1].spelling == "RPROPERTY"


    FindPos = list(filter(SearchCheck, enumerate(token)))
    begin = False
    end = False

    if FindPos:
        for i in range(FindPos[0][0] + 1, len(token)):
            if token[i].spelling == "(":
                begin = True
            elif token[i].spelling == ")":
                end = True
                break
            elif token[i].spelling not in (",", ";"):
                ResCollect.append(token[i].spelling)

    return ResCollect


def CollectNamespaceOfProperty(cursor):
    parts = []
    parent = cursor.semantic_parent
    while parent and parent.kind != cindex.CursorKind.TRANSLATION_UNIT:
        if parent.spelling and parent.kind != cindex.CursorKind.NAMESPACE:
            parts.append(parent.spelling)
        elif parent.spelling and parent.kind in (cindex.CursorKind.CLASS_DECL, cindex.CursorKind.STRUCT_DECL):
            parts.append(parent.spelling)
        parent = parent.semantic_parent
    return "::".join(reversed(parts))

def CollectFullTypeName(tu, cursor) -> (str, ETypePrimitive, str):
    tokens = list(tu.get_tokens(extent=cursor.extent))
    if not tokens:
        tokens = [tok for tok in tu.get_tokens(extent=pr.cursor.extent) if
                  tok.location.line in (cursor.location.line - 1, cursor.location.line, cursor.location.line + 1) and tok.location.file.name == cursor.location.name]

    templateType = ExtractTemplateInnder("".join(i.spelling for i in tokens), "DArray")
    if templateType[0]:
        return f"DArray<{GetNamespace(cursor)}::{templateType[1]}>", ETypePrimitive.ARRAY, f"{GetNamespace(cursor)}::{templateType[1]}"
    if tokens[0].spelling in ("FVector", "FTransform"):
        return tokens[0].spelling, ETypePrimitive.CUSTOM_PRIMITIVE, ""

    declar = GetDeclarationFromType(cursor.type)
    if declar:
        namespaces = CollectNamespaceOfProperty(declar)
        Type = declar.type.spelling
        return f"{namespaces}::{Type}", ETypePrimitive.PRIMITIVE, ""
    spell = cursor.type.spelling
    if "::" in spell:
        last = spell.rfind("::")
        likelyNs = spell[:last]
        return likelyNs, ETypePrimitive.PRIMITIVE
    return spell, ETypePrimitive.PRIMITIVE, ""

def ExtractTemplateInnder(Type:str, SearchTemplate:str) -> (bool, str):
    """
    :param Type: type of variable
    :param SearchTemplate: string for search in Type
    :return: first param is successes, second param inner type
    """
    FindedTemp = Type.find(SearchTemplate)
    if FindedTemp < 0:
        return (False, "")

    InnerTypeStart = FindedTemp + len(SearchTemplate)
    start = Type.find("<", InnerTypeStart)
    if start < 0:
        return (False, "")
    i = start
    depth = 0
    end = -1
    while i < len(Type):
        char = Type[i]
        if char == "<":
            depth += 1
        elif char == ">":
            depth -= 1
            if depth == 0:
                end = i
        i += 1
    return (True, Type[start + 1:end])

def GarbageGeneratedBody(pr, cursor):
    for token in pr.get_tokens(extent=cursor.extent):
        if token.spelling == "GENERATED_BODY":
            FindGenBody = MacrosData()
            FindGenBody.Name = "GENERATED_BODY"
            FindGenBody.Location = token.location.line
            return FindGenBody

def GetParent(pr, cursor):
    IsCollect = False
    Res = ""
    Namespace = []
    for token in pr.get_tokens(extent=cursor.extent):
        if token.spelling == "{":
            break
        if token.spelling == ":":
            IsCollect = True
            continue
        if IsCollect:
            if token.spelling not in ("public", "protected", "private"):
                Res += token.spelling
    return Res

def GetParentWithNamepsace(NamespaceAbove:str, ParentFullName:str):
    if not ParentFullName:
        return ""
    Pos = ParentFullName.find("::")
    if Pos != -1 and NamespaceAbove in ParentFullName:
        return ParentFullName
    else:
        return NamespaceAbove + "::" + ParentFullName

def ParseFile(pr, FindedMacrosClass:list) -> list:
    if not FindedMacrosClass:
        return []
    classesRes = []
    for node in walk(pr.cursor):
        if node.kind == cindex.CursorKind.CLASS_DECL:
            macros = node.location.line - 1
            FindedMacros = list(filter(lambda el: el.Location == macros, FindedMacrosClass))
            if not FindedMacros:
                continue
            NewClass = ClassField()
            NewClass.Name = node.spelling
            NewClass.Namespace = GetNamespaceWithClass(node)
            NewClass.LineGenBody = GarbageGeneratedBody(pr, node)
            NewClass.Parent = GetParentWithNamepsace(NewClass.Namespace, GetParent(pr, node))
            if not NewClass.IsValidGeneretedBody():
                continue
            NewClass.Variable = GarbagePropertyFields(pr,node)
            classesRes.append(NewClass)
            print(node.displayname)
        if node.kind == cindex.CursorKind.CXX_BASE_SPECIFIER:
            print(node.spelling)
    return classesRes


def ParseClass(cursor):
    for fields in cursor:
        if fields.kind == cindex.CursorKind.FIELD_DECL and fields.location.file:
            pass

def PrintParse(cursor,ns="", depth=0):
    if cursor.kind == cindex.CursorKind.NAMESPACE:
        ns = ns + "::" + cursor.spelling if ns else cursor.spelling

    if cursor.kind == cindex.CursorKind.CLASS_DECL:
        print(f"Класс: {cursor.spelling}, пространство имён: {ns}")

    if cursor.kind == cindex.CursorKind.FIELD_DECL:
        print(f"  Переменная: {cursor.spelling} ({cursor.type.spelling})")

    for child in cursor.get_children():
        PrintParse(child, ns, depth + 1)

def walk(cursor):
    for c in cursor.get_children():
        yield c
        yield from walk(c)

def PrintTokens(tk):
    HasMacros = False
    HasClass = False
    for token in tk.get_tokens(extent=tk.cursor.extent):
        if token.spelling == ClassKeyWord:
            print("Найден макрос:", token.spelling, "строка:", token.location.line)

            for child in walk(tk.cursor):
                if child.kind == cindex.CursorKind.CLASS_DECL:
                    if child.location.line > token.location.line:
                        print(child.spelling)
                        break

index = cindex.Index.create()
for file in files:
    GenFileNextIteration = False
    pathg = os.path.abspath(file)
    pr = index.parse(pathg, args=["-x", "c++","-std=c++17","-nostdinc", "-I.", r"-IC:\Projects\C++\GameEngine\CoreEngine\Core", "-DRPROPERTY(x)="], options=cindex.TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD)
    # print(f"\n\nStart {file.name}\n\n")
    ParseClassesOfFile(ParseFile(pr, FindedClassMacros(pr)), file, OutputFiles)
    # for i in ParseFile(pr.cursor, FindedClassMacros(pr)):
    #    GenerateCodeClass(i.Name, GetNameFilesWithoutExtenshion(file.name),file, OutputFiles, i)
