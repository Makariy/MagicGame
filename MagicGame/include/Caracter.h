#pragma once 
#include "Map.h"
#include <exception>


#ifndef ANIMATION_H
	#error Animation.h must be included straight after GameEngine 
#endif

#ifndef MAP_H
	#error Map.h must be included before Caracter.h
#endif

#ifndef CARACTER_H
	#define CARACTER_H
#endif 


enum Side {
	Up,
	Jump,
	Right,
	Left
};

class Caracter {
public:
	Caracter() :pos_x(0), pos_y(0) {}
	Caracter(int x, int y) :pos_x(x), pos_y(y) {}
	Caracter(Point p) : pos_x(p.x), pos_y(p.y) {}
	~Caracter() {}
	bool atacking = false;

	Animation animation;

	//Получение всяких свойств
public:
	virtual inline float GetSpeed() {
		return speed;
	}
	virtual inline std::string GetStandSprite() {
		return sprite_stand;
	}
	virtual inline std::string GetWalkSprite() {
		return sprite_walk;
	}

	virtual inline Point GetPosition() {
		return Point(pos_x, pos_y);
	}

public:
	//Привязать к карте
	inline void AttachToMap(Map* m) { this->map = m; }

	//Обновляет положение на карте и всякие события 
	virtual void Update(olc::Sprite* sprite, float time) {
		if (!Touches(pos_x, pos_y * drop_speed, sprite)) {
			pos_y += 1 * drop_speed * time / 0.02;
			drop_speed += drop_speed * (1+drop_speed_coeficient);
		}
		else {
			drop_speed = 0.0f;
		}
	}

	//Проверяет если персонаж касается какой то поверхности карты 
	bool Touches(int x_pad, int y_pad, olc::Sprite* sprite) {

		int _pos_x = (int)pos_x;
		int _pos_y = (int)pos_y;
		for (int x = _pos_x + x_pad; x < _pos_x + x_pad + sprite->width; x++)
			for (int y = _pos_y + y_pad; y < _pos_y + y_pad + sprite->height; y++)
				if (map->At(x, y) != Grounds::None) {
					if (sprite->GetPixel(0, 0) == sprite->GetPixel(x - (int)(x_pad - pos_x), y - (int)(y_pad - pos_x)))
						continue;
					return true;
				}
		return false;
	}
	//Поменять спрайт
	void ChangeSprite(const std::string& stand, const std::string& walk) {
		sprite_stand = stand;
		sprite_walk = walk;
	}
	//Поменять скорость
	void ChangeSpeed(int num) {
		speed = num;
	}

	//Подвинуться, если не может то возвращает false
	virtual bool Move(Side side, olc::Sprite* sprite, float time) {
		int x = 0;
		int y = 0;
		switch (side) {
			case Side::Right:
				x = 1;
				break;
			case Side::Left:
				x = -1;
				break;
			case Side::Up:
				y = -1;
				break;
			case Side::Jump:
				y = -6;
				break;
			default:
				throw std::runtime_error("Wrong moving side argument in class Cararcter");
		}

	
		if (x > 0) {
			x = x + time / 0.018;

			if (this->Touches(x+2, y, sprite))
				if (this->Touches(x+2, y - 3, sprite))
					return false;
				else
					y = y - 2;
		}
		else if (x < 0) {
			x = x - time / 0.018;

			if (this->Touches(x - 2, y, sprite)) {
				if (this->Touches(x - 2, y - 3, sprite))
					return false;
				else
					y = y - 2;
			}
		}
		else {
			y = y - time / 0.012;
			if (this->Touches(x, y-3, sprite))
				return false;
		}

		pos_x += x * speed;
		pos_y += y * 2;
		
		return true;
	}
	Side NowSide = Side::Right;

protected:
	Map* map;

	float pos_x;
	int pos_y;

	float speed = 2.0f;
	//НЕ ОТОБРАЖАЕТ СКОРОСТЬ ПАДЕНИЯ КАК КОНСТАНТУ 
	//Отображает корость падения на днный момент 
	float drop_speed = 0.0f;
	//А вот это как раз отображает то с какой скоростью будет падать персонаж
	float drop_speed_coeficient = 0.03f;

	std::string sprite_stand;
	std::string sprite_walk;
};
