#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

pthread_t send_to_driverObj;

void *send_to_driver(void *param)
{
    int fd;
    char message[] = "message to send";
    int msg_size = sizeof(message);
    int ret;

    while (1)
    {
        sleep(1); //sleep for one sec.
        
        fd = open("/dev/mydev_joi", O_RDWR);
        ret = write(fd, message, msg_size);
        ret = close(fd);
    }
}

int main(void)
{
    pthread_create(&send_to_driverObj, NULL, send_to_driver, NULL);
    pthread_join(send_to_driverObj, NULL);
    
    return 0;
}