src = src/main.c src/renderer.c src/func.c src/vector2.c src/input.c
bin = -lgdi32 -lgmp


all:
	gcc -o doom $(src) -lgdi32 
debug:
	gcc -g -o doom $(src) -lgdi32
run: debug
	doom.exe