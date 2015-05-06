#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <pthread.h>
#include <iostream>

#include "../LRM30_serial.h"

class Shared_memory
{
public:
    Shared_memory();
    void laserOFF();
    void laserON();
    float getMeasure();
    void read_lrm30();

private:
    pthread_mutex_t mutex;
    LRM30_serial* lrm30;
    float measure;
};

#endif // SHARED_MEMORY_
