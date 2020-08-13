CC=gcc
CFLAGS=-lm
GUILIBS=-L ./iup/ -I ./iup/include/ 
GUIFLAGS=-liupimglib -liup -Wl,-rpath=/usr/lib64/

src_gui=core.c gui.c
src_cli=core.c cli.c

.DEFAULT_GOAL=gui

.PHONY:gui
gui:
	$(CC) $(src_gui) -o echo3d_gui $(GUILIBS) $(GUIFLAGS) $(CFLAGS)

.PHONY:cli
cli:
	$(CC) $(src_cli) -o echo3d_cli $(CFLAGS)

.PHONY:run
run:gui
	./echo3d_gui

.PHONY:clean
clean:
	rm -f echo3d_gui echo3d_cli
