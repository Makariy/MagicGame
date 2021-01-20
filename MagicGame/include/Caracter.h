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





class Caracter {
public:
	Caracter() : pos_x_(0), pos_y_(0) {  }
	Caracter(int x, int y) :pos_x_(x), pos_y_(y) {  }
	Caracter(Point p) : pos_x_(p.x), pos_y_(p.y) {  }
	~Caracter() {  }

	struct MoveInstruction {
		Side side; // Сторона для движения
		float time; // Требуется для функции Move
		int times; // Количество раз для повторения
		olc::Sprite* animation; 
		float call_down = kcall_down;

		float kcall_down = 0.5;

		bool is_on_calldown = false;
		bool is_moving_for = false;

		bool IsOnCallDown() {
			return is_on_calldown;
		}
	};

	static void AddCaracter(Caracter* caracter) {
		Caracter::caracters.push_back(caracter);
	}
	static bool RemoveCaracter(Caracter* caracter) {
		for (auto iter = Caracter::caracters.begin(); iter < Caracter::caracters.end(); iter++) {
			if (*iter == caracter) {
				Caracter::caracters.erase(iter);
				return true;
			}
		}
		return false;
	}

	static Caracter* CheckIfTouchesCaracter(Point p, olc::Sprite* sprite, Side& side) {

		for (Caracter* car : Caracter::caracters) {
			olc::Sprite* car_sprite = car->animation.GetNowSprite();
			if (Map::IsBetween(p, car->GetPosition(), Point(car->GetPosition().x + car_sprite->width, car->GetPosition().y + car_sprite->height),
				side, sprite->width, sprite->height))
				return car;
		}
		return NULL;
	}

	bool atacking = false;

	Animation animation;

	static std::vector<Caracter*> caracters;

	//Получение всяких свойств
public:
	virtual inline float GetSpeed() {
		return speed_;
	}
	virtual inline std::string GetStandSprite() {
		return sprite_stand_;
	}
	virtual inline std::string GetWalkSprite() {
		return sprite_walk_;
	}
	virtual inline Point GetPosition() {
		return Point(pos_x_, pos_y_);
	}
	inline bool IsDead() {
		return is_dead_;
	}

public:

	static void AttachAllCaractersToMap(Map* map) {
		for (Caracter* caracter : caracters)
			caracter->AttachToMap(map);
	}
	//Привязать к карте
	void AttachToMap(Map* m) { this->map_ = m; }

	//Обновляет положение на карте и всякие события 
	virtual void Update(olc::Sprite* sprite, float time) {
		UpdatePosition(sprite, time);
	}


	void UpdatePosition(olc::Sprite* sprite, float time) {
		if (!Touches(0, 1 * drop_speed_, sprite)) {
			pos_y_ += 1 * drop_speed_;
			drop_speed_ += 0.1f * time / 0.01;
		}
		else
			drop_speed_ = 0.0f;
	}

	virtual void Damage(int num) {
		health_ -= num;
		if (health_ < 0)
			is_dead_ = true;
	}

	virtual void Heal(int num) {
		if (health_ + num <= max_health_)
			health_ += num;
		else
			health_ = max_health_;
	}
	//Проверяет если персонаж касается какой то поверхности карты 
	bool Touches(int x_pad, int y_pad, olc::Sprite* sprite) {
		int pos_x = (int)pos_x_;
		int pos_y = (int)pos_y_;
		for (int x = pos_x + x_pad; x < pos_x + x_pad + sprite->width; x++) {
			for (int y = pos_y + y_pad; y < pos_y + y_pad + sprite->height; y++) {
				if (map_->At(x, y) != Grounds::None) {
					if (sprite->GetPixel(0, 0) == sprite->GetPixel(x - (int)(x_pad - pos_x_), y - (int)(y_pad - pos_x_)))
						continue;
					return true;
				}
			}
		}
		return false;
	}

	//Поменять спрайт
	void ChangeSprite(const std::string& stand, const std::string& walk) {
		sprite_stand_ = stand;
		sprite_walk_ = walk;
	}
	//Поменять скорость
	void ChangeSpeed(int num) {
		speed_ = num;
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

		pos_x_ += x * speed_;
		pos_y_ += y * 2;
		
		return true;
	}

	virtual bool Move(MoveInstruction instruction) {
		return Move(instruction.side, instruction.animation, instruction.time);
	}

	inline Side GetNowSide() {
		return NowSide;
	}

	Side NowSide = Side::Right;
protected:

	Map* map_;

	MoveInstruction move_instruction_;

	float pos_x_;
	int pos_y_;

	int max_health_ = 100;

	int health_ = 100;
	float speed_ = 2.0f;

	//НЕ ОТОБРАЖАЕТ СКОРОСТЬ ПАДЕНИЯ КАК КОНСТАНТУ 
	//Отображает корость падения на днный момент 
	float drop_speed_ = 0.0f;
	//А вот это как раз отображает то с какой скоростью будет падать персонаж
	float drop_speed_coeficient_ = 0.03f;

	bool is_dead_ = false;

	std::string sprite_stand_;
	std::string sprite_walk_;
};

std::vector<Caracter*> Caracter::caracters;