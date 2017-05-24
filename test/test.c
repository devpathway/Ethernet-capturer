#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef struct _ASYNC_READ
{
	OVERLAPPED ov;
	char *pszBuffer;
	UINT uiSize;
	HANDLE hfiler;
} ASYNC_READ, *PASYNC_READ;

VOID CALLBACK ReadCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
	PASYNC_READ pAsyncRead = (PASYNC_READ)lpOverlapped;
	unsigned char* ppb = pAsyncRead->pszBuffer;
	UINT8 p1 = ppb[26];
	UINT8 p2 = ppb[27];
	UINT8 p3 = ppb[28];
	UINT8 p4 = ppb[29];
	
	UINT8 p5 = ppb[30];
	UINT8 p6 = ppb[31];
	UINT8 p7 = ppb[32];
	UINT8 p8 = ppb[33];

	printf("source ip address: %d.%d.%d.%d destination ip address: %d.%d.%d.%d", p1, p2, p3, p4, p5, p6, p7, p8);

	printf("\r\n");

	memset(pAsyncRead->pszBuffer, 0, 1500);
	ReadFileEx(pAsyncRead->hfiler, pAsyncRead->pszBuffer, pAsyncRead->uiSize - 1, (LPOVERLAPPED)pAsyncRead, ReadCompletionRoutine);
}

int main()
{
	HANDLE hfilter = INVALID_HANDLE_VALUE;
	hfilter = CreateFile("\\\\.\\NDISLWF", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if (hfilter != INVALID_HANDLE_VALUE)
	{
		char buffer[1500] = { 0 };
		ASYNC_READ AsyncRead = { 0 };
		AsyncRead.pszBuffer = buffer;
		AsyncRead.hfiler = hfilter;
		AsyncRead.uiSize = sizeof(buffer);
		ReadFileEx(AsyncRead.hfiler, AsyncRead.pszBuffer, AsyncRead.uiSize - 1, (LPOVERLAPPED)&AsyncRead, ReadCompletionRoutine);

		BOOL bDoClientLoop = TRUE;
		while (bDoClientLoop)
		{
			SleepEx(25, TRUE);
//			int cChar = getchar();
//			switch (cChar)
//			{
//			case 27:
//				bDoClientLoop = FALSE;
//				break;
//			default:
//				break;
//			}
		}

		CloseHandle(hfilter);
	}

	return 0;
}
