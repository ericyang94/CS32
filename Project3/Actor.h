#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"
#include "StudentWorld.h"

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* world, int startX, int startY, int imageID);

	virtual ~Actor();
    virtual void doSomething() = 0;
    bool isAlive() const;
    void setDead();
    StudentWorld* getWorld() const;
	virtual void revealExit();
	virtual bool isVisible();
    
      // Take action when this actor is co-located with bug spray
    virtual void applyBugSpray();
    
      // Does this actor block the player from moving onto it?
    virtual bool blocksPlayer() const;
    
      // Does a Player on this actor die?
    virtual bool colocationKillsPlayer() const;
    
      // Does this actor block a Zumi from being moved onto it or a
      // bug sprayer from being dropped onto it?
    virtual bool blocksZumiAndSprayers() const;
    
      // Can bug spray be dropped onto this actor?
    virtual bool allowsSprayToBeDropped() const;
    
      // Can bug spray be sprayed beyond this actor?
    virtual bool allowsSprayToPass() const;

private:
	StudentWorld* m_world;
	bool m_isAlive;
};

class Player : public Actor
{
public:
    Player(StudentWorld* world, int startX, int startY);
	virtual ~Player();
    virtual void doSomething();
    virtual void applyBugSpray();
    void activateWalkThroughWalls(unsigned int lifetime);
    bool canWalkThroughWalls() const;
    void increaseSimultaneousSprayers(unsigned int max, unsigned int lifetime);
	void dropSprayer(int x, int y);
	void addSprayer();

private:
	int m_numSprayers;
	int m_max;
	int m_sprayerLifetime;
	int m_wallLifetime;
};

class Brick : public Actor
{
public:
    Brick(StudentWorld* world, int startX, int startY, int imageID);
	virtual ~Brick();
    virtual void doSomething();
    virtual bool colocationKillsPlayer() const;
    virtual bool blocksZumiAndSprayers() const;
    virtual bool allowsSprayToPass() const;
};

class PermanentBrick : public Brick
{
public:
    PermanentBrick(StudentWorld* world, int startX, int startY);
	virtual ~PermanentBrick();
	virtual bool blocksPlayer() const;
    virtual bool allowsSprayToBeDropped() const;
};

class DestroyableBrick : public Brick
{
public:
    DestroyableBrick(StudentWorld* world, int startX, int startY);
	virtual ~DestroyableBrick();
    virtual void applyBugSpray();
	virtual bool blocksPlayer() const;
    virtual bool colocationKillsPlayer() const;
};

class Exit : public Actor
{
public:
    Exit(StudentWorld* world, int startX, int startY);
	virtual ~Exit();
    virtual void doSomething();
	void revealExit();
	virtual bool isVisible();
private:
	bool m_visible;
};

class TimedLifetimeActor : public Actor
{
public:
    TimedLifetimeActor(StudentWorld* world, int startX, int startY, int imageID, unsigned int lifetime);
	virtual ~TimedLifetimeActor();
    virtual void doSomething();
	void decreaseLifetime();
	int getLifetime();

      // Set remining lifetime to 0
    void expireImmediately();

private:
	unsigned int m_lifetime;
};

class BugSprayer : public TimedLifetimeActor
{
public:
    BugSprayer(StudentWorld* world, int startX, int startY);
	virtual ~BugSprayer();
    virtual void doSomething();
    virtual void applyBugSpray();
    virtual bool blocksZumiAndSprayers() const;
};

class BugSpray : public TimedLifetimeActor
{
public:
    BugSpray(StudentWorld* world, int startX, int startY);
	virtual ~BugSpray();
    virtual void doSomething();
};

class Goodie : public TimedLifetimeActor
{
public:
    Goodie(StudentWorld* world, int startX, int startY, int imageID);
	virtual ~Goodie();
};

class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(StudentWorld* world, int startX, int startY);
	virtual ~ExtraLifeGoodie();
    virtual void doSomething();
};

class WalkThroughWallsGoodie : public Goodie
{
public:
    WalkThroughWallsGoodie(StudentWorld* world, int startX, int startY);
	virtual ~WalkThroughWallsGoodie();
    virtual void doSomething();
};

class IncreaseSimultaneousSprayersGoodie : public Goodie
{
public:
    IncreaseSimultaneousSprayersGoodie(StudentWorld* world, int startX, int startY);
	virtual ~IncreaseSimultaneousSprayersGoodie();
    virtual void doSomething();
};

class Zumi : public Actor
{
public:
    Zumi(StudentWorld* world, int startX, int startY, int imageID, unsigned int ticksPerMove);
	virtual ~Zumi();
    virtual void applyBugSpray();
    virtual bool blocksPlayer() const;
    virtual bool colocationKillsPlayer() const;
	int randInt(int lowest, int highest);
	bool increaseTick();
private:
	int m_currentTick;
	int m_ticksPerMove;
	int m_ID;
};

class SimpleZumi : public Zumi
{
public:
    SimpleZumi(StudentWorld* world, int startX, int startY);
	virtual ~SimpleZumi();
    virtual void doSomething();
private:
	int m_currentDirection;
};

class ComplexZumi : public Zumi
{
public:
    ComplexZumi(StudentWorld* world, int startX, int startY);
	virtual ~ComplexZumi();
    virtual void doSomething();
private:
	int m_currentDirection;
};

#endif // ACTOR_H_