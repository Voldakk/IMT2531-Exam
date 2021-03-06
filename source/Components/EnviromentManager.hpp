#pragma once

#include <string>

#include "EVA.hpp"
#include "EVA/UI.hpp"


class Terrain;

class EnviromentManager : public EVA::Component, public EVA::IUpdateComponent
{
	REGISTER_COMPONENT_HPP(EnviromentManager)
	
	struct Region
	{
		float minHeightWinter;
		float maxHeightWinter;
		float minHeightSummer;
		float maxHeightSummer;

		glm::vec3 colorWinter;
		glm::vec3 colorSummer;

		std::shared_ptr<EVA::Texture> diffuseTexture;

		float textureTiling = 1.0f;

		std::string treeName;
		float treeDensity = 0.0f;

		float MinHeight(const float season) const
		{
			if (season <= 6)
				return glm::mix(minHeightWinter, minHeightSummer, season / 6.0f);

			return glm::mix(minHeightSummer, minHeightWinter, (season - 6.0f) / 6.0f);
		}

		float MaxHeight(const float season) const
		{
			if (season <= 6)
				return glm::mix(maxHeightWinter, maxHeightSummer, season / 6.0f);

			return glm::mix(maxHeightSummer, maxHeightWinter, (season - 6.0f) / 6.0f);
		}

		glm::vec3 Color(const float season) const
		{
			if (season <= 6)
				return glm::mix(colorWinter, colorSummer, season / 6.0f);

			return glm::mix(colorSummer, colorWinter, (season - 6.0f) / 6.0f);
		}
	};


	EVA::Light* m_Sun = nullptr;
	Terrain* m_Terrain = nullptr;
	
	float m_Season = 6.0f;
	float m_Time = 12.0f;
	float m_SecondsPerDay = 10.0f;
	float m_SecondsPerYear = 40.0f;

	float m_DayLengthSummer = 16.0f;
	float m_DayLengthWinter = 8.0f;
	float m_TransitionLength = 1.0f;

	bool m_TimePaused = false;
	bool m_SeasonPaused = false;

	float m_MiddayAngle = 60.0f;

	glm::vec3 m_SunriseColor = glm::vec3(1.0f, 0.5f, 0.5f);
	glm::vec3 m_MiddayColor = glm::vec3(1.0f);
	glm::vec3 m_SunsetColor = glm::vec3(1.0f, 0.5f, 0.5f);
	glm::vec3 m_NightColor = glm::vec3(0.01f);

	std::vector<Region> m_Regions;

	std::string m_TimeOfDay;
	EVA::Label* m_TimeLabel = nullptr;
	EVA::Label* m_SeasonLabel = nullptr;

public:

	float regionBlendAmount = 0.01f;
	const std::vector<Region>& regions = m_Regions;

	const float& season = m_Season;

	void Start() override;
	void Update(float deltaTime) override;
	
	void UpdateTime();
	void PlaceTrees();
	
	void Load(const EVA::DataObject data) override;
	void Save(EVA::DataObject& data) override;
	void Inspector() override;
};