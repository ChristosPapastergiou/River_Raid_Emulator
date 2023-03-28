# Makefile for game.a & libraylib.a

#Find what OS is in use
ifdef WASM
	SUFFIX = wasm
else ifeq ($(OS),Windows_NT)
	SUFFIX = win
else ifeq ($(shell uname -s),Linux)
	SUFFIX = linux
else
	SUFFIX = macos
endif

all: game.a libraylib.a

# Making game.a just by copying the OS
game.a:
	cp game_$(SUFFIX).a game.a

# Same for libraylib.a 
libraylib.a:
	cp libraylib_$(SUFFIX).a libraylib.a

clean:
	$(RM) game.a libraylib.a