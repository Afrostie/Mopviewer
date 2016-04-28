# Installation Steps

## Required Libraries
These required libraries are common across Windows and Linux
1.	OpenGL 3.3 or higher compatible GPU/drivers
2.	[GLFW3](http://www.glfw.org/)
3.	[GLEW](http://glew.sourceforge.net/)
4.	nCurses *(libncurses-dev)* (or PDCurses if on Windows)
5.	[glm](http://glm.g-truc.net/0.9.7/index.html)
6.	[STB](https://github.com/nothings/stb) for image loading
7.	[Assimp](https://github.com/assimp/assimp) for model loading (Use newer version off of github or it won't compile)
8.	[zLib](http://www.zlib.net/) for compression in Mopfile (not in use currently)


Mopfile is included as a submodule in mopViewer, this means there are two ways to download.

##### Method 1: Clone the repository using the --recursive tag

```bash
  git clone https://github.com/afrostie/mopViewer.git --recursive
```

##### Method 2: Alternative method

```bash
git submodule init
git submodule update
git pull origin master
```
Download the latest [release](https://github.com/Afrostie/mopViewer/releases/tag/1.2), unpack it and run make

### Linux
Make sure that all above Libraries are installed, most are available via apt-get, except for STB and GLFW3 I believe.

GLFW3 you will need to download and build yourself, STB needs to be downloaded and moved to /usr/include or similar

```shell
git clone https://github.com/afrostie/mopViewer.git --recursive
cd mopViewer/
make
./mopViewer
```

### Windows
Windows is quite a lot more complicated than Windows

A Visual Studio 2015 solution file is included

To start with clone the repository the same way as you do usually
```bash
  git clone https://github.com/afrostie/mopViewer.git --recursive
```
Next is to acquire the libraries. Most of the libraries will require building yourself as the prebuilt binaries often don't work for me. I place all my libraries within a Library folder with a lib subfolder and an include subfolder.

In Visual Studio create a project from existing code and choose the mopViewer folder as the directory. Next is to edit the Project Settings.

Under VC++ Directories make sure that Include Directories and Library Directories link to the correct path.

Under Linker -> Input -> Additional Dependencies you will need:
opengl32.lib
glfw3.lib
glew32s.lib
pdcurses.lib
assimp.lib
zlib.lib

Assimp, PDCurses and glew require their .dll files to be placed in the same folder as the executable.
