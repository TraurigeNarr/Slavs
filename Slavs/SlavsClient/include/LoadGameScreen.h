#ifndef LoadGameScreen_h
#define LoadGameScreen_h

#include "ScreenBase.h"

class LoadGameScreen : public ScreenBase
{
public:
	LoadGameScreen();
	~LoadGameScreen();

	void Init();
	void Update(float elapsedTime);
};

#endif