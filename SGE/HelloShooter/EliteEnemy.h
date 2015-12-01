#ifndef INCLUDED_ELITEENEMY_H
#define INCLUDED_ELITEENEMY_H

#include <SGE.h>
using namespace SGE;

#include "Enemy.h"

class EliteEnemy : public Enemy
{
public:
	EliteEnemy();
	virtual ~EliteEnemy();

	virtual void Load();
	virtual void Unload();
	virtual bool Update(float deltaTime);
	virtual void Render();
	
	virtual void Spawn();
	virtual bool checkfire();
	virtual bool Kill();

private:
	int mHits;
};

#endif // #ifndef INCLUDED_ELITEENEMY_H