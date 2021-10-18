#include <stdio.h>
#include <iostream>
#include <cstdint>
#include <unistd.h>
#include <fcntl.h>
#include "pi_controller.h"
#include <string.h>
#include <time.h>

constexpr double error_threshold = 0.05;
const uint16_t period_ms = 10;
const double Kp = 0.7875, Ki = 4.46; //If K_p is = 2.0 and reference speed to 1000, the output oscillates
const int MAX_SPEED = 1300; 

static uint32_t duty_cycle = 80; //percentage
static int reference = -1000;
static int duty_ns = (100000/100)*duty_cycle;
static long int pps;
static char message[5];

pi_controller controller(Kp, Ki, -1*MAX_SPEED, MAX_SPEED, static_cast<double>(period_ms/1000.0));

int main()
{
    // Export the pwm pins
    int fd = open("/sys/class/pwm/pwmchip0/export", O_WRONLY);
    write(fd, "0", 1);
    write(fd, "1", 1);
    close(fd);

    // set the period and duty cycle of the two pwm channel
    // They are two because to go forward or backwards
    fd = open("/sys/class/pwm/pwmchip0/pwm0/period", O_WRONLY);
    write(fd, "1000000" , 7);
    close(fd);

    fd = open("/sys/class/pwm/pwmchip0/pwm1/period", O_WRONLY);
    write(fd, "1000000", 7);
    close(fd);

    fd = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_WRONLY);
    write(fd, "500000" , 6);
    close(fd);

    fd = open("/sys/class/pwm/pwmchip0/pwm1/duty_cycle", O_WRONLY);
    write(fd, "500000", 6);
    close(fd);


    fd = open("/sys/class/pwm/pwmchip0/pwm0/enable", O_WRONLY);
    write(fd, "1", 1);
    close(fd);

    fd = open("/sys/class/pwm/pwmchip0/pwm1/enable", O_WRONLY);
    write(fd, "1", 1);
    close(fd);

    int counter = 0;

    struct timespec time;
    time.tv_sec = 0;
    time.tv_nsec = 100*1000000L;

    while (counter < 60*10)
    {
        // Open the device driver file that will return current pps
        // of the encoder.
        int fd_pulse = open("/dev/encoder",O_RDONLY);
        read(fd_pulse, (char *) &pps, 4); 
        
        std::cout << "pps: " << pps << std::endl;
        
        close(fd_pulse);

        double new_val = controller.update(reference, pps); 
        unsigned long int new_duty = static_cast<unsigned long int>(abs(new_val)*1000*1000/1350);
        sprintf(message, "%lu", new_duty);
        //std::cout << "New_duty: " << new_duty << std::endl;
        //std::cout << "New_val: " << new_val << std::endl << std::endl;


        if (new_val < 0) {

        fd = open("/sys/class/pwm/pwmchip0/pwm1/duty_cycle", O_WRONLY);
        write(fd, message, 6);
        close(fd);

        fd = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_WRONLY);
        write(fd, "0", 1);
        close(fd);

        } else {

        fd = open("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", O_WRONLY);
        write(fd, message, 6);
        close(fd);

        fd = open("/sys/class/pwm/pwmchip0/pwm1/duty_cycle", O_WRONLY);
        write(fd, "0", 1);
        close(fd);

        }
        counter++;
        
        nanosleep(&time, NULL);
    }
    

    fd = open("/sys/class/pwm/pwmchip0/pwm0/enable", O_WRONLY);
    write(fd, "0", 1);
    close(fd);

    fd = open("/sys/class/pwm/pwmchip0/pwm1/enable", O_WRONLY);
    write(fd, "0", 1);
    close(fd);


    fd = open("/sys/class/pwm/pwmchip0/unexport", O_WRONLY);
    write(fd, "0", 1);
    write(fd, "1", 1);
    close(fd);
    return 0;
}