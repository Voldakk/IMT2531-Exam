#include "EnviromentManager.hpp"

REGISTER_COMPONENT_CPP(EnviromentManager, "EnviromentManager")

void EnviromentManager::Awake()
{
	m_Sun = scene->GetLights()[0].get();
}

void EnviromentManager::Load(const EVA::DataObject data)
{
	m_MiddayAngle = data.GetFloat("middayAngle", m_MiddayAngle);

	m_SunriseColor = data.GetVec3("sunriseColor", m_SunriseColor);
	m_MiddayColor = data.GetVec3("middayColor", m_MiddayColor);
	m_SunsetColor = data.GetVec3("sunsetColor", m_SunsetColor);
	m_NightColor = data.GetVec3("nightColor", m_NightColor);
}

void EnviromentManager::Save(EVA::DataObject& data)
{
	data.SetFloat("middayAngle", m_MiddayAngle);

	data.SetVec3("sunriseColor", m_SunriseColor);
	data.SetVec3("middayColor", m_MiddayColor);
	data.SetVec3("sunsetColor", m_SunsetColor);
	data.SetVec3("nightColor", m_NightColor);
}

void EnviromentManager::Inspector()
{
	ImGui::DragFloat("Time", &m_Time, 0.01f, 0.0f, 2.0f);
	ComponentInspector::Float("Season", m_Season);

	ComponentInspector::Float("Midday angle", m_MiddayAngle);

	ComponentInspector::ColorPicker("Sunrise color", m_SunriseColor);
	ComponentInspector::ColorPicker("Midday color", m_MiddayColor);
	ComponentInspector::ColorPicker("Sunset color", m_SunsetColor);
	ComponentInspector::ColorPicker("Night color", m_NightColor);

	UpdateTime();
}

void EnviromentManager::Update(const float deltaTime)
{
	m_Time += deltaTime / m_SecondsPerDay;
	if (m_Time >= 1.0f)
		m_Time -= 1.0f;

	UpdateTime();
}

void EnviromentManager::UpdateTime() const
{
	if (m_Sun == nullptr)
		return;

	const auto yaw = m_Time * 180.0f; // 0 - 360 degrees
	auto pitch = 0.0f;
	auto color = m_Sun->color;

	if (m_Time <= 0.5f) // 00-06
	{
		const auto t = m_Time * 2.0f;

		pitch = 0.0f;
		color = glm::mix(m_NightColor, m_SunriseColor, t);
	}
	else if (m_Time <= 1.0f) // 06-12
	{
		const auto t = m_Time * 2.0f - 1.0f;

		pitch = glm::mix(0.0f, m_MiddayAngle, t);
		color = glm::mix(m_SunriseColor, m_MiddayColor, t);
	}
	else if (m_Time <= 1.5f) // 12-18
	{
		const auto t = (m_Time - 1.0f) * 2.0f;

		pitch = glm::mix(m_MiddayAngle, 0.0f, t);
		color = glm::mix(m_MiddayColor, m_SunsetColor, t);
	}
	else // 18-24
	{
		const auto t = m_Time * 2.0f - 3.0f;

		pitch = 0.0f;
		color = glm::mix(m_SunsetColor, m_NightColor, t);
	}

	m_Sun->SetRotation({ pitch, yaw });
	m_Sun->color = color;
}
