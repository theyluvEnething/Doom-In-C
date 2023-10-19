src = src/main.c
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