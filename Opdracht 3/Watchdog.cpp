#include "mbed.h"
#include <atomic>

// main() runs in its own thread in the OS
atomic<bool> lichttime;
atomic<bool> knoptime;


void lichtsensor() {
    AnalogIn ls(A3);
    DigitalOut led(D6);
    while(1) {
        lichttime = true;
        int voltage = ls.read_u16();
        if (voltage < 30000) {
            led.write(1);
        }
        else {
            led.write(0);
        }
        ThisThread::sleep_for(500ms);
        printf("De lichtsensor %d\n", voltage);
    }
}

void knopsensor() {
    DigitalIn knop(D4);
    DigitalOut led(D6);
    while(1) {
        knoptime = true;
        int knopstatus = knop;
        if (knop) {
            led =! led;
            ThisThread::sleep_for(500ms);
        }
    }
}

int main() {
    lichttime = false;
    knoptime = false;
    int stop = 0;
    Thread t1,t2;
    printf("Welkom\n");
    t1.start(lichtsensor);
    t2.start(knopsensor);
    Watchdog &watchdog = Watchdog::get_instance();
    watchdog.start(5000);
    while (true) {
        if (knoptime && lichttime) {
            watchdog.kick();
            stop++;
            ThisThread::sleep_for(500ms);
            printf("Watchdog kicked. Teller: %d\n", stop);
            knoptime = false;
            lichttime = false;
        }
        if (stop == 10) {
            t1.terminate();
        }
    }
}