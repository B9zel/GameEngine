import os
import pathlib as pl


def ParseArrayConfig(value):
    """Parse the configured source paths and derive their module names."""
    paths = [""]
    modules = [""]
    value = value.replace("[", "", 1)
    value = value[::-1].replace("]", "", 1)[::-1]

    current_index = 0
    for character in value:
        if not paths[current_index] and character == " ":
            continue
        if character != ",":
            paths[current_index] += character
            if character.isalpha():
                modules[current_index] += character
        else:
            paths.append("")
            modules.append("")
            current_index += 1
    return paths, modules


def SerchAllFiles(paths, modules, extension):
    """Collect matching files for every configured module."""
    result = {}
    for path_index, source_path in enumerate(paths):
        module = modules[path_index]
        result[module] = []
        target_path = pl.Path(__file__).parent / source_path
        for root, _, files in os.walk(target_path):
            for file in files:
                if file.endswith(extension):
                    result[module].append(pl.PurePath(root) / file)
    return result


def GetNameFilesWithoutExtenshion(file: str):
    """Return the part of a file name before its first dot."""
    extension_start = file.find(".")
    if extension_start != -1:
        return file[:extension_start]
    return file


def ParseFile(file_lines: list, class_macro, callback, file_path, output_directory, current_module):
    """Call *callback* for classes immediately following the reflection macro."""
    class_macro_text = ""
    class_is_on_next_line = False
    for line in file_lines:
        if class_is_on_next_line:
            line = line.replace(" ", "")
            class_keyword = "class"
            class_start = line.find(class_keyword)
            line_ends_declaration = line.find(";")
            if line_ends_declaration == -1 and class_start != -1:
                inheritance_start = line.find(":", class_start + len(class_keyword))
                class_name_end = len(line) - 1 if inheritance_start == -1 else inheritance_start
                callback(
                    class_macro_text,
                    line[class_start + len(class_keyword):class_name_end],
                    file_path.name,
                    file_path,
                    output_directory,
                    current_module,
                )
            class_is_on_next_line = False
            continue
        macro_start = line.find(class_macro)
        if macro_start != -1:
            closing_bracket = line.find(")", macro_start)
            class_macro_text = line[macro_start:closing_bracket + 1]
            class_is_on_next_line = True
