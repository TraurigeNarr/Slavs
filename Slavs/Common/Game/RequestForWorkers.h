#ifndef RequestForWorkers_h
#define RequestForWorkers_h

#include "GameResources.h"
#include <map>

struct RequestForWorkers
{
public:
  RequestForWorkers();

  int			    otNeededType;
  float				fNeededSkill;
  float       fAverageSkill;
  //may be for type processing while choosing work
  GameResourceType	grType;
  size_t				uiPayment;
  bool				bNeedStore;

  //for calculating relational payment, updated by each manufacture on creating
  static size_t		uiMaxPayment;
  bool bAvailability;
public:
  float               NeededSkill() const { return fNeededSkill; }
  float               AverageSkill() const { return fAverageSkill; }
  GameResourceType    ResourceType() const { return grType; }
  size_t              Payment() const { return uiPayment; }
  float               RelativePayment() const { return uiMaxPayment/(uiPayment*1.f); }
  bool                NeedStore() const { return bNeedStore; }

  int									Serialize(char *buf_end, int size) const;
  int									Deserialize(char* buf);

  static int					NeededSize();
};

#endif