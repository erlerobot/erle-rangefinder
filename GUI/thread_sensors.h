#ifndef THREAD_SENSORS_H
#define THREAD_SENSORS_H
#include <QtGui>

#include <iostream>
#include <sys/time.h>

#include "GUI/shared_memory.h"

#define cycle_gui 50 //miliseconds

class Thread_sensors: public QThread
{
public:
    Thread_sensors(Shared_memory* shared_memory);
protected:
    void run();
private:
    Shared_memory* shared_memory;
};

#endif // THREAD_SENSORS_H
