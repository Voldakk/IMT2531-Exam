#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "BoundingBox.hpp"
#include "../ConstPointer.hpp"

namespace EVA
{

	class UiElement
	{
	protected:

		UiElement* m_Parent;
		std::vector<UiElement *> m_Children;

		
		glm::vec2 m_Position;
		BoundingBox m_BoundingBox;

		glm::vec2 m_Anchor;
		glm::vec2 m_Pivot;
		glm::vec2 m_Offset;

		bool m_StaticPosition = false;

	public:

		const ConstPointer<UiElement> parent = &m_Parent;
		const glm::vec2& position = m_Position;
		const BoundingBox& boundingBox = m_BoundingBox;
		
		UiElement();

		void SetParent(UiElement* newParent);

		UiElement* AddChild(UiElement * newChild);

		virtual void OnChildrenUpdated();


		int GetIndexOfChild(UiElement* child) const;
		UiElement * GetChild(unsigned int index) const;

		virtual void SetPosition(glm::vec2 newPosition, bool usePivot = true);

		virtual void Update() {}

		virtual void Render() const {}

		virtual void UpdateBoundingBox();

		UiElement * SetAnchor(glm::vec2 newAnchor);
		UiElement * SetAnchor(float newAnchorX, float newAnchorY);

		UiElement * SetPivot(glm::vec2 newPivot);
		UiElement * SetPivot(float newPivotX, float newPivotY);

		UiElement * SetOffset(glm::vec2 newOffset);
		UiElement * SetOffset(float newOffsetX, float newOffsetY);

		UiElement * SetAnchorAndPivot(glm::vec2 newValue);
		UiElement * SetAnchorAndPivot(float newValueX, float newValueY);
		UiElement * SetAnchorAndPivot(float newValue);

		UiElement * SetOffsetFromAnchor(glm::vec2 newOffset);
		UiElement * SetOffsetFromAnchor(float newOffsetX, float newOffsetY);
		UiElement * SetOffsetFromAnchor(float newOffset);

		void UpdatePosition();

		virtual void OnScreenResize();
	};
}
