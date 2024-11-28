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
    Sensor s1(1,10,1000,&opslag); //te genereren data tssen de 1 en de 9 aantal 100
    Verwerker vw1(&opslag,1000);
    cout<<"start" <<endl;
    for(int n=0;n<30;n++) {
      thread t1(&Sensor::genereerdData,&s1);
      thread t2(&Verwerker::verwerkData,&vw1);

      t1.join();
      t2.join();
    }
    std::cout << "Hello, IoT "<<vw1.hetResultaat()<<endl;
    vector<int> ts=vw1.deTussenstanden();

    for(auto i:ts) {
        cout<<i <<" ";
    }
    cout<<endl;

    return 0;
}
