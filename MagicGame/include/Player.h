#pragma once
#include "Map.h"
#include "Caracter.h"
#include "Gun.h"
#include "Animation.h"
#include "HP-bottle.h"

#ifndef ANIMATION_H
	#error Animation.h must be included straight after GameEngine 
#endif

#ifndef GUN_H
	#error Gun.h must be included before Player.h
#endif 

#ifndef PLAYER_H
	#define PLAYER_H
#endif


//Player(Me)
class Player : public Caracter {
public:
	using Caracter::Caracter;
	MagicGun1 gun = MagicGun1("images/magic-gun-1.png");


public:
	//Возвращает всякие спрайты
	std::string GetStandSprite() override {
		return "images/magic.png";
	}
	std::string GetWalkSprite() override {
		return "images/magic-step.png";
	}
	HP_Bottle GetBottle() {
		return bottle_;
	}

public:
	void Update(olc::Sprite* sprite, float time) override {
		
		time_passed_ += time;

		if(time_after_getting_damage_ < 1)
			time_after_getting_damage_ += time;

		if (health_ <= 100 && time_passed_ > 2 && !is_dead_) {
			time_passed_ = 0;
			if (100 - health_ >= 10)
				health_ += health_add_coef_;
			else
				health_ += 100 - health_;

			bottle_.SetHealth(health_);
		}

		if (Caracter::CheckIfTouchesCaracter(GetPosition(), animation.GetNowSprite()))
			Damage(10);

		gun.Update(time);

		UpdatePosition(sprite, time);
	}

	void PlayerMove(Side side, olc::Sprite* sprite, float time) {

		Move(side, sprite, time);
	}

	void Atack1() {
		atacking = true;
		gun.Use(this);
	}

	inline bool IsAtacking() {
		return atacking;
	}

	inline void StopAtacking() {
		atacking = false;
	}

	inline void Damage(int num) {
		if (time_after_getting_damage_ < 0.5)
			return;
		else
			time_after_getting_damage_ = 0;

		if(health_ > 0)
			health_ -= num;
		if (health_ > 100)
			health_ = 100;
		time_passed_ = 0;
		if (health_ < 1)
			is_dead_ = true;
		bottle_.SetHealth(health_);
	}

	inline int GetHealth() {
		return health_;
	}
	
	inline int GetMana() {
		return mana_;
	}


private:
	HP_Bottle bottle_ = HP_Bottle();
	int mana_ = 100;
	int health_ = 100;
	int health_add_coef_ = 10;
	float time_passed_ = 0;
	float time_after_getting_damage_ = 0;
};