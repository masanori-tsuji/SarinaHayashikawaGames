#ifndef _MAIN_H_
#define _MAIN_H_
#pragma comment (lib,"ws2_32.lib")

#define BUFFER_NUM 1024

typedef struct
{
	char aRcevData[BUFFER_NUM];
	char aSendData[BUFFER_NUM];
}STRDATA;

#endif