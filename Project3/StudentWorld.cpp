#include "StudentWorld.h"
#include "Level.h"
#include "Actor.h"
#include <vector>
using namespace std;

GameWorld* createStudentWorld()
{
	return new StudentWorld();
}

StudentWorld::StudentWorld()
{
	m_bonus = 0;
	m_levelFinished = false;
}

StudentWorld::~StudentWorld()
{
	for(unsigned int i = 0; i < m_vector.size();i++)
	{
		if(!m_vector[i]->isAlive())
		{
			delete m_vector[i];
			m_vector.erase(m_vector.begin() + i);
			i--;
		}
	}
	delete m_player;
}

int StudentWorld::init()
{
	m_levelFinished = false;
	string curLevel = "level";
	curLevel += intToString(getLevel(),2,"0");
	curLevel += ".dat";
	Level::LoadResult result = m_lev.loadLevel(curLevel);

	if (result == Level::load_fail_file_not_found && getLevel() == 0)
		return GWSTATUS_NO_FIRST_LEVEL;

	if (result == Level::load_fail_file_not_found)
		return GWSTATUS_PLAYER_WON;

	if (result == Level::load_fail_bad_format)
		return GWSTATUS_LEVEL_ERROR;

	for(int x = 0; x < 15; x++)
	{
		for(int y = 0; y < 15; y++)
		{
			Level::MazeEntry item = m_lev.getContentsOf(x,y);
			if (item == Level::player)
			{
				m_player = new Player(this,x,y);
			}
			if (item == Level::destroyable_brick)
			{
				m_vector.push_back(new DestroyableBrick(this,x,y));
			}
			if (item == Level::perma_brick)
			{
				m_vector.push_back(new PermanentBrick(this,x,y));
			}
			if (item == Level::exit)
			{
				m_vector.push_back(new Exit(this,x,y));
			}
			if (item == Level::simple_zumi)
			{
				m_vector.push_back(new SimpleZumi(this,x,y));
			}
			if (item == Level::complex_zumi)
			{
				m_vector.push_back(new ComplexZumi(this,x,y));
			}
		}
	}

	m_bonus = m_lev.getOptionValue(optionLevelBonus);

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	if(m_player->isAlive())
	{
		m_player->doSomething();
	}

	if(m_player->isAlive())
	{
		Actor* ap;
		for(unsigned int i = 0; i < m_vector.size(); i++)
		{
			ap = m_vector[i];
			Exit* exit = dynamic_cast<Exit*>(ap);
			if (exit != nullptr)
				break;
		}
		int x,y;
		getPlayerLocation(x,y);
		if (m_levelFinished)
		{
			increaseScore(m_bonus);
			return GWSTATUS_FINISHED_LEVEL;
		}
	}

	for(unsigned int i = 0; i < m_vector.size(); i++)
	{
		if(m_vector[i]->isAlive())
		{
			m_vector[i]->doSomething();
		}
		if(!m_player->isAlive())
		{
			playSound(SOUND_PLAYER_DIE);
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}
	}

	for(unsigned int i = 0; i < m_vector.size();i++)
	{
		if(!m_vector[i]->isAlive())
		{
			delete m_vector[i];
			m_vector.erase(m_vector.begin() + i);
			i--;
		}
	}

	if(!m_player->isAlive())
	{
		playSound(SOUND_PLAYER_DIE);
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	if (!anyLiveZumi())
	{
		Actor* ap;
		for(unsigned int i = 0; i < m_vector.size(); i++)
		{
			ap = m_vector[i];
			Exit* exit = dynamic_cast<Exit*>(ap);
			if (exit != nullptr)
				break;
		}
		if (!ap->isVisible())
			ap->revealExit();
	}

	setDisplayText();

	if(m_bonus>0)
		m_bonus--;

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	for(unsigned int i = 0; i < m_vector.size();i++)
	{
			delete m_vector[i];
			m_vector.erase(m_vector.begin() + i);
			i--;
	}
	delete m_player;
}

Player* StudentWorld::getPlayer() const
{
	return m_player;
}

Actor* StudentWorld::getActor(int i) const
{
	return m_vector[i];
}

void StudentWorld::getPlayerLocation(int& x,int& y) const
{
	x = m_player->getX();
	y = m_player->getY();
}

Actor* StudentWorld::getActorAtPosition( int x, int y) const
{
	Actor* ap;
	for(unsigned int i = 0; i < m_vector.size(); i++)
	{
		ap = m_vector[i];
		if (ap->getX() == x && ap->getY() == y)
			return ap;
	}
	return NULL;
}

Player* StudentWorld::getCollidingPlayer(Actor* a) const
{
	if (a->getX() == m_player->getX() && a->getY() == m_player->getY())
		return m_player;
	else return NULL;
}

  // Turn on player's ability to walk through destroyable bricks
void StudentWorld::activatePlayerWalkThroughWalls()
{
	m_player->activateWalkThroughWalls(m_lev.getOptionValue(optionWalkThruLifetimeTicks));
}
  // Can player walk through destroyable bricks?
bool StudentWorld::playerCanWalkThroughWalls() const
{
	return m_player->canWalkThroughWalls();
}
  // Increase the maximum simultaneous sprayers that can be dropped
void StudentWorld::increasePlayerSimultaneousSprayers()
{
	m_player->increaseSimultaneousSprayers(m_lev.getOptionValue(optionMaxBoostedSprayers), m_lev.getOptionValue(optionBoostedSprayerLifetimeTicks));
}
  // Drop a sprayer here if allowed
void StudentWorld::addSprayerIfPossible(int x, int y, unsigned int maxSprayers)
{
	if (zumiAndSprayersBlockedAt(x, y))
		return;
	Actor* ap;
	for(unsigned int i = 0; i < m_vector.size(); i++)
	{
		ap = m_vector[i];
		if (ap->getX() == x && ap->getY() == y)
				m_vector.push_back(new BugSprayer(this,x,y));
	}		
}
  // Does this location contain an actor that blocks a Zumi from being
  // moved to here or a bug sprayer from being dropped here?
bool StudentWorld::zumiAndSprayersBlockedAt(int x, int y) const
{
	Actor* ap;
	for(unsigned int i = 0; i < m_vector.size(); i++)
	{
		ap = m_vector[i];
		if (ap->getX() == x && ap->getY() == y)
			return (ap->blocksZumiAndSprayers());
	}
	return false;
}
  // Drop bug spray here if allowed and return true if bug spray can be
  // sprayed beyond the actors at this position (e.g., false for a brick).
bool StudentWorld::tryDroppingSprayAtAndIndicateContinuable(int x, int y)
{
	Actor* ap;
	for(unsigned int i = 0; i < m_vector.size(); i++)
	{
		ap = m_vector[i];
		PermanentBrick* bp = dynamic_cast<PermanentBrick*>(ap);
		if (bp != nullptr)
		{
			if (ap->getX() == x && ap->getY() == y)
				return false;
		}
	}
	m_vector.push_back(new BugSpray(this, x, y));
	for(unsigned int i = 0; i < m_vector.size(); i++)
	{
		ap = m_vector[i];
		Brick* bp = dynamic_cast<Brick*>(ap);
		if (bp != nullptr)
		{
			if (ap->getX() == x && ap->getY() == y)
				return false;
		}
	}
	return true;
}
  // Apply bug spray to each actor at this position
void StudentWorld::applyBugSprayToActorsAt(int x, int y)
{
	Actor* ap;
	for(unsigned int i = 0; i < m_vector.size(); i++)
	{
		ap = m_vector[i];
		if (ap->getX() == x && ap->getY() == y)
			ap->applyBugSpray();
	}
	if (m_player->getX() == x && m_player->getY() == y)
		m_player->applyBugSpray();
}
  // Are there any living Zumi left?
bool StudentWorld::anyLiveZumi() const
{
	bool alive = false;
	for(unsigned int i = 0; i < m_vector.size(); i++)
	{
		Actor* ap = m_vector[i];
		Zumi* zumi = dynamic_cast<Zumi*>(ap);
		if (zumi != nullptr)
		{
			alive = true;
			break;
		}
	}

	return alive;
}
  // Indicate that the player has finished the level
void StudentWorld::setLevelFinished()
{
	m_levelFinished = true;
}
  // Add an actor to the world
void StudentWorld::addActor(Actor* a)
{
	m_vector.push_back(a);
}

void StudentWorld::setDisplayText()
{
	int score = getScore();
	int level = getLevel();
	unsigned int bonus = m_bonus;
	int livesLeft = getLives();
	string display = "Score: ";
	display += intToString(score, 8, "0");
	display += "  Level: ";
	display += intToString(level, 2, "0");
	display += "  Lives: ";
	display += intToString(livesLeft, 3, "0");
	display += "  Bonus: ";
	display += intToString(bonus, 6, " ");
	setGameStatText(display);
}

string StudentWorld::intToString(int value, int digits, string s)
{
	string text = "";
	int count = 0;
	int value2 = value;
	while(value2 != 0)
	{
		value2 /= 10;
		count++;
	}
	for(int i = count; i < digits; i++)
	{
		text += s;
	}
	int convert = value;
	int convert2 = 0;
	string holder = "";
	for(int i = 0; i < count; i++)
	{
		convert2 = convert % 10;
		convert = (convert - convert2) / 10;
		holder += convert2 + '0';
	}
	for(int i = 0; i < count / 2; i++)
	{
		char temp = holder[i];
		holder[i] = holder[count - 1 - i];
		holder[count - 1 - i] = temp;
	}
	text += holder;
	return text;
}

unsigned int StudentWorld::getProbOfGoodieOverall() const
{
	return m_lev.getOptionValue(optionProbOfGoodieOverall);
}

unsigned int StudentWorld::getProbOfExtraLifeGoodie() const
{
	return m_lev.getOptionValue(optionProbOfExtraLifeGoodie);
}

unsigned int StudentWorld::getProbOfWalkThruGoodie() const
{
	return m_lev.getOptionValue(optionProbOfWalkThruGoodie);
}

unsigned int StudentWorld::getProbOfMoreSprayersGoodie() const
{
	return m_lev.getOptionValue(optionProbOfMoreSprayersGoodie);
}

unsigned int StudentWorld::getTicksPerSimpleZumiMove() const
{
	return m_lev.getOptionValue(optionTicksPerSimpleZumiMove);
}

unsigned int StudentWorld::getTicksPerComplexZumiMove() const
{
	return m_lev.getOptionValue(optionTicksPerComplexZumiMove);
}

unsigned int StudentWorld::getGoodieLifetimeInTicks() const
{
	return m_lev.getOptionValue(optionGoodieLifetimeInTicks);
}

unsigned int StudentWorld::getComplexZumiSearchDistance() const
{
	return m_lev.getOptionValue(optionComplexZumiSearchDistance);
}