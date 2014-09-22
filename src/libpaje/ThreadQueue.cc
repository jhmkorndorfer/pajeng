#include "ThreadQueue.h"

ThreadQueue::ThreadQueue() :   stop(false),start(false)
{
    return;
}
void ThreadQueue::startWork()
{
    while(!this->containers_set.empty()){
        //Only one thread access the containers set
        this->containers_mutex.lock();
        //Verifies the existence of more containers in the set. The previous thread may have removed the last container.
            if(this->containers_set.empty()){
                this->containers_mutex.unlock();
                return;
            }
            std::set<PajeContainer*>::iterator it_container;
            it_container = this->containers_set.begin();
            this->containers_set.erase(it_container);
            PajeContainer *cont = *it_container;
        this->containers_mutex.unlock();
        while(!cont->container_events.empty()){
            cont->demuxer(cont->container_events.front());
            cont->container_events.pop();
        }
    }
}
void ThreadQueue::enqueue(PajeEvent *event, PajeContainer *container) 
{
    container->container_events.push(event);
    this->containers_set.insert(container); 
}  
void ThreadQueue::setQueue()
{
     this->t1 = std::thread(&ThreadQueue::startWork,this);
     this->t2 = std::thread(&ThreadQueue::startWork,this);
     this->t1.join();
     this->t2.join();
}

ThreadQueue::~ThreadQueue()
{ 
}