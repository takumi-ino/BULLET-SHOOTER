#include "PlayerBullet.h"
#include "../game/ScenePlay/ScenePlay.h"
#include "../game/ScenePlay/Character/Player/Player.h"
#include "../game/Utility/CustomException.h"


namespace inl {

	float PlayerBullet::_bulletPowerRate;

	void PlayerBullet::SetDirectionAndPosition(const tnl::Vector3& spawn_pos, const tnl::Vector3& direction)
	{
		_mesh->pos_ = spawn_pos;
		_moveDirection = direction;
		_moveDirection.normalize();
	}

	void PlayerBullet::ResetDirectionAndPosition()
	{
		_mesh->pos_ = ScenePlay::GetInstance()->_player->_mesh->pos_;
		_moveDirection = { 0,0,0 };
	}

	void PlayerBullet::Render() {

		_mesh->render(ScenePlay::GetInstance()->_mainCamera);
	}


	void PlayerBullet::Update(const float deltaTime) {

		_timer += deltaTime;

		static tnl::Vector3 start = _mesh->pos_;

		_mesh->pos_ += _moveDirection * deltaTime * _speed;

		if (_timer > 5.f) {

			_isActive = false;
			_timer = 0.f;
			ResetDirectionAndPosition();
		}
	}
}