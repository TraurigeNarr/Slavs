#ifndef RequestForWorkers_h
#define RequestForWorkers_h

#include "Enumerations.h"
#include "GameResources.h"
#include <map>

struct RequestForWorkers
{
  RequestForWorkers();

  ObjectType			otNeededType;
  float				fNeededSkill;
  float       fAverageSkill;
  //may be for type processing while choosing work
  GameResourceType	grType;
  size_t				uiPayment;
  bool				bAvailability;
  bool				bNeedStore;

  //for calculating relational payment, updated by each manufacture on creating
  static size_t		uiMaxPayment;


  static int					NeededSize();

  int									Serialize(char *buf_end, int size) const;
  int									Deserialize(char* buf);
};

#endif