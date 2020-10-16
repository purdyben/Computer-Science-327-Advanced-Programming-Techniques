Make: Check.o linkedlist.o Advance.o
	gcc -I/share/cs327/include game.c /share/cs327/lib/libtermbox.a -o game
	gcc check.c linkedlist.c -o check   
	gcc advance.c linkedlist.c linkedlist.h -o advance
Advance.o: advance.c linkedlist.o
	gcc advance.c linkedlist.o	
Check.o: check.o linkedlist.h
	gcc -c check.c linkedlist.c
linkedlist.o: linkedlist.c linkedlist.h
	gcc -c linkedlist.c linkedlist.h
clean: 
	rm *.o
	rm game
	rm check
	rm advance
