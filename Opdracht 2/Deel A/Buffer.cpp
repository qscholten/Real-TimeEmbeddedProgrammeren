//
//  Buffer.cpp
//  sensoren
//
//  Created by John Vi on 4/26/19.
//  Copyright © 2019 John Vi. All rights reserved.
//

#include "Buffer.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;
void Buffer::zetInBuf(int d) {
    //usleep(10);
    while (teller == GROOTTE);
    opslag[in++]=d;
    in %=GROOTTE;
    m1.lock();
    teller++;
    m1.unlock();
}

int Buffer::haalUitBuf() {
    int waarde;
    //usleep(10);
    while(teller==0);
    waarde=opslag[out++];
    out %=GROOTTE;
    m1.lock();
    teller--;
    m1.unlock();
    return waarde;
}

