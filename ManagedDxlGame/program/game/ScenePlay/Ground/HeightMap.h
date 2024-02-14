#pragma once
#include "../../DxLibEngine.h"

class HeightMap
{
public:

	HeightMap(){}
	HeightMap(int stage_id);


	Shared<dxe::Mesh> _mesh = nullptr;

	void Update();
	void Render(Shared<dxe::Camera> camera);
};