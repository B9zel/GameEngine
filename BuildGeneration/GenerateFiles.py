import configparser
import pathlib as pl
import os
from pathlib import PurePath
from GeneralFile import *

OpenConfig = configparser.ConfigParser()
OpenConfig.read(f"{pl.Path(__file__).parent}/BuildGenConfig.ini")
Path = OpenConfig["DEFAULT"]["Path"]
ClassKeyWord = OpenConfig["Macros"]["Class"]
OutputFiles = OpenConfig["DEFAULT"]["OutputGenFiles"]

if not os.path.exists(pl.Path(__file__).parent /OutputFiles):
    os.makedirs(pl.Path(__file__).parent / OutputFiles)


files = SerchAllFiles(ParseArrayConfig(Path), ".h")


def CreateFile(ClassLine, ClassNameLine,FileName,PathToFileName, Output):
    if not ClassLine or not ClassNameLine:
        return False
    openFilePath = (pl.Path(__file__).parent / Output).absolute() / (
            GetNameFilesWithoutExtenshion(file.name) + ".generated.h")
    newFile = open(openFilePath, "w")
    newFile.close()
    openFilePath = (pl.Path(__file__).parent / Output).absolute() / (
            GetNameFilesWithoutExtenshion(file.name) + ".gen.cpp")
    newFile = open(openFilePath, "w")
    newFile.close()



def ParseFileForGenerater(FileList:list):
    for line in FileList:
        findedClass = line.find(ClassKeyWord)
        if findedClass != -1:
            closedBracket = line.find(')', findedClass)
            ClassKeyWoldBuffer = line[findedClass:closedBracket + 1]
            NextLineClass = True


i = 0
for file in files:
    GenFileNextIteration = False
    if i >= 98:
        print()
    i += 1


    with open(file, "r") as f:
        line = f.readlines()
        ParseFile(line, ClassKeyWord, CreateFile, file ,OutputFiles)

# templateCmake = open(f"{pl.Path(__file__).parent}/TemplateCMakeForGenFiles.txt", 'r')
# GenCmake = open(f"{pl.Path(__file__).parent / OutputFiles}/CMakeLists.txt", 'w')
# GenCmake.write("".join(templateCmake.readlines()))
#
# templateCmake.close()
# GenCmake.close()