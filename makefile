all: mm_shell.c
	gcc -o mm_shell mm_shell.c
clean:
	rm *~
	rm a.out
	rm mm_shell
run: all
	./mm_shell
