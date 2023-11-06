src = src/main.c src/lLib.c src/level.c src/renderer.c
lib = -lgdi32
warning = -Wall -Wextra -Q --help=warning
sdl = -I lib/SDL2/include -L lib/SDL2/lib -lmingw32 -lSDL2main -lSDL2

extra_src = src/lLib.c src/level.c

all:
	gcc -o doom $(src) $(lib) 
debug:
	gcc -g -o doom $(src) $(lib) 
warning:
	gcc -g -o doom $(src) $(lib) $(warning)
run: debug
	doom.exe
jdh:
	gcc -o jdh -g src/jdh.c -I lib/SDL2/include -L lib/SDL2/lib -lmingw32 -lSDL2main -lSDL2
	jdh.exe
	