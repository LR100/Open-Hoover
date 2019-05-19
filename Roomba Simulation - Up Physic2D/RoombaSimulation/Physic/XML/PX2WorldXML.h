#pragma once

#include "../PX2World.h"
#include "../../Tools/XMLNode.h"

#include <unordered_map>

class PX2WorldXML
{
public:
	
	static void		Import(PX2World* world, XMLNode* nodeWorld);
	static XMLNode*	Export(PX2World* world);

private:

	/// EXPORT ///
	// Shapes
	static void ExportShapes(PX2World* world, XMLNode* nodeWorld, const std::unordered_map<size_t, PX2Shape*>& shapes);
	static void ExportShape(PX2Shape* shape, XMLNode* nodeShapes);

	static void ExportShapeBox(PX2ShapeBox* shape, XMLNode* nodeShape);
	static void ExportShapeCircle(PX2ShapeCircle* shape, XMLNode* nodeShape);
	// Bodies
	static void ExportBodies(PX2World* world, XMLNode* nodeWorld, std::unordered_map<size_t, PX2Shape*>& shapes);
	static void ExportBody(PX2Body* body, XMLNode* nodeBodies);

	/// IMPORT ///
	// Shapes
	static void			ImportShapes(std::unordered_map<size_t, PX2Shape*>& shapes, XMLNode* nodeWorld);
	static PX2Shape*	ImportShape(XMLNode* nodeShape);

	static PX2Shape*	ImportShapeBox(XMLNode* nodeShape);
	static PX2Shape*	ImportShapeCircle(XMLNode* nodeShape);
	// Bodies
	static void			ImportBodies(PX2World* world, XMLNode* nodeWorld, std::unordered_map<size_t, PX2Shape*>& shapes);
	static bool			ImportBody(XMLNode* nodeBody, std::unordered_map<size_t, PX2Shape*>& shapes, PX2Body::Properties& prop);

	PX2WorldXML() {};
	virtual ~PX2WorldXML() {};
};