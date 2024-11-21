#ifndef WERKER
#define WERKER

#include <queue>

using namespace std;

class Werker {
    public:
        Werker();
        void plaatsIets(char c, int aantal);
        void haalIets(int aantal);
    private:
        static queue<char> buffer;
};