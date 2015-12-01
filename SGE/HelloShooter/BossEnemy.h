#ifndef INCLUDED_BOSSENEMY_H
#define INCLUDED_BOSSENEMY_H

#include <SGE.h>
using namespace SGE;

#include "Enemy.h"

class BossEnemy : public Enemy
{
public:
	BossEnemy();
	virtual ~BossEnemy();

	virtual void Load();
	virtual void Unload();
	virtual bool Update(float deltaTime);
	virtual void Render();
	
	virtual void Spawn();
	virtual bool checkfire4();
	virtual bool Kill();

private:
	int mHits;
};

#endif // #ifndef INCLUDED_BOSSENEMY_H