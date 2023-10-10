src = src/main.c src/renderer.c src/func.c src/vector2.c src/input.c
lib = -lgdi32
warning = -Wall -Wextra -Q --help=warning

all:
	gcc -o doom $(src) $(lib) 
debug:
	gcc -g -o doom $(src) $(lib)
warning:
	gcc -g -o doom $(src) $(lib) $(warning)
run: debug
	doom.exe