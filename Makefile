CC=g++
FILES=./src/main.cpp
INCLUDE=-I./depend/ -I./depend/glew/include -I./depend/glfw/include/ -I./depend/glm/
FLAGS=-ggdb -O0 -lGLEW -lglfw -lGL

all:
	$(CC) $(FLAGS) $(INCLUDE) $(FILES)
