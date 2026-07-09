@echo off


IF NOT EXIST "%CD%/BuildGeneration/venv" (
    python -m venv %CD%\BuildGeneration\venv
)
CALL %CD%/BuildGeneration/venv/Scripts/activate

python -c "import clang" 2>nul
if %errorlevel% neq 0 (
    python -m pip install libclang
) 


python ./BuildGeneration/GenerateFiles.py
cmake . -B ./Bin/

pause()