
/*
									Magic Game 2020 
									Magic Game 2021
	������ ���������� � ������ Game � ����� main.cpp ������� ��������� �� olc::PixelGameEngine.
 ��� ������������� ������ ���������� ������� OnUserCreate � ������� ���������������� ��� ���������. ������ ���
 ��� ���������� ������ ���������� ������� OnUserUpdate � ������� �������������� � ������������� ������ ������.
 
	� ������������ ����� Map.h ��������� ����� Map (�����), ���� ������������ � �������� ������ � ������ ������.
 ����� Map ������������ ��� �������� ����� � �������������� � ��� ( ������������ ��������, ����������� ��������, ��������
 ������� ���������� ���� ����� ��� ��������, �������� ������� ������ � ��������� ���� �����, � ����� ������������ ����� �� ����������. 
 ������ �����! ��� �� ������� ���� ��� � ������� � ������������� ������� ����� Paint � Photoshop ������������ ������� ���� ����������.
 �� ����� �������� ���������� � ������������� ���������� ��������� �� ��������� � ������� rgb � ���� (254, 255, 255, 1) �������� ���� ������� 
 �������� ������ ����� ������� � �� ������ ��������������, ����� ���� ������� ������� ������������� ����� ������� ������� ������� ������� �����������
 ������ ��� ���� ������ ��� � ����� ������� � ������� ������ Game::DrawPlayer ��� ������������ ������ ������� � �������� ������� ���� ��
 ����������� (0,0), ���� ��� ��������� �� ������ �� ��������������. � ���������� ����� �� �� ������� ������ ��� ����������. 

	� ������������ ����� Caracter.h ��������� ������� ����� Caracter (��������), ���� ����� ��������
 ���������� (� ���������� ����� �������� ������ ������� ����� ����������� ������� �� ������ Caracter).

	� ������������ ����� Gun.h ��������� ������� ����� Gun (������), ���� ����� �������� ������ (����� ������������
 � �������� �������� ��� �������� ����� ����� ������)

	� ������������ ����� Player.h ��������� ����������� ����� Player (�����), ���� ����� �������� ������.

	� ������������ ����� Animation.h ��������� ����� ��� �������� Animation, ���� ����� ��� �� ��������
 ������ ����� ������� ��� Caracter � ����������� (Player), �������� ����� �������������� � � ������� Gun ��� Map

	� ������������ ����� Item.h ��������� ����������� ��������� Point � ������ Item ����������� �������� � �������� ����� ����������������� 
 � ������� ����������� ����� Box (�������) ������������ ������� ����� ��������� ��� ������ ������ MagicGun

	����� �������� �������� �� ����� �� ���������� left_padding_ ������ Game ����� �������� ��������� �� ��������� � ���������.
 ���������� �� ������� ����������� �������� �� ����� ��������� � ������ Map � ����������������� � ��� ����� ����� ������� 
 SetPadding � GetPadding. ��� ���������� ���������� ���������� ������� ��� ������� + ������ ������ ������ ������ �� �������������, 
 � �������� ������� �� ��� �� ���������� ��������.

*/

#ifndef OLC_PGE_APPLICATION
	#define OLC_PGE_APPLICATION
#endif 

//��� ����������� ������ � ������
#ifndef WITH_FPS
	#define WITH_FPS
#endif


#include <iostream>
#include "lib/olcPixelGameEngine.h"
#include "Animation.h"
#include "Map.h"
#include "Caracter.h"
#include "Gun.h"
#include "HP-Bottle.h"
#include "Player.h"
#include "Enemy.h"
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



//������ ������������ � ���� 
class Caracter;
class Player;
class Map;
class Gun;
class Item;
class MagicGun1;
class Animation;
class Enemy;


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
	vector<Enemy*> enemys_;
	vector<Item> items_list_;

	olc::Pixel background_color_ = olc::Pixel(0, 0, 0);

	vector<olc::Sprite*> sprites_;
	//���������� ������ �� ����� ������� ������ 
	//� ������� ����� �������� ��������� �� ����������
	const int left_padding_ = 200;


	
public:
	bool OnUserCreate() override {

		player_sprites_.push_back(new olc::Sprite(player_.GetStandSprite()));
		player_sprites_.push_back(new olc::Sprite(player_.GetWalkSprite()));

		enemys_.push_back(new Enemy(300, 100));

		map_ = new Map(background_sprite_);

		for (Enemy* enemy : enemys_)
			Caracter::AddCaracter(enemy);

		for (Item i : map_->GetItems())
			items_list_.push_back(i);


		for (int i = 0; i < player_sprites_.size(); i++)
			player_.animation.AddSprite(player_sprites_[i]);

		player_.AttachToMap(map_);
		Caracter::AttachAllCaractersToMap(map_);
		
		return true;
	}

	//���������� ��� ������ �����, �������� � �������� ��������� ����� ��������� � ������� ��������� �������� ����� 
	//��� ������� ��������� ������� � �������� ������� ��� ��������� ������ �������� 
	bool OnUserUpdate(float fElapsedTime) override {

		CheckKeyboardEvents(fElapsedTime);

		olc::Sprite* player_sprite = player_.animation.GetSprite();
		player_.Update(player_sprite, fElapsedTime);
		if (player_.IsDead()) {
			std::cout << "dead\n";
		}

		bool map_moved = 
		  FillScreen();

		DrawPlayer(player_sprite, map_moved);
		UpdateAndDrawCaracters(fElapsedTime);
		DrawItems();
		DrawGun();
		DrawHPStatistics();
		
		DrawMapBorders();

		return true;
	}

private:
	void UpdateAndDrawCaracters(float time) {
		for (Caracter* enemy : Caracter::caracters) {
			olc::Sprite* sprite = enemy->animation.GetSprite();
			enemy->Update(sprite, time);
			DrawCaracter(sprite, enemy->GetPosition());
		}
	}

	//���������� ���� ���������� ����������� �� ����� 
	void DrawCaracter(olc::Sprite* sprite, Point pos) {
		for (int x = 0; x < sprite->width; x++) {
			for (int y = 0; y < sprite->height; y++) {
				olc::Pixel p = sprite->GetPixel(x, y);
				if (p == sprite->GetPixel(0, 0))
					continue;

				if(pos.x - map_->GetPadding() +  sprite->width > 0)
					Draw(pos.x + x - map_->GetPadding(), pos.y + y, p);
			}
		}
	}

	//���������� ������
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


				//��� ����� ������ ����������� �����������!!!
				//��� ���� ����������� ���� � ����� � ����������� ������� FillScreen,
				//������� � ���������� ����� ��������, � �� ��� �������� ������
				if(map_moved)
					Draw(x + left_padding_, point.y + y, p);
				else{
					int position = background_sprite_->width - player_.GetPosition().x;
					if (position < screen_width)
					{
						position = screen_width - position;
						Draw(position + x, point.y + y, p);
					}
					else
						Draw(point.x + x, point.y + y, p);
				}
			}
		}
	}

	//��������� ����� �������� ���������� 
	//��������������!!! ��� ����� ����������� �����������!!!
	//����� ����������� ������������ ���������� ��� �������� ������
	//� ���������� �������� ��� ����� ��������
	bool FillScreen() {

		bool map_moved = false;

		int y_pad = background_sprite_->height - ScreenHeight();
		int x_pad = 0;

		if (background_sprite_->width - player_.GetPosition().x + left_padding_ > 0 &&
			player_.GetPosition().x - left_padding_ > 0) {
			if (player_.GetPosition().x + screen_width - left_padding_ >= background_sprite_->width) {
				x_pad = background_sprite_->width - screen_width;
			}
			//�� ���
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

	//���������� ��� �������� ����������� �� �����  
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

	//���������� ����� 
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

	//���������� ���������� ������ ( �����, ���� )
	void DrawHPStatistics() {
		olc::Sprite* sprite = player_.GetBottle().GetSprite();

		int x_padding = 12;
		int y_padding = 5;

		int bottle_position_x = screen_width - sprite->width - 10;
		int bottle_position_y = 10;

		for (int x = 0; x < sprite->width; x++) {
			for (int y = 0; y < sprite->height; y++) {
				olc::Pixel p = sprite->GetPixel(x, y);
				if (p == sprite->GetPixel(0, 0))
					continue;
				Draw(bottle_position_x + x, bottle_position_y + y, p);
			}
		}

		HP_Bottle bottle = player_.GetBottle();
		for (int x = bottle.GetStart().x; x < bottle.GetEnd().x; x++) {
			for (int y = bottle.GetStart().y; y < bottle.GetEnd().y; y++) {
				olc::Pixel p = sprite->GetPixel(x, y);
				if (p != sprite->GetPixel(0, 0))
					continue;
				Draw(bottle_position_x + x, bottle_position_y + y, olc::RED);
			}
		}
	}

	//������� ������� � ����� � ����� �� ������� ���� ���������, ��� ����� ��������� ��� �������� � ��������� 
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

	//��������� �������� ������, ���� ����� ��� �� ����� �� �������� �������������� ������� ��� ������ Player
	void CheckKeyboardEvents(float fElapsedTime) {
		if (GetKey(olc::W).bHeld)player_.PlayerMove(Side::Up, player_sprites_[0], fElapsedTime);
		if (GetKey(olc::W).bPressed)player_.PlayerMove(Side::Jump, player_sprites_[0], fElapsedTime);
		if (GetKey(olc::A).bHeld) {
			player_.PlayerMove(Side::Left, player_sprites_[0], fElapsedTime);
			player_.NowSide = Side::Left;
			player_.animation.AddTime(fElapsedTime);
		}
		if (GetKey(olc::D).bHeld) {
			player_.PlayerMove(Side::Right, player_sprites_[0], fElapsedTime);
			player_.NowSide = Side::Right;
			player_.animation.AddTime(fElapsedTime);
		}

		if (GetKey(olc::D).bReleased || GetKey(olc::A).bReleased)
			player_.animation.ResetIterator();

		if (GetKey(olc::R).bPressed)
			if (!player_.IsAtacking())
				player_.Atack1();
		if (GetKey(olc::N).bPressed)
			player_.Damage(-50);
	}

};


int main(){

	Game* game = new Game();
	if (game->Construct(screen_width, screen_height, 1, 1, false, true))
		game->Start();
	return 0;

}

