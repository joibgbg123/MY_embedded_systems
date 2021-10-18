#include <unistd.h>
#include <fcntl.h>

int main(void) {
    //enable pin 24
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    write(fd, "24", 2);
    write(fd, "17", 2);
    close(fd);

   //Set gpio24 as input
    fd = open("/sys/class/gpio/gpio24/direction", O_WRONLY);
    write(fd, "in", 2);
    close(fd);

    //Set gpio17 as output
    fd = open("/sys/class/gpio/gpio17/direction", O_WRONLY);
    write(fd, "out", 3);
    close(fd);

    //Set events
    int gpio17fd = open("/sys/class/gpio/gpio17/value", O_WRONLY);
    char c;
    
    while(1) 
    {
        int gpio24fd = open("/sys/class/gpio/gpio24/value", O_RDONLY);
        read(gpio24fd, &c, 1);
        close(gpio24fd);
        write(gpio17fd, &c, 1);
        usleep(1265/2);
    }

    close(gpio17fd);
    fd = open("sys/class/gpio/unexport", O_WRONLY);
    write(fd, "24", 2);
    write(fd, "17", 2);
    close(fd);

    return 0;
}