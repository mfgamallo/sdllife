life:
	gcc main.c life.c -o life `pkg-config --cflags --libs sdl2` -lSDL2_ttf

clean:
	rm life
