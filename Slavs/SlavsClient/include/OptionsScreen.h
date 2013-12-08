#ifndef OptionsScreen_h
#define OptionsScreen_h

#include "ScreenBase.h"

class OptionsScreen : public ScreenBase
{
public:
	OptionsScreen();
	~OptionsScreen();

	void Init();
	void Update(float elapsedTime);
};

#endif