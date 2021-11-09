SOURCES = shared_mem_counter.c shared_mem_print.c

all: shared

shared: $(SOURCES)
	gcc -o shared -lpthread -lrt shared_mem_counter.c
	gcc -o print -lpthread -lrt shared_mem_print.c

clean:
	rm shared
	rm print