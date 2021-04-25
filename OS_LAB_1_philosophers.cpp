//#include "windows.h"
//#include <iostream>
//#include "process.h"
//#include <string>
//#include <cstdlib>
//
//#define NUMBER_OF_PHILOSOPHERS 4
//
//using namespace std;
//
//// event for every fork
//HANDLE forks[NUMBER_OF_PHILOSOPHERS];
//
//// event for console print
//HANDLE event_print;
//
//unsigned __stdcall philosopher(void* number)
//{
//	int* philosopher_number = (int*)number;
//
//	WaitForSingleObject(event_print, 1000000);
//	cout << "Philosopher " << *philosopher_number << " is thinking" << endl;
//	SetEvent(event_print);
//
//	while (1)
//	{
//		//take left fork
//		DWORD left_fork = WaitForSingleObject(forks[*philosopher_number], 1);
//
//		if (left_fork == WAIT_OBJECT_0)
//		{
//			// take right fork only if left fork was available
//			DWORD right_fork = WaitForSingleObject(forks[(*philosopher_number + 1) % NUMBER_OF_PHILOSOPHERS], 1);
//			if (right_fork == WAIT_OBJECT_0)
//			{
//				WaitForSingleObject(event_print, 1000000);
//				cout << "Philosopher " << *philosopher_number << " is eating"<<endl;
//				SetEvent(event_print);
//
//				// simulation eating
//				Sleep(100 + rand() % 1000);
//
//				WaitForSingleObject(event_print, 1000000);
//				cout << "Philosopher " << *philosopher_number << " is thinking"<<endl;
//				SetEvent(event_print);
//
//				// put right fork after eating
//				SetEvent(forks[(*philosopher_number + 1) % NUMBER_OF_PHILOSOPHERS]);
//			}
//			// put left fork after eating or if rigth fork is used by another philosopher
//			SetEvent(forks[*philosopher_number]);
//		}
//		Sleep(rand() % 500);
//	}
//
//	return 0;
//}
//
//void main()
//{
//	event_print = CreateEvent(NULL, FALSE, TRUE, NULL);
//
//	// creating events for forks
//	for (int i = 0; i < NUMBER_OF_PHILOSOPHERS; i++)
//	{
//		forks[i] = CreateEvent(NULL, FALSE, TRUE, NULL);
//
//		if (forks[i] == NULL)
//		{
//			cout << "error create event" << endl;
//		}
//	}
//
//	int A[NUMBER_OF_PHILOSOPHERS];
//
//	// starting threads for philosophers
//	for (int i = 0; i < NUMBER_OF_PHILOSOPHERS; i++)
//	{
//		A[i] = i;
//		if (_beginthreadex(NULL, 0, philosopher, &A[i], 0, NULL) == -1)
//		{
//			cout << "Error begin thread " << endl;
//		}
//		Sleep(100);
//	}
//
//	while (TRUE)
//	{
//
//	}
//}