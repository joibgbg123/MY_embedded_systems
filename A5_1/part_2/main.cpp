#include<stdio.h>
#include<iostream>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>   // using the termios.h library

using namespace std;

int main(){
   int file, count;
   unsigned char receive[50]; //declare a buffer for receiving data
   for (int k = 0; k < 10;k++)
   { 
      if ((file = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY))<0){
         perror("UART: Failed to open the file.\n");
         return -1;
      }

      struct termios options;       // the termios structure is vital
      tcgetattr(file, &options);    // sets the parameters for the file

      // Set up the communications options:
      // 115200 baud, 8-bit, enable receiver, no modem control lines
      //options.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
      //options.c_cflag = B57600;
      options.c_cflag &= ~(CSIZE | PARENB | CSTOPB);
      options.c_cflag |= CS8 | B57600 | CREAD | CLOCAL;
      options.c_iflag |= IGNPAR;
      tcflush(file, TCIFLUSH);            // discard file information
      tcsetattr(file, TCSANOW, &options); // changes occur immmediately

         
      string transmit_Str = "";
      
      getline(cin,transmit_Str);
      transmit_Str.append("\n");

      usleep(500000); 
      if ((count = write(file, transmit_Str.c_str(), transmit_Str.length()))<0){         // 
         perror("Failed to write to the output\n");
         return -1;
      }

      usleep(200000);             // give the remote machine a chance to respond

      // unsigned char receive[100]; //declare a buffer for receiving data

      if ((count = read(file, (void*)receive, 100))<0){   //receive data
         perror("Failed to read from the input\n");
         return -1;
      }

      if (count==0) printf("There was no data available to read!\n");
      else printf("%s",receive);
      // else printf("The following was read in [%d]: %s\n",count,receive);
      for(int k1 = 0;k1 < (count+1);k1++)
      {
         receive[k1] = 0;
      }
      close(file);
   }
   
   
   return 0;
}