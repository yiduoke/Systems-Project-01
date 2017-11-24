all: project.c
	gcc project.c
clean:
	rm *~
	rm a.out
run: all
	./a.out
