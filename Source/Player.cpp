#include "Player.h"

Player* Player::instance = 0;

Player::Player()
{
	health = 100;
	damage = 10;
}

Player::~Player()
{

}

Player* Player::getInstance()
{
	if (!instance)
	{
		instance = new Player();
		return instance;
	}
	else
	{
		return instance;
	}
}
