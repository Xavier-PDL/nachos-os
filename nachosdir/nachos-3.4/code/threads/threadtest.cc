// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "switch.h"
#include "synch.h"
#include "system.h"

// testnum is set in main.cc
int testnum = 1;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

#if defined(CHANGED) && defined(THREADS)
#define wait   P
#define signal V
int SharedVariable;
#if defined(HW1_SEMAPHORE) && defined(THREADS)
int numberOfThreads;
int count = 0;
Semaphore * semSharedVar = new Semaphore("sem_sharedvar", 1);
Semaphore * semBarrier = new Semaphore("sem_barrier", 0);
#endif // HW1_SEMAPHORE
#endif // CHANGED
void
SimpleThread(int which)
{
#if defined(CHANGED) && defined(THREADS)
    int num, val;
    for (num = 0; num < 5; num++) {
#if defined(HW1_SEMAPHORE) && defined(THREADS)
        semSharedVar->wait();
#endif // HW1_SEMAPHORE 
        val = SharedVariable;
        printf("*** thread %d sees value %d\n", which, val);
        currentThread->Yield();
        SharedVariable = val+1;
#if defined(HW1_SEMAPHORE) && defined(THREADS)
        semSharedVar->signal();
#endif // HW1_SEMAPHORE
        currentThread->Yield();
    }

#if defined(HW1_SEMAPHORE) && defined(THREADS)
    semSharedVar->wait();
    count++;
    semSharedVar->signal();
    currentThread->Yield();

    if(count == numberOfThreads)
    {
        semBarrier->signal();
    }
    semBarrier->wait();
    semBarrier->signal();
#endif // HW1_SEMAPHORE

    val = SharedVariable;
    printf("Thread %d sees final value %d\n", which, val);
#else
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
#endif // CHANGED
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1\n");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);
}

#if defined(CHANGED) && defined(THREADS)
//----------------------------------------------------------------------
// ThreadTestN
// 	Set up a ping-pong between N threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------
void
ThreadTestN(int n)
{
    DEBUG('t', "Entering ThreadTestN\n");
#if defined(HW1_SEMAPHORE)
    numberOfThreads = n + 1; // Number of threads = n + main_thread
#endif // HW1_SEMAPHORE
    for(int i = 1; i <= n; i++)
    {
        Thread *t = new Thread("forked thread");
        t->Fork(SimpleThread, i);
    }
    SimpleThread(0);
}
#endif // CHANGED
//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

#if defined(CHANGED) && defined(THREADS)
void
ThreadTest(int n)
#else
void
ThreadTest()
#endif // CHANGED
{
    switch (testnum) {
    case 1:
#if defined(CHANGED) && defined(THREADS)
    ThreadTestN(n);
#else
	ThreadTest1();
#endif // CHANGED
	break;
    default:
	printf("No test specified.\n");
	break;
    }
}
