#include "ThreadQueue.h"

ThreadQueue::ThreadQueue() :   stop(false),start(false)
{
    return;
}
void ThreadQueue::startWork()
{
     
    while(true)
            {
                std::unique_lock<std::mutex> lkcont(this->containers_mutex);
                while(!this->stop && this->containers_set.empty()){
                       this->cvCont.wait(lkcont);
                }
                if(this->stop && this->containers_set.empty()){
                   return;
                }
                std::set<PajeContainer*>::iterator it_container;
                    it_container = this->containers_set.begin();
                    PajeContainer *cont = *it_container;
                    this->containers_set.erase(it_container);
                lkcont.unlock();

                std::unique_lock<std::mutex> lkevent(this->events_mutex);
                this->cvEvent.wait(lkevent);
                    while(!cont->container_events.empty()){
                        cont->demuxer(cont->container_events.front());
                        //PajeEvent *event = it_container->container_events.front();
                        cont->container_events.pop();
                    }
                lkevent.unlock(); 

            }
}
void ThreadQueue::enqueue(PajeEvent *event, PajeContainer *container) 
{
    std::unique_lock<std::mutex> lkcont(this->containers_mutex);
        this->containers_set.insert(container);
        lkcont.unlock();
        this->cvCont.notify_one();
    std::unique_lock<std::mutex> lkevent(this->events_mutex);
        container->container_events.push(event);
        lkevent.unlock();
        cvEvent.notify_one();
      
}  

void ThreadQueue::setQueue()
{
     this->stop = false;
     this->t1 = std::thread(&ThreadQueue::startWork,this);
     this->t2 = std::thread(&ThreadQueue::startWork,this);
     this->t3 = std::thread(&ThreadQueue::startWork,this);
}

ThreadQueue::~ThreadQueue()
{
    this->stop = true;
    this->cvEvent.notify_all();
    this->cvCont.notify_all();
    this->t1.join();
    this->t2.join();
    this->t3.join();
    exit(1);
}