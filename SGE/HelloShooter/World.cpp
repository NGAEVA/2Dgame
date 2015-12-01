#include "World.h"

#include "MinionEnemy.h"
#include "EliteEnemy.h"
#include "BossEnemy.h"

World::World()
	: myBulletIndex(0)
	, mCollidableIndex(0)
	, mGenkidamaIndex(0)
	, mEnemyBullets1Index(0)
	, mEnemyBullets2Index(0)
	, myEnemyIndex(0)
	, myExplosionIndex(0)
	, mKills(0)
	, mBossIndex(-1)
	, spawnTimer(0.0f)
	, mActive(false)
{
}

World::~World()
{
}

void World::Activate()
{
	mActive = true;
}

void World::Deactivate()
{
	mActive = false;
}

void World::Initialize()
{
	for (int i = 0; i < kMaxEnemies; ++i)
	{
		myEnemies[i] = nullptr;
	}
	for (int i = 0; i < kMaxBullets; ++i)
	{
		myBullets[i].Load();
	}
	for (int i = 0; i < kMaxBullets; ++i)
	{
		mCollidables[i].Load();
	}
	for (int i = 0; i < MaxEnemyBullets; ++i)
	{
		mEnemyBullets1[i].Load();
	}
	for (int i = 0; i < MaxEnemyBullets2; ++i)
	{
		mEnemyBullets2[i].Load();
	}
	for (int i = 0; i < MaxGenkidama; ++i)
	{
		mGenkidama[i].Load();
	}
	for (int i = 0; i < kMaxExplosions; ++i)
	{
		myExplosions[i].Load("explosion.txt");
	}
	myFighter.Load();
	Score.Load(30);
	HighestScore.Load(30);
	myBackground.Load("namek_water.jpg");
	myFireSound1.Load("photongun1.wav");
	myFireSound2.Load("deathball_fire.wav");
	myExplosionSound.Load("explosion.wav");

	//mCollidable.Load();

	const int kWinWidth = IniFile_GetInt("WinWidth", 800);
	const int kWinHeight = IniFile_GetInt("WinHeight", 600);
	const float x = kWinWidth * 0.5f;
	const float y = kWinHeight * 0.75f;
	myFighter.SetPosition(Vector2(x, y));
	myFighter.SetSpeed(SGE::IniFile_GetFloat("DefaultSpeed", 200.0f));
}

void World::Terminate()
{
	//mCollidable.Unload();

	for (int i = 0; i < kMaxEnemies; ++i)
	{
		if(myEnemies[i] != nullptr)
		{
			myEnemies[i]->Unload();
			delete myEnemies[i];
		}
	}
	for (int i = 0; i < kMaxBullets; ++i)
	{
		myBullets[i].Unload();
	}
	for (int i = 0; i < kMaxBullets; ++i)
	{
		mCollidables[i].Unload();
	}
	for (int i = 0; i < MaxGenkidama; ++i)
	{
		mGenkidama[i].Unload();
	}
	for (int i = 0; i < MaxEnemyBullets; ++i)
	{
		mEnemyBullets1[i].Unload();
	}
	for (int i = 0; i < MaxEnemyBullets2; ++i)
	{
		mEnemyBullets2[i].Unload();
	}
	for (int i = 0; i < kMaxExplosions; ++i)
	{
		myExplosions[i].Unload();
	}
	myFighter.Unload();
	Score.Unload();
	HighestScore.Unload();
	myBackground.Unload();
	myFireSound1.Unload();
	myFireSound2.Unload();
	myExplosionSound.Unload();
}

void World::Update(float dt)
{
	if(mActive == true)
	{
		UpdateGameObjects(dt);
		SpawnEnemies(dt);
		FireBullets();
		CheckCollision();
	}
}

void World::Render(float dt)
{
	if(mActive == true)
	{
		myBackground.Render();

		for (int i = 0; i < kMaxEnemies; ++i)
		{
			if(myEnemies[i] != nullptr)
			{
				myEnemies[i]->Render();
			}
		}

		for (int i = 0; i < kMaxBullets; ++i)
		{
			myBullets[i].Render();
		}

		for (int i = 0; i < kMaxBullets; ++i)
		{
			mCollidables[i].Render();
		}
		for (int i = 0; i < MaxGenkidama; ++i)
		{
			mGenkidama[i].Render();
		}
		for (int i = 0; i < MaxEnemyBullets; ++i)
		{
			mEnemyBullets1[i].Render();
		}
		for (int i = 0; i < MaxEnemyBullets2; ++i)
		{
			mEnemyBullets2[i].Render();
		}
		//mCollidable.Render();

		myFighter.Render();

		for (int i = 0; i < kMaxExplosions; ++i)
		{
			myExplosions[i].Render(true);
		}

		char score[64];
		sprintf_s(score, "Score: %d", mKills);
		Score.SetColor(0,120,168);
		Score.Print(score,850,20);

		int highestsocre=10;
		if(highestsocre <= mKills)
		{
			sprintf_s(score, "Highest Score: %d", mKills);
			HighestScore.SetColor(0,120,168);
			HighestScore.Print(score,400,20);
		}

		else
		{
			sprintf_s(score, "Highest Score: %d", highestsocre);
			HighestScore.SetColor(0,120,168);
			HighestScore.Print(score,400,20);
		}
	}

}

void World::UpdateGameObjects(float deltaTime)
{
	for (int i = 0; i < kMaxEnemies; ++i)
	{
		if(myEnemies[i] != nullptr)
		{
			bool isActive = myEnemies[i]->Update(deltaTime);
			if(isActive == false)
			{
				if(mBossIndex == i)
				{
					mBossIndex = -1;
				}
				myEnemies[i]->Unload();
				delete myEnemies[i];
				myEnemies[i] = nullptr;
			}
		}
	}
	for (int i = 0; i < kMaxBullets; ++i)
	{
		myBullets[i].Update(deltaTime);
	}
	for (int i = 0; i < kMaxBullets; ++i)
	{
		mCollidables[i].Update(deltaTime);
	}
	for (int i = 0; i < MaxGenkidama; ++i)
	{
		mGenkidama[i].Update(deltaTime);
	}
	for (int i = 0; i < MaxEnemyBullets; ++i)
	{
		mEnemyBullets1[i].Update(deltaTime);
	}
	for (int i = 0; i < MaxEnemyBullets2; ++i)
	{
		mEnemyBullets2[i].Update(deltaTime);
	}
	for (int i = 0; i < kMaxExplosions; ++i)
	{
		myExplosions[i].Update(deltaTime);
	}
	//mCollidable.Update(deltaTime);
	myFighter.Update(deltaTime);
}

void World::SpawnEnemies(float deltaTime)
{
	spawnTimer -= deltaTime;
	if (spawnTimer <= 0.0f)
	{
		const int index = myEnemyIndex;
		if (myEnemies[index] == nullptr)
		{
			if(mBossIndex == -1)
			{
				if(mKills % SGE::IniFile_GetInt("MinionsBeforeBoss", 10) == 0 && mKills >0)
				{
					myEnemies[index] = new BossEnemy();
					mBossIndex = index;
				}
				else
				{
					int randomNumber = Random_GetInt(0, 20);
					
					if (randomNumber == 4 || randomNumber == 5 || randomNumber == 6)
					{
						myEnemies[index] = new EliteEnemy();
					}
					else if (randomNumber == 10 || randomNumber == 11)
					{
						myEnemies[index] = new EliteEnemy2();
					}

					else if (randomNumber == 18)
					{
						myEnemies[index] = new EliteEnemy3();
					}

					else
					{
						myEnemies[index] = new MinionEnemy();
					}
				}

				myEnemies[index]->Load();
				myEnemies[index]->Spawn();
				myEnemyIndex = (index + 1) % kMaxEnemies;
			}

			spawnTimer = Random_GetFloat(SGE::IniFile_GetFloat("MinSpawnInterval", 0.1f), SGE::IniFile_GetFloat("MaxSpawnInterval", 0.5f));
		}
	}
}

void World::FireBullets()
{
	if (Input_IsMousePressed(0))
	{
		const int index0 = myBulletIndex;
		const int index1 = (myBulletIndex + 1) % kMaxBullets;
		const int index2 = (myBulletIndex + 2) % kMaxBullets;
		if (!myBullets[index0].IsActive() &&
			!myBullets[index1].IsActive() &&
			!myBullets[index2].IsActive())
		{
			const Vector2 pos0 = myFighter.GetPosition();
			//const Vector2 pos1 = pos0 + Vector2(-20.0f, 20.0f);
			//const Vector2 pos2 = pos0 + Vector2(20.0f, 20.0f);
			const Vector2 vel0(0.0f, -500.0f);
			//const Vector2 vel1(-100.0f, -500.0f);
			//const Vector2 vel2(100.0f, -500.0f);
			myBullets[index0].Fire(pos0, vel0);
			//myBullets[index1].Fire(pos1, vel1);
			//myBullets[index2].Fire(pos2, vel2);
			myBulletIndex = (index2 + 1) % kMaxBullets;
			myFireSound1.Play();
		}
	}

	for (int e = 0; e < kMaxEnemies; ++e)
	{
		if (myEnemies[e] != nullptr && myEnemies[e]->IsActive())
		{
			if(myEnemies[e]->checkfire() == true)
			{
				const int index0 = myEnemyIndex;
				//const int index1 = (myBulletIndex + 1) % kMaxBullets;
				//const int index2 = (myBulletIndex + 2) % kMaxBullets;
				if (!mEnemyBullets1[index0].IsActive() /*&&
					!myBullets[index1].IsActive() &&
					!myBullets[index2].IsActive()*/)
				{
					const Vector2 pos0 = myEnemies[e]->GetPosition();
					//const Vector2 pos1 = pos0 + Vector2(-20.0f, 20.0f);
					//const Vector2 pos2 = pos0 + Vector2(20.0f, 20.0f);
					const Vector2 vel0(0.0f, 500.0f);
					//const Vector2 vel1(-100.0f, -500.0f);
					//const Vector2 vel2(100.0f, -500.0f);
					mEnemyBullets1[index0].Fire(pos0, vel0);
					//myBullets[index1].Fire(pos1, vel1);
					//myBullets[index2].Fire(pos2, vel2);
					//myBulletIndex = (index2 + 1) % kMaxBullets;
					myFireSound1.Play();
				}
			}
		}
	}

	for (int e = 0; e < kMaxEnemies; ++e)
	{
		if (myEnemies[e] != nullptr && myEnemies[e]->IsActive())
		{
			if (myEnemies[e]->checkfire2() == true)
			{
				const int index0 = myEnemyIndex;
				const int index1 = (myEnemyIndex + 1) % kMaxBullets;
				const int index2 = (myEnemyIndex + 2) % kMaxBullets;
				if (!mEnemyBullets1[index0].IsActive() &&
					!mEnemyBullets1[index1].IsActive() &&
					!mEnemyBullets1[index2].IsActive())
				{
					const Vector2 pos0 = myEnemies[e]->GetPosition();
					const Vector2 pos1 = pos0 + Vector2(-20.0f, 20.0f);
					const Vector2 pos2 = pos0 + Vector2(20.0f, 20.0f);
					const Vector2 vel0(0.0f, 500.0f);
					const Vector2 vel1(-100.0f, 500.0f);
					const Vector2 vel2(100.0f, 500.0f);
					mEnemyBullets1[index0].Fire(pos0, vel0);
					mEnemyBullets1[index1].Fire(pos1, vel1);
					mEnemyBullets1[index2].Fire(pos2, vel2);
					mEnemyBullets1Index = (index2 + 1) % kMaxBullets;
					myFireSound1.Play();
				}
			}
		}
	}

	for (int e = 0; e < kMaxEnemies; ++e)
	{
		if (myEnemies[e] != nullptr && myEnemies[e]->IsActive())
		{
			if (myEnemies[e]->checkfire3() == true)
			{
				const int index0 =  mEnemyBullets2Index;
				//const int index1 = (mCollidableIndex + 1) % kMaxBullets;
				//const int index2 = (mCollidableIndex + 2) % kMaxBullets;
				if (!mEnemyBullets2[index0].IsActive() /*&&
												   !mCollidables[index1].IsActive() &&
												   !mCollidables[index2].IsActive()*/)
				{
					const Vector2 pos0 = myEnemies[e]->GetPosition() + Vector2(-20.0f, 20.0f);
					//const Vector2 pos1 = pos0 + Vector2(-20.0f, 20.0f);
					//const Vector2 pos2 = pos0 + Vector2(20.0f, 20.0f);
					const Vector2 vel0(0.0f, 500.0f);
					//const Vector2 vel1(0.0f, -500.0f);
					//const Vector2 vel2(0.0f, -500.0f);
					mEnemyBullets2[index0].Fire(pos0, vel0);
					//mCollidables[index1].Fire(pos1, vel1);
					//mCollidables[index2].Fire(pos2, vel2);
					//mCollidableIndex = (index2 + 1) % kMaxBullets;
					myFireSound2.Play();
				}
			}
		}
	}

	for (int e = 0; e < kMaxEnemies; ++e)
	{
		if (myEnemies[e] != nullptr && myEnemies[e]->IsActive())
		{
			if (myEnemies[e]->checkfire4() == true)
			{
				const int index0 = myEnemyIndex;
				const int index1 = (myEnemyIndex + 1) % kMaxBullets;
				const int index2 = (myEnemyIndex + 2) % kMaxBullets;
				const int index3 = (myEnemyIndex + 3) % kMaxBullets;
				const int index4 = (myEnemyIndex + 4) % kMaxBullets;
				const int index5 = (myEnemyIndex + 5) % kMaxBullets;
				if (!mEnemyBullets1[index0].IsActive() &&
					!mEnemyBullets1[index1].IsActive() &&
					!mEnemyBullets1[index2].IsActive() &&
					!mEnemyBullets1[index3].IsActive() &&
					!mEnemyBullets1[index4].IsActive() &&
					!mEnemyBullets1[index5].IsActive())
				{
					const Vector2 pos0 = myEnemies[e]->GetPosition();
					const Vector2 pos1 = pos0 + Vector2(-20.0f, 20.0f);
					const Vector2 pos2 = pos0 + Vector2(20.0f, 20.0f);
					const Vector2 pos3 = pos0 + Vector2(40.0f, 40.0f);
					const Vector2 pos4 = pos0 + Vector2(-40.0f, 40.0f);
					const Vector2 pos5 = pos0 + Vector2(60.0f, 60.0f);
					const Vector2 vel0(0.0f, 500.0f);
					const Vector2 vel1(-100.0f, 500.0f);
					const Vector2 vel2(100.0f, 500.0f);
					const Vector2 vel3(200.0f, 500.0f);
					const Vector2 vel4(300.0f, 500.0f);
					const Vector2 vel5(400.0f, 500.0f);
					mEnemyBullets1[index0].Fire(pos0, vel0);
					mEnemyBullets1[index1].Fire(pos1, vel1);
					mEnemyBullets1[index2].Fire(pos2, vel2);
					mEnemyBullets1[index3].Fire(pos3, vel3);
					mEnemyBullets1[index4].Fire(pos4, vel4);
					mEnemyBullets1[index5].Fire(pos5, vel5);
					mEnemyBullets1Index = (index5 + 1) % kMaxBullets;
					myFireSound1.Play();
				}
			}
		}
	}

	if (Input_IsMousePressed(1))
	{
		const int index0 = mCollidableIndex;
		const int index1 = (mCollidableIndex + 1) % kMaxBullets;
		const int index2 = (mCollidableIndex + 2) % kMaxBullets;
		if (!mCollidables[index0].IsActive() &&
			!mCollidables[index1].IsActive() &&
			!mCollidables[index2].IsActive())
		{
			const Vector2 pos0 = myFighter.GetPosition();
			const Vector2 pos1 = pos0 + Vector2(-20.0f, 20.0f);
			const Vector2 pos2 = pos0 + Vector2(20.0f, 20.0f);
			const Vector2 vel0(0.0f, -500.0f);
			const Vector2 vel1(0.0f, -500.0f);
			const Vector2 vel2(0.0f, -500.0f);
			mCollidables[index0].Fire(pos0, vel0);
			mCollidables[index1].Fire(pos1, vel1);
			mCollidables[index2].Fire(pos2, vel2);
			mCollidableIndex = (index2 + 1) % kMaxBullets;
			myFireSound1.Play();
		}
	}

	if (Input_IsMousePressed(2))
	{
		const int index0 = mGenkidamaIndex;
		//const int index1 = (mCollidableIndex + 1) % kMaxBullets;
		//const int index2 = (mCollidableIndex + 2) % kMaxBullets;
		if (!mGenkidama[index0].IsActive() /*&&
			!mCollidables[index1].IsActive() &&
			!mCollidables[index2].IsActive()*/)
		{
			const Vector2 pos0 = myFighter.GetPosition();
			//const Vector2 pos1 = pos0 + Vector2(-20.0f, 20.0f);
			//const Vector2 pos2 = pos0 + Vector2(20.0f, 20.0f);
			const Vector2 vel0(0.0f, -500.0f);
			//const Vector2 vel1(0.0f, -500.0f);
			//const Vector2 vel2(0.0f, -500.0f);
			mGenkidama[index0].Fire(pos0, vel0);
			//mCollidables[index1].Fire(pos1, vel1);
			//mCollidables[index2].Fire(pos2, vel2);
			//mCollidableIndex = (index2 + 1) % kMaxBullets;
			myFireSound2.Play();
		}
	}
}

void World::CheckCollision()
{
	for (int e = 0; e < kMaxEnemies; ++e)
	{
		if (myEnemies[e] != nullptr && myEnemies[e]->IsActive())
		{
			const Circle& enemyCircle = myEnemies[e]->GetBoundingCircle();
			const Circle& fighterCircle = myFighter.GetBoundingCircle();

			for (int b = 0; b < kMaxBullets; ++b)
			{
				if (myBullets[b].IsActive())
				{
					if (SGE::Physics_BoundingCircleTest(myBullets[b].GetBoundingCircle(), enemyCircle) == true)
					{
						myBullets[b].Collide(enemyCircle.center);

						bool isActive = myEnemies[e]->Kill();
						if(isActive == false)
						{
							if(e == mBossIndex)
							{
								mBossIndex = -1;
							}
							++mKills;
							myEnemies[e]->Unload();
							delete myEnemies[e];
							myEnemies[e] = nullptr;

						}

						myBullets[b].Kill();
						myExplosions[myExplosionIndex].SetPosition(enemyCircle.center);
						myExplosions[myExplosionIndex].Start(100.0f, true);
						myExplosionIndex = (myExplosionIndex + 1) % kMaxExplosions;
						myExplosionSound.Play();
						break;
					}
				}
			}

			for (int b = 0; b < kMaxBullets; ++b)
			{
				if (mCollidables[b].IsActive())
				{
					if (SGE::Physics_BoundingCircleTest(mCollidables[b].GetBoundingCircle(), enemyCircle) == true)
					{
						mCollidables[b].Collide(enemyCircle.center);

						bool isActive = myEnemies[e]->Kill();
						if(isActive == false)
						{
							if(e == mBossIndex)
							{
								mBossIndex = -1;
							}
							++mKills;
							myEnemies[e]->Unload();
							delete myEnemies[e];
							myEnemies[e] = nullptr;
						} 

						//mCollidables[b].Kill();
						myExplosions[myExplosionIndex].SetPosition(enemyCircle.center);
						myExplosions[myExplosionIndex].Start(100.0f, true);
						myExplosionIndex = (myExplosionIndex + 1) % kMaxExplosions;
						myExplosionSound.Play();
						break;
					}
				}
			}

			if(myFighter.IsActive() == true)
			{
				for (int b = 0; b < MaxEnemyBullets; ++b)
				{
					if (mEnemyBullets1[b].IsActive())
					{
						if (SGE::Physics_BoundingCircleTest(mEnemyBullets1[b].GetBoundingCircle(), fighterCircle) == true)
						{
							mEnemyBullets1[b].Collide(fighterCircle.center);

							bool isActive = myFighter.Kill();
							if(isActive == false)
							{
								myFighter.Unload();
								
							}

							mEnemyBullets1[b].Kill();
							myExplosions[myExplosionIndex].SetPosition(fighterCircle.center);
							myExplosions[myExplosionIndex].Start(100.0f, true);
							myExplosionIndex = (myExplosionIndex + 1) % kMaxExplosions;
							myExplosionSound.Play();
							break;
						}
					}
				}
			}

			if (myFighter.IsActive() == true)
			{
				for (int b = 0; b < MaxEnemyBullets2; ++b)
				{
					if (mEnemyBullets2[b].IsActive())
					{
						if (SGE::Physics_BoundingCircleTest(mEnemyBullets2[b].GetBoundingCircle(), fighterCircle) == true)
						{
							mEnemyBullets2[b].Collide(fighterCircle.center);

							bool isActive = myFighter.Kill();
							if (isActive == false)
							{
								myFighter.Unload();

							}

							//mEnemyBullets1[b].Kill();
							myExplosions[myExplosionIndex].SetPosition(fighterCircle.center);
							myExplosions[myExplosionIndex].Start(100.0f, true);
							myExplosionIndex = (myExplosionIndex + 1) % kMaxExplosions;
							myExplosionSound.Play();
							break;
						}
					}
				}
			}

			for (int b = 0; b < MaxGenkidama; ++b)
			{
				if (mGenkidama[b].IsActive())
				{
					if (SGE::Physics_BoundingCircleTest(mGenkidama[b].GetBoundingCircle(), enemyCircle) == true)
					{
						mGenkidama[b].Collide(enemyCircle.center);

						bool isActive = myEnemies[e]->Kill();
						if (isActive == false)
						{
							if (e == mBossIndex)
							{
								mBossIndex = -1;
							}
							++mKills;
							myEnemies[e]->Unload();
							delete myEnemies[e];
							myEnemies[e] = nullptr;
						}

						//mGenkidama[b].Kill();
						myExplosions[myExplosionIndex].SetPosition(enemyCircle.center);
						myExplosions[myExplosionIndex].Start(100.0f, true);
						myExplosionIndex = (myExplosionIndex + 1) % kMaxExplosions;
						myExplosionSound.Play();
						break;
					}
				}
			}
			//// Collidable Test
			//if (mCollidable.IsActive())
			//{
			//	if (SGE::Physics_BoundingCircleTest(mCollidable.GetBoundingCircle(), enemyCircle) == true)
			//	{
			//		mCollidable.Collide(enemyCircle.center);
			//		myEnemies[e]->Kill();
			//		//mCollidable.Kill();
			//		myExplosions[myExplosionIndex].SetPosition(enemyCircle.center);
			//		myExplosions[myExplosionIndex].Start(100.0f, true);
			//		myExplosionIndex = (myExplosionIndex + 1) % kMaxExplosions;
			//		myExplosionSound.Play();
			//		break;
			//	}
			//}

			// Fighter collision
			{
				if(myFighter.IsActive())
				{
					if (SGE::Physics_BoundingCircleTest(myFighter.GetBoundingCircle(), enemyCircle) == true)
					{
						myEnemies[e]->Kill();
						myFighter.Kill();
						myExplosions[myExplosionIndex].SetPosition(enemyCircle.center);
						myExplosions[myExplosionIndex].Start(100.0f, true);
						myExplosionIndex = (myExplosionIndex + 1) % kMaxExplosions;
						myExplosionSound.Play();
					}
				}
			}
		}
	}
}

