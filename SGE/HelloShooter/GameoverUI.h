#ifndef INCLUDED_GAMEOVERUI_H
#define INCLUDED_GAMEOVERUI_H

#include <SGE.h>
using namespace SGE;
#include "GameStates.h"

class GameoverUI
{
public:
	GameoverUI();
	~GameoverUI();

	void Activate();
	void Deactivate();

	// Implements BaseState
	void Load();
	void Unload();
	GameState Update(float deltaTime);
	void Render();

private:
	SGE_Cursor mCursor;
	SGE_Font mFont;
	SGE_Button mStartButton;
	/*SGE_Button mLevelButton1;
	SGE_Button mLevelButton2;
	SGE_Button mLevelButton3;*/
	SGE_Button mQuitButton;
	SGE_Sprite background;

	bool mActive;
};

#endif 