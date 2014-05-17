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
void ThreadQueue::setQueue(){
        workers.emplace_back(
            [this]
            {
                for(;;)
                {
                    while(!this->stop && this->tasks.empty())
                            return;
                    PajeContainer *container = this->tasks.front()->container();
                    container->demuxer(this->tasks.front());
                    this->tasks.pop();
                }
            }
        );
}
void ThreadQueue::enqueue(PajeEvent *event) 
{
        tasks.push(event);
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