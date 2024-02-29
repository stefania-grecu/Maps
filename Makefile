build:
	gcc -o tema3 grafuri_main.c grafuri_functii.c
run:
	./tema3
clean:
	rm tema3
valgrind:
	valgrind --leak-check=yes --track-origins=yes ./tema3