import os.path
import pathlib as pl
from GeneralFile import GetNameFilesWithoutExtenshion
from FieldTypes import *


def GenerateHeader(ClassNameLine, NameOpenFile, PathToOpenedFile, DirectoryOuputFile, FieldClass:list):
    # Check
    openFilePath = (pl.Path(__file__).parent / DirectoryOuputFile).absolute() / (
            GetNameFilesWithoutExtenshion(NameOpenFile) + ".generated.h")
    if not os.path.exists(openFilePath):
        return (False, "")
    #
    newFile = open(openFilePath, "r+")
    write = ""
    conteinFile = "".join(newFile.readlines())
    preGenClasses = f"#ifdef File{NameOpenFile} \n" \
                    f"#error \"{openFilePath.name} already included, missing '#pragma once'\" \n" \
                    f"#endif \n" \
                    f"#define File{NameOpenFile} \n" \
                    f"#include <ReflectionSystem/Include/ReflectionMacros.h> \n"

    # if preGenClasses not in conteinFile:
    write += preGenClasses
    CurrentFileId = f"{NameOpenFile}_{FieldClass[0].Name}"
    for Field in FieldClass:
        if Field.Namespace:
            write += f"namespace {Field.Namespace} "
            write +="{\n" + \
                    f"class {Field.Name}; \n" \
                    "}\n"
        else:
            write += f"class {Field.Name}; \n"

        write += f"struct Construct_{Field.Name}_Statics; \n"
        write +=f"DeclareNewClass({Field.Name}Generated) \n" + \
                f"#define {CurrentFileId}_{Field.LineGenBody.Location}_GENERATED_BODY \\" \
                "\npublic: \\" \
                f"\n          static CoreEngine::Reflection::ClassField* GetStaticClass(); \\" \
                f"\n          friend struct Construct_{Field.Name}_Statics; \\" \
                f"\nprivate:\n"
    write += f"GenetateHeaderRegistryClass({Field.Name}, {Field.Namespace})\n"

    write += f"#undef CURRENT_FILE_ID \n" \
             f"#define CURRENT_FILE_ID {CurrentFileId}"

    if write not in conteinFile:
        newFile.write(write)
    newFile.close()
    return (write, preGenClasses)

def GenerateSource(ClassNameLine, NameOpenFile, PathToOpenedFile, DirectoryOuputFile, FieldClass):
    openFilePathCpp = (pl.Path(__file__).parent / DirectoryOuputFile).absolute() / (
                GetNameFilesWithoutExtenshion(NameOpenFile) + ".gen.cpp")
    if not os.path.exists(openFilePathCpp):
        return (False, "")
    NewCppFile = open(openFilePathCpp, "r+")
    writeCpp = ""
    conteinFile = "".join(NewCppFile.readlines())
    preGenImplement = f'#include <{PathToOpenedFile}> \n\n'





    VariableGen = ""
    GenPropertyName = []
    Implement = ""
    for Field in FieldClass:
        Implement += f"struct Construct_{Field.Name}_Statics \n" + "{\n" \
                    f"Construct_{Field.Name}_Statics() {r"{}"}\n"

        for Var in Field.Variable:
            Params = "CoreEngine::Reflection::EPropertyFieldParams()" if not Var.Params else ""
            for Pararm in Var.Params:
                if not Params:
                    Params += f"CoreEngine::Reflection::EPropertyFieldParams::{Pararm}"
                else:
                    Params += f"|CoreEngine::Reflection::EPropertyFieldParams::{Pararm}"
            if Var.TypePrimitive in (ETypePrimitive.PRIMITIVE, ETypePrimitive.CUSTOM_PRIMITIVE):
                if Var.IsPointer:
                    Implement += f"\tGenerateClassPropertyFiled({Var.NameVar}, {Var.Type}, offsetof({Field.Namespace}::{Field.Name}, {Var.NameVar}), {Params})\n"
                else:
                    Implement += f"\tGeneratePropertyFiled({Var.NameVar}, {Var.Type}, offsetof({Field.Namespace}::{Field.Name}, {Var.NameVar}), {"true" if Var.IsPointer else "false"}, {Params})\n"
            elif Var.TypePrimitive == ETypePrimitive.ARRAY:
                if Var.IsPointer:
                    PosPointerChr = Var.InnerType.find("*")
                    Implement += f"\tGenerateClassArrayPropertyFiled({Var.NameVar}, {Var.Type}, {Var.InnerType[:PosPointerChr]}, offsetof({Field.Namespace}::{Field.Name}, {Var.NameVar}), {"true" if Var.IsPointer else "false"}, CoreEngine::Reflection::EPropertyFieldParams())\n"
                else:
                    Implement += f"\tGenerateArrayPropertyFiled({Var.NameVar}, {Var.Type}, offsetof({Field.Namespace}::{Field.Name}, {Var.NameVar}), {"true" if Var.IsPointer else "false"}, CoreEngine::Reflection::EPropertyFieldParams())\n"
            GenPropertyName.append(f"Field_{Var.NameVar}")
        DeclareGenVer = f"\tstatic DArray<UniquePtr<CoreEngine::Reflection::PropertyField>>& GetPropertyFieldArray()" + " { \n"
        DeclareGenVer += f"\t\tstatic bool HasInit = false;\n"
        DeclareGenVer += f"\t\tstatic DArray<UniquePtr<CoreEngine::Reflection::PropertyField>> {Field.Name}Generated_Fields; \n"
        DeclareGenVer += "\t\tif (!HasInit) {\n"
        for i in GenPropertyName:
            DeclareGenVer += f"\t\t{Field.Name}Generated_Fields.emplace_back(MakeUniquePtr<Construct_{Field.Name}_Statics::{i}>());\n"
        DeclareGenVer += f"\t\t\tHasInit = true;"
        DeclareGenVer += "\n\t\t}\n"

        DeclareGenVer += f"\t\treturn {Field.Name}Generated_Fields;\n"
        DeclareGenVer += "\t} \n"
        Implement += DeclareGenVer
        Implement += "\n};\n\n"
       # DeclareGenVer = f"DArray<UniquePtr<CoreEngine::Reflection::PropertyField>> Construct_{FieldClass.Name}_Statics::{FieldClass.Name}Generated_Fields = " + "std::move([]() {\n" \
        #                "\tDArray<UniquePtr<CoreEngine::Reflection::PropertyField>> Vec;\n"

        Parent = f"{Field.Parent}::GetStaticClass()" if Field.Parent else "nullptr"
        Implement += f"ImplementNewClass({Field.Name}Generated, {Field.Name},{Field.Namespace}, EClassFieldParams::NONE,sizeof({Field.Namespace}::{Field.Name}), Construct_{Field.Name}_Statics::GetPropertyFieldArray(), {Parent})\n" \
                    f"ImplementStaticClass({Field.Namespace}::{Field.Name}, {ClassNameLine}Generated,\"{Field.Name}\")\n"
        Implement += f"GenetateSourceRegistryClass({Field.Name}, {Field.Namespace})"


    if preGenImplement not in conteinFile:
        writeCpp += preGenImplement
    if Implement not in conteinFile:
        writeCpp += Implement
    if writeCpp not in conteinFile:
        NewCppFile.write(writeCpp)
    NewCppFile.close()
    return (preGenImplement,Implement)

def ParseClassesOfFile(Classes, file, OutputFiles):
    if not Classes:
        return
    finish_header = []
    finish_source = []
    if Classes:
        res_gen = GenerateCodeClass(Classes[0].Name, GetNameFilesWithoutExtenshion(file.name),file, OutputFiles, Classes)
        for j in res_gen[1]:
            finish_header.append(j)
        for j in res_gen[2]:
            finish_source.append(j)
    openHeaderFilePath = (pl.Path(__file__).parent / OutputFiles).absolute() / (
            GetNameFilesWithoutExtenshion(GetNameFilesWithoutExtenshion(file.name)) + ".generated.h")
    openFilePathCpp = (pl.Path(__file__).parent / OutputFiles).absolute() / (
            GetNameFilesWithoutExtenshion(GetNameFilesWithoutExtenshion(file.name)) + ".gen.cpp")
    header = open(openHeaderFilePath, 'r+')
    source = open(openFilePathCpp, 'r+')
    header_contein = "".join(header.readlines())
    source_contein = "".join(source.readlines())
    header_res = ""
    source_res = ""
    for i in finish_header:
        # if i in header_contein and i in header_res:
        if i not in header_res:
            header_res += i
    for i in finish_source :
        # if i in source_contein and i  in source_res:
        if i not in source_res:
            source_res += i

    if header_res != header_contein:
        header.seek(0)
        header.truncate(0)
        header.write(header_res)
    if source_res != source_contein:
        source.seek(0)
        source.truncate(0)
        source.write(source_res)
    header.close()
    source.close()


def GenerateCodeClass(ClassNameLine, NameOpenFile, PathToOpenedFile, DirectoryOuputFile, FieldClass):
    if not ClassNameLine:
        return False
    header = GenerateHeader(ClassNameLine, NameOpenFile, PathToOpenedFile, DirectoryOuputFile, FieldClass)
    source = GenerateSource(ClassNameLine, NameOpenFile, PathToOpenedFile, DirectoryOuputFile, FieldClass)
    if not header[0] or not source[0]:
        return (False, header, source)

    return (True, header, source)

def ParseClassesOfFile(Classes, file, OutputFiles):
    if not Classes:
        return
    finish_header = []
    finish_source = []
    if Classes:
        res_gen = GenerateCodeClass(Classes[0].Name, GetNameFilesWithoutExtenshion(file.name),file, OutputFiles, Classes)
        for j in res_gen[1]:
            finish_header.append(j)
        for j in res_gen[2]:
            finish_source.append(j)
    openHeaderFilePath = (pl.Path(__file__).parent / OutputFiles).absolute() / (
            GetNameFilesWithoutExtenshion(GetNameFilesWithoutExtenshion(file.name)) + ".generated.h")
    openFilePathCpp = (pl.Path(__file__).parent / OutputFiles).absolute() / (
            GetNameFilesWithoutExtenshion(GetNameFilesWithoutExtenshion(file.name)) + ".gen.cpp")
    header = open(openHeaderFilePath, 'r+')
    source = open(openFilePathCpp, 'r+')
    header_contein = "".join(header.readlines())
    source_contein = "".join(source.readlines())
    header_res = ""
    source_res = ""
    for i in finish_header:
        # if i in header_contein and i in header_res:
        if i not in header_res:
            header_res += i
    for i in finish_source :
        # if i in source_contein and i  in source_res:
        if i not in source_res:
            source_res += i

    if header_res != header_contein:
        header.seek(0)
        header.truncate(0)
        header.write(header_res)
    if source_res != source_contein:
        source.seek(0)
        source.truncate(0)
        source.write(source_res)
    header.close()
    source.close()

