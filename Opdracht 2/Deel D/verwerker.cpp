

#include "verwerker.hpp"
#include "Buffer.hpp"
#include <iostream>
#include <bits/this_thread_sleep.h>
using namespace std;

vector<int> Verwerker::tussenstanden = vector<int>();
atomic<int> Verwerker::result = 0;

Verwerker::Verwerker(Buffer* b,int h): buf(b), status(true),hoeveel(h) {
    
}

void Verwerker::zetUit() {
    status=false;
}

int Verwerker::hetResultaat()const {
    return result;
}
void Verwerker::aantalTeLezenData(int d) {
    hoeveel=d;
}
vector<int> Verwerker::deTussenstanden() const {
    return tussenstanden;
}
void Verwerker::verwerkData() {
    status=true;
    int h=hoeveel;
    while(h-- && status) {
        int temp;
        temp=buf->haalUitBuf();
        unsigned int rd = (rand() % 2000) + 1;
        std::this_thread::sleep_for(std::chrono::nanoseconds(rd));
        result += temp;
    }
    tussenstanden.push_back(result);
}
