#include "PX2WorldXML.h"

#include "../../Maths/XML/VectorXML.h"
#include "../../Maths/XML/AABBXML.h"

#include <iostream> // TMPPP

void PX2WorldXML::Import(PX2World * world, XMLNode * nodeWorld)
{
	std::cout << "PX2WorldXML::Import()" << std::endl;

	/// Parameters Of the World ///
	// Gravity
	XMLNode*	nodeGravity = nodeWorld->GetNodeNamedWithValue("Vec2", "id", "gravity");
	if (nodeGravity) 
	{
		std::cout << "Ok Gravity" << std::endl;
		Vec2		gravity;
		Vec2TXML::Import(gravity, nodeGravity); 
		world->SetGravity(gravity);
	}
	
	// Shapes first
	std::unordered_map<size_t, PX2Shape*>	shapes;
	ImportShapes(shapes, nodeWorld);

	// Then Bodies
	ImportBodies(world, nodeWorld, shapes);
	std::cout << "PX2WorldXML::Import() OK" << std::endl;
}

XMLNode * PX2WorldXML::Export(PX2World * world)
{
	XMLNode* nodeWorld = new XMLNode();
	nodeWorld->SetName("World");

	/// Parameters Of the World ///
	// Gravity
	XMLNode* nodeGravity = Vec2TXML::Export(world->GetGravity());
	nodeGravity->SetValue("id", "gravity");
	nodeWorld->PushBackChild(nodeGravity);

	std::unordered_map<size_t, PX2Shape*> shapes;
	// Bodies first -> To Get only Used Shapes
	ExportBodies(world, nodeWorld, shapes);
	// Then Shapes
	ExportShapes(world, nodeWorld, shapes);

	return (nodeWorld);
}

/// EXPORT ///

void PX2WorldXML::ExportShapes(PX2World * world, XMLNode * nodeWorld, const std::unordered_map<size_t, PX2Shape*>& shapes)
{
	XMLNode* nodeShapes = new XMLNode();
	nodeShapes->SetName("Shapes");
	nodeWorld->PushBackChild(nodeShapes);

	std::unordered_map<size_t, PX2Shape*>::const_iterator it = shapes.begin();
	std::unordered_map<size_t, PX2Shape*>::const_iterator itE = shapes.end();
	
	for (; it != itE; it++)
		ExportShape(it->second, nodeShapes);
}

void PX2WorldXML::ExportShape(PX2Shape * shape, XMLNode * nodeShapes)
{
	XMLNode* nodeShape = new XMLNode();

	nodeShape->SetName("Shape");

	int type = (int)shape->GetType(); // Avoid Type to be interpreted as a Char (in other terms to become a character)
	nodeShape->SetValue("type", type);
	size_t idShape = (size_t)shape;
	nodeShape->SetValue("id", idShape);

	nodeShapes->PushBackChild(nodeShape);

	if (shape->GetType() == PX2Shape::TYPE::BOX)
		ExportShapeBox((PX2ShapeBox*)shape, nodeShape);
	else if (shape->GetType() == PX2Shape::TYPE::CIRCLE)
		ExportShapeCircle((PX2ShapeCircle*)shape, nodeShape);
}

void PX2WorldXML::ExportShapeBox(PX2ShapeBox * shape, XMLNode * nodeShape)
{
	nodeShape->SetValue("width", shape->GetWidth());
	nodeShape->SetValue("height", shape->GetHeight());
}

void PX2WorldXML::ExportShapeCircle(PX2ShapeCircle * shape, XMLNode * nodeShape)
{
	nodeShape->SetValue("radius", shape->GetRadius());
}


void PX2WorldXML::ExportBodies(PX2World * world, XMLNode * nodeWorld, std::unordered_map<size_t, PX2Shape*>& shapes)
{
	XMLNode* nodeBodies = new XMLNode();
	nodeBodies->SetName("Bodies");
	nodeWorld->PushBackChild(nodeBodies);

	std::vector<PX2Body*>	bodies = world->GetBodies();

	for (size_t i = 0; i < bodies.size(); i += 1)
	{
		ExportBody(bodies.at(i), nodeBodies);
		if (bodies.at(i)->GetShape())
		{
			size_t shapeID = (size_t)bodies.at(i)->GetShape();
			if (!shapes.count(shapeID))
				shapes.emplace(shapeID, (PX2Shape*)bodies.at(i)->GetShape());
		}
	}
}

void PX2WorldXML::ExportBody(PX2Body * body, XMLNode * nodeBodies)
{
	// Basics
	XMLNode* nodeBody = new XMLNode();
	nodeBody->SetName("Body");
	int type = (int)body->GetType(); // Same as shape - Avoid Type to be interpreted as a Char (in other terms to become a character)
	nodeBody->SetValue("type", type);

	// Shape
	size_t shapeID = (size_t)body->GetShape();
	nodeBody->SetValue("shape", shapeID);

	// Main Info
	XMLNode* nodeInfo;

	/// Vec2s
	nodeBody->PushBackChild(nodeInfo = Vec2TXML::Export(body->GetPosition())); // Position
	nodeInfo->SetValue("id", "position");

	nodeBody->PushBackChild(nodeInfo = Vec2TXML::Export(body->GetVelocityLinear())); // Velocity Linear
	nodeInfo->SetValue("id", "velocityLinear");

	/// floats (In Values)
	nodeBody->SetValue("mass", body->GetMass()); // Mass
	nodeBody->SetValue("restitution", body->GetRestitution()); // Restitution
	nodeBody->SetValue("friction", body->GetFriction()); // Friction

	nodeBodies->PushBackChild(nodeBody);
}

/// IMPORT ///

void PX2WorldXML::ImportShapes(std::unordered_map<size_t, PX2Shape*>& shapes, XMLNode * nodeWorld)
{
	XMLNode* nodeShapes = nodeWorld->GetNodeNamed("Shapes");
	if (!nodeShapes) { return; }

	XMLNode * nodeShape = nodeShapes->GetNodeNamed("Shape");

	PX2Shape*	shape;
	size_t		shapeID;

	while (nodeShape)
	{
		// Create Shape From Node
		PX2Shape* shape = ImportShape(nodeShape);
		if (shape)
		{
			nodeShape->GetValue("id", shapeID);
			shapes[shapeID] = shape;
		}
		// Then Remove It And Search For Next One
		nodeShape->Remove();
		nodeShape = nodeShapes->GetNodeNamed("Shape");
	}
}

PX2Shape * PX2WorldXML::ImportShape(XMLNode * nodeShape)
{
	int typeu;
	nodeShape->GetValue("type", typeu);
	std::cout << "After Get Value Type:" << typeu << std::endl;
	PX2Shape::TYPE type = (PX2Shape::TYPE)typeu;

	if (type == PX2Shape::TYPE::BOX)
		return (ImportShapeBox(nodeShape));
	else if (type == PX2Shape::TYPE::CIRCLE)
		return (ImportShapeCircle(nodeShape));
	return (NULL);
}

PX2Shape * PX2WorldXML::ImportShapeBox(XMLNode * nodeShape)
{
	float width, height;

	nodeShape->GetValue("width", width);
	nodeShape->GetValue("height", height);
	PX2ShapeBox* box = new PX2ShapeBox(width, height);
	return (box);
}

PX2Shape * PX2WorldXML::ImportShapeCircle(XMLNode * nodeShape)
{
	float radius;

	nodeShape->GetValue("radius", radius);
	PX2ShapeCircle* circle = new PX2ShapeCircle(radius);
	return (circle);
}

void PX2WorldXML::ImportBodies(PX2World * world, XMLNode * nodeWorld, std::unordered_map<size_t, PX2Shape*>& shapes)
{
	XMLNode* nodeBodies = nodeWorld->GetNodeNamed("Bodies");
	if (!nodeBodies) { return; }

	XMLNode * nodeBody = nodeBodies->GetNodeNamed("Body");

	PX2Body::Properties	prop;
	

	while (nodeBody)
	{
		// Create Shape From Node
		if (ImportBody(nodeBody, shapes, prop))
		{
			world->CreateBody(prop);
		}
		// Then Remove It And Search For Next One
		nodeBody->Remove();
		nodeBody = nodeBodies->GetNodeNamed("Body");
	}
}

bool PX2WorldXML::ImportBody(XMLNode * nodeBody, std::unordered_map<size_t, PX2Shape*>& shapes, PX2Body::Properties& prop)
{
	uint16_t	typeu;

	nodeBody->GetValue("type", typeu);
	PX2Body::TYPE type = (PX2Body::TYPE)typeu;
	prop.type = type;

	nodeBody->GetValue("mass", prop.mass);
	nodeBody->GetValue("restitution", prop.restitution);
	nodeBody->GetValue("friction", prop.friction);

	size_t	shapeID;
	nodeBody->GetValue("shape", shapeID);
	if (shapes.count(shapeID))
		prop.shape = shapes.at(shapeID);

	XMLNode*	nodeInfo;
	
	if (nodeInfo = nodeBody->GetNodeNamedWithValue("Vec2", "id", "position"))
		Vec2TXML::Import(prop.position, nodeInfo);
	if (nodeInfo = nodeBody->GetNodeNamedWithValue("Vec2", "id", "velocityLinear"))
		Vec2TXML::Import(prop.velocityLinear, nodeInfo);

	return (true);
}

