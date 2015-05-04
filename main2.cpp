#include <string>
#include <iostream>
#include <bitset>
#include <unistd.h>

#include "serial/serial.h"

/*
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/ahcorde/serial/serial/build/devel/lib

g++ main2.cpp -o main2 -I/home/ahcorde/serial/serial/include -L/home/ahcorde/serial/serial/build/devel/lib -lserial
*/

int main(int argc, char* argv[])
{
	unsigned long baud = 9600;
	std::string port=std::string("/dev/ttyACM0");
	serial::Serial my_serial(port, 
							 baud,
							 serial::Timeout::simpleTimeout(1000),
							 serial::eightbits,
							 serial::parity_none,
							 serial::stopbits_one, 
							 serial::flowcontrol_none);

	std::cout << "Is the serial port open?";
	if(my_serial.isOpen())
		std::cout << " Yes." << std::endl;
	else
		std::cout << " No." << std::endl;

	  // Get the Test string
	int count = 0;
	std::string test_string;
	test_string = "C0401096";

	std::vector<uint8_t> v1;
	v1.push_back(0xC0);
	v1.push_back(0x41);
	v1.push_back(0x0);
	v1.push_back(0x96);

	// Test the timeout, there should be 1 second between prints
	std::cout << "Timeout == 1000ms, asking for 1 more byte than written." << std::endl;
	while (count < 1) {
		size_t bytes_wrote = my_serial.write(v1);

		//std::string result = my_serial.read(v1.size());
		std::vector<uint8_t> v;
		int n  = my_serial.read(v, 5);

		std::cout << "n: " << n << std::endl;
		for(int i = 0; i < n;i ++){
			printf(" - %.2X\n", v[i]);
		}
		std::cout << "Iteration: " << count << ", Bytes written: ";
		std::cout << bytes_wrote << ", Bytes read: ";
		std::cout << n  << std::endl;

		count += 1;
	}

	std::vector<uint8_t> v2;
	v2.push_back(0xC0);
	v2.push_back(0x40);
	v2.push_back(0x01);
	v2.push_back(0x00);
	v2.push_back(0xFA);

	test_string = "C04010FA";
	count = 0;
	std::cout << "Timeout == 1000ms, asking for 1 more byte than written." << std::endl;
	while (count < 1) {
		size_t bytes_wrote = my_serial.write(v2);

//		std::vector<uint8_t> v;
//		std::string result = my_serial.read(v2, );

		std::vector<uint8_t> v;
		int n  = my_serial.read(v, 10);
		std::cout << "n: " << n << std::endl;
		for(int i = 0; i < n;i ++){
			//std::cout << "v[" << i <<"]: "<< std::hex << v[i];
			printf(" - %.2X - ", v[i]);
			std::bitset<8> x(v[i]);
			std::cout << x << std::endl;
		}

		uint32_t b = 0x0000310C;

		printf("value: %d\n", b);

		//std::string result = my_serial.read(v2.size());

		std::cout << "Iteration: " << count << ", Bytes written: ";
		std::cout << bytes_wrote << ", Bytes read: ";
		std::cout << n << std::endl;

		count += 1;
	}

	std::vector<uint8_t> v3;
	v3.push_back(0xC0);
	v3.push_back(0x42);
	v3.push_back(0x0);
	v3.push_back(0x1E);

	count = 0;
	// Test the timeout, there should be 1 second between prints
	std::cout << "Timeout == 1000ms, asking for 1 more byte than written." << std::endl;
	while (count < 1) {
		size_t bytes_wrote = my_serial.write(v3);

//		std::string result = my_serial.read(v3.size());
		std::vector<uint8_t> v;
		int n  = my_serial.read(v, 5);
		std::cout << "n: " << n << std::endl;
		for(int i = 0; i < n;i ++){
			//std::cout << "v[" << i <<"]: "<< std::hex << v[i];
			printf(" - %.2X\n", v[i]);
		}
		std::cout << "Iteration: " << count << ", Bytes written: ";
		std::cout << bytes_wrote << ", Bytes read: ";
		std::cout << n << std::endl;

		count += 1;
	}


}