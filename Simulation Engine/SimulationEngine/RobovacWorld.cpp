#include "RobovacWorld.h"

#include "Tools\XMLParser.h"

#include "Physic/Physic2DWorldB2D.h"

RobovacWorld::RobovacWorld(const std::string & pathMap)
{
	XMLParser	xmlParser;

	_p2dWorld = NULL;
	xmlParser.Parse(pathMap);

	// Get Size Of World
	_idLastRobovac = 0;

	_objectsCount = 0;
}

//#include "Physic/XML/Physic2DWorldXML.h" // Physic2D World XML Interpreter

RobovacWorld::RobovacWorld()
{
	// GRAVITY IS Null For Robovac World (View From Top)
	_p2dWorld = new Physic2DWorldB2D(Vec2(0.0f, 0.0f));
	
	Import("robovacWorld.xml");
	_p2dWorld->SetGravity(Vec2(0.0f, 0.0f)); // In case World Set Other Gravity
	_roombaBasics = new RobovacBasics(_p2dWorld);
	_idLastRobovac = 0;
	AddRobovac(Vec2(200, 200));
	//AddRobovac(Vec2(400, 400));
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
	drawer->AddSprite("roomba", _roombaBasics->sprite);
}

void RobovacWorld::DrawRobovacs(IDrawer2D* drawer)
{
	std::unordered_map<size_t, Robovac*>::iterator it = _robovacs.begin();
	std::unordered_map<size_t, Robovac*>::iterator itEnd = _robovacs.end();

	Robovac* robovac;
	for (; it != itEnd; it++)
	{
		robovac = it->second;
		robovac->Draw(drawer);
	}
}

void RobovacWorld::Draw(IDrawer2D * drawer)
{
	std::cout << "Draw" << std::endl;
	std::vector<Physic2DBody*>	bodies = _p2dWorld->GetBodies();

	_objectsSelectedToDraw = _objectsSelected;
	_objectsCount = bodies.size();

	
	for (size_t i = 0; i < bodies.size(); i += 1) // for the moment iterate over all bodies
	{
		DrawBody(bodies.at(i), drawer);
	}
	DrawRobovacs(drawer); // Draw Robovacs First
	DrawInfoRobovac(drawer);
}

void RobovacWorld::DrawBody(Physic2DBody * body, IDrawer2D * drawer)
{
	//std::cout << "Draw Body" << std::endl;
	const std::vector<Physic2DFixture*>& fixtures = body->GetFixtures();

	Color	colorWall(170, 140, 110);
	Color	colorWallBorder(180, 175, 110);
	Color	colorWallExtraBorder(220, 220, 220);
	Color	colorRobovac(55, 255, 55);
	Color	colorCurrent(55, 255, 255);

	bool	selected = false;

	//for (size_t i = 0; i < _objectsSelectedToDraw.size(); i += 1)
	//{
	//	if (body->GetID() == _objectsSelectedToDraw.at(i))
	//	{
	//		selected = true;
	//		colorCurrent = Color(255, 25, 25); // Color Selected
	//		_objectsSelectedToDraw.erase(_objectsSelectedToDraw.begin() + i);
	//		i = _objectsSelectedToDraw.size() + 1;
	//	}
	//}

	for (size_t i = 0; i < fixtures.size(); i += 1)
	{
	//	std::cout << "Draw Fixture" << std::endl;
		const Physic2DShape* shape = fixtures.at(i)->GetShape();

	if (shape != NULL)
	{
		//std::cout << "Draw Shape of Type ("<< (int)shape->GetType() << ")" << std::endl;
		if (shape->GetType() == Physic2DShape::TYPE::BOX) // WALLS
		{
			Vec2 pos = body->GetPosition();
			Physic2DShapeBox* boxShape = (Physic2DShapeBox*)shape;

			int x, y, w, h;


			x = pos.x - (boxShape->GetWidth() / 2);
			y = pos.y - (boxShape->GetHeight() / 2);
			w = boxShape->GetWidth();
			h = boxShape->GetHeight();

			drawer->DrawRect(x, y, w, h, colorWall);

			float sizeBorder;
			float fillScale;


			sizeBorder = 5;
			//fillScale = sizeBorder / std::min(body->GetAABB().halfSize.x, body->GetAABB().halfSize.y);

			if (selected)
			{
				//drawer->DrawBorder(body->GetAABB().min.x - 1, body->GetAABB().min.y - 1, body->GetAABB().halfSize.x * 2.0f + 2, body->GetAABB().halfSize.y * 2.0f + 2, 0.2f, colorCurrent, colorCurrent);
			}
			//drawer->DrawBorder(body->GetAABB().min.x, body->GetAABB().min.y, body->GetAABB().halfSize.x * 2.0f, body->GetAABB().halfSize.y * 2.0f, fillScale, colorWallBorder, colorWallBorder);

			sizeBorder = 2;
			//fillScale = sizeBorder / std::min(body->GetAABB().halfSize.x, body->GetAABB().halfSize.y);
			//drawer->DrawBorder(body->GetAABB().min.x, body->GetAABB().min.y, body->GetAABB().halfSize.x * 2.0f, body->GetAABB().halfSize.y * 2.0f, fillScale, colorWallExtraBorder, colorWallExtraBorder);
			return;
		}
		else if (shape->GetType() == Physic2DShape::TYPE::CIRCLE) // ROBOVAC
		{
			Vec2 pos = body->GetPosition();
			//std::cout << "Draw Shape Circle" << std::endl;
			Physic2DShapeCircle* circleShape = (Physic2DShapeCircle*)shape;

			//std::cout << "Draw Shape Circle at position : " << body->GetPosition().ToString() << " Of Radius ("<< circleShape->GetRadius() <<")" <<std::endl;
			//body->GetID();
			//drawer->DrawCircleFill(pos.x, pos.y, circleShape->GetRadius() * 2.0f, colorCurrent);

			drawer->SetSprite("roomba");
			drawer->DrawSprite(pos.x - circleShape->GetRadius(), pos.y - circleShape->GetRadius());
			if (selected)
			{
				drawer->DrawCircle(pos.x, pos.y, circleShape->GetRadius() * 2.0f, colorCurrent);
			}
			// DISPLAY DIRECTION
			Vec2i posi;
			Vec2i dir;

			//pos.x = (int)pos.x;
			//pos.y = (int)pos.y;

			//dir.x = (int)(body->GetDirNorm().x * body->GetShape()->GetRadius());
			//dir.y = (int)(body->GetDirNorm().y * body->GetShape()->GetRadius());

			// drawer->DrawLine(pos.x, pos.y, pos.x + dir.x, pos.y + dir.y, Color::RED());
			
			
		}
	}

	}
}


void RobovacWorld::DrawRobovac(IDrawer2D* drawer, Robovac* robovac)
{
	robovac->Draw(drawer);
}

#include <sstream>

void RobovacWorld::DrawInfoRobovac(IDrawer2D * drawer)
{
	Physic2DBody* roomba = _p2dWorld->GetBody(_idLastRobovac);

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
		//ss << "Vel Lin (" << roomba->GetVelocityLinear().ToString() << ")";
		drawer->LoadText(ss.str(), 16, font, colorText);
		drawer->DrawText(pos.x, pos.y);

	}
}

void RobovacWorld::UpdateRobovacs(const float& dtMs)
{
	//std::cout << "Update Robovac" << std::endl;
	std::unordered_map<size_t, Robovac*>::iterator it = _robovacs.begin();
	std::unordered_map<size_t, Robovac*>::iterator itEnd = _robovacs.end();

	Robovac* robovac;
	for (; it != itEnd; it++)
	{
		robovac = it->second;
		if (robovac)
			robovac->Update(dtMs);
	}
}

void RobovacWorld::AddWall(const AABB2& wall)
{
	std::cout << "Add WALL" << std::endl;
	Physic2DBodyProperties		propBody;
	Physic2DFixtureProperties	propFixture;

	propBody.type = Physic2DBodyProperties::TYPE::STATIC;
	propBody.position = wall.center;

	propFixture.restitution = 0.001f; // Wall of an appartment In real world almost restitute nothing 
	//// (Except if Really Bouncing material encounter it) -> So set on 0.7 to avoid bug
	
	Physic2DShapeBox* box = _p2dWorld->CreateShapeBox();
	box->SetSize(wall.halfSize.x * 2, wall.halfSize.y * 2);
	propFixture.shape = box;
	propFixture.density = 1.0f;
	propFixture.friction = 0.94f;

	Physic2DBody* body = _p2dWorld->CreateBody(propBody);
	body->CreateFixture(propFixture);
}

const Robovac* RobovacWorld::AddRobovac(const Vec2 & pos)
{
	std::cout << "Add ROBOVAC" << std::endl;
	Physic2DBodyProperties		propBody;
	Physic2DFixtureProperties	propFixture;

	propBody.type = Physic2DBodyProperties::TYPE::DYNAMIC;
	propBody.position = pos;


	//propFixture.mass = 10.0f;
	propFixture.density = 1.0f;
	propFixture.friction = 0.98f;
	propFixture.restitution = 0.001f; // Robovac In real world  almost restitute nothing 
	propFixture.shape = _roombaBasics->shape;


	Physic2DBody* roomba = _p2dWorld->CreateBody(propBody);
	propFixture.userdata = roomba;
	roomba->CreateFixture(propFixture);

	if (roomba)
	{
		_idLastRobovac = roomba->GetID();
		
		Robovac* robovac = new Robovac();
		_robovacs.emplace(roomba->GetID(), robovac);

		robovac->SetBody(roomba);
		robovac->SetRadius((uint8_t)_roombaBasics->shape->GetRadius());
		robovac->Start();
		return (robovac);
	}
	return (NULL);
}

#include <iostream> // TMP

void RobovacWorld::SelectObjectsAtPosition(const Vec2 & pos)
{
	/*std::vector<Physic2DBody*>	bodies = _p2dWorld->GetBodiesAtPosition(pos);

	std::cout << "Select Objects At Position (" << pos.ToString() << ")" << std::endl;

	_objectsSelected.clear();
	for (size_t i = 0; i < bodies.size(); i += 1)
	{
		std::cout << "- (" << bodies.at(i)->GetID() << ")" << std::endl;
		_objectsSelected.push_back(bodies.at(i)->GetID());
	}*/
}

void RobovacWorld::RemoveObjectsAtPosition(const Vec2 & pos)
{
	//std::vector<Physic2DBody*>	bodies = _p2dWorld->GetBodiesAtPosition(pos);

	//std::cout << "Remove Objects At Position (" << pos.ToString() << ") :" << std::endl;

	//for (size_t i = 0; i < bodies.size(); i += 1)
	//{
	//	std::cout << "- (" << bodies.at(i)->GetID() << ")" << std::endl;
	//	if (_robovacs.count(bodies.at(i)->GetID())) // Erase From Robovacs List
	//	{
	//		_robovacs.erase(bodies.at(i)->GetID());
	//	}
	//	_p2dWorld->DestroyBody(bodies.at(i));
	//	
	//}
}

void RobovacWorld::Update(const float & dtS)
{
	std::cout << "World Gravity: (" << _p2dWorld->GetGravity().ToString() << ")" << std::endl;
	_p2dWorld->Simulate(dtS);
	UpdateRobovacs(dtS * 1000.0f); // Update Robovacs after simulation
}

void RobovacWorld::Import(const std::string & pathMap)
{
	//// Search For World at robovacWorld.wml in curFolder
	//XMLParser	xmlParser;

	//if (xmlParser.Parse(pathMap))
	//{
	//	XMLNode*	tree = xmlParser.GetTree();

	//	if (tree)
	//	{
	//		XMLNode* world = tree->GetNodeNamed("World");
	//		if (world)
	//		{
	//			Physic2DWorldXML::Import(_p2dWorld, world);
	//		}
	//	}
	//}
}

void RobovacWorld::Clear()
{
	_p2dWorld->Clear();
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
	Physic2DBody* roomba = _p2dWorld->GetBody(_idLastRobovac);

	if (roomba)
	{
		float speed = 5000.0f;

		//roomba->ApplyForce(dir * speed);
	}
}

void RobovacWorld::Crazy()
{
	size_t NB_OBJ = 100;
	for (size_t i = 0; i < NB_OBJ; i += 1)
	{
		//Vec2 pos;

		//pos.x = (float)(rand() % 1000) + 100;
		//pos.y = (float)(rand() % 800) + 100;

		//Physic2DBody::Properties prop;

		//prop.type = Physic2DBody::TYPE::DYNAMIC;
		//prop.mass = 10.0f;
		//prop.restitution = 1.0f; // Those are Crazy balls
		//prop.position = pos;
		//prop.shape = _roombaBasics.shape;
		//

		//pos.x = (float)(rand() % 2000) - 1000;
		//pos.y = (float)(rand() % 2000) - 1000;

		//prop.velocityLinear = pos;

		//Physic2DBody* roomba = _p2dWorld->CreateBody(prop);
		//if (roomba)
		//	_idLastRobovac = roomba->GetID();
	}
}


#include "Graphic/Drawer2DSDL.h"

RobovacWorld::RobovacBasics::RobovacBasics(Physic2DWorld* world)
{
	radius = 15.0f;
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

	
	shape = world->CreateShapeCircle();
	std::cout << "Robovac Basic Shape Type(" << (int)shape->GetType() << ")" << std::endl;
	shape->SetRadius(radius);
}

