#pragma once
#include "Entity.h"

class Player : public Entity {

public: 
	Player();
	~Player();
	
	bool Init();
	bool Update( unsigned int msec_elapsed, const bool upd_logic = false);

	void AddScore(int addition);
	void IncreaseHelp(int amount);
	void CheatCodes();

protected:
	void handleCollision(Collider& other) override;

private:
	void ModifyLives(int mod_to_add);
	void ReSpawn();

	void UpdatePosition(const iPoint new_speed);
	int  UpdateJumpVerticalSpeed();
	void UpdateHoldingSwapMotion();
	void UpdateThrowingFrontMotion();
	void UpdateThrowingBackMotion();

	void GetInput();
	void ResetInput();

	void UseHelp();

	bool LoadFromConfigFile(const char* file_path);

private: 
	bool respawn_fall = true;

	int input_horizontal = 0; 
	int input_vertical = 0;
	bool input_help = false;
	bool input_attack = false;
	bool input_hold_front_throw = false;
	bool input_attack_back = false;
	bool input_jump = false;

	std::string input_queue = "";

	int max_hold_swaps = 2;
	int current_hold_swaps = 0;

	//debug
	bool debug = false;
};
