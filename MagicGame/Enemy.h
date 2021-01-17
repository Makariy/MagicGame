#pragma once
#include "include/lib/olcPixelGameEngine.h"
#include "include/Caracter.h"


class Enemy : public Caracter {
public:
	Enemy() : Caracter() { Construct(); }
	Enemy(int x, int y) : Caracter(x, y) { Construct(); }
	Enemy(Point p) : Caracter(p) { Construct(); }


	void Update(olc::Sprite* sprite, float time) override {
		Move(side, sprite, time);
		if (side == Side::Left)
			distance_done -= 1;
		else
			distance_done += 1;
		if (distance_done > distance_to_do && side == Side::Right)
			side = Side::Left;

		else if (distance_done < 0 && side == Side::Left)
			side = Side::Right;
		UpdatePosition(sprite, time);
	}

protected:
	void Construct() {
		sprite_stand_ = "images/Enemy.png";
		sprite_walk_ = "images/Enemy.png";
		animation.AddSprite(new olc::Sprite(sprite_stand_));
		animation.AddSprite(new olc::Sprite(sprite_walk_));
	}

private:
	Side side = Side::Right;
	int distance_done = 0;
	int distance_to_do = 100;
};