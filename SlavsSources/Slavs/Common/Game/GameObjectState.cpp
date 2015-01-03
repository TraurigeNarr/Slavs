#include "Enumerations.h"
#include "GameObjectState.h"
#include "../Utilities/TemplateFunctions.h"
#include "../Math/Vector2D.h"

GameObjectState::GameObjectState()
	: m_iNeededSize(0), pPosition(NULL), iFlags(0), request_for_workers(NULL), 
  containersCount(0), oType(OT_None), iMask(0), uiSkill(0), iOwnerMask(0),
	informationToShow(InformationToShow::None), informationId(-1)
{}

GameObjectState::~GameObjectState()
{
	if(NULL != pPosition)
		delete pPosition;
  if(NULL != request_for_workers)
    delete request_for_workers;
}

int GameObjectState::Serialize(char* buf_end, int size) const
{
	if(0 == m_iNeededSize)
		NeededSize();
	//check if there is enough buffer size
	if(m_iNeededSize > size || NULL == buf_end)
		return 0;
	//serialize id
	ToChar(lID, buf_end, sizeof(long));
	buf_end += sizeof(long);
	//serialize flags
	ToChar(iFlags, buf_end, sizeof(GOStateFlags));
	buf_end += sizeof(GOStateFlags);
	if(0 != (iFlags & GOF_Destroyed))
		return m_iNeededSize;
	//serialize type
	ToChar(oType, buf_end, sizeof(int));
	buf_end += sizeof(int);
	//serialize mask
	ToChar(iMask, buf_end, sizeof(int));
	buf_end += sizeof(int);
  // serialize owner nask
  ToChar(iOwnerMask, buf_end, sizeof(int));
  buf_end += sizeof(int);
#pragma region Data
	//serialize position
	if(0 != (iFlags & GOF_Position))
	{
		//pass to serialize function pointer to the end of array and function shifts it
		//so that bug_end will points to the next byte after class data.
		pPosition->Serialize(buf_end);
		buf_end += Vector2D::NeededSize();
	}
	//serialize containers
	if(0 != (iFlags & GOF_Resources))
	{
		ToChar(vContainers.size(), buf_end, sizeof(size_t));
		buf_end += sizeof(size_t);
		for(size_t i = 0; i < vContainers.size(); ++i)
		{
			vContainers[i]->Serialize(buf_end, m_iNeededSize);
			buf_end += GameResourceContainer::NeededSize();
		}
	}
	//serialize number of workers
	if(0 != (iFlags & GOF_HasWorkers))
	{
		ToChar(iWorkersNumber, buf_end, sizeof(size_t));
		buf_end += sizeof(size_t);
	}
	//serialize human`s skill
	if(0 != (iFlags & GOF_HasSkill))
	{
		ToChar(uiSkill, buf_end, sizeof(size_t));
		buf_end += sizeof(size_t);
	}
  if(0 != (iFlags & GOF_Money))
  {
    ToChar(uiMoney, buf_end, sizeof(size_t));
    buf_end += sizeof(size_t);
  }
  if(NULL != request_for_workers && 0 != (iFlags & GOF_RequestForWorkers))
  {
    buf_end += request_for_workers->Serialize(buf_end, m_iNeededSize);
  }
  if(0 != (iFlags & GOF_HouseInformation))
  {
    //number of inhabitants
    ToChar(uiInhabitantNumber, buf_end, sizeof(size_t));
    buf_end += sizeof(size_t);
    //max number
    ToChar(uiInhabitantsMaxNumber, buf_end, sizeof(size_t));
    buf_end += sizeof(size_t);
    //number of working
    ToChar(uiInhabitantsWorking, buf_end, sizeof(size_t));
    buf_end += sizeof(size_t);
  }


	ToChar(informationToShow, buf_end, sizeof(InformationToShow));
	buf_end += sizeof(InformationToShow);
	if (informationToShow != InformationToShow::None)
		{
		ToChar(informationId, buf_end, sizeof(int));
		buf_end += sizeof(int);
		}

#pragma endregion
	return m_iNeededSize;
}

int GameObjectState::Deserialize(char* buf)
{
	int deserializeCount = 0;

	if(NULL == buf)
		return 0;
	//deserialize id
	lID = FromChar<long>(buf);
	deserializeCount += sizeof(long);
	//deserialize flags
	iFlags = FromChar<GOStateFlags>(buf + deserializeCount);
	deserializeCount += sizeof(GOStateFlags);
	if(0 != (iFlags & GOF_Destroyed))
		return deserializeCount;
	//deserialize type
	oType = FromChar<int>(buf + deserializeCount);
	deserializeCount += sizeof(int);
	//deserialize mask
	iMask = FromChar<int>(buf + deserializeCount);
	deserializeCount += sizeof(int);
  // deserialize owner mask
  iOwnerMask = FromChar<int>(buf + deserializeCount);
  deserializeCount += sizeof(int);
#pragma region Data
	//deserialize position
	if(0 != (iFlags & GOF_Position))
	{
		pPosition = new Vector2D();
		deserializeCount += pPosition->Deserialize(buf + deserializeCount);
	}
	//deserialize resource containers
	if(0 != (iFlags & GOF_Resources))
	{
		//deserialize count
		size_t iCount = 0;
		iCount = FromChar<size_t>(buf + deserializeCount);
		deserializeCount += sizeof(size_t);

		for(size_t i = 0; i < iCount; ++i)
		{
			std::shared_ptr<GameResourceContainer> newContainer = std::make_shared<GameResourceContainer>(GameResourceContainer(GR_None, 0));
			newContainer->Deserialize(buf + deserializeCount);
			deserializeCount += GameResourceContainer::NeededSize();
			vContainers.push_back(newContainer);
		}
	}
	//deserialize number of workers
	if(0 != (iFlags & GOF_HasWorkers))
	{
		iWorkersNumber = FromChar<size_t>(buf + deserializeCount);
		deserializeCount += sizeof(size_t);
	}
	//deserialize skill
	if(0 != (iFlags & GOF_HasSkill))
	{
		uiSkill = FromChar<size_t>(buf + deserializeCount);
		deserializeCount += sizeof(size_t);
	}
  if(0 != (iFlags & GOF_Money))
  {
    uiMoney = FromChar<size_t>(buf + deserializeCount);
    deserializeCount += sizeof(size_t);
  }
  if(0 != (iFlags & GOF_RequestForWorkers))
  {
    request_for_workers = new RequestForWorkers();
    deserializeCount += request_for_workers->Deserialize(buf + deserializeCount);
  }
  if(0 != (iFlags & GOF_HouseInformation))
  {
    //number
    uiInhabitantNumber = FromChar<size_t>(buf + deserializeCount);
    deserializeCount += sizeof(size_t);
    //max
    uiInhabitantsMaxNumber = FromChar<size_t>(buf + deserializeCount);
    deserializeCount += sizeof(size_t);
    //working
    uiInhabitantsWorking = FromChar<size_t>(buf + deserializeCount);
    deserializeCount += sizeof(size_t);
  }

	informationToShow = static_cast<InformationToShow>(FromChar<int>(buf + deserializeCount));
	deserializeCount += sizeof(InformationToShow);
	if (informationToShow != InformationToShow::None)
		{
		informationId = FromChar<int>(buf + deserializeCount);
		deserializeCount += sizeof(int);
		}

#pragma endregion

	return deserializeCount;
}

int GameObjectState::NeededSize() const
{
	//if the object should be destroyed it needs only two fields -- > id and flags
	if(0 != (iFlags & GOF_Destroyed))
	{
		m_iNeededSize = sizeof(long)//id
			+ sizeof(GOStateFlags);//flags
		return m_iNeededSize;
	}
	m_iNeededSize = sizeof(long)//id
		+ sizeof(GOStateFlags)//flags
		+ sizeof(int)//type
		+ sizeof(int)//mask
    + sizeof(int);//owner mask

	//calculates needed size
	if(0 != (iFlags & GOF_Position))
		m_iNeededSize += Vector2D::NeededSize();

	//calculates for containers
	if(0 != (iFlags & GOF_Resources))
	{
		m_iNeededSize += sizeof(size_t);//for count
		m_iNeededSize += GameResourceContainer::NeededSize()*vContainers.size();//for each container
	}
	//calculate for number of workers
	if(0 != (iFlags & GOF_HasWorkers))
	{
		m_iNeededSize += sizeof(size_t);
	}
	//calculate for humans skill
	if(0 != (iFlags & GOF_HasSkill))
	{
		m_iNeededSize += sizeof(size_t);
	}
  if(0 != (iFlags & GOF_RequestForWorkers))
    m_iNeededSize += RequestForWorkers::NeededSize();
  if(0 != (iFlags & GOF_Money))
    m_iNeededSize += sizeof(size_t);
  if(0 != (iFlags & GOF_HouseInformation))
    m_iNeededSize += 3*sizeof(size_t);

	if (informationToShow == InformationToShow::None)
		m_iNeededSize += sizeof(InformationToShow);
	else
		m_iNeededSize += sizeof(InformationToShow) + sizeof(int);

	return m_iNeededSize;
}