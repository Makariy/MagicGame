#pragma once
#include <vector>
#include "lib/olcPixelGameEngine.h"




#ifndef ANIMATION_H
	#define ANIMATION_H
#endif

//����� ��� �������� �������� 
class Animation {
public:
	Animation() {}
	//�������� ������� � ����������� �� � ������ � ����� ���������
	Animation(std::initializer_list<olc::Sprite*> list) {
		for (olc::Sprite* s : list)
			sprites.push_back(s);
	}

public:
	//�������� ������� ��� �������� 
	void AddSprite(olc::Sprite* sprite) {
		sprites.push_back(sprite);
	}
	//�������� ����� ��� ��������
	void AddTime(float time) { animation_time += time; }
	//�������� ������� ������ �������� 
	olc::Sprite* GetSprite() {
		if (animation_time > animation_speed / 10) {
			animation_time = 0;
			iterator++;
			if (iterator >= sprites.size())
				ResetIterator();
		}

		return sprites[iterator];
	}
	inline void ResetIterator() {
		iterator = 0;
	}


private:
	std::vector<olc::Sprite*> sprites;
	int iterator = 0;
	float animation_time = 0.0f;
	float animation_speed = 3.0f;
};