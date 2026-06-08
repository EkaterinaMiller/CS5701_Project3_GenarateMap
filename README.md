# CS5701 Project 3 - Generate Map

This project is an interactive terrain map generator built with C++ and SFML 3.1.
It creates a square map with a user-selected size and terrain types, then improves the map using a min-conflicts style local search algorithm.

## Features

- Interactive UI for selecting:
	- Map size (`50x50`, `75x75`, `100x100`, `150x150`, `200x200`)
	- Terrain set (Water, Beach, Lowland, Forest, Hill, Ridges, Mountain, Snow Tops)
	- Diagonal constraints (`On`/`Off`)
	- Animation (`On`/`Off`)
- Random initial map generation
- Conflict-reduction algorithm that iteratively improves tile assignments
- Real-time map display and conflict status updates
- Assets copied automatically to the build output directory by CMake

## Tech Stack

- C++20
- CMake (3.22+)
- SFML 3 (Graphics, Window, System)

## Project Structure

- `src/` - Application and algorithm source files
- `include/` - Header files
- `assets/` - Runtime assets copied next to the executable
- `CMakeLists.txt` - Build configuration

## Prerequisites

Install the following before building:

- A C++ compiler with C++20 support
- CMake 3.22 or newer
- SFML 3 development package

## Environment Setup and Build

### Windows (tested on Windows 11, 64-bit)

1. Install tools:
	 - VS Code: Download from https://code.visualstudio.com/Download
	 - CMake: Download installer from https://cmake.org/download/ 
	 - SFML 3.1.0 (or compatible SFML 3 release): Download SFML and corresponding compiler from https://www.sfml-dev.org/download/sfml/3.1.0/. Unzip and move SFML-3.1.0 and mingw64 (in my case) to the `C:`
2. Ensure CMake can find SFML:
	 - Option A: install SFML to `C:/SFML-3.1.0` (matches current `CMakeLists.txt` default on Windows)
	 - Option B: pass one of these while configuring:
		 - `-DCMAKE_PREFIX_PATH="C:/path/to/SFML"`
		 - `-DSFML_DIR="C:/path/to/SFML/lib/cmake/SFML"`
3. Configure and build from the project root:

```powershell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

4. Run:

```powershell
cd build
.\mapGenarator.exe
```

Notes:
- If you see `cannot open output file `mapGenerator.exe`: Permission denied`, close any running `mapGenerator.exe` and rebuild. 

### Linux (tested on WSL)
You cannot install SFML 3 using apt because Ubuntu’s official package repositories still only offer older versions (usually SFML 2.x). One of the way to instal SFML 3 on Ubuntu is to build from source.
1. Install dependencies (Ubuntu/Debian example):

```bash
sudo apt update
sudo apt install -y build-essential cmake libsfml-dev\
libx11-dev libxrandr-dev libxcursor-dev libxi-dev \
libgl1-mesa-dev libudev-dev libfreetype-dev \
libopenal-dev libvorbis-dev libflac-dev libogg-dev \
libmbedtls-dev libssh2-1-dev libharfbuzz-dev\
````

2. Clone the SFML repository from GitHub and specify the version tag you want
```bash
git clone https://github.com/SFML/SFML.git
cd SFML
````
3. Clone the official repository from the SFML GitHub Releases page. To target SFML 3, explicitly check out the master branch or a specific 3.x tag
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
> Note: If you get an error about not meeting CMake minimum requirements. You need to update CMake to the newer version. Unfortunately, `apt` can't do it for the same reason it can't do SFML 3. You can do it with Snap manager.
3.1 Remove current CMake and install a newer version with snap:
```bash
sudo apt purge --auto-remove cmake
sudo snap install cmake --classic
````
3.2 Rerun cmake command from step 3. If you get a "No such file or directory" error, you need to clear local history
```bash
hash -r
rm -rf CMakeCache.txt CMakeFiles
````
Re-run the `cmake` command from step 3 again
> Note: If you get an error, Google the error, install required dependencies, then clear the cmake cache and re-run the command

4. Compile the codebase and install the generated binaries to your system
```bash
cmake --build . --config Release -j$(nproc)
sudo cmake --install .
````
5. If you installed SFML to a standard global directory like /usr/local/lib, configure the dynamic linker to recognize the new `.so` files:
```bash
sudo ldconfig
````
6. Clone the project repository and go to the project directory
```bash
git clone https://github.com/EkaterinaMiller/CS5701_Project3_GenerateMap.git GenerateMap
cd GenerateMap
````

7. Configure and build from the project root:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

8. Change to the build directory and run:

```bash
cd build
./mapGenerator
```

> Note (AI generated, not tested): If CMake cannot find SFML, configure with:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/SFML
```

### macOS (AI generated, not tested)

1. Install tools (Homebrew example):

```bash
brew install cmake sfml
````
2. Clone the project repository and go to the project directory
```bash
git clone https://github.com/EkaterinaMiller/CS5701_Project3_GenerateMap.git GenerateMap
cd GenerateMap
````
3. Configure and build from the project root:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

4. Change to the build directory and run:

```bash
cd build
./mapGenerator
```

> Note: If SFML is installed in a non-default location, configure with:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=/opt/homebrew
```

## How to Use

1. Launch the application.
2. Select map size and terrain types.
3. Click `Generate Map` to create an initial map.
4. Choose diagonal and animation options.
5. Click `Run` to start conflict reduction.
6. Watch the status text for conflict counts, iterations, and completion/stuck state.

## Algorithm Summary

- The map is initialized with random terrain values.
- Each pass evaluates conflict counts for tiles relative to neighbors.
- The algorithm tries to reduce conflicts by assigning better local values.
- If stuck in a local minimum, it perturbs a high-conflict tile (and neighbors) to escape.

## Troubleshooting

- CMake cannot find SFML:
	- Pass `CMAKE_PREFIX_PATH` or `SFML_DIR` explicitly.
	- Clear/reconfigure the `build/` directory if cached paths are stale.
- Executable builds but does not run:
	- Ensure the `assets/` copy step completed.
- Build fails after app was running on Windows:
	- Stop the running executable and rebuild.
