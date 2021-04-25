#include "windows.h"
#include <iostream>
#include "process.h"
#include <string>
#include <cstdlib>
#include <queue>

#define NUMBER_OF_PLACES_IN_QUEUE 3

using namespace std;

class Client
{
public:
    int client_number;

    // binary semaphore signaling if client is first in queue
    HANDLE semaphore_client_is_first;

    Client(int number)
    {
        this->client_number = number;
        this->semaphore_client_is_first = CreateSemaphore(NULL, 0, 1, NULL);
    }
};

HANDLE semaphore_queue_counter;

HANDLE semaphore_barber_is_working;

HANDLE queue_places[NUMBER_OF_PLACES_IN_QUEUE];

HANDLE semaphore_barber_finished_haircaut;

HANDLE semaphore_print;

int queue_counter = 0;

queue <Client> client_queue;

unsigned __stdcall barber(void*)
{
    while (1)
    {
        if (client_queue.empty())
        {
            WaitForSingleObject(semaphore_print, 1000000);
            cout << "Barber is sleeping" << endl;
            ReleaseSemaphore(semaphore_print, 1, NULL);
        }

        // barber is waiting for client (threads stops, not wasting cpu resources)
        WaitForSingleObject(semaphore_barber_is_working, 1000000);

        // first client comes to barber
        ReleaseSemaphore(client_queue.front().semaphore_client_is_first, 1, NULL);
        int client_number = client_queue.front().client_number;
        client_queue.pop();

        // decrement number of clients in queue
        WaitForSingleObject(semaphore_queue_counter, 1000000);
        queue_counter--;
        ReleaseSemaphore(semaphore_queue_counter, 1, NULL);

        WaitForSingleObject(semaphore_print, 1000000);
        cout << "Barber is working with client number " << client_number << endl;
        ReleaseSemaphore(semaphore_print, 1, NULL);

        // simulation working with client
        Sleep(1000 + rand() % 2000);

        WaitForSingleObject(semaphore_print, 1000000);
        cout << "Barber has finished work with client number " << client_number << endl;
        ReleaseSemaphore(semaphore_print, 1, NULL);

        // client leaves
        ReleaseSemaphore(semaphore_barber_finished_haircaut, 1, NULL);
    }
    return 0;
}

unsigned __stdcall client(void* client)
{
    Client current_client = *(Client*)client;

    // client can not enter if queue is full
    if (queue_counter == NUMBER_OF_PLACES_IN_QUEUE)
    {
        WaitForSingleObject(semaphore_print, 10000000);
        cout << "Queue is full, client number " << current_client.client_number << " cann't enter" << endl;
        ReleaseSemaphore(semaphore_print, 1, NULL);
        _endthread();
    }
    else
    {
        WaitForSingleObject(semaphore_queue_counter, 10000000);

        WaitForSingleObject(semaphore_print, 10000000);
        cout << "Client number " << current_client.client_number << " entered queue" << endl;
        ReleaseSemaphore(semaphore_print, 1, NULL);

        // increment number of clients in queue
        client_queue.push(current_client);
        queue_counter++;
        ReleaseSemaphore(semaphore_queue_counter, 1, NULL);

        ReleaseSemaphore(semaphore_barber_is_working, 1, NULL);
    }

    // wait for barber starts
    WaitForSingleObject(current_client.semaphore_client_is_first, 10000000);

    // wait for barber finishes
    WaitForSingleObject(semaphore_barber_finished_haircaut, 10000000);

    WaitForSingleObject(semaphore_print, 10000000);
    cout << "Client number " << current_client.client_number << " is done" << endl;
    ReleaseSemaphore(semaphore_print, 1, NULL);

    _endthread();

    return 0;
}

void main()
{
    semaphore_queue_counter = CreateSemaphore(NULL, 1, 1, NULL);
    semaphore_barber_is_working = CreateSemaphore(NULL, 0, NUMBER_OF_PLACES_IN_QUEUE, NULL);


    semaphore_barber_finished_haircaut = CreateSemaphore(NULL, 0, 1, NULL);

    semaphore_print = CreateSemaphore(NULL, 1, 1, NULL);

    // start thread for barber
    if (_beginthreadex(NULL, 0, barber, NULL, 0, NULL) == -1)
    {
        cout << "Error begin thread " << endl;
    }

    Sleep(100);

    // start threads for clients
    for (int i = 1; i < 7000; i++)
    {
        Client cl = Client(i);
        if (_beginthreadex(NULL, 0, client, (void*)&cl, 0, NULL) == -1)
        {
            cout << "Error begin thread " << endl;
        }
        Sleep(1000 + rand() % 400);
    }
}