#pragma once


//Типы поверхностей
enum Grounds {
	None,		//Ничего (воздух(ничего нет))
	Normal,		//Простая земля 
	NormalItem,	//Для предметов чтобы можно было удалять их с карты без реинициализации
	Ice,		//Лёд  (возможно в дальнейшем персонаж будет по этой поверхности скользить)
	Water,		//Вода (возможно в дельнейшем персонаж будет там плавать)
	Lava		//Лава (возможно в дальнейшем персонаж будет на этой поверхности гореть)
};


enum Side {
	Up,
	Jump,
	Right,
	Left
};

//Точка, отражает точку на экране 
struct Point {
	int x;
	int y;

	explicit Point() :x(0), y(0) {}
	explicit Point(int x, int y) : x(x), y(y) {}

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

	bool operator<=(Point point) {
		if (x <= point.x && y <= point.y)
			return true;
		return false;
	}
	bool operator>=(Point point) {
		if (x >= point.y && y >= point.y)
			return true;
		return false;
	}
};