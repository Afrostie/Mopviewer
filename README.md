# Mopviewer
A new version of the Mopviewer for use with moody: https://gitlab.com/carey.pridgeon/moody
It is going to use an nCurses screen for the main interface with OpenGL for main viewer

##Current Features:
1. Enter 1 to look at Particle information
2. Enter 2 to Open Mopviewer
3. Enter any other key on main screen to exit
4. Uses a scale value to determine how many particles to load, speeds up the loading of extremly large data files

##Particle Information:
1. Use the left and right arrow keys to cycle between particles
2. Use 2 to exit program to main menu
3. Up arrow to cycle through states
4. Down arrow to reset to original state

##Requirments
1. glfw
2. glew
3. nCurses
4. OpenGL 3.3 compatible or higher
There is a good chance I missed off some important parts

##MopFile
1. MopFiles should be placed in the Resources Folder
2. When asked for the File Name it automatically adds the file location and the extension.
3. E.G to use the 10000 particles file simply enter 10000 when prompted

#TODO:
1. Start working on the actual Mopviewer
2. Create option to change scale value while program is running/on startup
3. Potentially use a wrapper for opengl such as oglplus (http://oglplus.org/)
