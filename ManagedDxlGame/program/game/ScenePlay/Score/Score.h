#pragma once
#include "../../DxLibEngine.h"

namespace inl {

	class Score
	{
	public:

		Score() : _value(0) {}

		void Add(int point);

		int Get() const;

	private:

		int _value;
	};
};