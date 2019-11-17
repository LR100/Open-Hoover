#pragma once

#include "../Vector.h"
#include "../../Tools/XMLNode.h"

// XML I/O 
// For Vectors

class Vec2TXML
{
public:

	template <typename T>
	static void Import(Vec2T<T>& vec, XMLNode* node);

	template <typename T>
	static XMLNode* Export(const Vec2T<T>& vec);
};

template<typename T>
inline void Vec2TXML::Import(Vec2T<T>& vec, XMLNode * node)
{
	node->GetValue("x", vec.x);
	node->GetValue("y", vec.y);
}

template<typename T>
inline XMLNode * Vec2TXML::Export(const Vec2T<T>& vec)
{
	XMLNode*	node = new XMLNode();

	node->SetName("Vec2");
	node->SetValue("x", vec.x);
	node->SetValue("y", vec.y);

	return (node);
}
