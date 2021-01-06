#pragma once 
#include "Map.h"
#include "Caracter.h"


#ifndef MAP_H
	#error Map.h must be included before Gun.h
#endif 

#ifndef CARACTER_H 
	#error Caracter.h must be included before Gun.h
#endif

#ifndef GUN_H
	#define GUN_H
#endif


class Gun {
public:
	//Конструкор, принимает название файла со спрайтом
	Gun(std::string fname) {
		_gun_sprite = new olc::Sprite(fname);
	}
	//Использовать пушку
	virtual bool Use(Side side) {
		return true;
	}
	inline olc::Sprite* GetSprite() {
		return _gun_sprite;
	}


private:
	olc::Sprite* _gun_sprite;
};



class MagicGun1 : public Gun {
public:
	using Gun::Gun;

public:
	//Обновляет положение оружия 
	void Update(float time) {

		x += speed * time / 0.01;
		distance += speed * time / 0.01;

		if(speed > 0)
			if (distance >= max_distance) {
				player->atacking = false;
				speed = 0;
				distance = 0;
			}
		if (speed < 0) {
			if (distance <= max_distance * -1) {
				player->atacking = false;
				speed = 0;
				distance = 0;
			}
		}
	}
	//Использовать пушку возвращает false если не может использовать
	bool Use(Caracter* player) {

		distance = 0;

		if (player->NowSide == Side::Right)
			speed = 3;
		else if (player->NowSide == Side::Left)
			speed = -3;

		x = player->GetPosition().x + 10;
		y = player->GetPosition().y - (int)(GetSprite()->height / 2) + 40;

		if (player->NowSide == Side::Left)
			x -= GetSprite()->width;

		this->player = player;

		return true;
	}


	Point GetPosition() {
		return Point(x, y);
	}


private:
	int x, y;
	//Дистанция на данный момент и максимальная дистанция которую может пролететь снаряд
	int distance, max_distance = 150;
	float speed;

	//Владелец пушки
	Caracter* player;
};
