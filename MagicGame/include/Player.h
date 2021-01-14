#pragma once
#include "Map.h"
#include "Caracter.h"
#include "Gun.h"
#include "Animation.h"

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
	// онструтор базового класса, там инициализируетс€ позици€ 
	using Caracter::Caracter;
	MagicGun1 gun = MagicGun1("images/magic-gun-1.png");


public:
	template<typename T>
	void Draw(T func) {
		func();
	}

public:
	//¬озвращает вс€кие спрайты
	std::string GetStandSprite() override {
		return "images/magic.png";
	}
	std::string GetWalkSprite() override {
		return "images/magic-step.png";
	}
public:
	void Update(olc::Sprite* sprite, float time) override {
		if (!Touches(0, 1 * drop_speed_, sprite)) {
			pos_y_ += 1 * drop_speed_;
			drop_speed_ += 0.1f * time / 0.01;
		}
		else
			drop_speed_ = 0.0f;
		gun.Update(time);
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

private:
	float mana = 100;
};