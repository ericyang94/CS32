#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"
#include "Level.h"
#include <algorithm>

/////////
//ACTOR//
/////////

Actor::Actor(StudentWorld* world, int startX, int startY, int imageID):GraphObject(imageID, startX, startY)
{
	m_world = world;
	m_isAlive = true;
}

Actor::~Actor()
{
}

bool Actor::isAlive() const
{
	return m_isAlive;
}

void Actor::setDead()
{
 m_isAlive = false;
}

StudentWorld* Actor::getWorld() const
{
 return m_world;
}

void Actor::revealExit()
{
	return;
}

bool Actor::isVisible()
{
	return true;
}

void Actor::applyBugSpray()
{
	return;
}

bool Actor::blocksPlayer() const
{
	return false;
}

bool Actor::colocationKillsPlayer() const
{
	return false;
}

bool Actor::blocksZumiAndSprayers() const
{
	return false;
}

bool Actor::allowsSprayToBeDropped() const
{
	return true;
}

bool Actor::allowsSprayToPass() const
{
	return true;
}

//////////
//PLAYER//
//////////

Player::Player(StudentWorld* world, int startX, int startY):Actor(world, startX, startY, IID_PLAYER)
{
	m_max = 2;
	m_sprayerLifetime = 0;
	m_wallLifetime = 0;
	m_numSprayers = 0;
	setVisible(true);
}

Player::~Player()
{

}

void Player::doSomething()
{
	if (!isAlive())
		return;

	if (getWorld()->getActorAtPosition(getX(),getY()) != NULL)
		if(getWorld()->getActorAtPosition(getX(),getY())->colocationKillsPlayer())
		{
			setDead();
			getWorld()->playSound(SOUND_PLAYER_DIE);
			return;
		}

	if (m_wallLifetime>0)
		m_wallLifetime--;

	if (m_sprayerLifetime>0)
		m_sprayerLifetime--;

	int ch;
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
			case KEY_PRESS_LEFT:
				if(getWorld()->getActorAtPosition(getX() - 1, getY())!= NULL)
				{
					if (getWorld()->getActorAtPosition(getX() - 1, getY())->blocksPlayer())
						break;
				}
				else
					moveTo(getX() - 1, getY());
				break;
			case KEY_PRESS_RIGHT:
				if(getWorld()->getActorAtPosition(getX() + 1, getY())!= NULL)
				{
					if (getWorld()->getActorAtPosition(getX() + 1, getY())->blocksPlayer())
						break;
				}
				else
					moveTo(getX() + 1, getY());
				break;
			case KEY_PRESS_UP:
				if(getWorld()->getActorAtPosition(getX() , getY()+1)!= NULL)
				{
					if(getWorld()->getActorAtPosition(getX(), getY()+1)->blocksPlayer())
					break;
				}
				else
					moveTo(getX(), getY()+1);
				break;
			case KEY_PRESS_DOWN:
				if(getWorld()->getActorAtPosition(getX(), getY()-1)!= NULL)
				{
					if(getWorld()->getActorAtPosition(getX(), getY()-1)->blocksPlayer())
						break;
				}
				else
					moveTo(getX(), getY()-1);
				break;
			case KEY_PRESS_SPACE:
				if(getWorld()->getActorAtPosition(getX(), getY())!= NULL)
				{
					if(getWorld()->getActorAtPosition(getX(), getY()-1)->blocksZumiAndSprayers())
						break;
				}
				else
					dropSprayer(getX(),getY());
				break;
		}
	}
}

void Player::applyBugSpray()
{
	setDead();
}

void Player::activateWalkThroughWalls(unsigned int lifetime)
{
	m_wallLifetime = lifetime;
}

bool Player::canWalkThroughWalls() const
{
	if (m_wallLifetime > 0)
		return true;
	else return false;
}

void Player::increaseSimultaneousSprayers(unsigned int max, unsigned int lifetime)
{
	m_max = max;
	m_sprayerLifetime = lifetime;
}

void Player::dropSprayer(int x, int y)
{
	if (m_numSprayers != m_max)
	{
		getWorld()->addActor(new BugSprayer(getWorld(),x,y));
		m_numSprayers++;
	}
}

void Player::addSprayer()
{
	m_numSprayers--;
}

/////////
//BRICK//
/////////

Brick::Brick(StudentWorld* world, int startX, int startY, int imageID):Actor(world, startX, startY, imageID)
{

}

Brick::~Brick()
{
}

void Brick::doSomething()
{
	return;
}

bool Brick::colocationKillsPlayer() const
{
	return true;
}

bool Brick::blocksZumiAndSprayers() const
{
	return true;
}

bool Brick::allowsSprayToPass() const
{
	return false;
}

//////////////////
//PERMANENTBRICK//
//////////////////

PermanentBrick::PermanentBrick(StudentWorld* world, int startX, int startY): Brick(world,startX,startY,IID_PERMA_BRICK)
{
	setVisible(true);
}

PermanentBrick::~PermanentBrick()
{
}

bool PermanentBrick::blocksPlayer() const
{
	return true;
}

bool PermanentBrick::allowsSprayToBeDropped() const
{
	return false;
}

////////////////////
//DESTROYABLEBRICK//
////////////////////

DestroyableBrick::DestroyableBrick(StudentWorld* world, int startX, int startY): Brick(world,startX,startY,IID_DESTROYABLE_BRICK)
{
	setVisible(true);
}

DestroyableBrick::~DestroyableBrick()
{
}

bool DestroyableBrick::blocksPlayer() const
{
	if (getWorld()->getPlayer()->canWalkThroughWalls())
		return false;
	return true;
}

void DestroyableBrick::applyBugSpray()
{
	setDead();
}

bool DestroyableBrick::colocationKillsPlayer() const
{
	if (getWorld()->getPlayer()->canWalkThroughWalls())
		return false;
	else return true;
}

////////
//EXIT//
////////

Exit::Exit(StudentWorld* world, int startX, int startY): Actor(world,startX,startY,IID_EXIT)
{
	setVisible(false);
	m_visible = false;
}

Exit::~Exit()
{
}

void Exit::doSomething()
{
	if (m_visible && getX() == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY())
	{
		getWorld()->playSound(SOUND_FINISHED_LEVEL);
		getWorld()->setLevelFinished();
	}
}

bool Exit::isVisible()
{
	return m_visible;
}

void Exit::revealExit()
{
	setVisible(true);
	m_visible = true;
	getWorld()->playSound(SOUND_REVEAL_EXIT);
}

//////////////////////
//TIMEDLIFETIMEACTOR//
//////////////////////

TimedLifetimeActor::TimedLifetimeActor(StudentWorld* world, int startX, int startY, int imageID, unsigned int lifetime): Actor(world,startX,startY,imageID)
{
	m_lifetime = lifetime;
}

TimedLifetimeActor::~TimedLifetimeActor()
{
}

void TimedLifetimeActor::doSomething()
{
	return;
}

void TimedLifetimeActor::expireImmediately()
{
	m_lifetime = 0;
}

void TimedLifetimeActor::decreaseLifetime()
{
	if (m_lifetime != 0)
		m_lifetime--;
}

int TimedLifetimeActor::getLifetime()
{
	return m_lifetime;
}

//////////////
//BUGSPRAYER//
//////////////

BugSprayer::BugSprayer(StudentWorld* world, int startX, int startY): TimedLifetimeActor(world,startX,startY,IID_BUGSPRAYER,40)
{
	setVisible(true);
}

BugSprayer::~BugSprayer()
{
}

void BugSprayer::doSomething()
{
	if (!isAlive())
		return;
	decreaseLifetime();
	if (getLifetime() == 0)
	{
		getWorld()->addActor(new BugSpray(getWorld(),getX(),getY()));
		if (getWorld()->tryDroppingSprayAtAndIndicateContinuable(getX()+1, getY()))
			getWorld()->tryDroppingSprayAtAndIndicateContinuable(getX()+2, getY());
		if (getWorld()->tryDroppingSprayAtAndIndicateContinuable(getX()-1, getY()))
			getWorld()->tryDroppingSprayAtAndIndicateContinuable(getX()-2, getY());
		if (getWorld()->tryDroppingSprayAtAndIndicateContinuable(getX(), getY()+1))
			getWorld()->tryDroppingSprayAtAndIndicateContinuable(getX(), getY()+2);
		if (getWorld()->tryDroppingSprayAtAndIndicateContinuable(getX(), getY()-1))
			getWorld()->tryDroppingSprayAtAndIndicateContinuable(getX(), getY()-2);
		getWorld()->playSound(SOUND_SPRAY);
		getWorld()->getPlayer()->addSprayer();
		setDead();
	}
}

void BugSprayer::applyBugSpray()
{
	expireImmediately();
}

bool BugSprayer::blocksZumiAndSprayers() const
{
	return true;
}

////////////
//BUGSPRAY//
////////////

BugSpray::BugSpray(StudentWorld* world, int startX, int startY): TimedLifetimeActor(world,startX,startY,IID_BUGSPRAY,3)
{
	setVisible(true);
}

BugSpray::~BugSpray()
{
}

void BugSpray::doSomething()
{
	if (!isAlive())
		return;
	decreaseLifetime();
	if (getLifetime() == 0)
		setDead();
	getWorld()->applyBugSprayToActorsAt(getX(), getY());
}

//////////
//GOODIE//
//////////

Goodie::Goodie(StudentWorld* world, int startX, int startY, int imageID): TimedLifetimeActor(world,startX,startY,imageID,world->getGoodieLifetimeInTicks())
{

}

Goodie::~Goodie()
{
}

//////////////////
//EXTALIFEGOODIE//
//////////////////

ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* world, int startX, int startY): Goodie(world,startX,startY,IID_EXTRA_LIFE_GOODIE)
{
	setVisible(true);
}

ExtraLifeGoodie::~ExtraLifeGoodie()
{
}

void ExtraLifeGoodie::doSomething()
{
	if (!isAlive())
		return;
	decreaseLifetime();
	if (getLifetime() == 0)
		setDead();
	if (getX() == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY())
	{
		getWorld()->incLives();
		getWorld()->increaseScore(1000);
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
	}
}

//////////////////////////
//WALKTHROUGHWALLSGOODIE//
//////////////////////////

WalkThroughWallsGoodie::WalkThroughWallsGoodie(StudentWorld* world, int startX, int startY): Goodie(world,startX,startY,IID_WALK_THRU_GOODIE)
{
	setVisible(true);
}

WalkThroughWallsGoodie::~WalkThroughWallsGoodie()
{
}

void WalkThroughWallsGoodie::doSomething()
{
	if (!isAlive())
		return;
	decreaseLifetime();
	if (getLifetime() == 0)
		setDead();
	if (getX() == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY())
	{
		getWorld()->activatePlayerWalkThroughWalls();
		getWorld()->increaseScore(1000);
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
	}
}

//////////////////////////////////////
//INCREASESIMULTANEOUSSPRAYERSGOODIE//
//////////////////////////////////////

IncreaseSimultaneousSprayersGoodie::IncreaseSimultaneousSprayersGoodie(StudentWorld* world, int startX, int startY): Goodie(world,startX,startY,IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE)
{
	setVisible(true);
}

IncreaseSimultaneousSprayersGoodie::~IncreaseSimultaneousSprayersGoodie()
{
}

void IncreaseSimultaneousSprayersGoodie::doSomething()
{
	if (!isAlive())
		return;
	decreaseLifetime();
	if (getLifetime() == 0)
		setDead();
	if (getX() == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY())
	{
		getWorld()->increasePlayerSimultaneousSprayers();
		getWorld()->increaseScore(1000);
		setDead();
		getWorld()->playSound(SOUND_GOT_GOODIE);
	}
}

////////
//ZUMI//
////////

Zumi::Zumi(StudentWorld* world, int startX, int startY, int imageID, unsigned int ticksPerMove): Actor(world,startX,startY,imageID)
{
	m_currentTick = 1;
	m_ticksPerMove = ticksPerMove;
	m_ID = imageID;
}

Zumi::~Zumi()
{
}

void Zumi::applyBugSpray()
{
	setDead();
	getWorld()->playSound(SOUND_ENEMY_DIE);
	if (m_ID == IID_SIMPLE_ZUMI)
		getWorld()->increaseScore(100);
	if (m_ID == IID_COMPLEX_ZUMI)
		getWorld()->increaseScore(500);
	unsigned int random = randInt(0,99);
	if (getWorld()->getProbOfGoodieOverall() > random)
	{
		int totalProb = getWorld()->getProbOfExtraLifeGoodie() + getWorld()->getProbOfMoreSprayersGoodie() + getWorld()->getProbOfWalkThruGoodie();
		int lifeProb = getWorld()->getProbOfExtraLifeGoodie();
		int sprayProb = getWorld()->getProbOfExtraLifeGoodie() + getWorld()->getProbOfMoreSprayersGoodie();
		int random2 = randInt(1,totalProb);
		if (random2 <= lifeProb)
			getWorld()->addActor(new ExtraLifeGoodie(getWorld(),getX(),getY()));
		else if (random2 <= sprayProb)
			getWorld()->addActor(new IncreaseSimultaneousSprayersGoodie(getWorld(),getX(),getY()));
		else
			getWorld()->addActor(new WalkThroughWallsGoodie(getWorld(),getX(),getY()));
	}
}

bool Zumi::blocksPlayer() const
{
	return false;
}

bool Zumi::colocationKillsPlayer() const
{
	return true;
}

int Zumi::randInt(int lowest, int highest)
{
    if (highest < lowest)
        std::swap(highest, lowest);
    return lowest + (rand() % (highest - lowest + 1));
}

bool Zumi::increaseTick()
{
	if (m_currentTick==m_ticksPerMove)
	{
		m_currentTick++;
		if (m_currentTick>m_ticksPerMove)
			m_currentTick = 1;
		return true;
	}
	m_currentTick++;
	return false;
}

//////////////
//SIMPLEZUMI//
//////////////

SimpleZumi::SimpleZumi(StudentWorld* world, int startX, int startY): Zumi(world,startX,startY,IID_SIMPLE_ZUMI,world->getTicksPerSimpleZumiMove())
{
	setVisible(true);
	m_currentDirection = randInt(1,4);
}

SimpleZumi::~SimpleZumi()
{
}

void SimpleZumi::doSomething()
{
	if (!isAlive())
		return;
	if (getX() == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY())
		getWorld()->getPlayer()->setDead();
	if (increaseTick())
	{
		switch(m_currentDirection)
		{
		case 1:
			if(!getWorld()->zumiAndSprayersBlockedAt(getX()+1,getY()))
				moveTo(getX()+1,getY());
			else
				m_currentDirection = randInt(1,4);
			break;
		case 2:
			if(!getWorld()->zumiAndSprayersBlockedAt(getX()-1,getY()))
				moveTo(getX()-1,getY());
			else
				m_currentDirection = randInt(1,4);
			break;
		case 3:
			if(!getWorld()->zumiAndSprayersBlockedAt(getX(),getY()+1))
				moveTo(getX(),getY()+1);
			else
				m_currentDirection = randInt(1,4);
			break;
		case 4:
			if(!getWorld()->zumiAndSprayersBlockedAt(getX(),getY()-1))
				moveTo(getX(),getY()-1);
			else
				m_currentDirection = randInt(1,4);
			break;
		}
	}
}

///////////////
//COMPLEXZUMI//
///////////////

ComplexZumi::ComplexZumi(StudentWorld* world, int startX, int startY): Zumi(world,startX,startY,IID_COMPLEX_ZUMI,world->getTicksPerComplexZumiMove())
{
	setVisible(true);
	m_currentDirection = randInt(1,4);
}

ComplexZumi::~ComplexZumi()
{
}

void ComplexZumi::doSomething()
{
	if (!isAlive())
		return;
	if (getX() == getWorld()->getPlayer()->getX() && getY() == getWorld()->getPlayer()->getY())
		getWorld()->getPlayer()->setDead();
	if (increaseTick())
	{
		switch(m_currentDirection)
		{
		case 1:
			if(!getWorld()->zumiAndSprayersBlockedAt(getX()+1,getY()))
				moveTo(getX()+1,getY());
			else
				m_currentDirection = randInt(1,4);
			break;
		case 2:
			if(!getWorld()->zumiAndSprayersBlockedAt(getX()-1,getY()))
				moveTo(getX()-1,getY());
			else
				m_currentDirection = randInt(1,4);
			break;
		case 3:
			if(!getWorld()->zumiAndSprayersBlockedAt(getX(),getY()+1))
				moveTo(getX(),getY()+1);
			else
				m_currentDirection = randInt(1,4);
			break;
		case 4:
			if(!getWorld()->zumiAndSprayersBlockedAt(getX(),getY()-1))
				moveTo(getX(),getY()-1);
			else
				m_currentDirection = randInt(1,4);
			break;
		}
	}
}