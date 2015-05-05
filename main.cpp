#include "LRM30_serial.h"

/*
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/ahcorde/serial/serial/build/devel/lib

g++ main2.cpp LRM30_serial.cpp -o main2 -I/home/ahcorde/serial/serial/include -L/home/ahcorde/serial/serial/build/devel/lib -lserial
*/

int main(int argc, char* argv[])
{
	LRM30_serial lrm30(9600, "/dev/ttyACM0");
	lrm30.connect();

	lrm30.laserON();

	std::cout << "Single shot value: " << lrm30.singleshot() << std::endl; 

	std::cout << "continuous shot value: " << lrm30.continuousshot() << std::endl; 

	int cont = 0;

	while(cont < 10){
		float measure  = lrm30.getMeasure();
		std::cout << cont <<": ";
		if(measure != -1)
			std::cout << "read: " << measure << std::endl;
		else
			std::cout << " -1 " << std::endl;
		cont++;
	}

	lrm30.laserOFF();

}