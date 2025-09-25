@echo off


IF NOT EXIST "%CD%/BuildGeneration/venv" (
    python -m venv %CD%\BuildGeneration\venv
)

CALL %CD%/BuildGeneration/ven/Scripts/activate

python ./BuildGeneration/GenerateFiles.py
cmake . -B ./Bin/

pause()