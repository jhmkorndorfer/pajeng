/* 
 * File:   ThreadQueue.h
 * Author: jonas
 *
 * Created on 20 de Abril de 2014, 16:26
 */
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

#include "PajeEvent.h"
#include "PajeContainer.h"
#ifndef THREADQUEUE_H
#define	THREADQUEUE_H

class ThreadQueue {
public:
    ThreadQueue();
    ThreadQueue(const ThreadQueue& orig);
    void enqueue(PajeEvent *event); 
    void setQueue(size_t threads);
     ~ThreadQueue();
public:
    std::vector< std::thread > workers;
    std::queue<PajeEvent*> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

#endif	/* THREADQUEUE_H */

