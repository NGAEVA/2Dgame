#ifndef INCLUDED_ELITEENEMY2_H
#define INCLUDED_ELITEENEMY2_H

#include <SGE.h>
using namespace SGE;

#include "Enemy.h"

class EliteEnemy2 : public Enemy
{
public:
	EliteEnemy2();
	virtual ~EliteEnemy2();

	virtual void Load();
	virtual void Unload();
	virtual bool Update(float deltaTime);
	virtual void Render();

	virtual void Spawn();
	virtual bool checkfire2();
	virtual bool Kill();

private:
	int mHits;
};

#endif 