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

Modules = ParseArrayConfig(Path)
files = SerchAllFiles(Modules[0],Modules[1], ".h")


def CreateFile(ClassLine, ClassNameLine,FileName,PathToFileName, Output, module):
    if not ClassLine or not ClassNameLine:
        return False

    Path = (pl.Path(__file__).parent / Output).absolute() / module
    if not os.path.exists(Path):
        os.mkdir(Path)

    openFilePath = Path / (GetNameFilesWithoutExtenshion(file.name) + ".generated.h")
    newFile = open(openFilePath, "w")
    newFile.close()
    openFilePath = Path / (GetNameFilesWithoutExtenshion(file.name) + ".gen.cpp")
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
for module in files:
    GenFileNextIteration = False
    if i >= 98:
        print()
    i += 1
    for file in files[module]:
        with open(file, "r") as f:
            line = f.readlines()
            ParseFile(line, ClassKeyWord, CreateFile, file ,OutputFiles, module)

# templateCmake = open(f"{pl.Path(__file__).parent}/TemplateCMakeForGenFiles.txt", 'r')
# GenCmake = open(f"{pl.Path(__file__).parent / OutputFiles}/CMakeLists.txt", 'w')
# GenCmake.write("".join(templateCmake.readlines()))
#
# templateCmake.close()
# GenCmake.close()