#pragma once
#include "../../DxLibEngine.h"

class FreeLookCamera;

class SkyBox
{
public:

	SkyBox(){}
	explicit SkyBox(int stage_id);

	Shared<dxe::Mesh> skybox = nullptr;

	void Update();
	void Render(const Shared<FreeLookCamera> camera);
};