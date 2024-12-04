
#include "Buffer.hpp"
#include "Sensor.hpp"
#include <bits/this_thread_sleep.h>

void Sensor::genereerdData() {
    stand = begin;
    int h=hoeveel;
    while(h--) {
       int x=stand++;
       if (stand == eind) stand=begin;
       unsigned int rd = (rand() % 2000) + 1;
        std::this_thread::sleep_for(std::chrono::nanoseconds(rd));
       buf -> zetInBuf(x);
    }
}
void Sensor::aantalTeGenererenData(int d) {
    hoeveel=d;
}

Sensor::Sensor(int b,int e,int h,Buffer* bf): begin(b),eind(e),hoeveel(h),stand(0),buf(bf) {
}

