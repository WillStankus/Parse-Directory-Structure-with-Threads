all: defrag
	
defrag : defrag.c 
	gcc -pthread -o defrag defrag.c

.PHONY : all clean
clean: 
	rm defrag
	rm music.mp3
