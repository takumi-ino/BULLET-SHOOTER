#pragma once
#include "../../DxLibEngine.h"
#include "../../../library/tnl_sequence.h"
#include "SceneBase.h"



class SceneManager : public SceneBase
{
private:

	explicit SceneManager(SceneBase* start_scene);

	tnl::Sequence<SceneManager> _sequence = tnl::Sequence<SceneManager>(this, &SceneManager::SeqTransIn);

	float _transTime = 0.2f;
	int _transGraph_hdl = 0;

	SceneBase* _now_scene = nullptr;
	SceneBase* _next_scene = nullptr;

	bool SeqTransIn(const float delta_time);
	bool SeqTransOut(const float delta_time);
	bool SeqRunScene(const float delta_time);

public:

	 static SceneManager* GetInstance(SceneBase* startScene = nullptr);

	 void ChangeScene(SceneBase* next_scene, float trans_time = 0.5f);
	 void Update(const float deltaTime);
	 void ReleaseMem();
};