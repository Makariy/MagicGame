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
			sprites_.push_back(s);
	}

public:
	//�������� ������� ��� �������� 
	void AddSprite(olc::Sprite* sprite) {
		sprites_.push_back(sprite);
	}
	//�������� ����� ��� ��������
	void AddTime(float time) { animation_time_ += time; }
	//�������� ������� ������ �������� 
	olc::Sprite* GetSprite() {
		if (animation_time_ > animation_speed_ / 10) {
			animation_time_ = 0;
			iterator_++;
			if (iterator_ >= sprites_.size())
				ResetIterator();
		}

		return sprites_[iterator_];
	}
	inline void ResetIterator() {
		iterator_ = 0;
	}


private:
	std::vector<olc::Sprite*> sprites_;
	int iterator_ = 0;
	float animation_time_ = 0.0f;
	float animation_speed_ = 3.0f;
};