### Solving 4 basic problems of parallel synchronization using various synchronization tools
* Dining philosophers + windows.h/events
* Producer–consumer + windows.h/critical section
* Readers–writers + c++11 thread/shared mutex
* Sleeping barber + windows.h/semaphores

> #### Dining philosophers
N philosophers sit at a round table with bowls of spaghetti. Forks are placed between each pair of adjacent philosophers.
Each philosopher must alternately think and eat. However, a philosopher can only eat spaghetti when they have both left and right forks. Each fork can be held by only one philosopher and so a philosopher can use the fork only if it is not being used by another philosopher. After an individual philosopher finishes eating, they need to put down both forks so that the forks become available to others. A philosopher can only take the fork on their right or the one on their left as they become available and they cannot start eating before getting both forks.

>> ###### Events
An event object is a synchronization object whose state can be explicitly set to signaled by use of the SetEvent function. A thread uses the CreateEvent or CreateEventEx function to create an event object. The creating thread specifies the initial state of the object and whether it is a manual-reset or auto-reset event object. 
The creating thread can also specify a name for the event object. Threads in other processes can open a handle to an existing event object by specifying its name in a call to the OpenEvent.


> #### Producer–consumer
The producer–consumer problem is a classic example of a multi-process synchronization problem. In the first version of the problem, there are two cyclic processes, a producer and a consumer, which share a common, fixed-size buffer used as a queue. The producer repeatedly generates data and writes it into the buffer. The consumer repeatedly reads the data in the buffer, removing it in the course of reading it, and using that data in some way. In the first version of the problem, with an unbounded buffer, the problem is how to design the producer and consumer code so that, in their exchange of data, no data is lost or duplicated, data is read by the consumer in the order it is written by the producer, and both processes make as much progress as possible. In the later formulation of the problem, Dijkstra proposed multiple producers and consumers sharing a finite collection of buffers. This added the additional problem of preventing producers from trying to write into buffers when all were full, and trying to prevent consumers from reading a buffer when all were empty.

>The first case to consider is the one in which there is a single producer and a single consumer, and there is a finite-size buffer. The solution for the producer is to either go to sleep or discard data if the buffer is full. The next time the consumer removes an item from the buffer, it notifies the producer, who starts to fill the buffer again. In the same way, the consumer can go to sleep if it finds the buffer empty. The next time the producer puts data into the buffer, it wakes up the sleeping consumer.

>>###### Critical sections
The critical section object provides synchronization. Only one thread can own this object, which ensures synchronization.
To use a critical section, you need to create a variable of this type, and initialize it before using it using the InitializeCriticalSection () function. In order to enter the section, you need to call the EnterCriticalSection () function, and after completion of the work LeaveCriticalSection (). The critical section can be deleted by the DeleteCriticalSection () function.

>#### Readers–writers
The readers–writers problem is examples of a common computing problem in concurrency. Some threads may read and some may write, with the constraint that no thread may access the shared resource for either reading or writing while another thread is in the act of writing to it. (In particular, we want to prevent more than one thread modifying the shared resource simultaneously and allow for two or more readers to access the shared resource at the same time).

>>###### Shared mutex
While a regular mutex exposes 3 methods: lock, unlock and try_lock, a shared_mutex adds 3 more: lock_shared, unlock_shared, try_lock_shared. The first 3 methods work exactly the same as in a regular mutex. i.e. If a mutex is locked, all other threads will wait until it is unlocked. The shared versions are a little more complicated.
The implementation of shared_mutex uses a special algorithm: If there is no thread calling lock ever, all the threads calling lock_shared will be allowed to proceed without ever having to wait.


>#### Sleeping barber
This is a classic inter-process communication and synchronization problem between multiple operating system processes. The problem is analogous to that of keeping a barber working when there are customers, resting when there are none, and doing so in an orderly manner.

>>###### Semaphores
A semaphore object is a synchronization object that maintains a count between zero and a specified maximum value. The count is decremented each time a thread completes a wait for the semaphore object and incremented each time a thread releases the semaphore. When the count reaches zero, no more threads can successfully wait for the semaphore object state to become signaled. The state of a semaphore is set to signaled when its count is greater than zero, and nonsignaled when its count is zero.