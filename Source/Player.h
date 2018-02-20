#ifndef PLAYER_H
#define PLAYER_H

class Player
{
private:
	Player();
	~Player();
	static Player *instance;
public:
	static Player *getInstance();
	
	int health;
	int damage;
};

#endif 