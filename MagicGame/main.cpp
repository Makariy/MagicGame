
/*
									Magic Game 2020 
									Magic Game 2021
	Движок находиться в классе Game в файле main.cpp который наследует от olc::PixelGameEngine.
 При инициализации движка вызывается функция OnUserCreate в которой инициализируются все пременные. Каждый раз
 при обновлении кадров вызывается функция OnUserUpdate в которой отрисовывается и обсчитывается каждый объект.
 
	В заголовочном файле Map.h находится класс Map (карта), ТИПЫ ПОВЕРХНОСТЕЙ и констант высоты и ширины экрана.
 Класс Map предназначен для хранения карты и взаимодействия с ней ( привызявание объектов, отвыязвание объектов, преверка
 касания персонажей краёв карты или объектов, проверка касания оружия к предметам либо краям, а также передвижение карты за персонажем. 
 Крайне выжно! что по причине того что я рисовал и модифицировал спрайты через Paint и Photoshop прозрачность заднего фона потерялась.
 По этому пришлось прибегнуть к использованию магических константб их сочитание в системе rgb в виде (254, 255, 255, 1) означает цвет который 
 является задним фоном спрайта и не должен отрисовываться, также было принято решение устанавливать левый верхний пиксель каждого спрайта содержащего
 задний фон этим цветом что и можно увидеть в функции класса Game::DrawPlayer где сравнивается взятый пиксель с пикселем заднего фона по
 координатам (0,0), если они совпадают то первый не отрисовывается. В дальнейшем можно всё же сделать задний фон нормальным. 

	В заголовочном файле Caracter.h находится базовый класс Caracter (персонаж), этот класс отражает
 персонажей (в дельнейшем можно добавить врагов которые будут производным классом от класса Caracter).

	В заголовочном файле Gun.h находится базовый класс Gun (оружие), этот класс отражает оружие (можно использовать
 в качестве базового для создания новых типов оружия)

	В заголовочном файле Player.h находится производный класс Player (игрок), этот класс отражает игрока.

	В заголовочном файле Animation.h находится класс для анимации Animation, этот класс так же является
 членом таких классов как Caracter и производных (Player), возможно будет использоваться и в классах Gun или Map

	В заголовочном файле Item.h находится определение структуры Point и класса Item отражающего предметы с которыми можно взаимодействовать 
 к примеру производный класс Box (коробка) представляет предмет можно рузрушить при помощи оружия MagicGun

	Когда персонаж проходит по карте на расстояние left_padding_ класса Game карта начинает двигаться по отношению к персонажу.
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

#include <fstream>
#include <ostream>
#include <strstream>
#include <algorithm>
#include <string>

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

private:
	Player player_ = Player(100, 50);
	Map* map_;
	olc::Sprite* background_sprite_ = new olc::Sprite("images/First-location1.png");
	vector<olc::Sprite*> player_sprites_;
	vector<olc::Sprite*> caracter_sprites_;
	vector<Item> items_list_;

	olc::Pixel background_color_ = olc::Pixel(0, 0, 0);

	vector<olc::Sprite*> sprites_;
	//Отображает отступ от левой границы экрана 
	//с которым карта начинает двигаться за персонажем
	const int left_padding_ = 200;


	
public:
	bool OnUserCreate() override {

		player_sprites_.push_back(new olc::Sprite(player_.GetStandSprite()));
		player_sprites_.push_back(new olc::Sprite(player_.GetWalkSprite()));

		map_ = new Map(background_sprite_);

		for (Item i : map_->GetItems())
			items_list_.push_back(i);


		for (int i = 0; i < player_sprites_.size(); i++)
			player_.animation.AddSprite(player_sprites_[i]);

		player_.AttachToMap(map_);

		return true;
	}

	//Вызывается при каждом кадре, получает в качестве параметра время прошедшее с момента отрисовки прощлого кадра 
	//Эта функция обновляет события и вызывает функции для отрисовки других спрайтов 
	bool OnUserUpdate(float fElapsedTime) override {

		CheckKeyboardEvents(fElapsedTime);

		olc::Sprite* player_sprite = player_.animation.GetSprite();
		player_.Update(player_sprite, fElapsedTime);

		bool map_moved = 
		FillScreen();

		DrawPlayer(player_sprite, map_moved);
		DrawItems();
		DrawGun();
		DrawCaracters();
		
		DrawMapBorders();

		return true;
	}

private:
	//Нарисовать игрока
	void DrawPlayer(olc::Sprite* sprite, bool map_moved) {
		for (int x = 0; x < sprite->width; ++x) {
			for (int y = 0; y < sprite->height; ++y) {
				olc::Pixel p;
				if (player_.NowSide == Side::Right)
					p = sprite->GetPixel(sprite->width - x - 1, y);
				else
					p = sprite->GetPixel(x, y);
				if (p == sprite->GetPixel(0, 0))
					continue;
				Point point = player_.GetPosition();


				//Эта часть усеяна магическими выражениями!!!
				//Это были вынужденные меры в связи с реализацией функции FillScreen,
				//Которую в дальнейшем можно улучшить, и да код работает хорошо
				if(map_moved)
					Draw(x + left_padding_, point.y + y, p);
				else{
					int test = background_sprite_->width - player_.GetPosition().x;
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

		int y_pad = background_sprite_->height - ScreenHeight();
		int x_pad = 0;

		if (background_sprite_->width - player_.GetPosition().x + left_padding_ > 0 &&
			player_.GetPosition().x - left_padding_ > 0) {
			if (player_.GetPosition().x + screen_width - left_padding_ >= background_sprite_->width) {
				x_pad = background_sprite_->width - screen_width;
			}
			//Чи нет
			else {
				map_moved = true;
				x_pad = player_.GetPosition().x - left_padding_;
			}
			map_->SetPadding(x_pad);
		}

		for (int x = 0; x < ScreenWidth(); ++x) {
			for (int y = 0; y < ScreenHeight(); ++y) {
				Draw(x, y, background_sprite_->GetPixel(x + x_pad, y + y_pad));
			}
		}
		return map_moved;
	}

	//Нарисовать все предметы находящиеся на карте  
	void DrawItems() {
		for (Item item : items_list_) {
			for (int x = 0; x < item.Width(); ++x) {
				for (int y = 0; y < item.Height(); ++y) {
					olc::Pixel p = item.GetSprite()->GetPixel(x, y);
					if (p == item.GetSprite()->GetPixel(0, 0))
						continue;

					if (x + item.GetPosition().x - map_->GetPadding() > screen_width
						|| x + item.GetPosition().x - map_->GetPadding() <= 0)
						continue;
					
					Draw(x + item.GetPosition().x - map_->GetPadding(), y + item.GetPosition().y, p);
				}
			}
		}
	}

	//Нарисовать всех персонажей находящихся на карте 
	void DrawCaracters() {}

	//Нарисовать пушку 
	void DrawGun() {
		if (player_.IsAtacking()) {
			olc::Sprite* gun = player_.gun.GetSprite();
			for (int x = 0; x < gun->width; x++) {
				for (int y = 0; y < gun->height; y++) {
					olc::Pixel pixel = gun->GetPixel(x, y);
					if (pixel == gun->GetPixel(0, 0))
						continue;
					Draw(x + player_.gun.GetPosition().x - map_->GetPadding(), y + player_.gun.GetPosition().y, pixel);
				}
			}

			Grounds type;
			Item* touches = map_->CheckIfTouchesItem(player_.gun.GetPosition(), player_.gun.GetSprite(), &type);

			if (touches && type == Grounds::NormalItem) {
				player_.StopAtacking();
				RemoveItem(*touches);
			}
			else if (type != Grounds::None) {
				player_.StopAtacking();
			}
		}
	}

	//Убирает предмет с карты и затем из массива всех предметов, тем самым пепрестаёт его рисовать и учитывать 
	void RemoveItem(Item item) {
		map_->RemoveSprite(item);
		for (auto i = items_list_.begin(); i < items_list_.end(); i++) {
			if (i->GetPosition() == item.GetPosition()) {
				items_list_.erase(i);
				return;
			}
		}
	}

	void DrawMapBorders() {

		for (int x = 0; x < screen_width; x++) {
			for (int y = 0; y < screen_height; y++) {
				if (x >= 0 && x < screen_width) {
					switch (map_->At(x + map_->GetPadding(), y)) {
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
		if (GetKey(olc::W).bHeld)player_.Move(Side::Up, player_sprites_[0], fElapsedTime);
		if (GetKey(olc::W).bPressed)player_.Move(Side::Jump, player_sprites_[0], fElapsedTime);
		if (GetKey(olc::A).bHeld) {
			player_.Move(Side::Left, player_sprites_[0], fElapsedTime);
			player_.NowSide = Side::Left;
			player_.animation.AddTime(fElapsedTime);
		}
		if (GetKey(olc::D).bHeld) {
			player_.Move(Side::Right, player_sprites_[0], fElapsedTime);
			player_.NowSide = Side::Right;
			player_.animation.AddTime(fElapsedTime);
		}

		if (GetKey(olc::D).bReleased || GetKey(olc::A).bReleased)
			player_.animation.ResetIterator();

		if (GetKey(olc::R).bPressed)
			if (!player_.IsAtacking())
				player_.Atack1();
	}

};


int main(){

	Game* game = new Game();
	if (game->Construct(screen_width, screen_height, 1, 1, false, true))
		game->Start();
	return 0;

}

