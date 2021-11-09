SOURCES = shared_mem_demo.c

all: shared

shared: $(SOURCES)
	gcc -o shared -lpthread -lrt $(SOURCES)

clean:
	rm shared