#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <vector>
#include "Level.h"

class Actor;
class Player;

class StudentWorld : public GameWorld
{
public:
    StudentWorld();
    virtual ~StudentWorld();
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
      // Get the player's coordinates
    void getPlayerLocation(int& x, int& y) const;

      // Get a pointer to the player if at the same location as the actor,
      // otherwise nullptr
    Player* getCollidingPlayer(Actor* a) const;

	Actor* getActorAtPosition(int x ,int y) const;
      // Turn on player's ability to walk through destroyable bricks
    void activatePlayerWalkThroughWalls();

      // Can player walk through destroyable bricks?
    bool playerCanWalkThroughWalls() const;

      // Increase the maximum simultaneous sprayers that can be dropped
    void increasePlayerSimultaneousSprayers();

      // Drop a sprayer here if allowed
    void addSprayerIfPossible(int x, int y, unsigned int maxSprayers);

      // Does this location contain an actor that blocks a Zumi from being
      // moved to here or a bug sprayer from being dropped here?
    bool zumiAndSprayersBlockedAt(int x, int y) const;

      // Drop bug spray here if allowed and return true if bug spray can be
      // sprayed beyond the actors at this position (e.g., false for a brick).
    bool tryDroppingSprayAtAndIndicateContinuable(int x, int y);

      // Apply bug spray to each actor at this position
    void applyBugSprayToActorsAt(int x, int y);
    
      // Are there any living Zumi left?
    bool anyLiveZumi() const;
    
      // Indicate that the player has finished the level
    void setLevelFinished();
    
      // Add an actor to the world
    void addActor(Actor* a);

	Player* getPlayer() const;
	Actor* getActor(int i) const;
	void setDisplayText();
	std::string intToString(int value, int digits, std::string s);

      // Report the value of the level data file options
    unsigned int getProbOfGoodieOverall() const;
    unsigned int getProbOfExtraLifeGoodie() const;
    unsigned int getProbOfWalkThruGoodie() const;
    unsigned int getProbOfMoreSprayersGoodie() const;
    unsigned int getTicksPerSimpleZumiMove() const;
    unsigned int getTicksPerComplexZumiMove() const;
    unsigned int getGoodieLifetimeInTicks() const;
    unsigned int getComplexZumiSearchDistance() const;

private:
	Level m_lev;
	Player* m_player;
	std::vector<Actor*> m_vector;
	std::vector<Actor*>::iterator m_it;
	int m_bonus;
	bool m_levelFinished;
};

#endif // STUDENTWORLD_H_