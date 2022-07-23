#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath)
{
    peach = nullptr;
    m_levelEnd = false;
    m_gameEnd = false;
    m_level = 1;
}
StudentWorld::~StudentWorld()
{
    cleanUp();
}
int StudentWorld::init()
{
    m_levelEnd = false;


    ostringstream oss;
    oss.fill('0');
    Level lev(assetPath());
    oss << "level" << setw(2) << m_level << ".txt";
    string level_file = oss.str();
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found)
    {
        cerr << "Could not find " << level_file << " data file" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_fail_bad_format)
    {
        cerr << level_file << " is improperly formatted" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level" << endl;
        for (int x = 0; x < VIEW_WIDTH; x++)
        {
            for (int y = 0; y < VIEW_HEIGHT; y++)
            {
                Level::GridEntry ge = lev.getContentsOf(x, y);
                switch (ge)
                {
                case Level::peach:
                    peach = new Peach(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, this);
                    break;
                case Level::block:
                    actors.push_back(new Blocks(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, this, true, false, false, false, false));
                    break;
                case Level::star_goodie_block:
                    actors.push_back(new Blocks(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, this, false, true, false, false, false));
                    break;
                case Level::flower_goodie_block:
                    actors.push_back(new Blocks(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, this, false, false, true, false, false));
                    break;
                case Level::mushroom_goodie_block:
                    actors.push_back(new Blocks(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, this, false, false, false, true, false));
                    break;
                case Level::pipe:
                    actors.push_back(new Pipes(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, this));
                    break;
                case Level::flag:
                    actors.push_back(new Flag(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, this));
                    break;
                case Level::mario:
                    actors.push_back(new Mario(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, this));
                    break;
                case Level::goomba:
                    actors.push_back(new Goomba(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 180, this));
                    break;
                case Level::koopa:
                    actors.push_back(new Koopa(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 0, this));
                    break;
                case Level::piranha:
                    actors.push_back(new Piranha(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 0, this));
                    break;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}
int StudentWorld::move()
{
    for (Actor* a : actors)
    {
        if (a->getIsAlive())
            a->doSomething();
    }

    removeDeadActors();

    if (!peach->getIsAlive())
        return GWSTATUS_PLAYER_DIED;
    else
        peach->doSomething();

    if (m_levelEnd)
        return GWSTATUS_FINISHED_LEVEL;
    else if (m_gameEnd)
        return GWSTATUS_PLAYER_WON;

    return GWSTATUS_CONTINUE_GAME;
}
void StudentWorld::removeDeadActors()
{
    vector<Actor*>::const_iterator it = actors.begin();
    for (Actor* a : actors)
    {
        if (!a->getIsAlive())
        {
            it = actors.erase(it);
            delete a;
        }
        else
            it++;
    }
}
void StudentWorld::cleanUp()
{
    for (Actor* a : actors)
        delete a;

    actors.clear();
    delete peach;
}

bool StudentWorld::isBlockedBelow(Actor* a)
{
    for (int i = 0; i < SPRITE_WIDTH; i++)
    {
        if (getMoveBlocked(a->getX() + i, a->getY() - SPRITE_WIDTH) || getMoveBlocked(a->getX() - i, a->getY() - SPRITE_WIDTH))
            return true;
    }
    return false;
}
bool StudentWorld::overlapsPeach(Actor* a)
{
    for (int i = 0; i < SPRITE_HEIGHT; i++) // overlaps from sides
    {
        if (a->getX() + SPRITE_WIDTH == getPeach()->getX() && a->getY() - i == getPeach()->getY())
            return true;
        if (a->getX() - SPRITE_WIDTH == getPeach()->getX() && a->getY() - i == getPeach()->getY())
            return true;
        if (a->getX() + SPRITE_WIDTH == getPeach()->getX() && a->getY() + i == getPeach()->getY())
            return true;
        if (a->getX() - SPRITE_WIDTH == getPeach()->getX() && a->getY() + i == getPeach()->getY())
            return true;
    }
    for (int i = 0; i < SPRITE_WIDTH; i++) // overlaps from above/below
    {
        if (a->getY() + SPRITE_WIDTH == getPeach()->getY() && a->getX() - i == getPeach()->getX())
            return true;
        if (a->getY() - SPRITE_WIDTH == getPeach()->getY() && a->getX() - i == getPeach()->getX())
            return true;
        if (a->getY() + SPRITE_WIDTH == getPeach()->getY() && a->getX() + i == getPeach()->getX())
            return true;
        if (a->getY() - SPRITE_WIDTH == getPeach()->getY() && a->getX() + i == getPeach()->getX())
            return true;
    }

    return false;
}
Actor* StudentWorld::getActorOverlaps(Actor* a)
{
    for (Actor* a2 : actors)
    {
        if (a2->getIsDamageable())
        {
            for (int i = 0; i < SPRITE_HEIGHT; i++) // overlaps from sides
            {
                if (a->getX() + SPRITE_WIDTH == a2->getX() && a->getY() - i == a2->getY())
                    return a2;
                if (a->getX() - SPRITE_WIDTH == a2->getX() && a->getY() - i == a2->getY())
                    return a2;
                if (a->getX() + SPRITE_WIDTH == a2->getX() && a->getY() + i == a2->getY())
                    return a2;
                if (a->getX() - SPRITE_WIDTH == a2->getX() && a->getY() + i == a2->getY())
                    return a2;
            }
            for (int i = 0; i < SPRITE_WIDTH; i++) // overlaps from above/below
            {
                if (a->getY() + SPRITE_WIDTH == a2->getY() && a->getX() - i == a2->getX())
                    return a2;
                if (a->getY() - SPRITE_WIDTH == a2->getY() && a->getX() - i == a2->getX())
                    return a2;
                if (a->getY() + SPRITE_WIDTH == a2->getY() && a->getX() + i == a2->getX())
                    return a2;
                if (a->getY() - SPRITE_WIDTH == a2->getY() && a->getX() + i == a2->getX())
                    return a2;
            }
        }
    }
    return false;
}
bool StudentWorld::getMoveBlocked(int x, int y)
{
    for (Actor* a : actors)
    {
        if (a->getBlocksMovement())
        {
            if (a->getX() == x && a->getY() == y)
                return true;
        }
    }
    return false;
}

bool StudentWorld::doesOverlap(Actor* a)
{
    for (Actor* a2 : actors)
    {
        if (a2->getIsDamageable())
        {
            for (int i = 0; i < SPRITE_HEIGHT; i++) // overlaps from sides
            {
                if (a->getX() + SPRITE_WIDTH == a2->getX() && a->getY() - i == a2->getY())
                    return true;
                if (a->getX() - SPRITE_WIDTH == a2->getX() && a->getY() - i == a2->getY())
                    return true;
                if (a->getX() + SPRITE_WIDTH == a2->getX() && a->getY() + i == a2->getY())
                    return true;
                if (a->getX() - SPRITE_WIDTH == a2->getX() && a->getY() + i == a2->getY())
                    return true;
            }
            for (int i = 0; i < SPRITE_WIDTH; i++) // overlaps from above/below
            {
                if (a->getY() + SPRITE_WIDTH == a2->getY() && a->getX() - i == a2->getX())
                    return true;
                if (a->getY() - SPRITE_WIDTH == a2->getY() && a->getX() - i == a2->getX())
                    return true;
                if (a->getY() + SPRITE_WIDTH == a2->getY() && a->getX() + i == a2->getX())
                    return true;
                if (a->getY() - SPRITE_WIDTH == a2->getY() && a->getX() + i == a2->getX())
                    return true;
            }
        }
    }
    return false;
}



bool StudentWorld::getDamageable(Actor* a)
{
    return a->getIsDamageable();
}

Actor* StudentWorld::getActor(int x, int y)
{
    for (Actor* a : actors)
        if (a->getX() == x && a->getY() == y)
            return a;
}

void StudentWorld::pushActor(Actor* a)
{
    actors.push_back(a);
}

bool StudentWorld::isBlockedSide(Actor* a)
{
    for (Actor* x : actors)
    {
        if (x->getBlocksMovement())
        {
            for (int i = 0; i < SPRITE_WIDTH; i++)
            {
                if (a->getDirection() == 0)
                {
                    if (a->getY() + i == x->getY() && a->getX() + SPRITE_WIDTH == x->getX())
                        return true;
                    if (a->getY() - i == x->getY() && a->getX() + SPRITE_WIDTH == x->getX())
                        return true;
                }
                if (a->getDirection() == 180)
                {
                    if (a->getY() + i == x->getY() && a->getX() - SPRITE_WIDTH == x->getX())
                        return true;
                    if (a->getY() - i == x->getY() && a->getX() - SPRITE_WIDTH == x->getX())
                        return true;
                }
            }
        }
    }
    return false;
}


Peach* StudentWorld::getPeach()
{
    return peach;
}

void StudentWorld::reachedEnd(int x)
{
    if (x == 1)
    {
        m_levelEnd = true;
        m_level++;
    }

    else if (x == 25)
        m_gameEnd = true;
}

bool StudentWorld::isBlockedUnit(Actor* a)
{
    if (a->getDirection() == 0)
    {
        if (getMoveBlocked(a->getX() + SPRITE_WIDTH, a->getY()))
        {
            return true;
        }
        return false;
    }
    if (a->getDirection() == 180)
    {
        if (getMoveBlocked(a->getX() - SPRITE_WIDTH, a->getY()))
        {
            return true;
        }
        return false;
    }
}

bool StudentWorld::overlaps(Actor* a1, Actor* a2)
{
    if (a1->getX() == a2->getX() + SPRITE_WIDTH - 1) // if a1 is right of a2
    {
        return a1->getY() == a2->getY();
    }
    if (a1->getX() + SPRITE_WIDTH - 1 == a2->getX()) // if a1 is left of a2
    {
        return a1->getY() == a2->getY();
    }
    return false;
}

bool StudentWorld::walksOffEdge(Actor* a)
{
    bool flag = true;
    for (int i = 0; i < SPRITE_WIDTH; i++)
    {
        if (a->getDirection() == 0)
        {
            if (getMoveBlocked(a->getX() + SPRITE_WIDTH - 1 + i, a->getY() - SPRITE_HEIGHT))
                flag = false;
        }
        else
        {
            if (getMoveBlocked(a->getX() - SPRITE_WIDTH + 1 + i, a->getY() - SPRITE_HEIGHT))
                flag = false;
        }
    }
    return flag;
}

bool StudentWorld::isBlockedLeft(Actor* a)
{
    for (Actor* x : actors)
    {
        if (x->getBlocksMovement())
        {
            for (int i = 0; i < 4; i++)
            {
                {
                    if (a->getY() + i == x->getY() && a->getX() - SPRITE_WIDTH == x->getX())
                        return true;
                    if (a->getY() - i == x->getY() && a->getX() - SPRITE_WIDTH == x->getX())
                        return true;
                }
            }
        }
    }
    return false;
}
bool StudentWorld::isBlockedRight(Actor* a)
{
    for (Actor* x : actors)
    {
        if (x->getBlocksMovement())
        {
            for (int i = 0; i < 4; i++)
            {
                if (a->getY() + i == x->getY() && a->getX() + SPRITE_WIDTH == x->getX())
                    return true;
                if (a->getY() - i == x->getY() && a->getX() + SPRITE_WIDTH == x->getX())
                    return true;
            }
        }
    }
    return false;
}

Actor* StudentWorld::getActorOverlapsAbove(Actor* a)
{
    for (Actor* a2 : actors)
    {
        if (a2->getIsDamageable())
        {
            for (int i = 0; i < SPRITE_WIDTH; i++) // overlaps from above/below
            {
                if (a->getY() + SPRITE_WIDTH == a2->getY() && a->getX() - i == a2->getX())
                    return a2;
                if (a->getY() - SPRITE_WIDTH == a2->getY() && a->getX() - i == a2->getX())
                    return a2;
                if (a->getY() + SPRITE_WIDTH == a2->getY() && a->getX() + i == a2->getX())
                    return a2;
                if (a->getY() - SPRITE_WIDTH == a2->getY() && a->getX() + i == a2->getX())
                    return a2;
            }
        }
    }
    return false;
}
Actor* StudentWorld::getActorOverlapsSides(Actor* a)
{
    for (Actor* a2 : actors)
    {
        if (a2->getIsDamageable())
        {
            for (int i = 0; i < SPRITE_HEIGHT; i++) // overlaps from sides
            {
                if (a->getX() + SPRITE_WIDTH == a2->getX() && a->getY() - i == a2->getY())
                    return a2;
                if (a->getX() - SPRITE_WIDTH == a2->getX() && a->getY() - i == a2->getY())
                    return a2;
                if (a->getX() + SPRITE_WIDTH == a2->getX() && a->getY() + i == a2->getY())
                    return a2;
                if (a->getX() - SPRITE_WIDTH == a2->getX() && a->getY() + i == a2->getY())
                    return a2;
            }
        }
    }
    return false;
}