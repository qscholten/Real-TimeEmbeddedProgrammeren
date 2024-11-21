#include <iostream>
#include <unistd.h>
#include <iostream>
#include <thread>
#include "Timer.h"

using namespace std;

void doeIets(char c,unsigned int tijd,unsigned int aantal);
int main() {

  Timer tm1;
  tm1.Reset();

  thread t1(doeIets, 'p', 5, 8);
  thread t2(doeIets, 'a', 10, 8);
  thread t3(doeIets, 'b', 15, 8);
  thread t4(doeIets, 'c', 20, 8);

  //t1.join();
  //t2.join();
  //t3.join();
  //t4.join();
  usleep(10000000);

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