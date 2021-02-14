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
int nThreads = 0;
int SharedVariable;

#if defined(HW1_SEMAPHORE) && defined(THREADS)
Semaphore * semSharedVar = new Semaphore("sem_sharedvar", 1);
#endif // HW1_SEMAPHORE

#if defined(HW1_LOCKS) && defined(THREADS)
Lock * lockSharedVar = new Lock("lock_sharedvar");
#endif // HW1_LOCKS

#endif // CHANGED
void
SimpleThread(int which)
{
#if defined(CHANGED) && defined(THREADS)
    int num, val;
    for (num = 0; num < 5; num++) {
#if defined(HW1_SEMAPHORE) && defined(THREADS)
        semSharedVar->P();
#endif // HW1_SEMAPHORE 
#if defined(HW1_LOCKS) && defined(THREADS)
        lockSharedVar->Acquire();
#endif // HW1_LOCKS 

        val = SharedVariable;
        printf("*** thread %d sees value %d\n", which, val);
        currentThread->Yield();
        SharedVariable = val+1;

#if defined(HW1_SEMAPHORE) && defined(THREADS)
        semSharedVar->V();
#endif // HW1_SEMAPHORE
#if defined(HW1_LOCKS) && defined(THREADS)
        lockSharedVar->Release();
#endif // HW1_LOCKS
        currentThread->Yield();
    }

#if defined(HW1_SEMAPHORE) && defined(THREADS)
    semSharedVar->P();
    nThreads--;
    semSharedVar->V();

    while(nThreads)
        currentThread->Yield();
#endif // HW1_SEMAPHORE
#if defined(HW1_LOCKS) && defined(THREADS)
    lockSharedVar->Acquire();
    nThreads--;
    lockSharedVar->Release();

    while(nThreads)
        currentThread->Yield();
#endif // HW1_LOCKS

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
    nThreads = n + 1;
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
