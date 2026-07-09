# GameEngine

A modular game engine built with C++ and modern graphics APIs. This project provides a solid foundation for game development with core rendering capabilities and editor tools.

---
<p align="center">
	<img src="Resources\Editor1.png" alt="Editor Screenshot" width="800">
</p>

<p align="center">
	<img src="Resources\Editor2.png" alt="Editor Screenshot" width="800">
</p>


## 📋 Table of Contents

- [Features](#features)
- [Project Structure](#project-structure)
- [Technology Stack](#technology-stack)
- [Requirements](#requirements)
- [Building](#building)


## ✨ Features

### Core Functionality

- **Custom Reflection System** - metaprogramming similar to Unreal Engine. `RCLASS()`, `RPROPERTY()`, `GENERATED_BODY()` for convenient class definition
- **Core Engine**: Robust rendering pipeline with graphics APIs
- **Editor Engine**: Integrated visual editor for game development
- **3D Model Loading**: Support for various 3D model formats (via Assimp)
- **Entity-Component System**: Flexible game object management
- **Logging System**: Comprehensive logging with spdlog
- **Object System** - sophisticated object management with garbage collection

### Save & Serialization
- **Serialization/Deserialization** - full save data support
- **JSON** - config persistence via nlohmann/json
- **Asset Manager** - resource management

### Graphics & Rendering
- **OpenGL 4.5** integration via Glad
- **Shader System** with GLSL support
- **Material System** with async loading


## 🛠 Technology Stack

### Third-Party Libraries
- **GLFW** - Window and input management
- **Glad** - OpenGL function loader
- **GLM** - Mathematics library
- **Assimp** - 3D model loading
- **Bullet Physics** - Physics simulation
- **spdlog** - Logging library
- **ImGui** - GUI framework
- **ImGuizmo** - 3D transformation gizmos
- **JSON** - Data serialization (nlohmann/json)
- **STB Image** - Image loading

## 📦 Requirements

### System Requirements
- **Windows** (primary development target)
- **CMake** 3.30 or higher
- **Visual Studio 2022+** or compatible C++ compiler
- **OpenGL 4.5** compatible graphics card

### Development Tools
- **Python 3.8+** (for build generation scripts)
- **Git** (with submodule support)

## 🔨 Building

### Prerequisites
Clone the repository with submodules:
```bash
git clone --recursive https://github.com/B9zel/GameEngine.git
cd GameEngine
```
Then run the following commands to generate the necessary binary and metadata files:
```
start GenerateBinFiles 
start GenerateMetaFiles
```
Open the `GameEngine.sln` solution file in Visual Studio and build the project.
