import configparser
import pathlib as pl
from argparse import Namespace
from pkgutil import extend_path

from clang import cindex
from ParsClasses import GenerateCodeClass, ParseClassesOfFile
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


class VariableField:
    def __init__(self):
        self.NameVar = ""
        self.Type = ""
        self.Params = ""

class ClassField:
    def __init__(self):
        self.Name = ""
        self.Namespace = ""
        self.IsPointer = False
        self.LineGenBody = MacrosData()
        self.Parent = ""
        self.Variable:VariableField = []
    def __str__(self):
        return f"Name: {self.Name}    Namespace: {self.Namespace}"

    def IsValidGeneretedBody(self):
        return self.LineGenBody.Location != -1

class MacrosData:
    def __init__(self):
        self.Location = 0
        self.Name = ""

def GetNamespace(cursor):
    partsOfNamespace = []
    buffCur = cursor.semantic_parent
    while buffCur and buffCur.kind != cindex.CursorKind.TRANSLATION_UNIT:
        if buffCur.spelling and buffCur.kind in (cindex.CursorKind.NAMESPACE,
            cindex.CursorKind.CLASS_DECL,
            cindex.CursorKind.STRUCT_DECL):
            partsOfNamespace.append(buffCur.spelling)
        buffCur = buffCur.semantic_parent
    return "::".join(reversed(partsOfNamespace))

def FindedClassMacros(pr:cindex.TranslationUnit):
    res:MacrosData = []
    for token in pr.get_tokens(extent=pr.cursor.extent):
        if token.spelling == ClassKeyWord:
            NewMacros = MacrosData()
            NewMacros.Name = token.spelling
            NewMacros.Location = token.location.line
            res.append(NewMacros)
    return res

def GarbagePropertyFields(pr, cursor):
    garbageMacrosProperty = []
    for token in pr.get_tokens(extent=cursor.extent):
        if token.spelling == PropertyKeyWord:
            NewMacros = MacrosData()
            NewMacros.Name = token.spelling
            NewMacros.Location = token.location.line
            garbageMacrosProperty.append(NewMacros)
    PropertyFields = []
    for node in cursor.get_children():
        if node.kind == cindex.CursorKind.FIELD_DECL:
            PosMacros = node.location.line - 1
            FindedMacros = list(filter(lambda el: el.Location == PosMacros, garbageMacrosProperty))
            if not FindedMacros:
                continue
            NewProperty = VariableField()
            NewProperty.NameVar = node.spelling
            NewProperty.Type = node.type.spelling
            NewProperty.IsPointer = node.type.kind == cindex.TypeKind.POINTER
            PropertyFields.append(NewProperty)
    return PropertyFields

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
    if Pos != -1:
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
            NewClass.Namespace = GetNamespace(node)
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
    pr = index.parse(pathg, args=["-x", "c++","-std=c++17","-nostdinc",  r"-IC:\Projects\C++\GameEngine\CoreEngine\Core"], options=cindex.TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD)
    # print(f"\n\nStart {file.name}\n\n")
    ParseClassesOfFile(ParseFile(pr, FindedClassMacros(pr)), file, OutputFiles)
    # for i in ParseFile(pr.cursor, FindedClassMacros(pr)):
    #    GenerateCodeClass(i.Name, GetNameFilesWithoutExtenshion(file.name),file, OutputFiles, i)
