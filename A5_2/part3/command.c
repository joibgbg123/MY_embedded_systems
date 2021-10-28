/* Simple send and receive C example (line-mode terminal program with local echo) 
* for communicating with the Arduino using /dev/ttyS0. */

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include<stdint.h>
#include<stdlib.h>

int main(int argc, char *argv[]){
   int file, count;
   if(argc!=3){
       printf("Invalid number of arguments, exiting!\n");
       return -2;
   }

   size_t MSG_LEN = 2;
   uint8_t msg[2];
   // argv[0] is simply the name of the process, remember that.
   msg[0] = atoi(argv[1]);
   msg[1] = atoi(argv[2]);

   if ((file = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY))<0){
      perror("UART: Failed to open the file.\n");
      return -1;
   }

   struct termios options;
   struct termios old_options;

   tcgetattr(file, &options);
   tcgetattr(file, &old_options);

   //options.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
   //options.c_iflag = IGNPAR | ICRNL;
   cfmakeraw(&options);
   if(cfsetispeed(&options, B115200) < 0|| cfsetospeed(&options, B115200) < 0) 
   {// ... speed could not be set ...
   }
   options.c_cflag &= ~(PARENB | CSTOPB);
   //options.c_cc[VTIME]=0; /* no timer */
   options.c_cc[VMIN]=MSG_LEN;  /* minimum number of characters */
   tcflush(file, TCIFLUSH);
   tcsetattr(file, TCSANOW, &options);

   // send the string plus the null character
   if ((count = write(file, &msg, MSG_LEN)<0)){
      perror("Failed to write to the output\n");
      return -1;
   }

   usleep(100000);

   unsigned char receive[MSG_LEN];

   if ((count = read(file, (void*)receive, MSG_LEN))<0){
      perror("Failed to read from the input\n");
      return -1;
   }

   if (count==0) printf("There was no data available to read!\n");
   else {
      if (receive[0]==msg[0])
      {
         printf("The sent command was received back\n");
         printf("%i\n",receive[1]);
         switch (receive[1])
         {
            case 0xAA:
               printf("Everything is normal.\n");
               break;

            case 0xF0:
               printf("Error 1, value out of range\n");
               break;
            
            case 0xFF:
               printf("Arduino received unknown command\n");
               break;
            
            default:
               printf("Unknown message from Arduino\n");
         }
      }
      else
      {
         printf("The sent command was not received back\n");
      }
   }

   tcflush(file, TCIFLUSH);
   tcsetattr(file, TCSANOW, &old_options);
   close(file);
   return 0;
}
