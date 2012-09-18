#include "hexaview.h" 

void hexaview(unsigned char *ucp)
{
	int iLoop;
	int iCnt;

	
	printf("-------------------------------------------------------------------------\n"
			" Address ");

	for(iCnt = 0; iCnt < 16; ++iCnt)
	{
		printf("%02X ", iCnt);
	}
	printf("     ASCII\n");
	iLoop = 0;

	for(iLoop = 0; iLoop <= 10; ++iLoop)
	{
		printf("%08X ", ucp);
		for(iCnt = 0; iCnt < 16; ++iCnt)
		{
			printf("%02X ", *(ucp + iCnt)); 
		}

		for(iCnt = 0; iCnt < 16; ++iCnt)
		{
			if(' ' <= *((signed char *)ucp + iCnt))
			{
				printf("%c", *(ucp + iCnt)); 
			}
			else
			{
				putchar('.');
			}
		}
		putchar('\n');
		ucp = ucp + 16;
	}
	
	return;
}



