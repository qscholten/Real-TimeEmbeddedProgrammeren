#include <iostream>
#include <unistd.h>
#include <iostream>
#include "Timer.h"

using namespace std;

void doeIets(char c,unsigned int tijd,unsigned int aantal);
int main() {

  Timer tm1;
  tm1.Reset();
  pid_t pid;
  for (int i = 0; i < 3; i++) {
   pid = fork();
   if (pid == 0) {
      switch(i) {
         //child 1
         case(0):
            doeIets('p', 5, 8);
            break;
         //child 2
         case(1):
            doeIets('a', 10, 8);
            break;
         //child 3
         case(2):
            doeIets('b', 15, 8);
            break;
      }
      return 0;
   }
  }
  //parent 
  doeIets('c',20,8);

  tm1.Stop();
  cout<<tm1.deTijd()<<"  "<<tm1.deNtijd()<<endl;
   return 0;
}



void doeIets(char c,unsigned int tijd,unsigned int aantal) {
   
   for(int i=0;i<aantal;++i) {
     	cout<<c<<flush;       //flush leeg output buffer (print direct)
        //usleep(100000*tijd);
    }
        cout<<endl;
	
}
