/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include <arm_math.h>
#include "mbed.h"
#include "dsp.h"
#include <atomic>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <mutex>

// Blinking rate in milliseconds
#define KNIPPER_Tijd   1000ms
#define M_PI 3.14159265359
#define FFT_SIZE 256  
using ThisThread::sleep_for;

MemoryPool<float, 100> mp;


AnalogIn   ain(A0);

Thread thread,thrSin,thrAnalog, threadsignaal, threadfft, threadinverse, threaddisplay;
float buf[256];
int counter = 0;
Mutex mcount;
atomic<int> grootbuf;
Semaphore sembuf(256);

//float buf1[FFT_SIZE * 2];
//float buf2[FFT_SIZE * 2];
float filteruitkomst[FFT_SIZE];


float samples[FFT_SIZE * 2];
float samples2[FFT_SIZE * 2];
float timeDomain[FFT_SIZE];
float signaal1rij[FFT_SIZE];
float signaal2rij[FFT_SIZE];

Queue<float, 512> buffer1;
MemoryPool<float, 512> mempool1;
Queue<float, 512> buffer2;
MemoryPool<float, 512> mempool2;
Queue<float, 512> signaalprint1;
MemoryPool<float, 512> mempool3;
Queue<float, 512> signaalprint2;
MemoryPool<float, 512> mempool4;
Queue<float, 512> optelprintbuf;
MemoryPool<float, 512> mempooloptel;
Queue<float, 512> freqdomain1buf;
MemoryPool<float, 512> mempoolfreq1;

float signaal1buf[FFT_SIZE];
float signaal2buf[FFT_SIZE];
float samplesfft[FFT_SIZE * 2];
float samplesfftuitkomst[FFT_SIZE * 2];
float samplesinversefftinput[FFT_SIZE * 2];
float freqDomain[FFT_SIZE];
float freqDomain2[FFT_SIZE];

void signaal() {
    const unsigned int gr = 255;
    unsigned i = 0;
    while (true) {
        float k = i * (2 * M_PI) / gr;
        float *signaal1 = mempool3.try_alloc_for(Kernel::wait_for_u32_forever);
        *signaal1 = sin(4 * k);
        signaalprint1.try_put_for(Kernel::wait_for_u32_forever, signaal1);
        float *signaal2 = mempool4.try_alloc_for(Kernel::wait_for_u32_forever);
        *signaal2 = sin((ain.read_u16() / 2000.0) * k);
        signaalprint2.try_put_for(Kernel::wait_for_u32_forever, signaal2);
        float *optel = mempool1.try_alloc_for(Kernel::wait_for_u32_forever);
        *optel = *signaal1 + *signaal2;
        buffer1.try_put_for(Kernel::wait_for_u32_forever, optel);
        float *optelprint = mempooloptel.try_alloc_for(Kernel::wait_for_u32_forever);
        *optelprint = *optel;
        optelprintbuf.try_put_for(Kernel::wait_for_u32_forever, optelprint);
        //printf("%.3f\t%.3f\t%.3f\n", 3 + *signaal1, 5 + *signaal2, *optel);
        i++;
        if (k > 2 * M_PI) {
            k = 0;
            i = 0;
        }
    }
}

void FFT() {
    arm_cfft_radix4_instance_f32 fft_inst;  
    arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 0, 1);
    while (true) {
        for (int j = 0; j < FFT_SIZE; j++) {
            float *optel;
            buffer1.try_get_for(Kernel::wait_for_u32_forever, &optel);
            samplesfft[2 * j] = *optel;
            samplesfft[2 * j + 1] = 0.0;
            mempool1.free(optel);
        }
        arm_cfft_radix4_f32(&fft_inst, samplesfft);
        arm_cmplx_mag_f32(samplesfft, freqDomain, FFT_SIZE);
        for (int j = 0; j < FFT_SIZE * 2; j++) {
            float *fftuikomst = mempool2.try_alloc_for(Kernel::wait_for_u32_forever);
            *fftuikomst = samplesfft[j];
            buffer2.try_put_for(Kernel::wait_for_u32_forever, fftuikomst);
        }
    }
}

void InverseFFT() {
    arm_cfft_radix4_instance_f32 fft_inst;
    arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 1, 1);   //initialisatie voor de inverse FFT
    while (true) {
        for (int m = 0; m < FFT_SIZE * 2; m++) {
            float *fftuikomst;
            buffer2.try_get_for(Kernel::wait_for_u32_forever, &fftuikomst);
            samplesfftuitkomst[m] = *fftuikomst;
            mempool2.free(fftuikomst);
        }
        for (int i=0; i<FFT_SIZE; i++) {
            if(i > 0 && i < 10 && (samplesfftuitkomst[i] > 1 || samplesfftuitkomst[i] < -1  ) ) {
                samplesinversefftinput[i]=samplesinversefftinput[i-1];
            }
            else {
                samplesinversefftinput[i]=samplesfftuitkomst[i];
            }
        }
        arm_cmplx_mag_f32(samplesinversefftinput, freqDomain2, FFT_SIZE);     //absolute waarde van het freq domein na aanpassingen
        arm_cfft_radix4_f32(&fft_inst, samplesinversefftinput);
        for (int i = 1; i < 127; i++) { // print only first half, the others are imaginary
            //printf("%.3f\t%.3f\t%.3f\t", 3 + signaal1rij[i], 5 + signaal2rij[i],buf[i]);
            //printf("%f\n", freqDomain[i]);
            //printf("%f\t%f\t",freqDomain[i],freqDomain2[i]);
            //printf("%f\n",samplesinversefftinput[2 * i]);
            //printf("%f\n", freqDomain2[i]);
        }
    }
}

void printer() {
    float signal1temp[127];
    float signal2temp[127];
    float opteltemp[127];
    float freqdom1temp[127];
    uint16_t l;
    while(true) {
        //printf("Hoi");
        float *signaal1output;
        signaalprint1.try_get_for(Kernel::wait_for_u32_forever, &signaal1output);
        signal1temp[l] = *signaal1output;
        mempool3.free(signaal1output);

        float *signaal2output;
        signaalprint2.try_get_for(Kernel::wait_for_u32_forever, &signaal2output);
        signal2temp[l] = *signaal2output;
        mempool4.free(signaal2output);

        float *opteloutput;
        optelprintbuf.try_get_for(Kernel::wait_for_u32_forever, &opteloutput);
        opteltemp[l] = *opteloutput;
        mempooloptel.free(opteloutput);

        printf("%f\t%f\t%f\t", 3 + signal1temp[l], 5 + signal2temp[l], opteltemp[l]);
        printf("%f\t%f\t", freqDomain[l], freqDomain2[l]);
        printf("%f\n", samplesinversefftinput[2 * l]);
        l++;
        if (l == 127) {
            l = 0;
        }
    }
    //printf("%.3f\t%.3f\t%.3f\t", 3 + signaal1rij[i], 5 + signaal2rij[i],buf[i]);
    //printf("%f\n", freqDomain[i]);
    //printf("%f\t%f\t",freqDomain[i],freqDomain2[i]);
    //printf("%f\n",samplesinversefftinput[2 * i]);
}

void sinus()
{
    const unsigned int gr=255;
    unsigned i=0;
    while(true) {

        float k= i*(2*M_PI)/gr;
   
        float signaal1 = sin(4 *k);
        signaal1rij[i]=signaal1;
        float signaal2 = sin((ain.read_u16()/2000.0) *k);


        signaal2rij[i]=signaal2;
        float optel = signaal1 +signaal2;
       
        buf[i]=optel;
        samples[2*i]=optel;
        samples[2*i+1] = 0.0;  
        timeDomain[i] = samples[2*i];


        i++;
        if(k >2*M_PI) {
            k=0;i=0;
            arm_cfft_radix4_instance_f32 fft_inst;  
            arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 0, 1);
            arm_cfft_radix4_f32(&fft_inst, samples);
            arm_cmplx_mag_f32(samples, freqDomain, FFT_SIZE);
     

            for (int i=0; i<FFT_SIZE; i++) {
                 if( i < 10 && (samples[i] > 1 || samples[i] < -1  ) ) {
                     samples2[i]=samples2[i-1];
                 }
                 else {
                     samples2[i]=samples[i];
                }
            }

            arm_cmplx_mag_f32(samples2, freqDomain2, FFT_SIZE);     //absolute waarde van het freq domein na aanpassingen
            arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 1, 1);   //initialisatie voor de inverse FFT
            arm_cfft_radix4_f32(&fft_inst, samples2);
            printf("einde ronde\n");
            for (int i = 1; i < 127; i++) { // print only first half, the others are imaginary
                printf("%.3f\t%.3f\t%.3f\t", 3 + signaal1rij[i], 5 + signaal2rij[i],buf[i]);
                printf("%f\t%f\t",freqDomain[i],freqDomain2[i]);
                printf("%f\n",samples2[2 * i]);
            }
        
       }  

    }
}    

void analog()
{
printf("in thread analog\n");
while(true) {
printf("%d\n",ain.read_u16());
sleep_for(100ms);
}
}



int main()
{
    
    DigitalOut led1(D6);
    printf("Hoi2\n");
    grootbuf = 0;
    //thread.start(sinus);
    threadsignaal.start(signaal);
    threadfft.start(FFT);
    threadinverse.start(InverseFFT);
    threaddisplay.start(printer);

    while (true) {
        led1 = !led1;
        ThisThread::sleep_for(KNIPPER_Tijd);
    }
}
