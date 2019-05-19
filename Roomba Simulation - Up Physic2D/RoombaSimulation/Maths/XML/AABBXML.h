#pragma once

#include "../AABB.h"
#include "../../Tools/XMLNode.h"
// NEED Vector XML
#include "../XML/VectorXML.h"

// XML I/O 
// For AABBs

class AABB2TXML
{
public:

	template <typename T>
	static void Import(AABB2T<T>& aabb, XMLNode* node);

	template <typename T>
	static XMLNode* Export(const AABB2T<T>& aabb);
};

template<typename T>
inline void AABB2TXML::Import(AABB2T<T>& aabb, XMLNode * node)
{
	XMLNode*	node = new XMLNode();

	XMLNode*	center = node->GetNodeNamed("Vec2", 0);
	Vec2TXML::Import(aabb.center, center);

	XMLNode*	halfSize = node->GetNodeNamed("Vec2", 1);
	Vec2TXML::Import(aabb.halfSize, halfSize);

	node->SetName("AABB2");
	node->PushBackChild(center);
	node->PushBackChild(halfSize);
}

template<typename T>
inline XMLNode * AABB2TXML::Export(const AABB2T<T>& aabb)
{
	XMLNode*	node = new XMLNode();

	XMLNode*	center = Vec2TXML::Export(aabb.center);
	center->SetValue("id", "center");

	XMLNode*	halfSize = Vec2TXML::Export(aabb.halfSize);
	halfSize->SetValue("id", "halfSize");

	node->SetName("AABB2");
	node->PushBackChild(center);
	node->PushBackChild(halfSize);

	return (node);
}
