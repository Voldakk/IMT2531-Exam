#pragma once

#include "EVA.hpp"
#include "Json.hpp"

namespace EVA
{

	class PrefabParser
	{

	public:

		static GameObject* Load (Scene* scene, const std::string& path)
		{
			const auto sd = Json::Open(path);
			auto& d = (*sd);

			auto gameObject = scene->CreateGameObject();

			// Components
			if (d.HasMember("components") && d["components"].IsArray())
			{
				auto components = d["components"].GetArray();

				for (unsigned int i = 0; i < components.Size(); ++i)
				{
					const auto& c = components[i];

					const std::string id = c["id"].GetString();

					const auto component = ComponentMap::CreateComponent(id);
					if(component != nullptr)
					{
						gameObject->AttachComponent(component);
					}

				}
			}

			gameObject->Start();

			return gameObject.get();
		}
	};

}
