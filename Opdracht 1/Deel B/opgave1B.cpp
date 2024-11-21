#include <iostream>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sys/wait.h>
#include "Timer.h"

using namespace std;

void doeIets(char c,unsigned int tijd,unsigned int aantal);
int main() {

  Timer tm1;
  tm1.Reset();
  pid_t pid;

  int fd[2];
  char buffer[128];
  memset(buffer, 0, sizeof(buffer));
  if (pipe(fd) == -1 ) {
   perror("pipe");
   return 1;
  }

  for (int i = 0; i < 3; i++) {
   pid = fork();
   if (pid == 0) {
      close(fd[0]);
      switch(i) {
         //child 1
         case(0):
            for (int j = 0; j < 8; j++) {
               write(fd[1], "p", 1);
               usleep(10);
            }
            break;
         //child 2
         case(1):
            for (int j = 0; j < 8; j++) {
               write(fd[1], "a", 1);
               usleep(10);
            }
            break;
         //child 3
         case(2):
            for (int j = 0; j < 8; j++) {
               write(fd[1], "b", 1);
               usleep(10);
            }
            break;
      }
      close(fd[1]);
      return 0;
   }
  }
  //parent
  close(fd[1]);
  while (wait(NULL) > 0);
  read(fd[0], buffer, sizeof(buffer));
  close(fd[0]);
  cout << buffer << endl;

  tm1.Stop();
  cout<<tm1.deTijd()<<"  "<<tm1.deNtijd()<<endl;
  return 0;
}



void doeIets(char c,unsigned int tijd,unsigned int aantal) {
   
   for(int i=0;i<aantal;++i) {
     	cout<<c<<flush;       //flush leeg output buffer (print direct)
        usleep(100000*tijd);
    }
        cout<<endl;
	
}
