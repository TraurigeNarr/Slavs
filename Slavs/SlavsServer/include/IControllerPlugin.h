#pragma once
#include "SlavsServerAPI.h"
#include "NotificationData.h"

/*
Provide interface for extension of controller.
AI controller contains list of all plugins and invokes
there methods. All modules must be in dynamic libraries (dlls) and be near executable.
*/
class SLAVS_SERVER_EXPORT IControllerPlugin
{
public:
  virtual ~IControllerPlugin(){}
  //response if AI module ready for update
  virtual bool Ready() = 0;
  //invokes each tick if module is ready
  virtual void Update(long i_elapsed_time) = 0;
  //notify module about changes in game world
  virtual void Notify(INotificationData* ip_notification) = 0;
  //specific actions on initializing module
  virtual void Initialize() = 0;
};