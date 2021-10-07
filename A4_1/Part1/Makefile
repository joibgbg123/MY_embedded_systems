all: hello

# This line specifies hello as the make target and src/main.cpp as a depenency.
# The following command is executed if a dependency has a more recent modification date than the target.
hello: src/main.cpp
	g++ -o hello src/main.cpp