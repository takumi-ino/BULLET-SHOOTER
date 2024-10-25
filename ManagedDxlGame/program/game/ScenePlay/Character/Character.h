#pragma once
#include "../../DxLibEngine.h"

namespace inl {

	class Player;

	class Character
	{
	protected:

		virtual ~Character() {}

	public:

		Shared<dxe::Mesh>  _mesh = nullptr;

	public:

		tnl::Vector3       _collideSize{};

	protected:

		int                _at{};        // CSV
		int                _def{};       // CSV
		int                _MAX_HP{};
	};
}