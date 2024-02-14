#pragma once

class SceneBase
{
public:

	virtual void Update(const float delta_time) {}
	virtual void Render() {}
	virtual void ReleaseMem() {}
};