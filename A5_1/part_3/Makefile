SOURCES = main.cpp

all: app

app: $(SOURCES)
	g++ -o app -lpthread $(SOURCES)

test: test.cpp
	g++ -o test test.cpp

clean:
	rm app
	rm test