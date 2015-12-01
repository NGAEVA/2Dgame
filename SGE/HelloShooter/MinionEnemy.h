#ifndef INCLUDED_MINIONENEMY_H
#define INCLUDED_MINIONENEMY_H

#include <SGE.h>
using namespace SGE;

#include "Enemy.h"

class MinionEnemy : public Enemy
{
public:
	MinionEnemy();
	virtual ~MinionEnemy();

	virtual void Load();
	virtual void Unload();
	virtual bool Update(float deltaTime);
	virtual void Render();
	
	virtual void Spawn();
	virtual bool Kill();

private:
};

#endif // #ifndef INCLUDED_MINIONENEMY_H