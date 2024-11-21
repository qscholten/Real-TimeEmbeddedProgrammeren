#ifndef TIMER_H
#define TIMER_H


#include <chrono>
#include <ostream>
           typedef std::chrono::high_resolution_clock high_resolution_clock;
            typedef std::chrono::milliseconds milliseconds;

class Timer {
 
        public:
            Timer();

            void Reset();
            void Stop();
            unsigned long deTijd() const;
            unsigned long deNtijd() const;
            milliseconds Elapsed() const;

 
        private:
            high_resolution_clock::time_point _start;
            high_resolution_clock::time_point _stop;
};

#endif // TIMER_H