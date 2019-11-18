#pragma once

#include "../Physic2DWorld.h"
#include "../../Tools/XMLNode.h"

#include <unordered_map>

class Physic2DWorldXML
{
public:

	static void		Import(Physic2DWorld* world, XMLNode* nodeWorld);
	static XMLNode* Export(Physic2DWorld* world);

private:

	/// EXPORT ///
	// Shapes
	static void ExportShapes(Physic2DWorld* world, XMLNode* nodeWorld, const std::unordered_map<size_t, Physic2DShape*>& shapes);
	static void ExportShape(Physic2DShape* shape, XMLNode* nodeShapes);

	static void ExportShapeBox(Physic2DShapeBox* shape, XMLNode* nodeShape);
	static void ExportShapeCircle(Physic2DShapeCircle* shape, XMLNode* nodeShape);
	// Bodies
	static void ExportBodies(Physic2DWorld* world, XMLNode* nodeWorld, std::unordered_map<size_t, Physic2DShape*>& shapes);
	static void ExportBody(Physic2DBody* body, XMLNode* nodeBodies, std::unordered_map<size_t, Physic2DShape*>& shapes);
	// Fixtures
	static void ExportFixtures(Physic2DBody* body, XMLNode* nodeBody, std::unordered_map<size_t, Physic2DShape*>& shapes);

	/// IMPORT ///
	// Shapes
	static void				ImportShapes(Physic2DWorld* world, std::unordered_map<size_t, Physic2DShape*>& shapes, XMLNode* nodeWorld);
	static Physic2DShape*	ImportShape(Physic2DWorld* world, XMLNode* nodeShape);

	static Physic2DShape*	ImportShapeBox(Physic2DWorld* world, XMLNode* nodeShape);
	static Physic2DShape*	ImportShapeCircle(Physic2DWorld* world, XMLNode* nodeShape);
	// Bodies
	static void				ImportBodies(Physic2DWorld* world, XMLNode* nodeWorld, std::unordered_map<size_t, Physic2DShape*>& shapes);
	static bool				ImportBody(XMLNode* nodeBody, std::unordered_map<size_t, Physic2DShape*>& shapes, Physic2DBodyProperties& prop);
	static bool				ImportFixtures(XMLNode* nodeBody, Physic2DBody* body, std::unordered_map<size_t, Physic2DShape*>& shapes);

	Physic2DWorldXML() {};
	virtual ~Physic2DWorldXML() {};
};