#include "logExport.h"
#include <Windows.h>
#include <iostream>
#include <time.h>

DWORD WINAPI threadfunc(LPVOID pParam)
{
	static int i = 0;
	char msg[128] = { 0x00 };
	sprintf_s(msg, "test log %d", i);
	for (int i = 0; i < 10000; i++)
	{
		WriteLog("600600", 1, msg);
		sprintf_s(msg, "test log %d", i);
	}
	return 0;
}

int main()
{
	InitLog();

	clock_t begin = clock();
	HANDLE threads[10];
	for (int i = 0; i < 10; i++)
		threads[i] = CreateThread(NULL, 0, threadfunc, NULL, 0, NULL);
	WaitForMultipleObjects(10, threads, TRUE, INFINITE);
	CloseLog();
	clock_t end = clock();
	std::cout << end - begin << std::endl;
	std::cin.get();
	return 0;
}