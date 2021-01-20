#pragma once
#include "include/lib/olcPixelGameEngine.h"
#include "include/Caracter.h"
#include <fstream>
#include <string>
#include <sstream>



class Enemy : public Caracter {
public:
	Enemy(const std::string& str) : Caracter() { Construct(str); }
	Enemy(int x, int y, const std::string& str) : Caracter(x, y) { Construct(str); }
	Enemy(Point p, const std::string& str) : Caracter(p) { Construct(str); }


	//File name without expension of the file
	static void InitEnemys(const std::string& file_name) {

		std::fstream stream;
		try {
			std::string str;
			stream.open(file_name + ".bin");

			while (getline(stream, str)) {

				int x, y;			//Позиция
				char par1, par2;	//Скобки
				char coma;			//Запятая
				char diff;			//Диффис
				char type;			//Тип, если 'e' то это враг
				std::string sprite_name;

				std::stringstream ss(str);

				ss >> type >> par1 >> x >> coma >> y >> par2 >> diff >> sprite_name;
				if (type == 'e' && coma == ',' && par1 == '(' && par2 == ')' && diff == '-') {
					Caracter::AddCaracter(new Enemy(x, y, sprite_name));
				}
			}
		}
		catch (std::exception e) {
			std::cout << "Error to open file " + file_name << std::endl;
		}
	}

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
	void Construct(const std::string& name) {
		speed_ = 1.0f;
		health_ = 10;
		sprite_stand_ = "images/" + name;
		sprite_walk_ = "images/" + name;
		animation.AddSprite(new olc::Sprite(sprite_stand_));
		animation.AddSprite(new olc::Sprite(sprite_walk_));
	}

private:
	int distance_done = 0;
	int distance_to_do = 100;
};