# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -lncurses -std=c++11 -lGLEW -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -lpthread -ldl -lz -D MOPFILELIGHT


# the build target executable:
TARGET = main

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) -o Mopviewer $(TARGET).cpp mopViewer.cpp window.cpp $(CFLAGS)

clean:
	$(RM) $(TARGET).o
