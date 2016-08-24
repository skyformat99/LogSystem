#include "logExport.h"
#include <Windows.h>
#include <iostream>
#include <time.h>

DWORD WINAPI threadfunc(LPVOID pParam)
{
	for (int i = 0; i < 10000; i++)
		WriteLog("600600", 10, "test log");
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
	std::cout << (end - begin) << std::endl;
	std::cin.get();
	return 0;
}