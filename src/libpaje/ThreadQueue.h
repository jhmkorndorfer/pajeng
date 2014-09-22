#include <queue>
#include <memory>
#include <thread>
#include <set>
#include <mutex>
#include <condition_variable>
#include <exception>
#include "PajeEvent.h"
#include "PajeContainer.h"
#ifndef THREADQUEUE_H
#define	THREADQUEUE_H

class ThreadQueue {
public:
    ThreadQueue();
    void enqueue(PajeEvent *event, PajeContainer *container); 
    void startWork();
    void setQueue();
     ~ThreadQueue();
public:
    std::thread t1;
    std::thread t2;
    std::set<PajeContainer*> containers_set;
    std::mutex containers_mutex;
    bool stop;
    bool start;
};

#endif	/* THREADQUEUE_H */