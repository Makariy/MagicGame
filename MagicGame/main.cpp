
/*
									Magic Game 2020 
									Magic Game 2021
	Движок находиться в классе Game который наследует от olc::PixelGameEngine.
 При инициализации движка вызывается функция OnUserCreate в которой инициализируются все пременные. Каждый раз
 при обновлении кадров вызывается функция OnUserUpdate в которой отрисовывается и обсчитывается каждый объект.

	В заголовочном файле Map.h находится класс Map (карта) и констант высоты и ширины экрана.

	В заголовочном файле Caracter.h находится базовый класс Caracter (персонаж), этот класс отражает
 персонажей (в дельнейшем можно добавить врагов которые будут производным классом от класса Caracter).

	В заголовочном файле Gun.h находится базовый класс Gun (оружие), этот класс отражает оружие (можно использовать
 в качестве базового для создания новых типов оружия)

	В заголовочном файле Player.h находится производный класс Player (игрок), этот класс отражает игрока

	В заголовочном файле Animation.h находится класс для анимации Animation, этот класс так же является
 членом таких классов как Caracter и производных (Player), возможно будет использоваться и в классах Gun или Map

	В заголовочном файле Item.h находится определение структуры Point и класса Item отражающего предметы с которыми можно взаимодействовать 
 к примеру производный класс Box (коробка) представляет предмет можно рузрушить при помощи оружия MagicGun

	Когда персонаж проходит по карте на расстояние m_LeftPadding класса Game карта начинает двигаться по отношению к персонажу.
 Расстояние на которое продвинулся персонаж по карте находится в классе Map и взаимодействовать с ним можно через функции 
 SetPadding и GetPadding. При достижении пероснажем расстояния равного его позиции + ширины экрана отступ больше не увеличивается, 
 в обратную сторону всё идёт по описанному сценарию.

*/

#ifndef OLC_PGE_APPLICATION
	#define OLC_PGE_APPLICATION
#endif 

//Для отображения кадров в секуду
#ifndef WITH_FPS
	#define WITH_FPS
#endif


#include <iostream>
#include "lib/olcPixelGameEngine.h"
#include "Animation.h"
#include "Map.h"
#include "Caracter.h"
#include "Gun.h"
#include "Player.h"
#include "Item.h"
using namespace std;


#ifndef NOT_DEFINE
#ifndef MAP_H
	#error Map.h must be included before Caracter.h
#endif 

#ifndef CARACTER_H 
	#error Caracter.h must be included before Gun.h
#endif

#ifndef GUN_H
	#error Gun.h must be included
#endif

#ifndef ANIMATION_H
	#error Animation.h must be included 
#endif

#ifndef PLAYER_H
	#error Player.h must be included 
#endif
#endif

#include <fstream>
#include <ostream>
#include <strstream>
#include <algorithm>
#include <string>


//Классы используемые в игре 
class Caracter;
class Player;
class Map;
class Gun;
class Item;
class MagicGun1;
class Animation;


class Game : public olc::PixelGameEngine {
public:
	Game() {
		sAppName = "Game";
	}

public:
	Player player = Player(100, 50);
	Map* map;
	olc::Sprite* background_sprite = new olc::Sprite("images/First-location1.png");
	vector<olc::Sprite*> player_sprites;
	vector<olc::Sprite*> caracter_sprites;
	vector<Item> items_list;

	Animation animation;
	olc::Pixel background_color = olc::Pixel(0, 0, 0);
	Box box = Box(200, 200);
	Box box2 = Box(300, 140);


	
public:
	bool OnUserCreate() override {

		player_sprites.push_back(new olc::Sprite(player.GetStandSprite()));
		player_sprites.push_back(new olc::Sprite(player.GetWalkSprite()));

		map = new Map(background_sprite);

		for (Item i : map->GetItems())
			items_list.push_back(i);


		for (int i = 0; i < player_sprites.size(); i++)
			player.animation.AddSprite(player_sprites[i]);

		player.AttachToMap(map);

		return true;
	}

	//Вызывается при каждом кадре, получает в качестве параметра время прошедшее с момента отрисовки прощлого кадра 
	//Эта функция обновляет события и вызывает функции для отрисовки других спрайтов 
	bool OnUserUpdate(float fElapsedTime) override {

		CheckKeyboardEvents(fElapsedTime);

		olc::Sprite* player_sprite = player.animation.GetSprite();
		player.Update(player_sprite, fElapsedTime);

		bool map_moved = 
		FillScreen();

		DrawPlayer(player_sprite, map_moved);
		DrawItems();
		DrawGun();
		DrawCaracters();
		
		//DrawMapBorders();

		return true;
	}

private:
	//Нарисовать игрока
	void DrawPlayer(olc::Sprite* sprite, bool map_moved) {
		for (int x = 0; x < sprite->width; ++x) {
			for (int y = 0; y < sprite->height; ++y) {
				olc::Pixel p;
				if (player.NowSide == Side::Right)
					p = sprite->GetPixel(sprite->width - x - 1, y);
				else
					p = sprite->GetPixel(x, y);
				if (p == sprite->GetPixel(0, 0))
					continue;
				Point point = player.GetPosition();


				//Эта часть усеяна магическими выражениями!!!
				//Это были вынужденные меры в связи с реализацией функции FillScreen,
				//Которую в дальнейшем можно улучшить, и да код работает хорошо
				if(map_moved)
					Draw(x + m_LeftPadding, point.y + y, p);
				else{
					int test = background_sprite->width - player.GetPosition().x;
					if (test < screen_width)
					{
						test = screen_width - test;
						Draw(test + x, point.y + y, p);
					}
					else
						Draw(point.x + x, point.y + y, p);
				}
			}
		}
	}

	//Заполняет экран спрайтом бэкграунда 
	//Предупреждение!!! Код усеян магическими выражениями!!!
	//После длительного тестирования выяснилось что работает хорошо
	//В дальнейшем возможно код можно улучшить
	bool FillScreen() {

		bool map_moved = false;

		int y_pad = background_sprite->height - ScreenHeight();
		int x_pad = 0;

		if (background_sprite->width - player.GetPosition().x + m_LeftPadding > 0 &&
			player.GetPosition().x - m_LeftPadding > 0) {
			if (player.GetPosition().x + screen_width - m_LeftPadding >= background_sprite->width) {
				x_pad = background_sprite->width - screen_width;
			}
			//Чи нет
			else {
				map_moved = true;
				x_pad = player.GetPosition().x - m_LeftPadding;
			}
			map->SetPadding(x_pad);
		}

		for (int x = 0; x < ScreenWidth(); ++x) {
			for (int y = 0; y < ScreenHeight(); ++y) {
				Draw(x, y, background_sprite->GetPixel(x + x_pad, y + y_pad));
			}
		}
		return map_moved;
	}

	//Нарисовать все предметы находящиеся на карте  
	void DrawItems() {
		for (Item item : items_list) {
			for (int x = 0; x < item.Width(); ++x) {
				for (int y = 0; y < item.Height(); ++y) {
					olc::Pixel p = item.GetSprite()->GetPixel(x, y);
					if (p == item.GetSprite()->GetPixel(0, 0))
						continue;

					if (x + item.GetPosition().x - map->GetPadding() > screen_width
						|| x + item.GetPosition().x - map->GetPadding() <= 0)
						continue;
					
					Draw(x + item.GetPosition().x - map->GetPadding(), y + item.GetPosition().y, p);
				}
			}
		}
	}

	//Нарисовать всех персонажей находящихся на карте 
	void DrawCaracters() {}

	//Нарисовать пушку 
	void DrawGun() {
		if (player.IsAtacking()) {
			olc::Sprite* gun = player.gun.GetSprite();
			for (int x = 0; x < gun->width; x++) {
				for (int y = 0; y < gun->height; y++) {
					olc::Pixel pixel = gun->GetPixel(x, y);
					if (pixel == gun->GetPixel(0, 0))
						continue;
					Draw(x + player.gun.GetPosition().x - map->GetPadding(), y + player.gun.GetPosition().y, pixel);
				}
			}

			Grounds type;
			Item* touches = map->CheckIfTouchesItem(player.gun.GetPosition(), player.gun.GetSprite(), &type);

			if (touches && type == Grounds::NormalItem) {
				player.StopAtacking();
				RemoveItem(*touches);
			}
			else if (type != Grounds::None) {
				player.StopAtacking();
			}
		}
	}

	//Убирает предмет с карты и затем из массива всех предметов, тем самым пепрестаёт его рисовать и учитывать 
	void RemoveItem(Item item) {
		map->RemoveSprite(item);
		for (auto i = items_list.begin(); i < items_list.end(); i++) {
			if (i->GetPosition() == item.GetPosition()) {
				items_list.erase(i);
				return;
			}
		}
	}

	void DrawMapBorders() {

		for (int x = 0; x < screen_width; x++) {
			for (int y = 0; y < screen_height; y++) {
				if (x >= 0 && x < screen_width) {
					switch (map->At(x + map->GetPadding(), y)) {
						case Grounds::None:
							break;
						case Grounds::Normal:
							Draw(x, y, olc::BLACK);
							break;
						case Grounds::NormalItem:
							Draw(x, y, olc::YELLOW);
							break;
						default:
							Draw(x, y, olc::RED);
							break;
					}				
				}
				
			}
		}
	}

	//Проверяет действия игрока, если игрок что то нажал то вызывает соответсвующую функцию для класса Player
	void CheckKeyboardEvents(float fElapsedTime) {
		if (GetKey(olc::W).bHeld)player.Move(Side::Up, player_sprites[0], fElapsedTime);
		if (GetKey(olc::W).bPressed)player.Move(Side::Jump, player_sprites[0], fElapsedTime);
		if (GetKey(olc::A).bHeld) {
			player.Move(Side::Left, player_sprites[0], fElapsedTime);
			player.NowSide = Side::Left;
			player.animation.AddTime(fElapsedTime);
		}
		if (GetKey(olc::D).bHeld) {
			player.Move(Side::Right, player_sprites[0], fElapsedTime);
			player.NowSide = Side::Right;
			player.animation.AddTime(fElapsedTime);
		}

		if (GetKey(olc::D).bReleased || GetKey(olc::A).bReleased)
			player.animation.ResetIterator();

		if (GetKey(olc::R).bPressed)
			if (!player.IsAtacking())
				player.Atack1();
	}


private:
	vector<olc::Sprite*> sprites;
	//Отображает отступ от левой границы экрана 
	//с которым карта начинает двигаться за персонажем
	const int m_LeftPadding = 200;
};


int main(){

	Game* game = new Game();
	if (game->Construct(screen_width, screen_height, 1, 1, false, true))
		game->Start();
	return 0;

}

