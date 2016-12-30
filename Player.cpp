#include "Player.h"
#include "Animation.h"


Player::Player() : Entity(Types::player) {}
Player::~Player(){}

bool Player::LoadFromConfigFile(const char* file_path)
{
	// load animations
	// load sounds
	return true;
}