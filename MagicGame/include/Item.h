#pragma once
#include "Animation.h"
#include "Map.h"
#include <iostream>
#include "lib/olcPixelGameEngine.h"

//Точка, отражает точку на экране 
struct Point {
	int x;
	int y;

	Point() :x(0), y(0) {}
	Point(int x, int y) : x(x), y(y) {}

	bool operator==(Point point) {
		if (point.x == this->x && point.y == this->y)
			return true;
		return false;
	}
	bool operator!=(Point point) {
		if (!(*this == point))
			return true;
		return false;
	}
};

class Item {

public:

	Item(int x, int y) : x_(x), y_(y) {
		sprite_ = new olc::Sprite(item_image_);
	}
	Item(int x, int y, std::string& file) : x_(x), y_(y) {
		sprite_ = new olc::Sprite(items_directory + file);
	}

	inline Point GetPosition() { return Point(x_, y_); }

	inline olc::Sprite* GetSprite() {
		return sprite_;
	}

	inline void Break() {
		is_broken_ = true;
	}

	inline bool IsBroken() { return is_broken_; }

	inline int Width() { return sprite_->width; }
	inline int Height() { return sprite_->height; }


private:
	int x_, y_;
	bool is_broken_ = false;
	olc::Sprite* sprite_;

protected:
	std::string item_image_ = "images/box.png";
	std::string items_directory = "images/";
};


class Box : public Item {
public:
	using Item::Item;

};