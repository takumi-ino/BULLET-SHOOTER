#pragma once

class SceneBase
{
public:

	virtual void Update(const float deltaTime) {}
	virtual void Render() {}
	virtual void ReleaseMem() {}

protected:

	virtual ~SceneBase() {}
};