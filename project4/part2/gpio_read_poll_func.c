#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

int main(void) {
    //enable pin 24
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    write(fd, "24", 2);
    write(fd, "17", 2);
    close(fd);

    //Set gpio24 as input
    int gpio24fd = open("/sys/class/gpio/gpio24/direction", O_WRONLY);
    write(gpio24fd, "in", 2);
    close(gpio24fd);

    //Set gpio24 interrupt command
    gpio24fd = open("/sys/class/gpio/gpio24/edge", O_WRONLY);
    write(gpio24fd, "rising", 6);
    close(gpio24fd);

    //Set gpio17 as output
    int gpio17fd = open("/sys/class/gpio/gpio17/direction", O_WRONLY);
    write(gpio17fd, "out", 3);
    close(gpio17fd);

    //preparing for reading of pin 24
    //gpio24fd = open("/sys/class/gpio/gpio24/value", O_RDONLY);

    gpio17fd = open("/sys/class/gpio/gpio17/value", O_WRONLY);
    
    struct pollfd pfd24;
    pfd24.fd = gpio24fd;
    pfd24.events = POLLPRI;

    while(1) {
        //Its faster to constantly reopen the file than use lseek.
        //lseek(gpio24fd, 0, SEEK_SET);
        
        gpio24fd = open("/sys/class/gpio/gpio24/value", O_RDONLY);
        int ret = poll(&pfd24, 1, 3000);
        char c;
        read(gpio24fd, &c, 1);
        close(gpio24fd);
        write(gpio17fd, &c, 1);
        
    }

    //close(gpio24fd);
    close(gpio17fd);

    fd = open("sys/class/gpio/unexport", O_WRONLY);
    write(fd, "24", 2);
    write(fd, "17", 2);
    close(fd);    

    return 0;
}