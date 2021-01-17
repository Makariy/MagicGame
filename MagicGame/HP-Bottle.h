#pragma once
#include "include/lib/olcPixelGameEngine.h"
#include "include/Item.h"

class HP_Bottle {
public:
	HP_Bottle() {}
	inline olc::Sprite* GetSprite() {
		return sprite_;
	}
	inline Point GetStart() {
		return Point(hp_start_x_, hp_start_y_);
	}
	inline Point GetEnd() {
		return Point(hp_now_width_, hp_now_height_);
	}
	inline void SetHealth(int num) {
		num = num * hp_width_ / 100;
		hp_now_width_ = num;
	}

private:
	olc::Sprite* sprite_ = new olc::Sprite("images/HP-bottle.png");
	const int hp_width_ = 98;
	const int hp_height_ = 21;
	const int hp_start_x_ = 5;
	const int hp_start_y_ = 5;
	int hp_now_width_ = hp_width_;
	int hp_now_height_ = hp_height_;
};