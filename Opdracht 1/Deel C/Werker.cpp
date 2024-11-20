#include "Werker.h"
#include <iostream>
queue<char> Werker::buffer;


Werker::Werker() {
}

void Werker::plaatsIets(char c, int aantal) {
    for (int i = 0; i < aantal; i++) {
        buffer.push(c);
    }
}

void Werker::haalIets(int aantal) {
    for (int i = 0; i< aantal; i++) {
        while (buffer.empty()){}
        char c = buffer.front();
        buffer.pop();
        cout<<c<<endl;
    }
}