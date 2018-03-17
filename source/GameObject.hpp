#pragma once

#include <vector>
#include <memory>

#include "glm/glm/glm.hpp"

class Scene;
class GameObject;

class Component
{
public:

	GameObject * gameObject;

	Component();
	virtual void Update(float deltaTime);
	virtual void Render();
};



class Transform : public Component
{
public:

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
};



class GameObject
{
public:

	Scene * scene;
	std::unique_ptr<Transform> transform;
	std::vector<std::shared_ptr<Component>> components;

	GameObject();

	void Update(float deltaTime);
	void Render();

	Component * AddComponent(std::shared_ptr<Component> component);
};

