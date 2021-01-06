#pragma once 
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include "lib/olcPixelGameEngine.h"
#include "Animation.h"
#include "Item.h"


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

//Типы поверхностей
enum Grounds {
	None,		//Ничего (воздух(ничего нет))
	Normal,		//Простая земля 
	NormalItem,	//Для предметов чтобы можно было удалять их с карты без реинициализации
	Ice,		//Лёд  (возможно в дальнейшем персонаж будет по этой поверхности скользить)
	Water,		//Вода (возможно в дельнейшем персонаж будет там плавать)
	Lava		//Лава (возможно в дальнейшем персонаж будет на этой поверхности гореть)
};




class Map {
private:
	//Открыть файл под названием _bg_name + .bin
	//Пройтись по этому файлу и провести линии между каждыми двумя точками в файле 
	//В дальнейшем в функции At можно проверить попадает ли спрайт на край 
	void m_InitBorders() {
		_border.reserve(_background_sprite->width);

		width = _background_sprite->width;
		height = _background_sprite->height;

		for (int i = 0; i < width; i++) {
			_border[i] = new Grounds[height];
		}

		for (int x = 0; x < width; ++x) {
			for (int y = 0; y < height; ++y) {
				_border[x][y] = Grounds::None;
			}
		}

		std::fstream stream;
		try {
			stream.open(_bg_name + ".bin");

			char par1;
			while (stream >> par1) {
				char mode, com1, com2, par2, dotcom;
				int x1, y1, x2, y2;
				stream >> mode >> com1 >> x1 >> com2 >> y1 >> par2;

				if (par1 == '(' && par2 == ')' && com1 == ',' && com2 == ',') {
					char dif;

					stream >> dif >> par1 >> x2 >> com2 >> y2 >> par2 >> dotcom;
					if (dif == '-' && par1 == '(' && par2 == ')' && com1 == ',' && com2 == ',' && dotcom == ';') {
						if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0 || x1 > width || x2 > width || y1 > height || y2 > height)
							continue;
						float delta = (float)(y2 - y1) / (float)(x2 - x1);
						float fy = (float)y1;
						if(x1 < x2)
							for (x1; x1 < x2; x1++) {
								_border[x1][(int)fy] = Grounds::Normal;
								fy += delta;
							}
						else if(x1 > x2)
							for (x1; x1 > x2; x1--) {
								_border[x1][(int)fy] = Grounds::Normal;
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
		stream.open(_bg_name  + ".bin");

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
		_background_sprite = new olc::Sprite(_bg_name);
		m_InitAll();
	}

	//Конструктор
	Map(std::string& sprite) {
		_background_sprite = new olc::Sprite(sprite);
		m_InitAll();

	}

	//Конструктор
	Map(olc::Sprite* sprite) {
		_background_sprite = sprite;
		m_InitAll();
	} 
	//Деструктор
	//Пока удаляю только _border
	~Map() {
		for (int i = 0; i < _border.size(); i++)
			delete[] _border[i];
	}

	//Принимает х и у которые являются позициями спрайта
	//Возвращает тип поверхности на которую попадает этот спрайт
	inline Grounds At(int x, int y) {
		if (x < 0 || x >= width || y < 0 || y >= height)
			return Grounds::Normal;
		return _border[x][y];
	}

public:
	//Добавление объектов на карту (коробки, враги)
	bool AddSprite(Item item, Grounds type = Grounds::NormalItem) {

		m_Items.push_back(item);
		bool not_touches = true;

		for (int xpos = item.GetPosition().x; xpos < item.GetPosition().x + item.GetSprite()->width; ++xpos) {
			for (int ypos = item.GetPosition().y; ypos < item.GetPosition().y + item.GetSprite()->height; ++ypos) {
				if (item.GetSprite()->GetPixel(0, 0) == item.GetSprite()->GetPixel(xpos - item.GetPosition().x, ypos - item.GetPosition().y))
					continue;
				if (_border[xpos][ypos] != Grounds::Normal)
					_border[xpos][ypos] = type;
				else
					not_touches = false;
			}
		}

		return not_touches;
	}

	void RemoveSprite(Item item) {
		for (int xpos = item.GetPosition().x; xpos < item.GetPosition().x + item.GetSprite()->width; ++xpos) {
			for (int ypos = item.GetPosition().y; ypos < item.GetPosition().y + item.GetSprite()->height; ++ypos) {
				if(_border[xpos][ypos] != Normal)
					_border[xpos][ypos] = Grounds::None;
			}
		}
		for (int i = 0; i < m_Items.size(); i++)
			if (m_Items[i].GetPosition() == item.GetPosition())
				m_Items.erase(m_Items.begin() + i);
	}

public:
	//Проверить попадает ли спрайт из параметра который находится на точке Point на этой карте 
	//на другой объект, если попадает то функция возвращает этот объект. Попасть спрайт может двумя краями либо серединой
	//Непонятный код но после тестирования выяснилось что работает.
	Item* CheckIfTouchesItem(Point point, olc::Sprite* sprite, Grounds* type) {

		Item* item = NULL;
		Grounds rtype = Grounds::None;
		for (int index = 0; index < m_Items.size(); index++) {
			Item i = m_Items[index];
			if (i.GetPosition().x + 2 <= point.x + 2 && i.GetPosition().x + i.Width() - 2 >= point.x 
				&& i.GetPosition().y < point.y + (sprite->height / 2) && i.GetPosition().y + i.Height() > point.y + (sprite->height/2)) {
				rtype = Grounds::NormalItem;
				item = &m_Items[index];
				break;
			}
			else if (i.GetPosition().x <= point.x + sprite->width - 2 && i.GetPosition().x + i.Width() >= point.x + sprite->width + 2
				&& i.GetPosition().y < point.y + sprite->height / 2 && i.GetPosition().y + i.Height() > point.y + sprite->height / 2) {
				rtype = Grounds::NormalItem;
				item = &m_Items[index];
				break;
			}
			else if (i.GetPosition().x <= point.x + sprite->width / 2 && i.GetPosition().x + i.Width() >= point.x + sprite->width / 2
				&& i.GetPosition().y < point.y + sprite->height / 2 && i.GetPosition().y + i.Height() > point.y + sprite->height / 2) {
				rtype = Grounds::NormalItem;
				item = &m_Items[index];
				break;
			}
		}

		int _border_padding = 5;

		if(item == NULL)
			for (int x = _border_padding; x < sprite->width - _border_padding; x++) {
				for (int y = _border_padding; y < sprite->height - _border_padding; y++) {
					if (this->At(point.x + x, point.y + y) != Grounds::None) {
						rtype = this->At(point.x + x, point.y + y);
						break;
					}
				}
			}
		*type = rtype;
		return item;
	}

public:
	inline void SetPadding(int padding) {
		m_NowPadding = padding;
	}

	inline int GetPadding() {
		return m_NowPadding;
	}

public:
	//Возвращает спрайт бэкграунда
	inline olc::Sprite* GetBackGroundSprite() {
		return _background_sprite;
	}
	//Возвращает все предметы прикреплённые к карте
	inline std::vector<Item>& GetItems() {
		return m_Items;
	}

private:
	//Отражает границы на карте, индексируя по X и Y возвращает тип поверзности на этой точке 
	std::vector<Grounds*> _border;
	//Имя файла с картой, изменяется в качестве параметра в конструкторе
	std::string _bg_name = "images/background-normal";
	//Спрайт карты
	olc::Sprite* _background_sprite;
	//Все предметы прикреплённые к карте
	std::vector<Item> m_Items;

	//Ширина и высота всей карты ( не экрана )
	int width;
	int height;

	//Отступ по которому продвинулся персонаж, часть карты от 0 + этот отступ не отрисовывается и от этого места происходят расчёты на карте 
	int m_NowPadding = 0;
};
