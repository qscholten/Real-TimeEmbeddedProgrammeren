#include <iostream>
#include <unistd.h>
#include <iostream>
#include <thread>
#include "Timer.h"
#include "Werker.h"

using namespace std;

void doeIets(char c,unsigned int tijd,unsigned int aantal);
int main() {

  Timer tm1;
  tm1.Reset();

  Werker w;

  thread t1(&Werker::plaatsIets, &w, 'p', 8);
  thread t2(&Werker::plaatsIets, &w, 'a', 8);
  thread t3(&Werker::plaatsIets, &w, 'b', 8);
  thread t4(&Werker::haalIets, &w, 24);

  t1.join();
  t2.join();
  t3.join();
  t4.join();

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