#ifndef INCLUDED_ELITEENEMY3_H
#define INCLUDED_ELITEENEMY3_H

#include <SGE.h>
using namespace SGE;

#include "Enemy.h"

class EliteEnemy3 : public Enemy
{
public:
	EliteEnemy3();
	virtual ~EliteEnemy3();

	virtual void Load();
	virtual void Unload();
	virtual bool Update(float deltaTime);
	virtual void Render();

	virtual void Spawn();
	virtual bool checkfire3();
	virtual bool Kill();

private:
	int mHits;
};

#endif 