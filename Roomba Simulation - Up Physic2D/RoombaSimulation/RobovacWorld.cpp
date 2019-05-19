#include "RobovacWorld.h"

#include "Tools\XMLParser.h"


RobovacWorld::RobovacWorld(const std::string & pathMap)
{
	XMLParser	xmlParser;

	xmlParser.Parse(pathMap);

	// Get Size Of World
	_idLastRobovac = 0;

	_objectsCount = 0;
}

#include "Physic/XML/PX2WorldXML.h" // PX2 World XML Interpreter

RobovacWorld::RobovacWorld()
{
	// GRAVITY IS Null For Robovac World (View From Top)
	_pxWorld = new PX2World(Vec2(0.0f, 980.0f));

	Import("robovacWorld.xml");

	_idLastRobovac = 0;
}

void RobovacWorld::SetEvents(IEventHandler * eventHandler)
{
	// Reset Objects Selected
	_idLastRobovac = 0;
	_objectsSelected.clear();

	eventHandler->AddHandlerToEvent(ControlKey::MOUSE_LEFT, ControlKeyState::RELEASED, &RobovacWorld::MouseLeftReleased, this, eventHandler->GetMouse());
	eventHandler->AddHandlerToEvent(ControlKey::MOUSE_MOTION, &RobovacWorld::MouseMotion, this, eventHandler->GetMouse());

	eventHandler->AddHandlerToEvent(ControlKey::KEY_LEFT, &RobovacWorld::MoveLastRobovac, this, Vec2(-1, 0));
	eventHandler->AddHandlerToEvent(ControlKey::KEY_RIGHT, &RobovacWorld::MoveLastRobovac, this, Vec2(1, 0));
	eventHandler->AddHandlerToEvent(ControlKey::KEY_UP, &RobovacWorld::MoveLastRobovac, this, Vec2(0, -1));
	eventHandler->AddHandlerToEvent(ControlKey::KEY_DOWN, &RobovacWorld::MoveLastRobovac, this, Vec2(0, 1));

	eventHandler->AddHandlerToEvent(ControlKey::KEY_RETURN, &RobovacWorld::Crazy, this);

}

// Init Sprites and everything need in drawer
void RobovacWorld::InitDrawer(IDrawer2D * drawer)
{
	drawer->AddSprite("roomba", _roombaBasics.sprite);
}

void RobovacWorld::Draw(IDrawer2D * drawer)
{
	std::vector<PX2Body*>	bodies = _pxWorld->GetBodies();

	_objectsSelectedToDraw = _objectsSelected;
	_objectsCount = bodies.size();

	for (size_t i = 0; i < bodies.size(); i += 1) // for the moment iterate over all bodies
	{
		DrawBody(bodies.at(i), drawer);
	}
	DrawInfoRobovac(drawer);
}

void RobovacWorld::DrawBody(PX2Body * body, IDrawer2D * drawer)
{
	const PX2Shape* shape = body->GetShape();

	Color	colorWall(170, 140, 110);
	Color	colorWallBorder(180, 175, 110);
	Color	colorWallExtraBorder(220, 220, 220);
	Color	colorRobovac(55, 255, 55);
	Color	colorCurrent;

	bool	selected = false;

	for (size_t i = 0; i < _objectsSelectedToDraw.size(); i += 1)
	{
		if (body->GetID() == _objectsSelectedToDraw.at(i))
		{
			selected = true;
			colorCurrent = Color(255, 25, 25); // Color Selected
			_objectsSelectedToDraw.erase(_objectsSelectedToDraw.begin() + i);
			i = _objectsSelectedToDraw.size() + 1;
		}
	}

	if (shape != NULL)
	{
		if (shape->GetType() == PX2Shape::TYPE::BOX) // WALLS
		{
			drawer->DrawRect(body->GetAABB().min.x, body->GetAABB().min.y, body->GetAABB().halfSize.x * 2.0f, body->GetAABB().halfSize.y * 2.0f, colorWall);

			float sizeBorder;
			float fillScale;


			sizeBorder = 5;
			fillScale = sizeBorder / std::min(body->GetAABB().halfSize.x, body->GetAABB().halfSize.y);

			if (selected)
			{
				drawer->DrawBorder(body->GetAABB().min.x - 1, body->GetAABB().min.y - 1, body->GetAABB().halfSize.x * 2.0f + 2, body->GetAABB().halfSize.y * 2.0f + 2, 0.2f, colorCurrent, colorCurrent);
			}
			drawer->DrawBorder(body->GetAABB().min.x, body->GetAABB().min.y, body->GetAABB().halfSize.x * 2.0f, body->GetAABB().halfSize.y * 2.0f, fillScale, colorWallBorder, colorWallBorder);

			sizeBorder = 2;
			fillScale = sizeBorder / std::min(body->GetAABB().halfSize.x, body->GetAABB().halfSize.y);
			drawer->DrawBorder(body->GetAABB().min.x, body->GetAABB().min.y, body->GetAABB().halfSize.x * 2.0f, body->GetAABB().halfSize.y * 2.0f, fillScale, colorWallExtraBorder, colorWallExtraBorder);
			return;
		}
		else if (shape->GetType() == PX2Shape::TYPE::CIRCLE) // ROOMBAS
		{

			//drawer->DrawCircleFill(body->GetPosition().x, body->GetPosition().y, body->GetShape()->GetRadius() * 2.0f, colorCurrent);
			drawer->SetSprite("roomba");
			drawer->DrawSprite(body->GetAABB().min.x, body->GetAABB().min.y);
			if (selected)
			{
				drawer->DrawCircle(body->GetPosition().x, body->GetPosition().y, body->GetShape()->GetRadius() * 2.0f, colorCurrent);
			}
			// DISPLAY DIRECTION
			Vec2i pos;
			Vec2i dir;

			pos.x = (int)body->GetPosition().x;
			pos.y = (int)body->GetPosition().y;

			dir.x = (int)(body->GetDirNorm().x * body->GetShape()->GetRadius());
			dir.y = (int)(body->GetDirNorm().y * body->GetShape()->GetRadius());

			drawer->DrawLine(pos.x, pos.y, pos.x + dir.x, pos.y + dir.y, Color::RED());

			return;
		}
	}
}

#include <sstream>

void RobovacWorld::DrawInfoRobovac(IDrawer2D * drawer)
{
	PX2Body* roomba = _pxWorld->GetBody(_idLastRobovac);

	if (roomba)
	{
		Color				colorText(100, 100, 255);
		std::stringstream	ss;
		Vec2i				pos(10, 30);
		std::string			font = "pixel.ttf";




		ss.str("");
		ss << "-ROOMBA INFO- Objects in Simulation (" << _objectsCount << ")";
		drawer->LoadText(ss.str(), 16, font, colorText);
		drawer->DrawText(pos.x, pos.y);

		pos.y += 13;
		ss.str("");
		ss << "Vel Lin (" << roomba->GetVelocityLinear().ToString() << ")";
		drawer->LoadText(ss.str(), 16, font, colorText);
		drawer->DrawText(pos.x, pos.y);

	}
}

void RobovacWorld::AddWall(const AABB2& wall)
{
	PX2Body::Properties prop;

	prop.type = PX2Body::TYPE::STATIC;
	prop.mass = 10.0f;
	prop.restitution = 1.0f; // Wall of an appartment In real world almost restitute nothing 
	// (Except if Really Bouncing material encounter it) -> So set on 0.7 to avoid bug
	prop.position = wall.center;
	prop.shape = new PX2ShapeBox(wall.halfSize.x * 2, wall.halfSize.y * 2);

	_pxWorld->CreateBody(prop);
}

void RobovacWorld::AddRobovac(const Vec2 & pos)
{
	PX2Body::Properties prop;

	prop.type = PX2Body::TYPE::DYNAMIC;
	prop.mass = 10.0f;
	prop.restitution = 0.8f; // Robovac In real world  almost restitute nothing 
	prop.position = pos;
	prop.shape = _roombaBasics.shape;


	PX2Body* roomba = _pxWorld->CreateBody(prop);
	if (roomba)
		_idLastRobovac = roomba->GetID();
}

#include <iostream> // TMP

void RobovacWorld::SelectObjectsAtPosition(const Vec2 & pos)
{
	std::vector<PX2Body*>	bodies = _pxWorld->GetBodiesAtPosition(pos);

	std::cout << "Select Objects At Position (" << pos.ToString() << ")" << std::endl;

	_objectsSelected.clear();
	for (size_t i = 0; i < bodies.size(); i += 1)
	{
		std::cout << "- (" << bodies.at(i)->GetID() << ")" << std::endl;
		_objectsSelected.push_back(bodies.at(i)->GetID());
	}
}

void RobovacWorld::RemoveObjectsAtPosition(const Vec2 & pos)
{
	std::vector<PX2Body*>	bodies = _pxWorld->GetBodiesAtPosition(pos);

	std::cout << "Remove Objects At Position (" << pos.ToString() << ") :" << std::endl;

	for (size_t i = 0; i < bodies.size(); i += 1)
	{
		std::cout << "- (" << bodies.at(i)->GetID() << ")" << std::endl;
		_pxWorld->DestroyBody(bodies.at(i));
	}
}

void RobovacWorld::Update(const float & dt)
{
	_pxWorld->Simulate(dt);
}

void RobovacWorld::Import(const std::string & pathMap)
{
	// Search For World at robovacWorld.wml in curFolder
	XMLParser	xmlParser;

	if (xmlParser.Parse(pathMap))
	{
		XMLNode*	tree = xmlParser.GetTree();

		if (tree)
		{
			XMLNode* world = tree->GetNodeNamed("World");
			if (world)
			{
				PX2WorldXML::Import(_pxWorld, world);
			}
		}
	}
}

void RobovacWorld::Clear()
{
	_pxWorld->Clear();
	_objectsSelected.clear();
	_objectsCount = 0;
}

void RobovacWorld::MouseLeftReleased(const Mouse * mouse)
{
	SelectObjectsAtPosition(_released);
}

void RobovacWorld::MouseMotion(const Mouse * mouse)
{
	_released.x = mouse->x;
	_released.y = mouse->y;
}

void RobovacWorld::MoveLastRobovac(Vec2 dir)
{
	PX2Body* roomba = _pxWorld->GetBody(_idLastRobovac);

	if (roomba)
	{
		float speed = 5000.0f;

		roomba->ApplyForce(dir * speed);
	}
}

void RobovacWorld::Crazy()
{
	size_t NB_OBJ = 100;
	for (size_t i = 0; i < NB_OBJ; i += 1)
	{
		Vec2 pos;

		pos.x = (float)(rand() % 1000) + 100;
		pos.y = (float)(rand() % 800) + 100;

		PX2Body::Properties prop;

		prop.type = PX2Body::TYPE::DYNAMIC;
		prop.mass = 10.0f;
		prop.restitution = 1.0f; // Those are Crazy balls
		prop.position = pos;
		prop.shape = _roombaBasics.shape;
		

		pos.x = (float)(rand() % 2000) - 1000;
		pos.y = (float)(rand() % 2000) - 1000;

		prop.velocityLinear = pos;

		PX2Body* roomba = _pxWorld->CreateBody(prop);
		if (roomba)
			_idLastRobovac = roomba->GetID();
	}
}


#include "Graphic/Drawer2DSDL.h"

RobovacWorld::RobovacBasics::RobovacBasics()
{
	radius = 20.0f;
	unsigned int diameter = (unsigned int)(radius * 2.0f);


	IDrawer2D*	drawer = new Drawer2DSDL();
	IImage*		image = drawer->CreateImage("roomba", diameter + 2, diameter + 2);
	Vec2i		pos;




	pos.x = (int)(radius);
	pos.y = (int)(radius);

	Color carton(210, 180, 140);


	drawer->SetCurrentImage(image);
	drawer->ClearImage();
	drawer->DrawCircleFill(pos.x, pos.y, diameter, Color(110, 180, 240));
	drawer->DrawCircle(pos.x, pos.y, diameter, Color(220, 220, 220));
	drawer->DrawCircle(pos.x, pos.y, diameter - 3, Color(25, 25, 25));
	drawer->DrawCircle(pos.x, pos.y, diameter - 2, Color(25, 25, 25));
	drawer->DrawCircle(pos.x, pos.y, diameter - 1, Color(25, 25, 25));

	std::cout << "Create Sprite" << std::endl;
	sprite = new Sprite(image, Color(0, 0, 0));

	shape = new PX2ShapeCircle(radius);
}

