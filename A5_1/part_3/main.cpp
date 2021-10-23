#include<stdio.h>
#include<iostream>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>   // using the termios.h library
#include<pthread.h>
#include<semaphore.h>

using namespace std;

sem_t SEM;

class file_handler {
   public:
      int init(){
         if ((file = open("/dev/ttyS0", O_RDWR | O_NOCTTY))<0){
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

file_handler FH;

void *write_chars_Thread(void* param)
{
   int file, count;
   unsigned char send[3] = "a\n";
   while (1)
   {
      //sem_wait(&SEM);
      send[0] = getchar();
      file = FH.fhopen();

      if ((count = write(file, &send, 1))<0){         // 
         perror("Failed to write to the output\n");
         //return -1;
      }
      FH.fhclose();
      usleep(100000); // Let the reciver get a chance to recieve the package.
      sem_post(&SEM);
      if (send[0]=='z')break;
   }
   return 0;
}

void *read_chars_Thread(void* param)
{
   int file, count;
   unsigned char receive[3]; //declare a buffer for receiving data
   while (1)
   {
      sem_wait(&SEM);
      file = FH.fhopen();

      if ((count = read(file, (void*)receive, 1))<0){   //receive data
         perror("Failed to read from the input\n");
         //return -1;
      }
      if (count==0) printf("There was no data available to read!\n");
      else printf("%c",receive[0]);

      FH.fhclose();
      if (receive[0]=='z')break;
      
   }
   return 0;
}

// Task context
pthread_t writeTaskObj;
pthread_t readTaskObj;
int main(){
   if (FH.init()<0)
   {
      perror("Failed initalize the file-handler\n");
      return -1;
   }

   if (sem_init(&SEM, 0, 0)<0)
   {
      perror("The Semaphore couldnt be initalized.\n");
      return -1;
   }

   stdin_set(1);

   pthread_create(&writeTaskObj, NULL, write_chars_Thread, NULL);
   pthread_create(&readTaskObj, NULL, read_chars_Thread, NULL);
   /* Allow the tasks to run. */
   pthread_join(writeTaskObj, NULL);
   pthread_join(readTaskObj, NULL);

   printf("\n");
   stdin_set(0);
   sem_destroy(&SEM);
   FH.reset_attr();
   return 0;
}
