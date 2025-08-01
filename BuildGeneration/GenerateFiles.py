import configparser
import pathlib
import pathlib as pl
import os
from pathlib import PurePath

openConfig = configparser.ConfigParser()
openConfig.read("BuildGenConfig.ini")
Path = openConfig["DEFAULT"]["Path"]

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
        targetPath = PurePath(pl.Path(__file__).parent / Path)
        for root, dirs, files in os.walk(targetPath):
            for file in files:
                point = file.find(".")
                if file.find(extenshion, point) != -1:
                    Res.append(PurePath(root) /  file)
    return Res

files = SerchAllFiles(ParseArrayConfig(Path), ".h")

print(ParseArrayConfig(Path))
print(openConfig.sections())

for file in files:
    with open(file, "r") as f:
        while True:
            line = f.readline()
            if not line:
                break
            print(line)
