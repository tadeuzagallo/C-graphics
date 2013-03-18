all:
	gcc $(CFLAGS) -c lib/graphics.c -o lib/graphics.o

samples: 
	cd examples/Convex\ Hull && make
	cd examples/balls && make

clean:
	-@rm *.o lib/*.o examples/Convex\ Hull/convex examples/balls/ball 2> /dev/null
