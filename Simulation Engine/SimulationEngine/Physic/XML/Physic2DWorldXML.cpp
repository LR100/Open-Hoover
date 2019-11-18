#include "Physic2DWorldXML.h"

#include "../../Maths/XML/VectorXML.h"
#include "../../Maths/XML/AABBXML.h"

#include <iostream> // TMPPP

void Physic2DWorldXML::Import(Physic2DWorld* world, XMLNode* nodeWorld)
{
	std::cout << "Physic2DWorldXML::Import()" << std::endl;

	/// Parameters Of the World ///
	// Gravity
	XMLNode* nodeGravity = nodeWorld->GetNodeNamedWithValue("Vec2", "id", "gravity");
	if (nodeGravity)
	{
		std::cout << "Ok Gravity" << std::endl;
		Vec2		gravity;
		Vec2TXML::Import(gravity, nodeGravity);
		world->SetGravity(gravity);
	}

	// Shapes first
	std::unordered_map<size_t, Physic2DShape*>	shapes;
	ImportShapes(world, shapes, nodeWorld);

	// Then Bodies
	ImportBodies(world, nodeWorld, shapes);
	std::cout << "Physic2DWorldXML::Import() OK" << std::endl;
}

XMLNode* Physic2DWorldXML::Export(Physic2DWorld* world)
{
	XMLNode* nodeWorld = new XMLNode();
	nodeWorld->SetName("World");

	/// Parameters Of the World ///
	// Gravity
	XMLNode* nodeGravity = Vec2TXML::Export(world->GetGravity());
	nodeGravity->SetValue("id", "gravity");
	nodeWorld->PushBackChild(nodeGravity);

	std::unordered_map<size_t, Physic2DShape*> shapes;
	// Bodies first -> To Get only Used Shapes
	ExportBodies(world, nodeWorld, shapes);
	// Then Shapes
	ExportShapes(world, nodeWorld, shapes);
	std::cout << "Physic2DWorldXML::Export() OK" << std::endl;
	return (nodeWorld);
}

/// EXPORT ///

void Physic2DWorldXML::ExportShapes(Physic2DWorld* world, XMLNode* nodeWorld, const std::unordered_map<size_t, Physic2DShape*>& shapes)
{
	XMLNode* nodeShapes = new XMLNode();
	nodeShapes->SetName("Shapes");
	nodeWorld->PushBackChild(nodeShapes);

	std::unordered_map<size_t, Physic2DShape*>::const_iterator it = shapes.begin();
	std::unordered_map<size_t, Physic2DShape*>::const_iterator itE = shapes.end();

	for (; it != itE; it++) {
		ExportShape(it->second, nodeShapes);
	}
}

void Physic2DWorldXML::ExportShape(Physic2DShape* shape, XMLNode* nodeShapes)
{
	XMLNode* nodeShape = new XMLNode();

	nodeShape->SetName("Shape");

	int type = (int)shape->GetType(); // Avoid Type to be interpreted as a Char (in other terms to become a character)
	nodeShape->SetValue("type", type);
	size_t idShape = (size_t)shape;
	nodeShape->SetValue("id", idShape);

	nodeShapes->PushBackChild(nodeShape);

	if (shape->GetType() == Physic2DShape::TYPE::BOX) {
		ExportShapeBox((Physic2DShapeBox*)shape, nodeShape);
	} else if (shape->GetType() == Physic2DShape::TYPE::CIRCLE) {
		ExportShapeCircle((Physic2DShapeCircle*)shape, nodeShape);
	}
}

void Physic2DWorldXML::ExportShapeBox(Physic2DShapeBox* shape, XMLNode* nodeShape)
{
	nodeShape->SetValue("width", shape->GetWidth());
	nodeShape->SetValue("height", shape->GetHeight());
}

void Physic2DWorldXML::ExportShapeCircle(Physic2DShapeCircle* shape, XMLNode* nodeShape)
{
	nodeShape->SetValue("radius", shape->GetRadius());
}


void Physic2DWorldXML::ExportBodies(Physic2DWorld* world, XMLNode* nodeWorld, std::unordered_map<size_t, Physic2DShape*>& shapes)
{
	XMLNode* nodeBodies = new XMLNode();
	nodeBodies->SetName("Bodies");
	nodeWorld->PushBackChild(nodeBodies);

	std::vector<Physic2DBody*>	bodies = world->GetBodies();

	for (size_t i = 0; i < bodies.size(); i += 1) {
		ExportBody(bodies.at(i), nodeBodies, shapes);
	}
}

void Physic2DWorldXML::ExportBody(Physic2DBody* body, XMLNode* nodeBodies, std::unordered_map<size_t, Physic2DShape*>& shapes)
{
	// Basics
	XMLNode* nodeBody = new XMLNode();
	Physic2DBodyProperties properties = body->GetProperties();
	nodeBody->SetName("Body");
	int type = (int)body->GetType(); // Same as shape - Avoid Type to be interpreted as a Char (in other terms to become a character)
	nodeBody->SetValue("type", type);
	nodeBody->SetValue("allowSleep", properties.allowSleep);
	nodeBody->SetValue("linearDamping", properties.linearDamping);

	// Main Info
	XMLNode* nodeInfo;

	/// Vec2s
	nodeBody->PushBackChild(nodeInfo = Vec2TXML::Export(body->GetPosition())); // Position
	nodeInfo->SetValue("id", "position");

	nodeBody->PushBackChild(nodeInfo = Vec2TXML::Export(body->GetVelocityLinear())); // Velocity Linear
	nodeInfo->SetValue("id", "velocityLinear");
	
	// Export Fixtures
	ExportFixtures(body, nodeBody, shapes);

	nodeBodies->PushBackChild(nodeBody);
}

void Physic2DWorldXML::ExportFixtures(Physic2DBody* body, XMLNode* nodeBody, std::unordered_map<size_t, Physic2DShape*>& shapes)
{
	XMLNode*								nodeFixtures = NULL;
	const std::vector<Physic2DFixture*>&	fixtures = body->GetFixtures();
	Physic2DFixture*						fixture = NULL;
	XMLNode*								nodeInfo = NULL; // Main Info
	XMLNode*								nodeFixture = NULL;
	size_t									shapeID = 0;

	// Node Fixtures
	nodeFixtures = new XMLNode();
	nodeFixtures->SetName("Fixtures");
	nodeBody->PushBackChild(nodeFixtures);
	
	for (size_t i = 0; i < fixtures.size(); i += 1)
	{
		// New Fixture
		nodeFixture = new XMLNode();
		fixture = fixtures.at(i);
		shapeID = (size_t)fixture->GetShape();
		nodeFixture->SetName("Fixture");
		// Properties
		const Physic2DFixtureProperties& properties = fixture->GetProperties();
		nodeFixture->SetValue("shape", shapeID);
		nodeFixture->SetValue("density", properties.density);
		nodeFixture->SetValue("restitution", properties.restitution);
		nodeFixture->SetValue("friction", properties.friction);
		nodeFixture->SetValue("isSensor", properties.isSensor);
		
		// Shape
		if (!shapes.count(shapeID)) // if shape do not exist -> emplace it
			shapes.emplace(shapeID, (Physic2DShape*)fixture->GetShape());
		// Push into Body
		nodeFixtures->PushBackChild(nodeFixture);
	}
}

/// IMPORT ///

void Physic2DWorldXML::ImportShapes(Physic2DWorld* world, std::unordered_map<size_t, Physic2DShape*>& shapes, XMLNode* nodeWorld)
{
	XMLNode* nodeShapes = nodeWorld->GetNodeNamed("Shapes");
	if (!nodeShapes) { return; }

	XMLNode* nodeShape = nodeShapes->GetNodeNamed("Shape");

	Physic2DShape* shape;
	size_t		shapeID;

	while (nodeShape)
	{
		// Create Shape From Node
		Physic2DShape* shape = ImportShape(world, nodeShape);
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

Physic2DShape* Physic2DWorldXML::ImportShape(Physic2DWorld* world, XMLNode* nodeShape)
{
	int typeu;
	nodeShape->GetValue("type", typeu);
	std::cout << "After Get Value Type:" << typeu << std::endl;
	Physic2DShape::TYPE type = (Physic2DShape::TYPE)typeu;

	if (type == Physic2DShape::TYPE::BOX)
		return (ImportShapeBox(world, nodeShape));
	else if (type == Physic2DShape::TYPE::CIRCLE)
		return (ImportShapeCircle(world, nodeShape));
	return (NULL);
}

Physic2DShape* Physic2DWorldXML::ImportShapeBox(Physic2DWorld* world, XMLNode* nodeShape)
{
	float width, height;

	nodeShape->GetValue("width", width);
	nodeShape->GetValue("height", height);
	
	Physic2DShapeBox* box = world->CreateShapeBox();
	box->SetSize(width, height);
	return (box);
}

Physic2DShape* Physic2DWorldXML::ImportShapeCircle(Physic2DWorld* world, XMLNode* nodeShape)
{
	float radius;

	nodeShape->GetValue("radius", radius);
	Physic2DShapeCircle* circle = world->CreateShapeCircle();
	circle->SetRadius(radius);
	circle->GetShape();
	return (circle);
}

void Physic2DWorldXML::ImportBodies(Physic2DWorld* world, XMLNode* nodeWorld, std::unordered_map<size_t, Physic2DShape*>& shapes)
{
	XMLNode* nodeBodies = nodeWorld->GetNodeNamed("Bodies");
	if (!nodeBodies) { return; }

	XMLNode* nodeBody = nodeBodies->GetNodeNamed("Body");

	Physic2DBodyProperties	properties;
	Physic2DBody*			body;

	while (nodeBody)
	{
		// Create Shape From Node
		if (ImportBody(nodeBody, shapes, properties))
		{
			body = world->CreateBody(properties);
			ImportFixtures(nodeBody, body, shapes);
		}
		// Then Remove It And Search For Next One
		nodeBody->Remove();
		nodeBody = nodeBodies->GetNodeNamed("Body");
	}
}

bool Physic2DWorldXML::ImportBody(XMLNode* nodeBody, std::unordered_map<size_t, Physic2DShape*>& shapes, Physic2DBodyProperties& properties)
{
	uint16_t	typeu;

	nodeBody->GetValue("linearDamping", properties.linearDamping);
	nodeBody->GetValue("allowSleep", properties.allowSleep);
	nodeBody->GetValue("type", typeu);
	Physic2DBodyProperties::TYPE type = (Physic2DBodyProperties::TYPE)typeu;
	properties.type = type;

	XMLNode* nodeInfo;


	if (nodeInfo = nodeBody->GetNodeNamedWithValue("Vec2", "id", "position")) {
		Vec2TXML::Import(properties.position, nodeInfo);
	}
	if (nodeInfo = nodeBody->GetNodeNamedWithValue("Vec2", "id", "velocityLinear")) {
		Vec2TXML::Import(properties.velocityLinear, nodeInfo);
	}

	return (true);
}

bool Physic2DWorldXML::ImportFixtures(XMLNode* nodeBody, Physic2DBody* body, std::unordered_map<size_t, Physic2DShape*>& shapes)
{
	XMLNode* nodeFixtures = nodeBody->GetNodeNamed("Fixtures");

	if (!nodeFixtures) { 
		return (false); 
	}

	Physic2DFixtureProperties	properties;
	XMLNode*					nodeFixture;
	size_t						shapeID;

	nodeFixture = nodeFixtures->GetNodeNamed("Fixture");

	while (nodeFixture)
	{
		nodeFixture->GetValue("density", properties.density);
		nodeFixture->GetValue("restitution", properties.restitution);
		nodeFixture->GetValue("friction", properties.friction);
		nodeFixture->GetValue("isSensor", properties.isSensor);
		nodeFixture->GetValue("shape", shapeID);
		if (shapes.count(shapeID)) {
			properties.shape = shapes.at(shapeID);
		}
		// Create Fixture
		body->CreateFixture(properties);

		// Then Remove It And Search For Next One
		nodeFixture->Remove();
		nodeFixture = nodeFixtures->GetNodeNamed("Fixture");
	}

	return (true);
}

