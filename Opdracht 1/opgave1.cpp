#include <iostream>
#include <unistd.h>
#include <iostream>
#include "Timer.h"

using namespace std;

void doeIets(char c,unsigned int tijd,unsigned int aantal);
int main() {

  Timer tm1;
  tm1.Reset();
   doeIets('p',5,8);   //print a wacht 0.5 sec doe het 8 keer
   doeIets('a',10,8);
   doeIets('b',15,8);
   doeIets('c',20,8);
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
