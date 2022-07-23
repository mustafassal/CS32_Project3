#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

//Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, int depth, int dir, double size, bool isAlive, bool blocksMove, bool isDamageable, StudentWorld* sw);
	virtual ~Actor();
	virtual void doSomething() = 0;
	bool getIsAlive();
	bool getIsDamageable();
	bool getBlocksMovement();
	StudentWorld* getWorld();
	virtual void bonk() = 0;
	virtual void damaged();
	void setIsAlive(bool x);

private:
	StudentWorld* m_world;
	bool m_blocksMovement;
	int m_depth;
	int m_direction;
	bool m_isAlive;
	double m_size;
	bool m_isDamageable;
};
class Peach : public Actor
{
public:
	Peach(int startX, int startY, StudentWorld* sw);
	bool getStarPow();
	bool getShootPow();
	bool getJumpPow();
	virtual void doSomething();
	int getRecharge();
	void setHP(int x);
	void setTempInv(int x);
	void setStarPow(bool x);
	void setShootPow(bool x);
	void setJumpPow(bool x);
	void setRecharge(int x);
	virtual void bonk();

private:
	int m_hp;
	bool m_hasTempInv;
	int m_rechargeTime;
	int m_remainingJumpDist;
	int m_tempInv;
	int m_starInv;
	bool m_hasStarPow;
	bool m_hasShootPow;
	bool m_hasJumpPow;
};

class Obstacle : public Actor
{
public:
	Obstacle(int imageID, int startX, int startY, StudentWorld* sw);
	virtual void doSomething();
	virtual void bonk();
};
class Blocks : public Obstacle
{
public:
	Blocks(int startX, int startY, StudentWorld* sw, bool none, bool starG, bool flowG, bool mushG, bool released);
	virtual void bonk();
private:
	bool m_noGoodie;
	bool m_starGoodie;
	bool m_flowerGoodie;
	bool m_mushroomGoodie;
	bool m_hasReleased;
};
class Pipes : public Obstacle
{
public:
	Pipes(int startX, int startY, StudentWorld* sw);
};

class LevelEnder : public Actor
{
public:
	LevelEnder(int imageID, int startX, int startY, int stopNum, StudentWorld* sw);
	virtual void doSomething();
	virtual void bonk();
private:
	int m_stoppingNum;
};
class Flag : public LevelEnder
{
public:
	Flag(int startX, int startY, StudentWorld* sw);
};
class Mario : public LevelEnder
{
public:
	Mario(int startX, int startY, StudentWorld* sw);
};

class Goodie : public Actor
{
public:
	Goodie(int imageID, int startX, int startY, int incScore, StudentWorld* sw);
	virtual void doSomething();
	virtual void bonk();
	virtual void setPower() = 0;
private:
	int m_scoreIncrease;
};
class Flower : public Goodie
{
public:
	Flower(int startX, int startY, StudentWorld* sw);
	virtual void setPower();
};
class Mushroom : public Goodie
{
public:
	Mushroom(int startX, int startY, StudentWorld* sw);
	virtual void setPower();
};
class Star : public Goodie
{
public:
	Star(int startX, int startY, StudentWorld* sw);
	virtual void setPower();
};

class Projectile : public Actor
{
public:
	Projectile(int imageID, int startX, int startY, int dir, StudentWorld* sw);
	virtual void doSomething();
	virtual void bonk();
	virtual void damaged();
};
class PiranhaFiredFireballs : public Projectile
{
public:
	PiranhaFiredFireballs(int startX, int startY, int dir, StudentWorld* sw);
};
class PeachFiredFireballs : public Projectile
{
public:
	PeachFiredFireballs(int startX, int startY, int dir, StudentWorld* sw);
};
class Shell : public Projectile
{
public:
	Shell(int startX, int startY, int dir, StudentWorld* sw);
};

class Enemy : public Actor
{
public:
	Enemy(int imageID, int startX, int startY, int dir, StudentWorld* sw);
	virtual void doSomething();
	virtual void bonk();
	virtual void damaged();
};
class Goomba : public Enemy
{
public:
	Goomba(int startX, int startY, int dir, StudentWorld* sw);
};
class Koopa : public Enemy
{
public:
	Koopa(int startX, int startY, int dir, StudentWorld* sw);
	virtual void bonk();
	void damaged();
};
class Piranha : public Enemy
{
public:
	Piranha(int startX, int startY, int dir, StudentWorld* sw);
	virtual void doSomething();
	void setFiringDelay(int x);
private:
	int m_firingDelay;
};

#endif // ACTOR_H_