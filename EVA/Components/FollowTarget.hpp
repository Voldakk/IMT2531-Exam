#pragma once

#include "EVA.hpp"

#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace EVA
{

	class FollowTarget : public Component, public IUpdateComponent
	{
		REGISTER_COMPONENT_HPP(FollowTarget)

	public:

		Transform * target = nullptr;
		glm::vec3 offset = glm::vec3(0.0f);
		float scalar = 1.0f;

		void Update(const float deltaTime) override;
	};
}