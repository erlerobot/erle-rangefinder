#include "shared_memory.h"

Shared_memory::Shared_memory()
{
    if (pthread_mutex_init(&mutex, NULL) != 0){
        std::cout << "mutex init failed" << std::endl;
    }

    lrm30 = new LRM30_serial(9600, "/dev/ttyACM0");
    lrm30->connect();
}

void Shared_memory::laserON()
{
    pthread_mutex_lock( &mutex );
    lrm30->laserON();
    pthread_mutex_unlock( &mutex );
}

void Shared_memory::laserOFF()
{
    pthread_mutex_lock( &mutex );
    lrm30->laserOFF();
    pthread_mutex_unlock( &mutex );
}
float Shared_memory::getMeasure()
{
    pthread_mutex_lock( &mutex );
    float result  = measure;
    pthread_mutex_unlock( &mutex );
    return result;
}

void Shared_memory::read_lrm30()
{
    pthread_mutex_lock( &mutex );
    measure  = lrm30->getMeasure();
    pthread_mutex_unlock( &mutex );
}

/*
void Shared_memory::setTune(int tune)
{
    pthread_mutex_lock( &mutex );
    this->tune = tune;
    pthread_mutex_unlock( &mutex );

}

int Shared_memory::getTune()
{
    int result;
    pthread_mutex_lock( &mutex );

    pthread_mutex_unlock( &mutex );
    return result;
}
*/
