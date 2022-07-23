#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, int startX, int startY, int depth, int dir, double size, bool isAlive, bool blocksMove, bool isDamageable, StudentWorld* sw) :
	GraphObject(imageID, startX, startY, dir, depth, size)
{
	m_blocksMovement = blocksMove;
	m_world = sw;
	m_depth = depth;
	m_direction = dir;
	m_size = size;
	m_isAlive = isAlive;
	m_isDamageable = isDamageable;
}

Actor::~Actor()
{
}
bool Actor::getIsAlive()
{
	return m_isAlive;
}
StudentWorld* Actor::getWorld()
{
	return m_world;
}
bool Actor::getBlocksMovement()
{
	return m_blocksMovement;
}
void Actor::setIsAlive(bool x)
{
	m_isAlive = x;
}
bool Actor::getIsDamageable()
{
	return m_isDamageable;
}
void Actor::damaged() { return; }

Peach::Peach(int startX, int startY, StudentWorld* sw) : Actor(IID_PEACH, startX, startY, 0, 0, 1.0, true, false, true, sw)
{
	m_hasJumpPow = false;
	m_hasShootPow = true;
	m_hasStarPow = false;
	m_hasTempInv = false;
	m_tempInv = 0;
	m_starInv = 0;
	m_remainingJumpDist = 0;
	m_hp = 1;
	m_rechargeTime = 0;
}
int Peach::getRecharge()
{
	return m_rechargeTime;
}
void Peach::setRecharge(int x)
{
	m_rechargeTime = x;
}
bool Peach::getStarPow()
{
	return m_hasStarPow;
}
bool Peach::getShootPow()
{
	return m_hasShootPow;
}
bool Peach::getJumpPow()
{
	return m_hasJumpPow;
}
void Peach::setStarPow(bool x)
{
	m_hasStarPow = x;
}
void Peach::setShootPow(bool x)
{
	m_hasShootPow = x;
}
void Peach::setJumpPow(bool x)
{
	m_hasJumpPow = x;
}
void Peach::setHP(int x)
{
	m_hp = x;
}
void Peach::setTempInv(int x)
{
	m_tempInv = x;
}
void Peach::bonk()
{
	if (m_hasStarPow)
		return;

	m_hp--;
	m_tempInv = 10;
	m_hasJumpPow = false;
	m_hasShootPow = false;
	if (m_hp > 0)
		getWorld()->playSound(SOUND_PLAYER_HURT);
	else
		setIsAlive(false);

}
void Peach::doSomething()
{
	if (!getIsAlive())
		return;
	if (m_hasStarPow)
	{
		if (m_starInv == 0)
			m_hasStarPow = false;
		else
			m_starInv--;
	}
	if (m_hasTempInv)
	{
		if (m_tempInv == 0)
			m_hasTempInv = false;
		else
			m_tempInv--;
	}
	if (m_hasShootPow && m_rechargeTime > 0)
		m_rechargeTime--;

	if (getWorld()->doesOverlap(this))
		getWorld()->getActorOverlaps(this)->bonk();

	if (m_remainingJumpDist > 0)
	{
		if (getWorld()->getMoveBlocked(getX(), getY() + SPRITE_HEIGHT))
		{
			getWorld()->getActor(getX(), getY() + SPRITE_HEIGHT)->bonk();
			m_remainingJumpDist = 0;
		}
		else if (getWorld()->getMoveBlocked(getX() - 4, getY() + SPRITE_HEIGHT))
		{
			getWorld()->getActor(getX() - 4, getY() + SPRITE_HEIGHT)->bonk();
			m_remainingJumpDist = 0;
		}
		else if (getWorld()->getMoveBlocked(getX() + 4, getY() + SPRITE_HEIGHT))
		{
			getWorld()->getActor(getX() + 4, getY() + SPRITE_HEIGHT)->bonk();
			m_remainingJumpDist = 0;
		}
		else
		{
			moveTo(getX(), getY() + 4);
			m_remainingJumpDist--;
		}
	}

	if (m_remainingJumpDist == 0)
	{
		if (!getWorld()->getMoveBlocked(getX(), getY() - SPRITE_HEIGHT) && !getWorld()->getMoveBlocked(getX() - 4, getY() - SPRITE_HEIGHT)
			&& !getWorld()->getMoveBlocked(getX() + 4, getY() - SPRITE_HEIGHT))
			moveTo(getX(), getY() - 4);
	}

	int ch;
	if (getWorld()->getKey(ch))
	{
		// user hit a key during this tick!
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			setDirection(180);
			if (getWorld()->getMoveBlocked(getX() - SPRITE_WIDTH, getY()))
			{
				getWorld()->getActor(getX() - SPRITE_WIDTH, getY())->bonk();
			}
			else if (getWorld()->getMoveBlocked(getX() - SPRITE_WIDTH, getY() + 4))
			{
				getWorld()->getActor(getX() - SPRITE_WIDTH, getY() + 4)->bonk();
			}
			else if (getWorld()->getMoveBlocked(getX() - SPRITE_WIDTH, getY() - 4))
			{
				getWorld()->getActor(getX() - SPRITE_WIDTH, getY() - 4)->bonk();
			}
			else
				moveTo(getX() - 4, getY());
			break;
		case KEY_PRESS_RIGHT:
			setDirection(0);
			if (getWorld()->getMoveBlocked(getX() + SPRITE_WIDTH, getY()))
			{
				getWorld()->getActor(getX() + SPRITE_WIDTH, getY())->bonk();
			}
			else if (getWorld()->getMoveBlocked(getX() + SPRITE_WIDTH, getY() + 4))
			{
				getWorld()->getActor(getX() + SPRITE_WIDTH, getY() + 4)->bonk();
			}
			else if (getWorld()->getMoveBlocked(getX() + SPRITE_WIDTH, getY() - 4))
			{
				getWorld()->getActor(getX() + SPRITE_WIDTH, getY() - 4)->bonk();
			}
			else
				moveTo(getX() + 4, getY());
			break;
		case KEY_PRESS_UP:
			if (getWorld()->getMoveBlocked(getX(), getY() - SPRITE_HEIGHT) || getWorld()->getMoveBlocked(getX() - 4, getY() - SPRITE_HEIGHT) || getWorld()->getMoveBlocked(getX() + 4, getY() - SPRITE_HEIGHT))
			{
				if (m_hasJumpPow)
					m_remainingJumpDist = 12;
				else
					m_remainingJumpDist = 8;
				getWorld()->playSound(SOUND_PLAYER_JUMP);
			}
			break;
		case KEY_PRESS_SPACE:
			if (!m_hasShootPow)
				break;
			else if (m_rechargeTime > 0)
				break;
			else
			{
				getWorld()->playSound(SOUND_PLAYER_FIRE);
				m_rechargeTime = 8;
				if (getDirection() == 180)
					getWorld()->pushActor(new PeachFiredFireballs(getX() - 4, getY(), getDirection(), getWorld()));
				if (getDirection() == 0)
					getWorld()->pushActor(new PeachFiredFireballs(getX() + 4, getY(), getDirection(), getWorld()));
			}
			break;
		}
	}
}

Obstacle::Obstacle(int imageID, int startX, int startY, StudentWorld* sw) :Actor(imageID, startX, startY, 2, 0, 1.0, true, true, false, sw) {}
void Obstacle::doSomething() { return; }
void Obstacle::bonk() { return; }
Blocks::Blocks(int startX, int startY, StudentWorld* sw, bool none, bool starG, bool flowG, bool mushG, bool released) : Obstacle(IID_BLOCK, startX, startY, sw)
{
	m_noGoodie = none;
	m_starGoodie = starG;
	m_flowerGoodie = flowG;
	m_mushroomGoodie = mushG;
	m_hasReleased = released;
}
void Blocks::bonk()
{
	if (m_noGoodie || m_hasReleased)
	{
		getWorld()->playSound(SOUND_PLAYER_BONK);
		return;
	}
	else
	{
		getWorld()->playSound(SOUND_POWERUP_APPEARS);
		if (m_flowerGoodie && !m_hasReleased)
		{
			getWorld()->pushActor(new Flower(getX(), getY() + SPRITE_HEIGHT, getWorld()));
			m_hasReleased = true;
			return;
		}
		if (m_mushroomGoodie && !m_hasReleased)
		{
			getWorld()->pushActor(new Mushroom(getX(), getY() + SPRITE_HEIGHT, getWorld()));
			m_hasReleased = true;
			return;
		}
		if (m_starGoodie && !m_hasReleased)
		{
			getWorld()->pushActor(new Star(getX(), getY() + SPRITE_HEIGHT, getWorld()));
			m_hasReleased = true;
			return;
		}
	}
}
Pipes::Pipes(int startX, int startY, StudentWorld* sw) : Obstacle(IID_PIPE, startX, startY, sw) {}

LevelEnder::LevelEnder(int imageID, int startX, int startY, int stopNum, StudentWorld* sw) : Actor(imageID, startX, startY, 1, 0, 1.0, true, false, false, sw)
{
	m_stoppingNum = stopNum;
}
void LevelEnder::doSomething()
{
	if (!getIsAlive())
		return;

	if (getWorld()->overlapsPeach(this))
	{
		getWorld()->increaseScore(1000);
		setIsAlive(false);
		getWorld()->reachedEnd(m_stoppingNum);
	}
}
void LevelEnder::bonk() { return; }
Flag::Flag(int startX, int startY, StudentWorld* sw) : LevelEnder(IID_FLAG, startX, startY, 1, sw) {}
Mario::Mario(int startX, int startY, StudentWorld* sw) : LevelEnder(IID_MARIO, startX, startY, 25, sw) {}

Goodie::Goodie(int imageID, int startX, int startY, int incScore, StudentWorld* sw) : Actor(imageID, startX, startY, 1, 0, 1.0, true, false, false, sw)
{
	m_scoreIncrease = incScore;
}
void Goodie::doSomething()
{
	if (!getIsAlive())
		return;

	if (getWorld()->overlapsPeach(this))
	{
		getWorld()->increaseScore(m_scoreIncrease);
		setPower();
		setIsAlive(false);
		getWorld()->playSound(SOUND_PLAYER_POWERUP);
		getWorld()->removeDeadActors();
		return;
	}

	if (!getWorld()->isBlockedBelow(this)) // if it is floating
		moveTo(getX(), getY() - 2);

	if (getWorld()->isBlockedSide(this)) // if it is blocked from either side
	{
		if (getDirection() == 0)
			setDirection(180);
		else
			setDirection(0);
		return;
	}
	else // not blocked or floating
	{
		if (getDirection() == 0)
			moveTo(getX() + 2, getY());
		else
			moveTo(getX() - 2, getY());
	}
	return;
}
void Goodie::bonk() { return; }
Flower::Flower(int startX, int startY, StudentWorld* sw) : Goodie(IID_FLOWER, startX, startY, 50, sw) {}
void Flower::setPower()
{
	getWorld()->getPeach()->setShootPow(true);
	getWorld()->getPeach()->setHP(2);
}
Mushroom::Mushroom(int startX, int startY, StudentWorld* sw) : Goodie(IID_MUSHROOM, startX, startY, 75, sw) {}
void Mushroom::setPower()
{
	getWorld()->getPeach()->setJumpPow(true);
	getWorld()->getPeach()->setHP(2);
}
Star::Star(int startX, int startY, StudentWorld* sw) : Goodie(IID_STAR, startX, startY, 100, sw) {}
void Star::setPower()
{
	getWorld()->getPeach()->setStarPow(true);
	getWorld()->getPeach()->setTempInv(150);
}

Projectile::Projectile(int imageID, int startX, int startY, int dir, StudentWorld* sw) : Actor(imageID, startX, startY, 1, dir, 1, true, false, false, sw) {}
void Projectile::doSomething()
{
	if (getWorld()->doesOverlap(this)) // overlaps with a damageable actor
	{
		if (!getWorld()->overlapsPeach(this)) // actor is not peach
		{
			getWorld()->getActorOverlaps(this)->damaged();
			setIsAlive(false);
			getWorld()->removeDeadActors();
			return;
		}
	}
	if (!getWorld()->isBlockedBelow(this)) // floating
	{
		moveTo(getX(), getY() - 2);
	}

	if (getDirection() == 0 && !getWorld()->isBlockedRight(this))
		moveTo(getX() + 2, getY());
	else if (getWorld()->isBlockedLeft(this))
	{
		setIsAlive(false);
		getWorld()->removeDeadActors();
		return;
	}
	if (getDirection() == 180 && !getWorld()->isBlockedLeft(this))
		moveTo(getX() - 2, getY());
	else if (getWorld()->isBlockedLeft(this))
	{
		setIsAlive(false);
		getWorld()->removeDeadActors();
		return;
	}
}
void Projectile::damaged() { return; }
void Projectile::bonk() { return; }
PeachFiredFireballs::PeachFiredFireballs(int startX, int startY, int dir, StudentWorld* sw) : Projectile(IID_PEACH_FIRE, startX, startY, dir, sw) {}
PiranhaFiredFireballs::PiranhaFiredFireballs(int startX, int startY, int dir, StudentWorld* sw) : Projectile(IID_PIRANHA_FIRE, startX, startY, dir, sw) {}
Shell::Shell(int startX, int startY, int dir, StudentWorld* sw) : Projectile(IID_SHELL, startX, startY, dir, sw) {}

Enemy::Enemy(int imageID, int startX, int startY, int dir, StudentWorld* sw) : Actor(imageID, startX, startY, 0, dir, 1.0, true, false, true, sw) {}
void Enemy::doSomething()
{
	if (!getIsAlive())
		return;

	if (getWorld()->overlapsPeach(this))
	{
		getWorld()->getPeach()->bonk();
		return;
	}

	if (getWorld()->isBlockedUnit(this)) // if a wall is one unit ahead
	{
		if (getDirection() == 0)
			setDirection(180);
		else
			setDirection(0);
	}
	else if (!getWorld()->isBlockedBelow(this)) // can't move to be under nothing
	{
		if (getDirection() == 0)
			setDirection(180);
		else
			setDirection(0);
	}
	else
	{
		if (getDirection() == 0) // moving along straight path
			moveTo(getX() + 1, getY());
		else
			moveTo(getX() - 1, getY());
	}
}
void Enemy::bonk()
{
	getWorld()->playSound(SOUND_PLAYER_KICK);
	getWorld()->increaseScore(100);
	setIsAlive(false);
	getWorld()->removeDeadActors();
}
void Enemy::damaged()
{
	getWorld()->increaseScore(100);
	setIsAlive(false);
	getWorld()->removeDeadActors();
}
Goomba::Goomba(int startX, int startY, int dir, StudentWorld* sw) : Enemy(IID_GOOMBA, startX, startY, dir, sw) {}
Koopa::Koopa(int startX, int startY, int dir, StudentWorld* sw) : Enemy(IID_KOOPA, startX, startY, dir, sw) {}
void Koopa::bonk()
{
	Enemy::bonk();
	getWorld()->pushActor(new Shell(getX(), getY(), getDirection(), getWorld()));
}
void Koopa::damaged()
{
	getWorld()->increaseScore(100);
	setIsAlive(false);
	getWorld()->pushActor(new Shell(getX(), getY(), getDirection(), getWorld()));
	getWorld()->removeDeadActors();
}
Piranha::Piranha(int startX, int startY, int dir, StudentWorld* sw) : Enemy(IID_PIRANHA, startX, startY, dir, sw)
{
	m_firingDelay = 0;
}
void Piranha::setFiringDelay(int x)
{
	m_firingDelay = x;
}
void Piranha::doSomething()
{
	if (!getIsAlive())
		return;

	increaseAnimationNumber();
	if (getWorld()->overlapsPeach(this))
	{
		getWorld()->getPeach()->bonk();
		return;
	}
	if (getWorld()->getPeach()->getY() <= getY() + (1.5 * SPRITE_HEIGHT) || getWorld()->getPeach()->getY() <= getY() - (1.5 * SPRITE_HEIGHT)) // on the same level
	{
		if (getWorld()->getPeach()->getX() > getX()) // peach to the right
			setDirection(0);
		else if (getWorld()->getPeach()->getX() < getX()) // peach to the left
			setDirection(180);

		if (m_firingDelay > 0)
		{
			m_firingDelay--;
			return;
		}
		else
		{
			if (getWorld()->getPeach()->getX() > getX())
			{
				if (getWorld()->getPeach()->getX() - getX() < 8.0 * SPRITE_WIDTH)
				{
					getWorld()->pushActor(new PiranhaFiredFireballs(getX(), getY(), getDirection(), getWorld()));
					getWorld()->playSound(SOUND_PIRANHA_FIRE);
					setFiringDelay(40);
				}
			}
			else if (getWorld()->getPeach()->getX() < getX())
			{
				if (getX() - getWorld()->getPeach()->getX() < 8.0 * SPRITE_WIDTH)
				{
					getWorld()->pushActor(new PiranhaFiredFireballs(getX(), getY(), getDirection(), getWorld()));
					getWorld()->playSound(SOUND_PIRANHA_FIRE);
					setFiringDelay(40);
				}
			}
		}
	}
	else
		return;


}