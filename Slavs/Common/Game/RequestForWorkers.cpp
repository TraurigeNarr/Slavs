#include "RequestForWorkers.h"

#include "Enumerations.h"

#include "../Utilities/TemplateFunctions.h"

#include <cassert>

RequestForWorkers::RequestForWorkers()
  : otNeededType(OT_None), fNeededSkill(0.5f), fAverageSkill(0.0f),
  bAvailability(true), bNeedStore(false)
{}

int RequestForWorkers::NeededSize()
{
  return sizeof(int) + sizeof(float) + 2*sizeof(bool);
}

int RequestForWorkers::Serialize(char *buf_end, int size) const
{

  ToChar(otNeededType, buf_end, sizeof(int));
  buf_end += sizeof(int);
  ToChar(bAvailability,buf_end,sizeof(bool));
  buf_end += sizeof(bool);
  ToChar(bNeedStore,buf_end,sizeof(bool));
  buf_end += sizeof(bool);
  if(bAvailability || bNeedStore)
  {
    ToChar(fNeededSkill, buf_end, sizeof(float));
    buf_end += sizeof(float);
  }
  else
  {
    ToChar(fAverageSkill, buf_end, sizeof(float));
    buf_end += sizeof(float);
  }
  return NeededSize();
}

int RequestForWorkers::Deserialize(char* buf)
{
  char *initial_position = buf;
  otNeededType = FromChar<int>(buf);
  buf += sizeof(int);
  bAvailability = FromChar<bool>(buf);
  buf += sizeof(bool);
  bNeedStore = FromChar<bool>(buf);
  buf += sizeof(bool);
  if(bAvailability)
  {
    fNeededSkill = FromChar<float>(buf);
    buf += sizeof(float);
    fAverageSkill = .0f;
  }
  else
  {
    fAverageSkill = FromChar<float>(buf);
    buf += sizeof(float);
    fNeededSkill = .0f;
  }
  assert(NeededSize() == (buf - initial_position));
  return buf - initial_position;
}