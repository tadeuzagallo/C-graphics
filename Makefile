all:
	gcc $(CFLAGS) -c graphics.c
test: all
	gcc  $(CFLAGS) -o main main.c graphics.o -framework OpenGL -framework GLUT

clean:
	rm main *.o
