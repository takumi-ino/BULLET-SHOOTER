#pragma once
#include "../../DxLibEngine.h"

class ScoreItem;
class PowerUpItem;


class ItemBase
{
public:

	ItemBase() {}
	ItemBase(const std::vector<Shared<ItemBase>>& scoreItem, const  std::vector<Shared<ItemBase>>& power) {}


public:

	Shared<dxe::Mesh> _mesh = nullptr;

public:

	float _lifeTimer{};

	bool _isActive = false;
	bool _hasSpawned = false;

	tnl::Vector3 _velocity{ 0,150,0 };
	tnl::Vector3 _gravity{ 0, -9.8f, 0 };
	tnl::Vector3 _collisionSize{};
	tnl::Vector3 _position{};

};

