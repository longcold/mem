#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>

#define	COMMAND_LENG	100
#define MAX_PROGRAM_SIZE    0x10000 //적재프로그램의 최대크기의 1/2
#define SECTION_SIZE	512

unsigned char File_Name[100] = {0,};
static unsigned char* mem;
static unsigned char* mem_end;
static unsigned char* code;
static unsigned char* data;
static unsigned char* stack;

enum commandNum  //문자를 문자화 한 장점 활용
{
    REGISTDISPLAY = 100,
    Q,
    HELP,
    H,
    MD_,
    QUIT,
    EXITNUM,
    GO,
    LOAD,
    CODE,
    DATA,
    STACK,
    MC, 
    MM,
    P 
};

typedef struct _context
{
    unsigned int efl;
    unsigned int eip;
    unsigned int edi;
    unsigned int esi;
    unsigned int ebp;
    unsigned int esp;
    unsigned int ebx;
    unsigned int edx;
    unsigned int ecx;
    unsigned int eax;
}Context;
Context cpuinfo = {0, };

typedef struct _commandmap
{
    char *cmd_command;  
    int (*CmdFp)(void *,int);
    enum commandNum cmdNum;
}CommandMap;
void STST(Context *);
unsigned char MD(void *);
void LDST(Context *);

int RegistDisplay(void *, int);
int MemoryDisplay(void *, int);
int Quit(void *, int);
int Help(void *, int);
int Go(void *, int);
int Code(void *, int);
int Data(void *, int);
int Stack(void *, int);
void hexaview(unsigned char *ucp, int iLoop);
int Clear_mem(void *vNotuse,int iNotuse);
int Memory_Modify(void *ucp, int iNotuse);
int Memory_Status(void *vNotuse,int iNotuse);

void init(void);
int Load(void *, int);
void Init_Mem(void);


int main()
{
    unsigned char *md_ucp = 0;
    int iLeng;
    char command[COMMAND_LENG];
    void *vp = 0;
    CommandMap *pMap = 0; 
    CommandMap cMap[] = {
	{"R\n",		RegistDisplay,	REGISTDISPLAY	},
	{"Q\n",		Quit,		Q		},
	{"QUIT\n",	Quit,		QUIT		},
	{"HELP\n",	Help,		HELP		},
	{"H\n",		Help,		H		},
	{"?\n",		Help,		H		},
	{"MD\n",	MemoryDisplay,	MD_		},
	{"GO\n",	Go,		GO		},
	{"LOAD\n",	Load,		LOAD		},
	{"CODE\n",	Code,		CODE		},
	{"DATA\n",	Data,		DATA		},
	{"STACK\n",	Stack,		STACK		},
	{"MC\n",	Clear_mem,	MC		},
	{"MM\n",	Memory_Modify,	MM		},
	{"P\n",	        Memory_Status,	P		},
	{0,		0,		EXITNUM		},
    }; 

    mem = (unsigned char *)malloc(MAX_PROGRAM_SIZE * 2); //128k
    mem_end = mem + (MAX_PROGRAM_SIZE * 2);		
    if(mem == 0)
    {
	printf("메모리가 부족합니다\n");
	return 0;
    }
    Init_Mem();
    STST(&cpuinfo); //시작시 cpu정보 읽기 
    RegistDisplay(&cpuinfo,0);

    while(1)
    {	    
	fflush(stdin);
	putchar('>');
	putchar(' ');

	fgets(command, COMMAND_LENG, stdin);

	if((pMap != 0) && (0 == strcmp(command, "\n")))
	{
	    if(pMap -> cmdNum == 109)
	    {
		code = code + 256;
		if(code >= mem_end)
		{
		    pMap = 0;
		}
		else
		{
		    Code(vp, iLeng);
		}
	    }
	    else if(pMap -> cmdNum == 110)
	    {
		data = data + 256;
		if(data >= mem_end)
		{
		    pMap = 0;
		}
		else
		{
		    Data(vp, iLeng);
		}
	    }
	    else if(pMap -> cmdNum == 111)
	    {
		stack = stack - 256;
		if(md_ucp <= code)
		{
		    pMap = 0;
		}
		else
		{
		    Stack(vp, iLeng);
		}
	    }
	    else if(pMap -> cmdNum == 104)
	    {
		md_ucp = md_ucp + 256;
		if(md_ucp >= mem_end)
		{
		    pMap = 0;
		}
		else
		{
		    MemoryDisplay((void *)md_ucp, iLeng);
		}
	    }
	    else if(pMap -> cmdNum == 113)
	    {
		md_ucp = md_ucp + 256;
		if(md_ucp >= mem_end)
		{
		    pMap = 0;
		}
		else
		{
		    MemoryDisplay((void *)md_ucp, iLeng);
		}
	    }
	    continue;
	}

	for(iLeng = strlen(command); 0 < iLeng ; --iLeng) 
	{
	    command[iLeng - 1] = toupper(command[iLeng - 1]);
	}

	for(pMap = cMap; 0 != pMap->cmd_command ; ++pMap) //행단위 주소이동 & 주소변경  
	{
	    if(0 == strcmp(command, pMap->cmd_command)) 
	    {
		break;
	    }
	}

	if(0 != pMap->cmd_command) //유효한 명령어 인가? 학인
	{
	    vp = 0;
	    iLeng = 0;

	    if(pMap->cmdNum == REGISTDISPLAY)
	    {
		vp = &cpuinfo; 
	    }
	    else if((pMap->cmdNum == MM) || (pMap->cmdNum == MD_))
	    {
		system("cls");
		printf("\n\n\n   * 원하는 곳의 주소를 16진수로 입력하세요? ");
		scanf_s("%x", (int *)&md_ucp); 
		vp = (void *)md_ucp;

		if((mem_end < md_ucp) || mem > md_ucp)
		{
		    system("cls");
		    printf("\n\n\n     출력할 수 없는 공간입니다.\n");
		    pMap = 0;
		    getch();
    		    RegistDisplay(&cpuinfo,0);
		    continue;
		}
	    }
	    else if((pMap->cmdNum == GO)||(pMap->cmdNum == LOAD)||(pMap->cmdNum == MC)||
		    (pMap->cmdNum == CODE)||(pMap->cmdNum == DATA)||(pMap->cmdNum == STACK))
	    {
		Init_Mem();
	    }

	
	    printf("	EAX : 0x%08X", cpuinfo.eax);
	    printf("	EAX : 0x%08X", cpuinfo.ebx);
	    printf("	xxx : 0x%08X", ((Context *)vp));
	    getch();
	    fflush(stdin);

	    (*(pMap->CmdFp))(vp, iLeng);
	}
	else if((0 == pMap->cmd_command) && (command[0] != '\n'))
	{
	    Help(vp, iLeng);
	}
    } 
    return 0;
}


int Memory_Modify(void *ucp, int iNotuse) 
{	
    hexaview((unsigned char *)ucp, 15);
    printf("    * 변경하고 싶은 16진수를 입력하세요? ");
    scanf_s("%02x", (unsigned char *)ucp);
    hexaview((unsigned char *)ucp, 0);
    return 0;
}


void Init_Mem(void)
{
    (unsigned int)code = ((unsigned int)mem & 0xffff0000) + MAX_PROGRAM_SIZE;
    (unsigned int)data = (unsigned int)code + 0x2000;
    (unsigned int)stack = (unsigned int)mem_end - 1;
}


int Go(void *vNotuse,int iNotuse)   // stack삽입, code삽입, cpu정보 초기값 삽입하여 초기화면으로 Go
{
   
    Context cputemp;
    memset(&cputemp, 0, sizeof(Context));
    
    cputemp.eax = (unsigned int)&cpuinfo; //주소값 
    cputemp.eip = (unsigned int)code;     //코드 
    cputemp.esp = (unsigned int)mem_end - 1; //스택
    system("cls");
    printf("\n\n\n       프로그램이 실행되었습니다.\n");
    getch();
    LDST(&cputemp);		


    printf("Kernel Panic\n");
    return 0;
}


int Code(void *vNotuse,int iNotuse)
{
    hexaview(code, 15);
    return 0;
}


int Data(void *vNotuse,int iNotuse)
{

    hexaview(data, 15);
    return 0;
}


int Stack(void *vNotuse,int iNotuse)
{
    hexaview(stack - 255, 15);
    return 0;
}


int Load(void *vNotuse,int iNotus)
{
    int Read_Num;
    int	Header_Size;
    int File_DS = 0;
    IMAGE_DOS_HEADER	    *dhp;  
    IMAGE_NT_HEADERS	    *php;   
    IMAGE_FILE_HEADER	    *fhp;
    IMAGE_OPTIONAL_HEADER32 *ohp;

    Clear_mem(0, 0);
    system("cls");
    printf("\n\n\n   * 읽어 들일 파일 이름을 입력하세요 : "); 
    scanf_s("%s", File_Name); 

    if((File_DS = open(File_Name, O_RDONLY | O_BINARY)) < 0)
    {
	system("cls");
	printf("\n\n\n    파일을 찾을 수 없거나 읽을 수 없습니다.\n");
	return 0;
    }

    if(read(File_DS, code, MAX_PROGRAM_SIZE) == -1)
    {
	system("cls");
	printf("\n\n\n    파일이 존재하지만 읽을 수 없습니다.\n");
	close(File_DS);
	free(mem);
	return 0;
    }
    else 
    {
	(unsigned char *)dhp = code; 
	(unsigned char *)php = code + (dhp -> e_lfanew); //E0
	(unsigned char *)fhp = (unsigned char *)php + sizeof(php -> Signature); //+4
	(unsigned char *)ohp = (unsigned char *)fhp + sizeof(IMAGE_FILE_HEADER);//+..
	Header_Size = ohp -> SizeOfHeaders;
	Clear_mem(0, 0);
    }

    if(0 > lseek(File_DS, Header_Size, SEEK_SET))// 헤더크기만큼이동 코드영역위치 찾기
    {
	printf("	파일의 헤더정보 skip 실패!!\n");
	printf("	파일을 적재할 수 없습니다..\n");
	close(File_DS);
	return 0;
    }
    else
    {
	Read_Num = read(File_DS, code, SECTION_SIZE);
    }

    if(0 > Read_Num)
    {
	printf("      파일이 존재하지만 읽을 수 없습니다. \n");
	close(File_DS);
	return 0;
    }
    else
    {
	Read_Num = read(File_DS, data, SECTION_SIZE);
    }
    close(File_DS);

    if(0 > Read_Num)
    {
	printf("     파일이 존재하지만 읽을 수 없습니다.\n");
	return 0;
    }
    else
    {
	system("cls");
	printf("\n\n\n	      파일을 성공적으로 메모리에 적재하였습니다.\n\n\n\n\n\n\n\n\n");
    }

    getch();
    return 0;
}


int MemoryDisplay(void *ucp, int iNotuse) 
{
    *(unsigned char *)ucp = MD((unsigned char *)ucp); 
    hexaview((unsigned char *)ucp, 15);
    return MD((unsigned char *)ucp); 
}


int Clear_mem(void *vNotuse,int iNotuse)
{
    memset(mem, 0, MAX_PROGRAM_SIZE * 2); 
    return 0;
}


int Quit(void *vNotuse,int iNotuse)
{
    memset(mem, 0, MAX_PROGRAM_SIZE * 2); 
    free(mem);
    exit(0);
    return 0;
}


void hexaview(unsigned char *ucp, int iLoop)
{
    int iCnt=0;
    int iLoopcnt;

    printf("\n\n  -------------------------------------------------------------------------- \n"
	    "   Address ");

    for(iCnt = 0; iCnt < 16; ++iCnt)
    {
	printf("%02X ", iCnt);
    }
    printf(" 0123456789ABCDEF\n");

    for(iLoopcnt = 0; iLoopcnt <= iLoop; ++iLoopcnt)
    {
	printf("  %08X ", ucp);
	for(iCnt = 0; iCnt < 16; ++iCnt)
	{
	    if((mem >= ucp + iCnt) || (mem_end <= ucp+iCnt))
	    {
		printf("\n      범위를 초과하였습니다\n");
		getch();
		STST(&cpuinfo); //시작시 cpu정보 읽기 
		RegistDisplay(&cpuinfo,0);
		return;
	    }
	    printf("%02X ", *(ucp + iCnt)); 
	}

	putchar(' ');

	for(iCnt = 0; iCnt < 16; ++iCnt)
	{
	    if(*(ucp + iCnt) == 0)
	    {
		putchar('.');
	    }	
	    else if((' ' >= *((signed char *)ucp + iCnt)))
	    {
		putchar('*');
	    }
	    else
	    {
		printf("%c", *(ucp + iCnt)); 
	    }
	}
	putchar('\n');
	ucp = ucp + 16;
    }
    printf("\n\n\n");
    return;
}


int RegistDisplay(void *r, int notuse)
{
	    printf("\n	EAX : 0x%08X", cpuinfo.ebx);
	    printf("	xxx : 0x%08X", ((Context *)r));
	    getch();
	    fflush(stdin);

    system("cls");
    printf("\n\n");
    printf("	* code  address          :  0x%08x\n", (unsigned int)code);
    printf("	* data  address          :  0x%08x\n", (unsigned int)data);
    printf("	* stack address          :  0x%08x\n", (unsigned int)stack);
    printf("	* dynamic memory address :  0x%08x to %08x (128Kbytes)\n\n", mem, mem_end);
    printf("\n");
    printf("    	************ Register Value ************\n");
    printf("	EAX : 0x%08X", ((Context *)r)->eax);
    printf("	EBX : 0x%08X\n", ((Context *)r)->ebx);
    printf("	ECX : 0x%08X", ((Context *)r)->ecx);
    printf("	EDX : 0x%08X\n", ((Context *)r)->edx);
    printf("	ESP : 0x%08X", ((Context *)r)->esp);
    printf("	EBP : 0x%08X\n", ((Context *)r)->ebp);
    printf("	ESI : 0x%08X", ((Context *)r)->esi);
    printf("	EDI : 0x%08X\n", ((Context *)r)->edi);
    printf("	EIP : 0x%08X", ((Context *)r)->eip);
    printf("	EFL : 0x%08X\n\n\n\n\n\n\n", ((Context *)r)->efl);
    return 0;
}


int Help(void *vNotuse,int iNotuse)
{
    system("cls");
    printf("\n\n\n\n\n        ****** 메모리 디버거 명령어 ****** \n\n");
    printf("	R         : Register Value Display\n");
    printf("	P         : Memory Status Display\n");
    printf("	MC        : Memory Clear\n");
    printf("	MM        : Memory Modify\n");
    printf("	MD        : Memory Display\n");
    printf("	LOAD      : Program Load\n");
    printf("	GO        : Loaded Program Execute\n");
    printf("	CODE      : Code Area Display\n");
    printf("	DATA      : Data Area Display\n");
    printf("	STACK     : Stack Area Display\n");
    printf("	HELP      : Help Message Display\n");
    printf("	QUIT(Q)   : Exit Program\n\n");
    return 0;
}


int Memory_Status(void *vNotuse,int iNotuse)
{
    system("cls");
    printf("\n\n\n\n\n\n\n\n\n");
    printf("	* code  address          :  0x%08x\n", (unsigned int)code);
    printf("	* data  address          :  0x%08x\n", (unsigned int)data);
    printf("	* stack address          :  0x%08x\n", (unsigned int)stack);
    printf("	* dynamic memory address :  0x%08x to %08x (128Kbytes)\n\n\n\n\n\n\n\n", mem, mem_end);
    return 0;
}

