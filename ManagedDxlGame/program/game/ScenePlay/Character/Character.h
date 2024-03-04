#pragma once
#include "../../DxLibEngine.h"

class Player;

class Character
{
public:

	Shared<dxe::Mesh>  _mesh = nullptr;

protected:

	Shared<Player>     _player_ref = nullptr;

public:

	tnl::Vector3       _collideSize{};

protected:

	int                _at{};        // CSV
	int                _def{};       // CSV
	int                _MAX_HP{};
};