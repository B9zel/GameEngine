import pathlib as pl
import os



def ParseArrayConfig(arr):
    res = [""]
    arr = arr.replace('[', '' ,1)
    arr = arr[-1::-1].replace(']', '', 1)[-1::-1]

    currentIndex = 0
    for i in arr:
        if not res[currentIndex] and i == " ":
            continue
        if i != ',':
            res[currentIndex] += i
        else:
            res.append("")
            currentIndex += 1
    return res

def SerchAllFiles(path, extenshion):
    Res = []
    for Path in path:
        targetPath = pl.PurePath(pl.Path(__file__).parent / Path)
        for root, dirs, files in os.walk(targetPath):
            for file in files:
                point = file.find(".")
                if file.find(extenshion, point) != -1:
                    Res.append(pl.PurePath(root) /  file)
    return Res

def GetNameFilesWithoutExtenshion(file:str):
    Point = file.find(".")
    if Point != -1:
        return file[:Point]
    return file

def ParseFile(FileList:list, FindedClassKeyWorld, callBack, FilePath, DirectoryOutputFile):
    ClassKeyWoldBuffer = ""
    NextLineClass = False
    for line in FileList:
        if NextLineClass:
            line = line.replace(" ", "")
            KeyWordClass = "class"
            findedClassKey = line.find(KeyWordClass)
            endLine = line.find(";")
            if endLine == -1 and findedClassKey != -1:
                InheritanceKeyCharacter = ":"
                findedInheritance = line.find(InheritanceKeyCharacter, findedClassKey + len(KeyWordClass))
                classNameEndPos = 0
                if findedInheritance == -1:
                    classNameEndPos = len(line) - 1
                else:
                    classNameEndPos = findedInheritance
                callBack(ClassKeyWoldBuffer, line[findedClassKey + len(KeyWordClass):classNameEndPos], FilePath.name, FilePath,DirectoryOutputFile)
            NextLineClass = False
            continue
        findedClass = line.find(FindedClassKeyWorld)
        if findedClass != -1:
            closedBracket = line.find(')', findedClass)
            ClassKeyWoldBuffer = line[findedClass:closedBracket + 1]
            NextLineClass = True