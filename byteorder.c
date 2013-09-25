/* Creates the endianness.h file */
#include <stdio.h>

int littleendian();

int main()
{
	printf("#define LITTLE_ENDIAN 1234\n#define BIG_ENDIAN 4321\n");

	if(littleendian())
	{
		printf("#define BYTE_ORDER LITTLE_ENDIAN\n");
	}
	else
	{
		printf("#define BYTE_ORDER BIG_ENDIAN\n");
	}
	return 0;
}


int littleendian()
{
	short int word = 0x0001;
	char *byte = (char *) &word;
	return(byte[0] ? 1 : 0);
}
