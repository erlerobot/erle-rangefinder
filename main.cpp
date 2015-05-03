#include <iostream>

#include "stdio.h"
#include "string.h"
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{

	std::cout << "Test sensor Bosch" << std::endl;

	char *sComPort = "/dev/ttyACM0";
	char reply[10];

	int iOut, fd; // file descriptor

	fd = open(sComPort, O_RDWR | O_NOCTTY | O_NONBLOCK);

	if(fd>0){
		std::cout << "Open successfully" << std::endl;

		iOut = write(fd, "IDN?\n", 5);
		read(fd, reply, 10);
		printf("-> %hhX\n", reply);

		std::cout << "Reply: " << std::hex << reply << std::endl;

		close(fd);
	}else{
		std::cout << "failed to open device" << std::endl;
	}

	return 0;
	
}