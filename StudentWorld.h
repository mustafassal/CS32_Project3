#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	~StudentWorld();
	virtual int init();
	virtual int move();
	void removeDeadActors();
	virtual void cleanUp();

	bool isBlockedBelow(Actor* a);
	bool overlapsPeach(Actor* a);
	Actor* getActorOverlaps(Actor* a);
	bool getMoveBlocked(int x, int y);

	bool doesOverlap(Actor* a);
	bool getDamageable(Actor* a);
	Actor* getActor(int x, int y);
	void pushActor(Actor* a);
	bool isBlockedSide(Actor* a);
	Peach* getPeach();
	void reachedEnd(int x);
	bool isBlockedUnit(Actor* a);
	bool overlaps(Actor* a1, Actor* a2);
	bool walksOffEdge(Actor* a);


	bool isBlockedLeft(Actor* a);
	bool isBlockedRight(Actor* a);
	Actor* getActorOverlapsAbove(Actor* a);
	Actor* getActorOverlapsSides(Actor* a);

private:
	std::vector<Actor*> actors;
	Peach* peach;
	bool m_levelEnd;
	bool m_gameEnd;
	int m_level;
};

#endif // STUDENTWORLD_H_
