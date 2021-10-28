all: command

# This line specifies hello as the make target and src/main.cpp as a depenency.
# The following command is executed if a dependency has a more recent modification date than the target.
command: command.c
	g++ -o command command.c

clean: 
	rm command