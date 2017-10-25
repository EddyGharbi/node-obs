# node-obs
'node-obs' is a Node.JS module integrating Open Broadcaster Software Studio. The project uses cmake-js for easy integration with CMake based projects and partially supports node-gyp.

## Prerequisites
* [Node.JS](https://nodejs.org/en/) with NPM
* [CMake](https://cmake.org/)
* Open Broadcaster Studio (Build for 32-bit in /build32 and 64-bit in /build64)
* cmake-js Node.JS module

### Mac OSX / Apple
* Node.js - Download and Install [Node.js](http://www.nodejs.org/download/). You can also follow [this gist](https://gist.github.com/isaacs/579814) for a quick and easy way to install Node.js and npm

```
brew update
brew install nodejs npm ffmpeg glew cmake qt5
```

### Debian/Ubuntu (Linux)
Linux/OSX: You can follow [this gist](https://gist.github.com/isaacs/579814) for a quick and easy installation.

```
apt-get update
apt-get install nodejs npm ffmpeg glew cmake qt5
```

### Windows
1. Install Node.JS and add it to PATH
2. Install CMake and add it to PATH
3. Install Visual Studio 2015

### All platforms
Install cmake-js globally:

```
npm install -g cmake-js
```

# Using cmake-js
cmake-js is pretty much a drop-in replacement for node-gyp and works about as you would expect a CMake based solution to work. We can choose where to build, have some control over what project generator is being used and can also easily choose what runtime we are building against.

## Choosing where to Build
This is pretty simple, we just attach `--out PATHHERE` to all commands, changing where it will generate the necessary files for building. This is directory will contain all project files and binaries, so make sure it doesn't exist already.

For multi-runtime builds, the following is recommended:

* /build/nodejs
* /build/electron
* /build/... (any other runtimes...)

## Controlling the Project Generator
Mac users might prefer building with Xcode instead of makefiles like it is common on Linux systems. To do that you appen `--prefer-xcode` to the cmake-js commands.

## Choose the Runtime
When building for Node.JS the commands are simple. But what about electron? cmake-js has an easy way to change the runtime used: `--runtime`. To build for electron we just need to specify the electron runtime and the version: `--runtime electron --runtime-version 1.6.3` (replace 1.6.3 with the version you build your actual project against).

## Issue: CMake isn't generating what I told it to!
Make sure that you ran `cmake-js clean` first, otherwise it will use the cached project builder configuration.

# Building
Building is pretty simple and does not differ too much from other build systems. It's recommended to have separate directories for each runtime build. 

## 1. Install Modules
Open a shell in the root directory and run the following:
```
npm install
npm install sleep
```

### 2. Configure
This is the most important step, as it sets up the basics. Run the following command while replacing {myPathToObsStudio} with the actual path to obs-studio:

```
npm config set cmake_OBSSTUDIOPATH {myPathToObsStudio}
npm run configure
```

### 3. Generate Project Files
Building is simple:

```
npm run build
```

This will create the necessary project files which can then be built in the OS's build system (Visual Studio, Xcode, make, ...). 

### 4. Build the Project
Open up the generated project (if it's an IDE one) and build it or just build it using make.
Once it's done you should have a /distribute directory under your chosen build directory (it's /build/ by default), which contains a node-obs module that uses your chosen settings. Copy or symlink this into the project's node_modules directory and then simply require() it.

## Testing
Unit tests can be run by calling `npm test` after building with the Node.JS runtime (see Building).

# License
Streamlabs Inc, all rights reserved.
