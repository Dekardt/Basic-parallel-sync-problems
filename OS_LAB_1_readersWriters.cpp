#include <thread>
#include <mutex>
#include <iostream>
#include <string>
#include <Windows.h>
#include <shared_mutex>

using namespace std;

#define AMOUNT_OF_WRITERS 1
#define AMOUNT_OF_READERS 11

int readers_counter = 0;

// mutex for changing readers counter
mutex counter_mutex;

// mutex for console print
mutex print_mutex;

// mutex for reader/writer access
shared_mutex enter_section_mutex;

void reader_function()
{
	while (true)
	{
		// reader can enter even if there are other readers, but not writer
		enter_section_mutex.lock_shared();

		// increase readers counter and print it
		{
			counter_mutex.lock();

			readers_counter++;

			print_mutex.lock();
			cout << "There are " << readers_counter << " readers in room" << endl;
			print_mutex.unlock();

			counter_mutex.unlock();
		}
		
		// simulation of readers activity
		Sleep(5000 + rand() % 4000);

		// decrease readers counter and print it
		{
			counter_mutex.lock();

			readers_counter--;

			print_mutex.lock();
			cout << "There are " << readers_counter << " readers in room" << endl;
			print_mutex.unlock();

			counter_mutex.unlock();

			enter_section_mutex.unlock_shared();
		}

		// wait before next iteration
		Sleep(3000 + rand() % 2000);
	}
}

void writer_function()
{
	while (true)
	{
		// writer came so readers can enter no more
		enter_section_mutex.lock();

		print_mutex.lock();
		cout << "There is writer in the room" << endl;
		print_mutex.unlock();

		// simulation of writter activity
		Sleep(5000 + rand() % 5000);

		// writer left so readers can enter
		enter_section_mutex.unlock();

		Sleep(1000 + rand() % 1000);
	}
}

void main()
{

	thread threads_array[AMOUNT_OF_READERS + AMOUNT_OF_WRITERS];

	// start threads for readers
	for (int i = 0; i < AMOUNT_OF_READERS; i++)
	{
		threads_array[i] = thread(reader_function);
		Sleep(200);
	}

	// start threads for writers
	for (int i = AMOUNT_OF_READERS; i < AMOUNT_OF_READERS + AMOUNT_OF_WRITERS; i++)
	{
		threads_array[i] = thread(writer_function);
		Sleep(200);
	}

	for (int i = 0; i < AMOUNT_OF_READERS + AMOUNT_OF_WRITERS; i++)
	{
		if (threads_array[i].joinable())
        {
            threads_array[i].join();
        }
		else
        {
            cout << "Error";
        }	
	}
}