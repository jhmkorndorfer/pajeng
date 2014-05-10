/* 
 * File:   ThreadQueue.cc
 * Author: jonas
 * 
 * Created on 20 de Abril de 2014, 16:26
 */

#include "ThreadQueue.h"

ThreadQueue::ThreadQueue() :   stop(false){
    return;
}

ThreadQueue::ThreadQueue(const ThreadQueue& orig) {
}
void ThreadQueue::setQueue(size_t threads){
//this->stop = false;
//for(size_t i = 0;i<threads;++i){
        workers.emplace_back(
            [this]
            {
                for(;;)
                {
                    //std::unique_lock<std::mutex> lock(this->queue_mutex);
                    while(!this->stop && this->tasks.empty())
                       //this->condition.wait(lock);
                    if(this->stop && this->tasks.empty())
                        return;
                    PajeContainer *container = this->tasks.front()->container();
                    container->demuxer(this->tasks.front());
                    this->tasks.pop();
                    //lock.unlock();
                }
            }
        );
   // }
}
void ThreadQueue::enqueue(PajeEvent *event) 
{
    if(stop)
        throw std::runtime_error("enqueue on stopped ThreadPool");
    {
        //std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.push(event);
    }
    condition.notify_one();
}

ThreadQueue::~ThreadQueue()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(size_t i = 0;i<workers.size();++i)
        workers[i].join();
}

