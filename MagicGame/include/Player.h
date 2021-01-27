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
	Player() : Caracter() { Construct(); }
	Player(int x, int y) : Caracter(x, y) { Construct(); }
	Player(Point p) : Caracter(p) { Construct(); }
	MagicGun1 gun = MagicGun1("images/magic-gun-1.png");


private:

	void Construct() {
		SetSprites({ "images/magic.png" , "images/magic-step.png" });
	}

public:
	HP_Bottle GetBottle() {
		return bottle_;
	}

public:
	void Update(olc::Sprite* sprite, float time) override {
		
		map_->CheckIfOnFinish(GetPosition(), animation.GetNowSprite()->width, animation.GetNowSprite()->height);

		time_passed_ += time;
		HealNaturaly(time);

		Side side;

		Caracter* caracter = Caracter::CheckIfTouchesCaracter(GetPosition(), animation.GetNowSprite(), side);
		if (caracter) {
			Damage(10);
			BounceOff(side);
		}
		UpdateBounce(time);

		gun.Update(time);

		UpdatePosition(sprite, time);
	}

	void UpdateBounce(float time) {
		//Если отбрасывание тоьлко что сработало то уменьшать время с момента срабатывания
		if (move_instruction_.is_on_calldown) {
			move_instruction_.call_down -= time;
		}
		if (move_instruction_.call_down <= 0) {
			move_instruction_.call_down = move_instruction_.kcall_down;
			move_instruction_.is_on_calldown = false;
		}
		//Если отбрасывается
		if (move_instruction_.is_moving_for) {
			Move(move_instruction_);
			move_instruction_.times--;
			if (move_instruction_.times <= 0) {
				move_instruction_.is_moving_for = false;
			}
		}
	}

	void HealNaturaly(float time) {
		if (time_after_bouncing_ < 1)
			time_after_bouncing_ += time;

		if (health_ <= 100 && time_passed_ > 2 && !is_dead_) {
			time_passed_ = 0;
			if (100 - health_ >= 10)
				health_ += health_add_coef_;
			else
				health_ += 100 - health_;

			bottle_.SetHealth(health_);
		}
	}

	void Teleport(Point p) {
		pos_x_ = p.x;
		pos_y_ = p.y;
	}

	void MoveFor(int times, Side side, float time, olc::Sprite* sprite = NULL) {
		move_instruction_.is_moving_for = true;
		move_instruction_.animation = sprite ? sprite : animation.GetNowSprite();
		move_instruction_.side = side;
		move_instruction_.time = time;
		move_instruction_.times = times;
	}

	void BounceOff(Side side, float power = 1.0f) {
		if (move_instruction_.is_moving_for)
			return;
		if (move_instruction_.IsOnCallDown())
			return;

		// Начать CallDown
		move_instruction_.is_on_calldown = true;
		Move(Side::Jump, animation.GetNowSprite(), power / 7);
		MoveFor(10, side, power / 20);
	}

	void PlayerMove(Side side, olc::Sprite* sprite, float time) {

		if (move_instruction_.is_moving_for) {	}
		else {
			Move(side, sprite, time);
		}
	}

	void Atack1() {
		atacking = true;
		gun.Use(this);
	}
	inline void StopAtacking() {
		atacking = false;
	}

	inline bool IsAtacking() {
		return atacking;
	}

	void Damage(int num) override {
		if (time_after_bouncing_ < 0.5)
			return;
		else
			time_after_bouncing_ = 0;

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
	int health_add_coef_ = 10;
	float time_passed_ = 0;					//Время с момента получения урона 
	float time_after_bouncing_ = 0;	//
};