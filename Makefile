CC=tcc
LDLIBS=-L ./iup/ -I ./iup/include/ 
CFLAGS=-liupimglib -liup -Wl,-rpath=/usr/lib64/

src=$(wildcard *.c)

.DEFAULT_GOAL=build_gui

.PHONY:build_gui
build_gui:
	$(CC) $(src) -o echo3d_gui $(LDLIBS) $(CFLAGS)

.PHONY:run
run:build_gui
	./echo3d_gui

.PHONY:clean
clean:
	rm echo3d_gui
