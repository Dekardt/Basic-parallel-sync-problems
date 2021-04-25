#include <iostream>
#include <string>
#include <Windows.h>
#include "process.h"
#include <conio.h>

using namespace std;

#define PRODUCT_BUFFER_SIZE 7
#define NUMBER_OF_PRODUCER 4
#define NUMBER_OF_CONSUMER 50

CRITICAL_SECTION producer_section;
CRITICAL_SECTION consumer_section;

CRITICAL_SECTION counter_section;

CRITICAL_SECTION q_is_empty_section;
CRITICAL_SECTION q_is_full_section;

CRITICAL_SECTION empty_signal;
CRITICAL_SECTION full_signal;

CRITICAL_SECTION one_product_section;

CRITICAL_SECTION print_section;

// handmade linked list for handmade queue
class LinkedList
{
public:

	int number;
	LinkedList* next;

	LinkedList(int data)
	{
		this->number = data;
		this->next = nullptr;
	}
};

// handmade queue
class Q
{
public:

	int queue_counter;
	LinkedList* head;
	LinkedList* tail;

	Q()
	{
		this->head = this->tail = nullptr;
	}

	int enqueue(int data)
	{
		LinkedList* node = new LinkedList(data);
		if (this->q_is_empty())
		{
			this->tail = node;
			this->head = this->tail;

			EnterCriticalSection(&counter_section);

			this->queue_counter = 1;

			cout << "Amount of products in queue: " << this->queue_counter << endl;

			LeaveCriticalSection(&counter_section);

			return 0;
		}
		else
		{
			this->tail->next = node;
			this->tail = this->tail->next;

			EnterCriticalSection(&counter_section);

			this->queue_counter++;

			cout << "Amount of products in queue: " << this->queue_counter << endl;

			LeaveCriticalSection(&counter_section);

			return 0;
		}
	}

	int dequeue()
	{
		if (!this->q_is_empty())
		{
			int data;

			data = this->head->number;
			LinkedList* temp = this->head;
			this->head = this->head->next;
			delete temp;

			EnterCriticalSection(&counter_section);

			this->queue_counter--;

			cout << "Amount of products in queue: " << this->queue_counter << endl;

			LeaveCriticalSection(&counter_section);

			return data;
		}
	}

	bool q_is_empty()
	{
		if (this->queue_counter == 0)
			return true;
		else
			return false;
	}
};

Q que = Q();

DWORD WINAPI block_empty_q(void*)
{
	EnterCriticalSection(&q_is_empty_section);
	_endthread();
	return 0;
}

DWORD WINAPI block_full_q(void*)
{
	EnterCriticalSection(&q_is_full_section);
	_endthread();
	return 0;
}

unsigned __stdcall producer(void*)
{
	for (int i = 0; i < 2500; i++)
	{	
		// only one producer can put elements in queue
		EnterCriticalSection(&producer_section);

		// if there is 1 el in queue, possible is only 1 action (producer take or consumer get)
		EnterCriticalSection(&one_product_section);

		// producer can not put el in full queue
		if (que.queue_counter == PRODUCT_BUFFER_SIZE)
		{
			LeaveCriticalSection(&one_product_section);

			EnterCriticalSection(&print_section);
			cout << "Queue is full" << endl;
			LeaveCriticalSection(&print_section);

			// creating thread to block this one until consumer take el, so queue would not be full 
			HANDLE additional_thread = CreateThread(NULL, 0, block_full_q, NULL, 0, NULL);

			if (additional_thread == NULL)
			{
				cout << "Error begin thread " << endl;
			}

			// wait for full_section is blocked
			WaitForSingleObject(additional_thread, INFINITE);

			// wait until consumer take el
			EnterCriticalSection(&q_is_full_section);
			LeaveCriticalSection(&q_is_full_section);
		}

		if (que.queue_counter == 1)
		{
			que.enqueue(rand() % 250);

			LeaveCriticalSection(&one_product_section);
		}
		else
		{
			LeaveCriticalSection(&one_product_section);

			que.enqueue(rand() % 250);
		}

		// signal that queue is not empty anymore
		LeaveCriticalSection(&q_is_empty_section);
		
		LeaveCriticalSection(&producer_section);

		Sleep(500 + rand() % 40);
	}

	return 0;
}

unsigned __stdcall consumer(void*)
{
	for (int i = 0; i < 2500; i++)
	{
		// only one consumer can get elements from queue
		EnterCriticalSection(&consumer_section);

		// if there is 1 el in queue, possible is only 1 action (producer take or consumer get)
		EnterCriticalSection(&one_product_section);

		// consumer can not get el from empty queue
		if (que.queue_counter == 0)
		{
			LeaveCriticalSection(&one_product_section);

			EnterCriticalSection(&print_section);
			cout << "Queue is empty" << endl;
			LeaveCriticalSection(&print_section);

			// creating thread to block this one until producer put el, so queue would not be empty 
			HANDLE additional_thread = CreateThread(NULL, 0, block_empty_q, NULL, 0, NULL);

			if (additional_thread == NULL)
			{
				cout << "Error starting new thread " << endl;
			}

			// wait for empty_section is blocked
			WaitForSingleObject(additional_thread, INFINITE);

			// wait until consumer take el
			EnterCriticalSection(&q_is_empty_section);
			LeaveCriticalSection(&q_is_empty_section);
		}

		if (que.queue_counter == 1)
		{
			que.dequeue();

			LeaveCriticalSection(&one_product_section);
		}
		else
		{
			LeaveCriticalSection(&one_product_section);

			que.dequeue();
		}

		// signal that queue is not full anymore
		LeaveCriticalSection(&q_is_full_section);

		LeaveCriticalSection(&consumer_section);

		Sleep(5000 + rand() % 2000);
	}

	return 0;
}

void main()
{
	InitializeCriticalSection(&producer_section);
	InitializeCriticalSection(&consumer_section);

	InitializeCriticalSection(&counter_section);

	InitializeCriticalSection(&one_product_section);

	InitializeCriticalSection(&print_section);

	InitializeCriticalSection(&q_is_full_section);
	InitializeCriticalSection(&q_is_empty_section);

	for (int i = 0; i < NUMBER_OF_PRODUCER; i++)
	{
		if (_beginthreadex(NULL, 0, producer, NULL, 0, NULL) == -1)
		{
			cout << "Error begin thread " << endl;
		}
		Sleep(100);
	}

	for (int i = 0; i < NUMBER_OF_CONSUMER; i++)
	{
		if (_beginthreadex(NULL, 0, consumer, NULL, 0, NULL) == -1)
		{
			cout << "Error begin thread " << endl;
		}
		Sleep(100);
	}
	_getch();

	DeleteCriticalSection(&producer_section);
	DeleteCriticalSection(&consumer_section);
	DeleteCriticalSection(&counter_section);
	DeleteCriticalSection(&one_product_section);
	DeleteCriticalSection(&print_section);
}