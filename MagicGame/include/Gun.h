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
		gun_sprite_ = new olc::Sprite(fname);
	}
	//Использовать пушку
	virtual bool Use(Side side) {
		return true;
	}
	inline olc::Sprite* GetSprite() {
		return gun_sprite_;
	}


protected:
	olc::Sprite* gun_sprite_;
};



class MagicGun1 : public Gun {
public:
	using Gun::Gun;

public:
	//Обновляет положение оружия 
	void Update(float time) {

		x_ += speed_ * time / 0.01;
		distance_ += speed_ * time / 0.01;

		if(speed_ > 0)
			if (distance_ >= max_distance_) {
				player_->atacking = false;
				speed_ = 0;
				distance_ = 0;
			}
		if (speed_ < 0) {
			if (distance_ <= max_distance_ * -1) {
				player_->atacking = false;
				speed_ = 0;
				distance_ = 0;
			}
		}

		Side side;
		Caracter* caracter = Caracter::CheckIfTouchesCaracter(Point(x_, y_), gun_sprite_, side);
		if (caracter) {
			Caracter::RemoveCaracter(caracter);
			player_->atacking = false;
		}
	}
	//Использовать пушку возвращает false если не может использовать
	bool Use(Caracter* player_) {

		distance_ = 0;

		if (player_->NowSide == Side::Right)
			speed_ = 3;
		else if (player_->NowSide == Side::Left)
			speed_ = -3;

		x_ = player_->GetPosition().x + 10;
		y_ = player_->GetPosition().y - (int)(GetSprite()->height / 2) + 40;

		if (player_->NowSide == Side::Left)
			x_ -= GetSprite()->width;

		this->player_ = player_;

		return true;
	}


	Point GetPosition() {
		return Point(x_, y_);
	}


private:
	int x_, y_;
	//Дистанция на данный момент и максимальная дистанция которую может пролететь снаряд
	int distance_, max_distance_ = 150;
	float speed_;

	//Владелец пушки
	Caracter* player_;
};
