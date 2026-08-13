import configparser
import pathlib as pl

from GeneralFile import GetNameFilesWithoutExtenshion, ParseArrayConfig, ParseFile, SerchAllFiles


SCRIPT_DIRECTORY = pl.Path(__file__).parent
CONFIG = configparser.ConfigParser()
CONFIG.read(SCRIPT_DIRECTORY / "BuildGenConfig.ini")
SOURCE_PATHS = CONFIG["DEFAULT"]["Path"]
CLASS_MACRO = CONFIG["Macros"]["Class"]
OUTPUT_DIRECTORY = CONFIG["DEFAULT"]["OutputGenFiles"]


def CreateFile(class_macro, class_name, file_name, _source_path, output, module):
    """Create an empty generated header/source pair for one reflected file."""
    if not class_macro or not class_name:
        return False

    module_output = (SCRIPT_DIRECTORY / output).absolute() / module
    module_output.mkdir(parents=True, exist_ok=True)
    file_stem = GetNameFilesWithoutExtenshion(file_name)

    # The second generation stage expects both files to exist and be empty.
    (module_output / f"{file_stem}.generated.h").write_text("")
    (module_output / f"{file_stem}.gen.cpp").write_text("")
    return True


def main():
    """Prepare empty output files for the libclang generation stage."""
    output_path = SCRIPT_DIRECTORY / OUTPUT_DIRECTORY
    output_path.mkdir(parents=True, exist_ok=True)

    source_paths, modules = ParseArrayConfig(SOURCE_PATHS)
    files_by_module = SerchAllFiles(source_paths, modules, ".h")
    for module, files in files_by_module.items():
        for file in files:
            with open(file, "r") as source_file:
                ParseFile(
                    source_file.readlines(),
                    CLASS_MACRO,
                    CreateFile,
                    file,
                    OUTPUT_DIRECTORY,
                    module,
                )


if __name__ == "__main__":
    main()
