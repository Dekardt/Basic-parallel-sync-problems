//#include <thread>
//#include <mutex>
//#include <iostream>
//#include <string>
//#include <Windows.h>
//#include <shared_mutex>
//
//using namespace std;
//
//#define NUMBER_OF_WRITERS 1
//#define NUMBER_OF_READERS 11
//
//int counter_readers_in_room = 0;
//
//// mutex for changing readers counter
//mutex counter_mutex;
//
//// mutex for console print
//mutex print_mutex;
//
//// mutex for reader/writer access
//shared_mutex enter_section_mutex;
//
//void reader_function()
//{
//	while (true)
//	{
//		// reader can enter even if there are others readers, but not writter
//		enter_section_mutex.lock_shared();
//
//		// increase readers counter and print it
//		{
//			counter_mutex.lock();
//
//			counter_readers_in_room++;
//
//			print_mutex.lock();
//			cout << "There are " << counter_readers_in_room << " readers in room" << endl;
//			print_mutex.unlock();
//
//			counter_mutex.unlock();
//		}
//		
//		// simulation of readers activity
//		Sleep(5000 + rand() % 4000);
//
//		// decrease readers counter and print it
//		{
//			counter_mutex.lock();
//
//			counter_readers_in_room--;
//
//			print_mutex.lock();
//			cout << "There are " << counter_readers_in_room << " readers in room" << endl;
//			print_mutex.unlock();
//
//			counter_mutex.unlock();
//
//			enter_section_mutex.unlock_shared();
//		}
//
//		// wait befor next iteration
//		Sleep(3000 + rand() % 2000);
//	}
//}
//
//void writerFunction()
//{
//	while (true)
//	{
//		// writer came so readers can enter no more
//		enter_section_mutex.lock();
//
//		print_mutex.lock();
//		cout << "There is writer in the room" << endl;
//		print_mutex.unlock();
//
//		// simulation of writter activity
//		Sleep(5000 + rand() % 5000);
//
//		// writer left so readers can enter
//		enter_section_mutex.unlock();
//
//		Sleep(1000 + rand() % 1000);
//	}
//}
//
//void main()
//{
//
//	thread array_of_threads[NUMBER_OF_READERS + NUMBER_OF_WRITERS];
//
//	// start threads for readers
//	for (int i = 0; i < NUMBER_OF_READERS; i++)
//	{
//		array_of_threads[i] = thread(reader_function);
//		Sleep(200);
//	}
//
//	// start threads for writers
//	for (int i = NUMBER_OF_READERS; i < NUMBER_OF_READERS + NUMBER_OF_WRITERS; i++)
//	{
//		array_of_threads[i] = thread(writerFunction);
//		Sleep(200);
//	}
//
//	for (int i = 0; i < NUMBER_OF_READERS + NUMBER_OF_WRITERS; i++)
//	{
//		if (array_of_threads[i].joinable())
//			array_of_threads[i].join();
//		else
//			cout << "Error";
//	}
//}