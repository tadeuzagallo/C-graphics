all:
	cd ../../ && make 
	gcc $(CGLAFS) -o convex main.c convex.c ../../lib/graphics.o -framework OpenGL -framework GLUT

clean:
	rm convex
