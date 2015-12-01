#ifndef INCLUDED_WORLD_H
#define INCLUDED_WORLD_H

#include <SGE.h>
using namespace SGE;

#include "Bullet.h"
#include "Collidable.h"
#include "Enemy.h"
#include "Genkidama.h"
#include "EliteEnemy.h"
#include "EliteEnemy2.h"
#include "EliteEnemy3.h"
#include "EnemyBullets1.h"
#include "EnemyBullets2.h"
#include "Fighter.h"
#include "GameoverUI.h"

class World
{
public:
	World();
	~World();

	void Activate();
	void Deactivate();

	void Initialize();
	void Terminate();
	void Update(float dt);
	void Render(float dt);

	bool IsActive() const { return mActive; }

private:
	void UpdateGameObjects(float deltaTime);
	void SpawnEnemies(float deltaTime);
	void FireBullets();
	void CheckCollision();

	static const int kMaxBullets = 5000;
	static const int MaxGenkidama = 1;
	static const int MaxEnemyBullets = 1000;
	static const int MaxEnemyBullets2 = 1;
	Bullet myBullets[kMaxBullets];
	Collidable mCollidables[kMaxBullets];
	Genkidama mGenkidama[MaxGenkidama];
	EnemyBullets1 mEnemyBullets1[MaxEnemyBullets];
	EnemyBullets2 mEnemyBullets2[MaxEnemyBullets2];

	int myBulletIndex;
	int mCollidableIndex;
	int mGenkidamaIndex;
	int mEnemyBullets1Index;
	int mEnemyBullets2Index;

	//Collidable mCollidable;

	static const int kMaxEnemies = 10;
	Enemy* myEnemies[kMaxEnemies];
	int myEnemyIndex;

	static const int kMaxExplosions = 10;
	SGE_Emitter myExplosions[kMaxExplosions];
	int myExplosionIndex;

	Fighter myFighter;
	GameoverUI GU;

	SGE_Sprite myBackground;
	SGE_Sound myFireSound1;
	SGE_Sound myExplosionSound;
	SGE_Sound myFireSound2;
	SGE_Font Score;
	SGE_Font HighestScore;


	int mKills;
	int mBossIndex;

	float spawnTimer;

	bool mActive;
};

#endif // #ifndef INCLUDED_WORLD_H