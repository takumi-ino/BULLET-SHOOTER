#include "Score.h"

void Score::Add(int point) {
	_value += point;
}


int Score::Get() const {
	return _value;
}