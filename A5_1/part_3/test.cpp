#include<stdio.h>
#include<iostream>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h> 

class file_handler {
   public:
      int init(){
         if ((file = open("/dev/ttyS0", O_RDWR | O_NOCTTY ))<0){
            perror("UART: Failed to open the file.\n");
            return -1;
         }
         tcgetattr(file, &options);
         tcgetattr(file, &old_options);
         //options.c_cflag &= ~(CSIZE | PARENB | CSTOPB);
         //options.c_cflag |= CS8 | B57600 | CREAD | CLOCAL;
         //options.c_iflag |= IGNPAR;
         //options.c_lflag |= ICANON; // &= ~ICANON;
         cfmakeraw(&options);
         if(cfsetispeed(&options, B57600) < 0|| cfsetospeed(&options, B57600) < 0) 
         {
             perror("Speed could not be set");
         }
         //options.c_lflag |=  ECHO;
         tcflush(file, TCIFLUSH);            // discard file information
         tcsetattr(file, TCSANOW, &options); // changes occur immmediately
         fhclose();
         return 0;
      }

      int fhopen()
      {
         if ((file = open("/dev/ttyS0", O_RDWR | O_NOCTTY))<0){
            perror("UART: Failed to open the file.\n");
            return -1;
         }
         tcflush(file, TCIFLUSH);            // discard file information
         tcsetattr(file, TCSANOW, &options); // changes occur immmediately
         return file;
      }
      
      void fhclose()
      {
         close(file);
      }

      void reset_attr()
      {
          fhopen();
          tcflush(file, TCIFLUSH);            // discard file information
          tcsetattr(file, TCSANOW, &old_options); // changes occur immmediately
          fhclose();
      }

   private:
      int file;
      struct termios options;
      struct termios old_options;
};

void stdin_set(int cmd)
{
   struct termios t;
   tcgetattr(STDIN_FILENO, &t);
   if (cmd==1)
   {
      t.c_lflag &= ~ICANON;
   }
   else
   {
      t.c_lflag |= ICANON;
   }
   tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

int main(){
   int file;
   int count;
   unsigned char send[3] = "a\n";
   unsigned char receive[2]; //declare a buffer for receiving data
   file_handler FH;
   FH.init();

   stdin_set(1);
   

   for (int k = 0; k < 5;k++)
   { 

      file = FH.fhopen();
      send[0] = getchar();

      if ((count = write(file, &send, 1))<0){         // 
         perror("Failed to write to the output\n");
         return -1;
      }

      usleep(200000);             // give the remote machine a chance to respond


      if ((count = read(file, (void*)receive, 1))<0){   //receive data
         perror("Failed to read from the input\n");
         return -1;
      }

      if (count==0) printf("There was no data available to read!\n");
      else printf("%c",receive[0]);
      //else printf("The following was read in [%d]: %s\n",count,receive);
      FH.fhclose();
      //close(file);
   }
   printf("\n");
   stdin_set(0);
   FH.reset_attr();
   return 0;
}