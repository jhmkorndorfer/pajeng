#include <memory>
#include <thread>
#include <set>
#include <vector>
#include <map>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include "PajeFileReader.h"
#include "PajeType.h"
#include "PajeContainer.h"
#ifndef PAJEPARALLELPROCESSOR_H
#define	PAJEPARALLELPROCESSOR_H

class PajeParallelProcessor {
public:
    PajeParallelProcessor();
    void enqueue(std::string fileName, PajeContainer *contUn); 
    void startWork(int nt);
    void setQueue(std::map<std::string,PajeType*> typeMapUn, int nt);
     ~PajeParallelProcessor();
public:
    std::set<std::string> fileNameSet;
    std::map<std::string,PajeContainer*> contUnMap;
    std::map<std::string,PajeType*> typeMapUnity;
    std::vector<std::thread> workers;
    std::mutex unity_mutex;
    std::condition_variable cvUnity;
    bool stop;
    bool start;
};

#endif	/* PAJEPARALLELPROCESSOR_H */