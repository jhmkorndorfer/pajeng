#include "PajeParallelProcessor.h"
#include "PajeUnity.h"

PajeParallelProcessor::PajeParallelProcessor() : stop(false), start(false) {
  return;
}

void PajeParallelProcessor::startWork(int nt) {
  for (int xx = 0; xx < nt; xx++) {
    //Creating the workers pool
    workers.emplace_back(
      [this] 
      {
        for (;;) {
          std::unique_lock<std::mutex> lkUnity(this->unity_mutex);
          while (!this->stop && this->fileNameSet.empty()) {
                  this->cvUnity.wait(lkUnity);
                }
          
                if (this->stop && this->fileNameSet.empty()) {
                  return;
                }
          
                std::set<std::string>::iterator it_file;
                it_file = this->fileNameSet.begin();
                std::string file = *it_file;
                PajeContainer *container = contUnMap[file];
                this->fileNameSet.erase(it_file);
                lkUnity.unlock();
                this->cvUnity.notify_one();
                  //Worker processing a new UNITY with part a of the trace file
                    PajeUnity *unity = new PajeUnity(0,
                    0,
                    file,
                    0,
                    0,
                    0,
                    false,
                    container,
                    typeMapUnity);
        }
      }
    );
  }

}

void PajeParallelProcessor::enqueue(std::string fileName, PajeContainer *contUn) {

  contUnMap[fileName] = contUn;
  std::unique_lock<std::mutex> lkUnity(this->unity_mutex);
  this->fileNameSet.insert(fileName);
  lkUnity.unlock();
  this->cvUnity.notify_one();

}

void PajeParallelProcessor::setQueue(std::map<std::string, PajeType*> typeMapUn, int nt) {
  this->typeMapUnity = typeMapUn;
  this->stop = false;
  startWork(nt);
}

PajeParallelProcessor::~PajeParallelProcessor() {
  this->stop = true;
  for (size_t i = 0; i < workers.size(); ++i) {
    workers[i].join();
  }
}