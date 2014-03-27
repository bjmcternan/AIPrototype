// AIPrototype.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <Windows.h>
#include <thread>
#include <mutex>

#define THREADCOUNT 2

std::mutex gMutexMessageReceived;
std::mutex gMutexTimerStart;

int gPacketNum = 0;
bool gTimeUp = false;

//Recive Thread
DWORD WINAPI receiveStart(LPVOID lpParam)
{
	UNREFERENCED_PARAMETER(lpParam);
	std::cout << "Receive thread started." << std::endl;

	//connect to server
	std::cout << "Connected to server." << std::endl;
	while(true)
	{
		Sleep(10000);
		std::cout << "Receive: Message Received!" << std::endl;
		//simulate locking and writing
		//Critical Section
		gMutexMessageReceived.lock();
		gPacketNum++;
		gMutexMessageReceived.unlock();
		if(gPacketNum == 100)
		{
			return 1;
		}
	}
}

//Timer Thread
DWORD WINAPI timerBegin(LPVOID lpParam)
{
	UNREFERENCED_PARAMETER(lpParam);
	int timerPacketNum = 0;
	

	std::cout << "Timer: timer started" << std::endl;
	while(true)
	{
		//Critical Section
		gMutexMessageReceived.lock();
		if(gPacketNum > timerPacketNum)
		{
			//Message Received
			std::cout << "Timer: Message recieved. Starting timer" << std::endl;
			gMutexMessageReceived.unlock();
			return 2;
		}
		gMutexMessageReceived.unlock();
	}
}

int main(int argc, char* argv[])
{
	HANDLE threads[THREADCOUNT];
	DWORD ThreadID;
	int mainPacketNum = 0;
	bool endtest = false;

	//initialize Timer thread
	threads[0] = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE) timerBegin,
		NULL,
		0,
		&ThreadID);

	//initialize Receive thread
	threads[1] = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE) receiveStart,
		NULL,
		0,
		&ThreadID);
	//main loop
	while(!endtest)
	{
		//Critical section
		gMutexMessageReceived.lock();
		if(gPacketNum > mainPacketNum)
		{
			std::cout << "Main: Message Received" << std::endl;
			endtest = true;
		}
		gMutexMessageReceived.unlock();
		//End Critical Section
		Sleep(10);
	}

	//closing, wait for other threads to join
	WaitForMultipleObjects(THREADCOUNT, threads, TRUE, INFINITE);
	CloseHandle(threads[0]);
	CloseHandle(threads[1]);
	return 0;
}

