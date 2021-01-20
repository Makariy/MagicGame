#pragma once 
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include "lib/olcPixelGameEngine.h"
#include "Animation.h"
#include "Item.h"
#include "Grounds.h"


#ifndef ANIMATION_H
	#error Animation.h must be included straight after GameEngine 
#endif

#ifndef MAP_H
	#define MAP_H
#endif




//Высота и ширина экрана
constexpr int screen_width = 820;
constexpr int screen_height = 430;


//Оператор вывода точки (для этапа отладки)
std::ostream& operator<<(std::ostream& stream, const Point& p) {
	return stream << p.x << ' ' << p.y << std::endl;
}



class Map {
private:
	//Открыть файл под названием bg_name_ + .bin
	//Пройтись по этому файлу и провести линии между каждыми двумя точками в файле 
	//В дальнейшем в функции At можно проверить попадает ли спрайт на край 
	void m_InitBorders() {
		border_.reserve(background_sprite_->width);

		width_ = background_sprite_->width;
		height_ = background_sprite_->height;

		for (int i = 0; i < width_; i++) {
			border_[i] = new Grounds[height_];
		}

		for (int x = 0; x < width_; ++x) {
			for (int y = 0; y < height_; ++y) {
				border_[x][y] = Grounds::None;
			}
		}

		std::fstream stream;
		try {
			stream.open(bg_name_ + ".bin");

			char par1;
			while (stream >> par1) {
				char mode, com1, com2, par2, dotcom;
				int x1, y1, x2, y2;
				stream >> mode >> com1 >> x1 >> com2 >> y1 >> par2;

				if (par1 == '(' && par2 == ')' && com1 == ',' && com2 == ',') {
					char dif;

					stream >> dif >> par1 >> x2 >> com2 >> y2 >> par2 >> dotcom;
					if (dif == '-' && par1 == '(' && par2 == ')' && com1 == ',' && com2 == ',' && dotcom == ';') {
						if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0 || x1 > width_ || x2 > width_ || y1 > height_ || y2 > height_)
							continue;
						float delta = (float)(y2 - y1) / (float)(x2 - x1);
						float fy = (float)y1;
						if(x1 < x2)
							for (x1; x1 < x2; x1++) {
								border_[x1][(int)fy] = Grounds::Normal;
								fy += delta;
							}
						else if(x1 > x2)
							for (x1; x1 > x2; x1--) {
								border_[x1][(int)fy] = Grounds::Normal;
								fy += delta;
							}
					}
				}
			}
		}
		catch (std::exception e)
		{
			std::cout << "Error\n" << e.what() << std::endl;
			stream.close();
		}
	}

	//Инициализировать все предметы на карте( коробки всё такое )
	void m_InitItems() {
		std::fstream stream;
		stream.open(bg_name_  + ".bin");

		std::string str;
		try {
			while (getline(stream, str)) {
				char p1, p2, minus;
				char coma;
				int pos_x, pos_y;
				std::string type;

				std::stringstream sstr(str);

				sstr >> p1 >> pos_x >> coma >> pos_y >> p2 >> minus >> type;

				if (p1 == '(') {
					if (coma == ',' && p2 == ')' && minus == '-') {
						Item item(pos_x, pos_y, type);
						this->AddSprite(item);
						
					}
				}
			}
		}
		catch (std::exception e) {
			std::cout << e.what() << std::endl;
		}
		
	}

	//Функция просто для облегчения конструктора
	//Инициализирует границы карты и предметы карты
	void m_InitAll() {
		this->m_InitBorders();
		this->m_InitItems();
	}

public:
	
	//Конструктор
	Map() {
		background_sprite_ = new olc::Sprite(bg_name_);
		m_InitAll();
	}

	//Конструктор
	Map(std::string& sprite) {
		background_sprite_ = new olc::Sprite(sprite);
		m_InitAll();

	}

	//Конструктор
	Map(olc::Sprite* sprite) {
		background_sprite_ = sprite;
		m_InitAll();
	} 
	//Деструктор
	//Пока удаляю только border_
	~Map() {
		for (int i = 0; i < border_.size(); i++)
			delete[] border_[i];
	}

	//Принимает х и у которые являются позициями спрайта
	//Возвращает тип поверхности на которую попадает этот спрайт
	Grounds At(int x, int y) {
		if (x < 0 || x > width_ - 1 || y < 0 || y > height_ - 1)
			return Grounds::Normal;
		return border_[x][y];
		
	}

public:
	//Добавление объектов на карту (коробки, враги)
	bool AddSprite(Item item, Grounds type = Grounds::NormalItem) {

		items_.push_back(item);
		bool not_touches = true;

		for (int xpos = item.GetPosition().x; xpos < item.GetPosition().x + item.GetSprite()->width; ++xpos) {
			for (int ypos = item.GetPosition().y; ypos < item.GetPosition().y + item.GetSprite()->height; ++ypos) {
				if (item.GetSprite()->GetPixel(0, 0) == item.GetSprite()->GetPixel(xpos - item.GetPosition().x, ypos - item.GetPosition().y))
					continue;
				if (border_[xpos][ypos] != Grounds::Normal)
					border_[xpos][ypos] = type;
				else
					not_touches = false;
			}
		}

		return not_touches;
	}

	void RemoveSprite(Item item) {
		for (int xpos = item.GetPosition().x; xpos < item.GetPosition().x + item.GetSprite()->width; ++xpos) {
			for (int ypos = item.GetPosition().y; ypos < item.GetPosition().y + item.GetSprite()->height; ++ypos) {
				if(border_[xpos][ypos] != Normal)
					border_[xpos][ypos] = Grounds::None;
			}
		}
		for (int i = 0; i < items_.size(); i++)
			if (items_[i].GetPosition() == item.GetPosition())
				items_.erase(items_.begin() + i);
	}

public:
	//Проверить попадает ли спрайт из параметра который находится на точке Point на этой карте 
	//на другой объект, если попадает то функция возвращает этот объект. Попасть спрайт может двумя краями либо серединой
	//Непонятный код но после тестирования выяснилось что работает.
	//Третий параметр возвращает тип поверхности которой касается персонаж ( не может быть Grounds::None )
	Item* CheckIfTouchesItem(Point point, olc::Sprite* sprite, Grounds* type) {

		Item* item = NULL;
		Grounds rtype = Grounds::None;
		for (int index = 0; index < items_.size(); index++) {
			Item i = items_[index];
			if (i.GetPosition().x + 2 <= point.x + 2 && i.GetPosition().x + i.Width() - 2 >= point.x 
				&& i.GetPosition().y < point.y + (sprite->height / 2) && i.GetPosition().y + i.Height() > point.y + (sprite->height/2)) {
				item = &items_[index];
				break;
			}
			else if (i.GetPosition().x <= point.x + sprite->width - 2 && i.GetPosition().x + i.Width() >= point.x + sprite->width + 2
				&& i.GetPosition().y < point.y + sprite->height / 2 && i.GetPosition().y + i.Height() > point.y + sprite->height / 2) {
				item = &items_[index];
				break;
			}
			else if (i.GetPosition().x <= point.x + sprite->width / 2 && i.GetPosition().x + i.Width() >= point.x + sprite->width / 2
				&& i.GetPosition().y < point.y + sprite->height / 2 && i.GetPosition().y + i.Height() > point.y + sprite->height / 2) {
				item = &items_[index];
				break;
			}
		}

		int border__padding = 5;

		if (item == NULL) {
			for (int x = border__padding; x < sprite->width - border__padding; x++) {
				for (int y = border__padding; y < sprite->height - border__padding; y++) {
					if (this->At(point.x + x, point.y + y) != Grounds::None) {
						rtype = this->At(point.x + x, point.y + y);
						break;
					}
				}
			}
		}
		else
			rtype = Grounds::NormalItem;

		*type = rtype;
		return item;
	}

	static bool IsBetween(Point p, int x1, int y1, int x2, int y2) {
		if (p.x >= x1 && p.x <= x2 && p.y >= y1 && p.y <= y2)
			return true;
		return false;
	}

	static bool IsBetween(Point p, Point p1, Point p2, Side& side, int width = 0, int height = 0) {
		for (int x = p.x; x < p.x + width; x++)
			for (int y = p.y; y < p.y + height; y++)
				if (p1.x <= x && x <= p2.x && p1.y <= y && y <= p2.y) {
					if (x >= p.x + (width / 2))
						side = Side::Left;
					else if (x < p.x + (width / 2))
						side = Side::Right;
					return true;
				}
		return false;
	}

public:
	inline void SetPadding(int padding) {
		now_padding_ = padding;
	}

	inline int GetPadding() {
		return now_padding_;
	}

public:
	//Возвращает спрайт бэкграунда
	inline olc::Sprite* GetBackGroundSprite() {
		return background_sprite_;
	}
	//Возвращает все предметы прикреплённые к карте
	inline std::vector<Item>& GetItems() {
		return items_;
	}


private:
	//Отражает границы на карте, индексируя по X и Y возвращает тип поверзности на этой точке 
	std::vector<Grounds*> border_;
	//Имя файла с картой, изменяется в качестве параметра в конструкторе
	std::string bg_name_ = "images/background-normal";
	//Спрайт карты
	olc::Sprite* background_sprite_;
	//Все предметы прикреплённые к карте
	std::vector<Item> items_;

	//Ширина и высота всей карты ( не экрана )
	int width_;
	int height_;

	//Отступ по которому продвинулся персонаж, часть карты от 0 + этот отступ не отрисовывается и от этого места происходят расчёты на карте 
	int now_padding_ = 0;
};
