#ifndef __PAJESIMULATOR_H__
#define __PAJESIMULATOR_H__
#include <map>
#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PajeTraceController.h"
#include "PajeComponent.h"
#include "PajeEvent.h"
#include "PajeType.h"

#define CALL_MEMBER_PAJE_SIMULATOR(object,ptr) ((object).*(ptr))

class PajeContainer {
public:
  std::string name;
  std::string alias;
public:
  PajeContainer *parent;
  PajeType *type;

  std::map<std::string,PajeContainer*> children;

  //keeps the values of variables
  std::map<PajeType*,std::vector<double> > variables;
  std::map<PajeType*,std::vector<double> > states;
  std::map<PajeType*,std::vector<double> > events;

public:
  PajeContainer (std::string name, std::string alias, PajeContainer *parent, PajeType *type);
  PajeContainer *getRoot (void);
  PajeContainer *addContainer (std::string name, std::string alias, PajeType *type);
  std::string identifier (void);
};

std::ostream &operator<< (std::ostream &output, const PajeContainer &container);

class PajeSimulator : public PajeComponent {
private:
  PajeContainerType *rootType;
  PajeContainer *root;
  std::map<std::string,PajeType*> typeMap;
  std::map<std::string,PajeContainer*> contMap;

public:
  PajeSimulator();
  
  void inputEntity (PajeObject *data);
  bool canEndChunkBefore (PajeObject *data);

  void startChunk (int chunkNumber);
  void endOfChunkLast (bool last);

private:
  void (PajeSimulator::*invocation[PajeEventIdCount])(PajeEvent*);

  void pajeDefineContainerType (PajeEvent *event);
  void pajeDefineLinkType (PajeEvent *event);
  void pajeDefineEventType (PajeEvent *event);
  void pajeDefineStateType (PajeEvent *event);
  void pajeDefineVariableType (PajeEvent *event);
  void pajeDefineEntityValue (PajeEvent *event);

  void pajeCreateContainer (PajeEvent *event);
  void pajeDestroyContainer (PajeEvent *event);

  void pajeNewEvent (PajeEvent *event);

  void pajeSetState (PajeEvent *event);
  void pajePushState (PajeEvent *event);
  void pajePopState (PajeEvent *event);

  void pajeSetVariable (PajeEvent *event);
  void pajeAddVariable (PajeEvent *event);
  void pajeSubVariable (PajeEvent *event);

  void pajeStartLink (PajeEvent *event);
  void pajeEndLink (PajeEvent *event);
};
#endif
