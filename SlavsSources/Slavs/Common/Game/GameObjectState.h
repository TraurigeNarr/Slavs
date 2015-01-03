#ifndef GameObjectState_h
#define GameObjectState_h

#include "Enumerations.h"
#include "GameResources.h"
#include "RequestForWorkers.h"

#include <vector>
#include <memory>

#define MAX_GOS_SIZE 24 //id + flags + type + mask + sizeof(Vector2D)

/*
-------------------------------------------------------------------
Serializable class. The order of serialization:
0. ID
1. Flags
2. Type
3. Mask
4. Position
5. containers count
6. containers
7. number of workers
8. human`s skill
9. money
10.request for workers
11.general number of humans in house
12.number of max inhabitants of house
13.number of working humans
-------------------------------------------------------------------
*/

class Vector2D;

typedef int GOStateFlags;
enum
{
	GOF_Position = 1 << 15,
	GOF_Destroyed = 1 << 14,
	GOF_ChangeType = 1 << 13,
	GOF_Resources = 1 << 12,
	GOF_HasWorkers = 1 << 11,
	GOF_Money = 1 << 10,
	GOF_Working = 1<<9,
	GOF_Waiting = 1<<8,
  GOF_RequestForWorkers = 1<<7,
  GOF_HasSkill = 1<<6,
  GOF_HouseInformation = 1 << 5,
	GOF_Selected = 1 << 4
};

enum class InformationToShow
	{
	None,
	Information,
	Dialog
	};

class GameObjectState
{
	mutable int                           m_iNeededSize;
public:
	long								                  lID;
	GOStateFlags						              iFlags;
	int							                      oType;
	int									                  iMask;
  int                                   iOwnerMask;
	Vector2D*							                pPosition;
	int									                  containersCount;
	TResourceContainers                   vContainers;
	size_t								                iWorkersNumber;
	size_t								                uiSkill;
  size_t                                uiMoney;
  RequestForWorkers*                    request_for_workers;
  size_t                                uiInhabitantNumber;
  size_t                                uiInhabitantsMaxNumber;
  size_t                                uiInhabitantsWorking;
	
	InformationToShow											informationToShow;
	int																		informationId;

										GameObjectState();
										~GameObjectState();
	
	int									NeededSize() const;

	int									Serialize(char *buf_end, int size) const;
	int									Deserialize(char* buf);
};

#endif