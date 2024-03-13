#pragma once

class FreeLookCamera;

class SkyBox
{
public:

	SkyBox();

	Shared<dxe::Mesh> _skybox = nullptr;

	void Update();
	void Render(const Shared<FreeLookCamera> camera);
};