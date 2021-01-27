#pragma once
#include "include/lib/olcPixelGameEngine.h"


class LoadingScreen {
public:
	LoadingScreen() {}

	void Start() {
		is_started_ = true;
	}

	void Update(float time) {
		if (is_started_) {
			time_passed_ += time;
			if (time_passed_ > time_to_pass_)
				is_started_ = false;
		}
	}

	bool IsGoing() {
		return is_started_;
	}

	float GetTimePassed() {
		return (time_passed_ * 100 / time_to_pass_);
	}


private:
	bool is_started_ = false;
	float time_passed_ = 0;
	float time_to_pass_ = 3;

};


