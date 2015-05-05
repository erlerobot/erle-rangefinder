#include <string>
#include <iostream>
#include <bitset>
#include <unistd.h>

#include "serial/serial.h"


#define CRC8_INITIAL_VALUE  0xAA
#define CRC8_POLYNOMIAL     0xA6

/*
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/ahcorde/serial/serial/build/devel/lib

g++ main2.cpp -o main2 -I/home/ahcorde/serial/serial/include -L/home/ahcorde/serial/serial/build/devel/lib -lserial
*/

//***************************************************************************************************************************
// Calculate the CRC8 checksum for the given value. No reflection, inversion, reversion or final XOR used. Just plain CRC8.
//   input:
//     -- Data:         Data to calculate the CRC for.
//     -- InitialValue: The initial value for the CRC (e.g. previous calculated value).
//   output:
//      -- Calculated CRC8 checksum
//
//***************************************************************************************************************************
uint8_t CalcCrc8(uint8_t Data, unsigned char InitialValue)
{
  uint8_t i;
  
  for (i=0; i<8; i++){
    if (((InitialValue & 0x80) != 0) != ((Data >> (7-i)) & 1))
      InitialValue = (InitialValue << 1) ^ CRC8_POLYNOMIAL;
    else
      InitialValue <<= 1;
  }
  return InitialValue;
}

//***************************************************************************************************************************
// Calculate the CRC8 checksum for the given array. No reflection, inversion, reversion or final XOR used. Just plain CRC8.
//   input:
//     -- pData:        Pointer to the array to calculate the CRC for.
//     -- NumBytes:     Size of array in bytes.
//     -- InitialValue: The initial value for the CRC (e.g. previous calculated value).
//   output:
//      -- Calculated CRC8 checksum
//
//***************************************************************************************************************************
uint8_t CalcCrc8FromArray(std::vector<uint8_t> pData, uint8_t InitialValue)
{
  for(int i = 0; i < pData.size(); i++){
    InitialValue = CalcCrc8(pData[i], InitialValue);
  }
  return InitialValue;
}

int main(int argc, char* argv[])
{
	unsigned long baud = 9600;
	std::string port=std::string("/dev/ttyACM0");
	serial::Serial my_serial(port, 
							 baud,
//							 serial::Timeout::simpleTimeout(1000),
							 serial::Timeout::simpleTimeout(100),
							 serial::eightbits,
							 serial::parity_none,
							 serial::stopbits_one, 
							 serial::flowcontrol_none);

	std::bitset<8> x40(0x40);
	std::cout << x40 << std::endl;
	my_serial.setTimeout(serial::Timeout::max(), 200, 0, 200, 0);

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
	v1.push_back( CalcCrc8FromArray ( v1, CRC8_INITIAL_VALUE ));

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
	v2.push_back(0x0D);

    uint8_t sum = CalcCrc8FromArray ( v2, CRC8_INITIAL_VALUE );
    printf("sum: %hhX\n", sum);

	v2.push_back(sum);

	test_string = "C04010FA";
	count = 0;
	std::cout << "Timeout == 1000ms, asking for 1 more byte than written." << std::endl;
	while (count < 1) {
		size_t bytes_wrote = my_serial.write(v2);

//		std::vector<uint8_t> v;
		// std::string result = my_serial.read(v2, );

		std::vector<uint8_t> v;
		int n  = my_serial.read(v, 10);
		std::cout << "n: " << n << std::endl;
		if(n==0) continue;
		for(int i = 0; i < n;i ++){
			//std::cout << "v[" << i <<"]: "<< std::hex << v[i];
			printf(" - %.2X - ", v[i]);
			std::bitset<8> x(v[i]);
			std::cout << x << std::endl;
		}


		uint32_t _Recv2 = v[5]<<24 | v[4]<<16 | v[3]<<8 | v[2];

		printf("value: %.4f \n", _Recv2*50e-6);
		printf("value: %08X \n", _Recv2);

		std::cout << "Iteration: " << count << ", Bytes written: ";
		std::cout << bytes_wrote << ", Bytes read: ";
		std::cout << n << std::endl;

		count += 1;
	}


	std::cout << " -------------" << std::endl;
	std::cout << "continious reading" << std::endl;
	count =0;
	while(count < 10){
		std::vector<uint8_t> v;
		count++;
		size_t bytes_wrote = my_serial.write(v2);
		int n  = my_serial.read(v, 10);
		std::cout << "N: "<< n << std::endl;
				if(n==0) continue;

		uint32_t _Recv2 = v[5]<<24 | v[4]<<16 | v[3]<<8 | v[2];
				printf("value: %.4f \n", _Recv2*50e-6);
		std::cout << " -------------" << std::endl;
	}

	std::vector<uint8_t> v3;
	v3.push_back(0xC0);
	v3.push_back(0x42);
	v3.push_back(0x0);
	v3.push_back( CalcCrc8FromArray ( v3, CRC8_INITIAL_VALUE ));
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