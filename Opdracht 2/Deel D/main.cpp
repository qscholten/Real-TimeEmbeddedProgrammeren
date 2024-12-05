//
//  main.cpp
//  sensoren
//
//  Created by John Vi on 4/26/19.
//  Copyright © 2019 John Vi. All rights reserved.
//

#include "Buffer.hpp"
#include "Sensor.hpp"
#include "verwerker.hpp"
#include <iostream>
#include <thread>
using namespace std;

int main(int argc, const char * argv[]) {

    srand((unsigned) time(0));
    Buffer opslag;
    Sensor s1(1,10,10,&opslag); //te genereren data tssen de 1 en de 9 aantal 100
    Sensor s2(100,110,10,&opslag);
    Sensor s3(1000,1010,10,&opslag);
    Verwerker vw1(&opslag,10);
    Verwerker vw2(&opslag,20);
    cout<<"start" <<endl;
    for(int n=0;n<5;n++) {
      thread t1(&Sensor::genereerdData,&s1);
      thread t2(&Verwerker::verwerkData,&vw1);

      thread t3(&Sensor::genereerdData,&s2);
      thread t4(&Sensor::genereerdData,&s3);
      thread t5(&Verwerker::verwerkData,&vw2);

      t1.join();
      t2.join();

      t3.join();
      t4.join();
      t5.join();
    }
    std::cout << "Hello, IoT "<<vw1.hetResultaat()<<endl;
    vector<int> ts1=vw1.deTussenstanden();

    for(auto i:ts1) {
        cout<<i <<" ";
    }
    cout<<endl;

    /*
    std::cout << "Hello, IoT "<<vw2.hetResultaat()<<endl;
    vector<int> ts2=vw2.deTussenstanden();

    for(auto i:ts2) {
        cout<<i <<" ";
    }
    cout<<endl;
    */

    return 0;
}
