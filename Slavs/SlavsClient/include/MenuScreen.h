#ifndef MenuScreen_h
#define MenuScreen_h

#include "ScreenBase.h"

class MenuScreen : public ScreenBase
{
public:
	MenuScreen();
	~MenuScreen();

	void Init();
	void Update(float elapsedTime);
};

#endif