#include <iostream>

#include "stdio.h"
#include "string.h"
#include <fcntl.h>
#include <unistd.h>


// commands:
typedef enum  {
  LASERON = 0,
  LASEROFF,
  MEAS_NORMAL,
  MEAS_CONT_START,
  MEAS_FAST,
  MEAS_CONT_STOP,
  
  NUM_CMDS
}COMMANDS;

char CMD[NUM_CMDS][10] = { 
  "\xC0\x41\x00",
  "\xC0\x42\x00",
  "\xC0\x40\x01\x00",
  "\xC0\x40\x01\x01",
  "\xC0\x40\x01\x0D",
  "\xC0\x40\x01\x02"
};

int LEN[NUM_CMDS] = { 
  3,
  3,
  4,
  4,
  4,
  4
};

int iOut, fd; // file descriptor


// defines:
#define CRC8_POLYNOMIAL     0xA6
#define CRC8_INITIAL_VALUE  0xAA

#define RET_NOERR          1
#define RET_ERR_TIMEOUT   -1
#define RET_ERR_STATUS    -2

#define DELAY 0.001

//***************************************************************************************************************************
// Calculate the CRC8 checksum for the given value. No reflection, inversion, reversion or final XOR used. Just plain CRC8.
//   input:
//     -- Data:         Data to calculate the CRC for.
//     -- InitialValue: The initial value for the CRC (e.g. previous calculated value).
//   output:
//      -- Calculated CRC8 checksum
//
//***************************************************************************************************************************
unsigned char CalcCrc8(unsigned char Data, unsigned char InitialValue)
{
  unsigned char i;
  
  for (i=0; i<8; i++)
  {
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
unsigned char CalcCrc8FromArray(const unsigned char* pData, unsigned short int NumBytes, unsigned char InitialValue)
{
  const unsigned char* pDataTemp = pData;
  const unsigned char* pEnd = pData + NumBytes;
  while(pDataTemp < pEnd)
  { 
    InitialValue = CalcCrc8(*pDataTemp++, InitialValue);
  }
  return InitialValue;
}

//***************************************************************************
// Transmit Standard Command
//***************************************************************************
int SendCommand(COMMANDS cmd, char *rx, int rxbytes, int timeout) 
{
  int n, i =0, status, length, txbytes;
  char sum, command[30];
  double test = 123;
  //FlushInQ(fd);
  //FlushOutQ(fd);
  
  memcpy ( command, CMD[cmd], LEN[cmd] );
  txbytes = LEN[cmd];
  
  // Calculate Checksum during sending of bytes
  sum = CalcCrc8FromArray ( command, txbytes, CRC8_INITIAL_VALUE );

  printf("Command: ");
  for(int i = 0; i< LEN[cmd]; i++)
	  printf("%hhX", command[i]);
  printf("\n");
  printf("Sum: %hhX\n", sum);

	std::cout << "Test sensor Bosch" << std::endl;

	char *sfd = "/dev/ttyACM0";
	char reply[10];

	fd = open(sfd, O_RDWR | O_NOCTTY | O_NONBLOCK);

	if(fd>0){
		std::cout << "Open successfully" << std::endl;

		iOut = write(fd, command, 6);
		iOut = write(fd, sum, 2);
		read(fd, reply, rxbytes);
		printf("Reply: ");
		for(int i = 0; i< 10; i++)
			printf("%hhX", reply);
		printf("\n");

		std::cout << "Reply: " << std::hex << reply << std::endl;

		close(fd);
	}else{
		std::cout << "failed to open device" << std::endl;
	}


  //ComWrt (fd, command, txbytes);
  /*
  ComWrtByte (fd, sum);
  
  WriteTxt(0, "Command send: Waiting for reply... p l e a s e   w a i t");
  
  i = 0;
  timeout /= DELAY;
  while(GetInQLen(fd) == 0 && i<timeout)
  {
    Delay(DELAY);
    i++;
  }
  
  // timeout occured
  if (GetInQLen(fd) == 0)
    return RET_ERR_TIMEOUT;
  
  // collect status byte
  status = ComRdByte(fd);
  // check status
  if (status != 0)
  {
    // error
    WriteTxt(1, "Error.");
    return RET_ERR_STATUS;
  }
  
  // get response length
  length = ComRdByte(fd);

  // wait until answer is received:
  i = 0;
  timeout = 1.0/DELAY; 
  while(GetInQLen(fd) < length+1 && i<timeout)
  {
    Delay(DELAY);
    i++;
  }
  ComRd(fd, rx, length );
  
  // debug msg
  WriteTxt(0, "Command send: Reply received.");  
  */
  return RET_NOERR;
}

//***************************************************************************
// Measure distance
//***************************************************************************
int MeasureDistance( double *Value, COMMANDS cmd ) 
{
  int ret;
  char read[11];
  
  // get new measurement value
  if (RET_NOERR != SendCommand( cmd, read, 10, 6))
  {
    printf("Measurement timed out. Reconnect the device and restart the program.");  
    return (RET_ERR_STATUS);
  } 

  if(Value) *Value = *((unsigned long int*)read)*50e-6;

  return RET_NOERR;
}



int main(int argc, char* argv[])
{

	int ret;
	double val;

    ret = SendCommand( LASERON, NULL, 0, 1);

	printf("LaserON: ret\n", ret); 
//	ret = MeasureDistance (&val, MEAS_NORMAL);

//	printf("%.8f\n", val);
//	printf("ret\n", ret); 

	return 0;
	
}