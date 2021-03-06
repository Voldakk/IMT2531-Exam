#include "Transformer.hpp"

namespace EVA
{
	REGISTER_COMPONENT_CPP(Transformer, "EVA::Transformer")

	void Transformer::Update(const float deltaTime)
	{
		transform->Translate(m_Translation * deltaTime);
		transform->SetScale(transform->localScale + m_Scaling * deltaTime);
		transform->Rotate(m_Rotation * deltaTime);
	}

	void Transformer::Load(const DataObject data)
	{
		m_Translation = data.GetVec3("translation", m_Translation);
		m_Scaling = data.GetVec3("scaling", m_Scaling);
		m_Rotation = data.GetVec3("rotation", m_Rotation);
	}

	void Transformer::Save(DataObject& data)
	{
		data.SetVec3("translation", m_Translation);
		data.SetVec3("scaling", m_Scaling);
		data.SetVec3("rotation", m_Rotation);
	}

	void Transformer::Inspector()
	{
		ComponentInspector::Float3("Translation", m_Translation);
		ComponentInspector::Float3("Scaling", m_Scaling);
		ComponentInspector::Float3("Rotation", m_Rotation);
	}
}
