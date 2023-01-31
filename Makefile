all:
	g++ -g --std=c++17 -Iinclude -Llib src/*.cpp src/glad.c -lglfw3dll -lopengl32 -lgdi32 -o bin/pseudocraft