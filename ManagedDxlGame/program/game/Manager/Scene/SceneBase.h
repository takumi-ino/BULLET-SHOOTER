#pragma once

class SceneBase
{
public:

	virtual ~SceneBase() {}

	virtual void Update(const float deltaTime) {}
	virtual void Render() {}
	virtual void ReleaseMem() {}
};