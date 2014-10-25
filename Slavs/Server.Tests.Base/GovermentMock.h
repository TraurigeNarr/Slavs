#pragma once

#include <SlavsServer/include/IController.h>

#include <SlavsServer/include/Management/Goverment.h>

#include <SlavsServer/include/Management/IMilitaryManager.h>
#include <SlavsServer/include/Management/IRelashionshipsManager.h>
#include <SlavsServer/include/Management/ITechnologyManager.h>

#include "EconomyManagerMock.h"
#include "StoreSystemMock.h"
#include "SocietyManagerMock.h"

class MockGoverment : public Goverment
  {
  public:
    MockGoverment(IController* ip_owner)
      : Goverment(ip_owner,
                  std::unique_ptr<IEconomyManager>(new MockIEconomyManager()),
                  std::unique_ptr<IMilitaryManager>(),
                  std::unique_ptr<IRelashionshipsManager>(),
                  std::unique_ptr<ISocietyManager>(new MockISocietyManager()),
                  std::unique_ptr<ITechnologyManager>())
      {      }
  };