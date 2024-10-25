#pragma once
#include "../../DxLibEngine.h"

namespace inl {

	class Bullet
	{
	public:

		Bullet() {}
		Bullet(const tnl::Vector3& spawnPos, const tnl::Vector3& direction) {}
		virtual ~Bullet() {}


		virtual void Update(const float deltaTime) {}
		virtual void Render() {}

	public:

		Shared<dxe::Mesh> _mesh{};

	public:

		int		 _id{};
		float	 _timer{0.f};
	};
}