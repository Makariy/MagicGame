#pragma once
#include "include/lib/olcPixelGameEngine.h"
#include "include/Caracter.h"


class Enemy : public Caracter {
public:
	Enemy() : Caracter() { Construct(); }
	Enemy(int x, int y) : Caracter(x, y) { Construct(); }
	Enemy(Point p) : Caracter(p) { Construct(); }


	void Update(olc::Sprite* sprite, float time) override {
		if (Move(NowSide, sprite, time)) {}
		else if (NowSide = Side::Left) {
			NowSide = Side::Right;
			distance_done = distance_to_do - distance_done;
		}
		else {
			NowSide = Side::Left;
		}
		if (NowSide == Side::Left)
			distance_done -= 1;
		else
			distance_done += 1;
		if (distance_done > distance_to_do && NowSide == Side::Right)
			NowSide = Side::Left;

		else if (distance_done < 0 && NowSide == Side::Left)
			NowSide = Side::Right;
		UpdatePosition(sprite, time);
	}

protected:
	void Construct() {
		speed_ = 1.0f;
		health_ = 10;
		sprite_stand_ = "images/Enemy.png";
		sprite_walk_ = "images/Enemy.png";
		animation.AddSprite(new olc::Sprite(sprite_stand_));
		animation.AddSprite(new olc::Sprite(sprite_walk_));
	}

private:
	int distance_done = 0;
	int distance_to_do = 100;
};