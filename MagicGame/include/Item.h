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
private:
	int x, y;
	bool m_IsBroken = false;
	olc::Sprite* m_Sprite;

protected:
	std::string m_ItemImage = "images/box.png";
	std::string m_ItemsDirectory = "images/";

public:

	Item(int x, int y) : x(x), y(y) {
		m_Sprite = new olc::Sprite(m_ItemImage);
	}
	Item(int x, int y, std::string& file) : x(x), y(y) {
		m_Sprite = new olc::Sprite(m_ItemsDirectory + file);
	}

	inline Point GetPosition() { return Point(x, y); }

	inline olc::Sprite* GetSprite() {
		return m_Sprite;
	}

	inline void Break() {
		m_IsBroken = false;
	}

	inline bool IsBroken() { return m_IsBroken; }

	inline int Width() { return m_Sprite->width; }
	inline int Height() { return m_Sprite->height; }

};


class Box : public Item {
public:
	using Item::Item;

};