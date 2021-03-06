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
	_screen = AABB2(Vec2(1000.0f, 1000.0f), Vec2(1000.0f, 1000.0f));
	std::cout << "Screen AABB: (" << _screen.ToString() << ")" << std::endl;

	_objectsCount = 0;
}

//#include "Physic/XML/Physic2DWorldXML.h" // Physic2D World XML Interpreter

RobovacWorld::RobovacWorld()
{
	// GRAVITY IS Null For Robovac World (View From Top)
	_p2dWorld = new Physic2DWorldB2D(Vec2(0.0f, 0.0f));
	
	Import("robovacWorld.xml");
	_p2dWorld->SetGravity(Vec2(0.0f, 0.0f)); // In case XML World Set Other Gravity
	
	_idLastRobovac = 0;
	


	_screen = AABB2(Vec2(1000.0f, 1000.0f), Vec2(1000.0f, 1000.0f));
	std::cout << "Screen AABB: (" << _screen.ToString() << ")" << std::endl;
	//AddRobovac(Vec2(200, 200));
	//AddRobovac(Vec2(400, 400));
	_debugDraw = true;
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
	_roombaBasics = new RobovacBasics(_p2dWorld);
	drawer->AddSprite("roomba", _roombaBasics->sprite);
	_p2dd = new Physic2DDebugDrawer(drawer);
	// Register Bodies Imported in the Debug Drawer
	std::vector<Physic2DBody*> bodies = _p2dWorld->GetBodies();
	for (size_t i = 0; i < bodies.size(); i += 1) {
		_p2dd->RegisterBody(bodies.at(i));
	}
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
	
	//std::cout << "Draw Screen AABB: (" << _screen.ToString() << ")" << std::endl;

	DrawRobovacs(drawer); // Draw Robovacs First

	if (_debugDraw)
	{
		_p2dd->ResetObjectsCount();
		_p2dWorld->GetBodiesInAABB(_p2dd, _screen);
		_objectsCount = _p2dd->GetObjectsCount();
	}
	else {
		DrawQueryCb drawquerycb(this, drawer);
		_p2dWorld->GetBodiesInAABB(&drawquerycb, _screen);
		_objectsSelectedToDraw = _objectsSelected;
		_objectsCount = drawquerycb.GetObjectsCount();
	}

	

	
	//std::cout << "Bodies to Draw: (" << _objectsCount << ")" << std::endl;

	
	DrawInfoRobovac(drawer);
}

void RobovacWorld::RemoveBody(Physic2DBody* body)
{
	if (_robovacs.count(body->GetID())) // Erase From Robovacs List
	{
		_robovacs.erase(body->GetID());
	}
	_p2dWorld->DestroyBody(body);
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
			//std::cout << "Draw Box" << std::endl;
			Vec2 pos = body->GetPosition();
			Physic2DShapeBox* boxShape = (Physic2DShapeBox*)shape;

			int x, y, w, h, wh, hh;


			x = pos.x - (boxShape->GetWidth() / 2);
			y = pos.y - (boxShape->GetHeight() / 2);
			w = boxShape->GetWidth();
			wh = (int)((float)w / 2.0f);
			h = boxShape->GetHeight();
			hh = (int)((float)h / 2.0f);
			
			drawer->DrawRect(x, y, w, h, colorWall);

			float sizeBorder;
			float fillScale;


			sizeBorder = 5;
			fillScale = sizeBorder / std::min(wh, hh);

			if (selected)
			{
				drawer->DrawBorder(x - 1, y - 1, w + 2, h + 2, 0.2f, colorCurrent, colorCurrent);
			}
			drawer->DrawBorder(x, y, w, h, fillScale, colorWallBorder, colorWallBorder);

			sizeBorder = 2;

			fillScale = sizeBorder / std::min(wh, hh);
			drawer->DrawBorder(x, y, w, h, fillScale, colorWallExtraBorder, colorWallExtraBorder);
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
		Color				colorText(10, 190, 255);
		std::stringstream	ss;
		Vec2i				pos(10, 30);
		std::string			font = "pixel.ttf";
		unsigned int		fontSize = 16;

		ss.str("");
		ss << "-ROOMBA INFO- Objects in Simulation (" << _objectsCount << ")";
		drawer->LoadText(ss.str(), fontSize, font, colorText);
		drawer->DrawText(pos.x, pos.y);

		pos.y += fontSize + 4;// fontSize + Margin
		ss.str("");
		ss << "Linear Velocity (" << roomba->GetVelocityLinear().ToString() << ")";
		drawer->LoadText(ss.str(), fontSize, font, colorText);
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

	propFixture.restitution = 0.7f; // Wall of an appartment In real world almost restitute nothing 
	//// (Except if Really Bouncing material encounter it) -> So set on 0.7 to avoid bug
	
	Physic2DShapeBox* box = _p2dWorld->CreateShapeBox();
	box->SetSize(wall.halfSize.x * 2, wall.halfSize.y * 2);
	propFixture.shape = box;
	propFixture.density = 100.0f;
	propFixture.friction = 10000.0f;

	Physic2DBody* body = _p2dWorld->CreateBody(propBody);
	if (body) {
		body->CreateFixture(propFixture);
		_p2dd->RegisterBody(body); // Register wall in debug drawer
	}
	else {
		std::cerr << "Too Much WALLs..." << std::endl;
	}
}

const Robovac* RobovacWorld::AddRobovac(const Vec2 & pos)
{
	std::cout << "Add ROBOVAC" << std::endl;
	Physic2DBodyProperties		propBody;
	Physic2DFixtureProperties	propFixture;

	propBody.type = Physic2DBodyProperties::TYPE::DYNAMIC;
	propBody.position = pos;
	propBody.linearDamping = 0.5f;

	//propFixture.mass = 10.0f;
	b2BodyDef def;
	
	propFixture.density = 1.0f;
	propFixture.friction = 10000.0f;
	propFixture.restitution = 1.000f; // Robovac In real world  almost restitute nothing 
	propFixture.shape = _roombaBasics->shape;

	Physic2DBody* roomba = _p2dWorld->CreateBody(propBody);

	if (roomba)
	{
		propFixture.userdata = roomba;
		roomba->CreateFixture(propFixture);
		_idLastRobovac = roomba->GetID();
	
		_p2dd->RegisterBody(roomba); // Register roomba in debug drawer

		Robovac* robovac = new Robovac();
		_robovacs.emplace(roomba->GetID(), robovac);

		robovac->SetBody(roomba);
		robovac->SetRadius((uint8_t)_roombaBasics->shape->GetRadius());
		robovac->Start();
		return (robovac);
	}
	else {
		std::cerr << "Too Much ROBOVACs..." << std::endl;
	}
	return (NULL);
}

void RobovacWorld::AddRobovacNoIA(const Vec2& pos, const float& radius)
{
	Physic2DBodyProperties		propBody;
	Physic2DFixtureProperties	propFixture;

	propBody.type = Physic2DBodyProperties::TYPE::DYNAMIC;
	propBody.position = pos;
	propBody.linearDamping = 0.5f;
	propBody.allowSleep = true;
	propBody.fixedRotation = false;
	
	propFixture.density = 100.0f; // Heavy Shit
	propFixture.friction = 10.0f;
	propFixture.restitution = .900f; // Bouncy Coefficient
	if (_roombaBasics->shape->GetRadius() == radius) {
		propFixture.shape = _roombaBasics->shape;
	}
	else {
		Physic2DShapeCircle* circle = _p2dWorld->CreateShapeCircle();
		circle->SetRadius(radius);
		propFixture.shape = circle;
	}
	

	Physic2DBody* roomba = _p2dWorld->CreateBody(propBody);
	if (roomba) {
		propFixture.userdata = roomba;
		roomba->CreateFixture(propFixture);
		_p2dd->RegisterBody(roomba); // Register roomba in debug drawer
	}
}

#include <iostream> // TMP

void RobovacWorld::SelectObjectsAtPosition(const Vec2 & pos)
{
	//std::vector<Physic2DBody*>	bodies = _p2dWorld->GetBodiesAtPosition(pos);

	std::cout << "Select Objects At Position (" << pos.ToString() << ")" << std::endl;

	/*_objectsSelected.clear();
	for (size_t i = 0; i < bodies.size(); i += 1)
	{
		std::cout << "- (" << bodies.at(i)->GetID() << ")" << std::endl;
		_objectsSelected.push_back(bodies.at(i)->GetID());
	}*/
}

void RobovacWorld::RemoveObjectsAtPosition(const Vec2 & pos)
{
	std::cout << "Remove Objects At Position (" << pos.ToString() << ") :" << std::endl;
	RemoveQueryCb removequerycb(this);
	//std::cout << "Draw Screen AABB: (" << _screen.ToString() << ")" << std::endl;
	AABB2 aabb(pos, Vec2(1.0f, 1.0f));

	_p2dWorld->GetBodiesInAABB(&removequerycb, aabb);
}

void RobovacWorld::Update(const float & dtS)
{
	//std::cout << "World Gravity: (" << _p2dWorld->GetGravity().ToString() << ")" << std::endl;
	_p2dWorld->Simulate(dtS);
	UpdateRobovacs(dtS * 1000.0f); // Update Robovacs after simulation
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
				Physic2DWorldXML::Import(_p2dWorld, world);
			}
		}
	}
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
	float	radius;
	Vec2	pos;
	AABB2	area;
	

	for (size_t i = 0; i < NB_OBJ; i += 1)
	{	
		radius = (float)(rand() % 20) + 2;
		pos.x = (float)(rand() % (unsigned int)_screen.max.x) + 20;
		pos.y = (float)(rand() % (unsigned int)_screen.max.y) + 20;
		area.center = pos;
		area.halfSize.x = radius;
		area.halfSize.y = radius;
		area.ComputeMinMax();

		LocationAvailableQueryCb locquery;

		_p2dWorld->GetBodiesInAABB(&locquery, area);
		if (locquery.LocationIsAvailable()) {
			AddRobovacNoIA(pos, radius);
		}
	}
}


#include "Graphic/Drawer2DSDL.h"

RobovacWorld::RobovacBasics::RobovacBasics(Physic2DWorld* world)
{
	std::cout << "RobovacBasics::RobovacBasics()" << std::endl;
	radius = 15.0f;
	unsigned int diameter = (unsigned int)(radius * 2.0f);

	IDrawer2D*	drawer = new Drawer2DSDL(ColorFactory::Get().GetFormat());
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

	ImageSDL* imageImported = new ImageSDL();

	imageImported->Import("OpenHooverLogo.bmp");

	std::cout << "Create Sprite" << std::endl;
	sprite = new Sprite(imageImported, Color(255, 255, 255).GetDef());
	
	shape = world->CreateShapeCircle();
	std::cout << "Robovac Basic Shape Type(" << (int)shape->GetType() << ")" << std::endl;
	shape->SetRadius(radius);
}

RobovacWorld::DrawQueryCb::DrawQueryCb(RobovacWorld* _world, IDrawer2D* _drawer)
{
	objectsCount = 0;
	world = _world;
	drawer = _drawer;
}

void RobovacWorld::DrawQueryCb::Reset()
{
	objectsCount = 0;
}

const size_t& RobovacWorld::DrawQueryCb::GetObjectsCount() const
{
	return (objectsCount);
}

bool RobovacWorld::DrawQueryCb::ReportBody(Physic2DBody* body)
{
	world->DrawBody(body, drawer);
	objectsCount += 1;
	return (true); // Continue to draw all bodies
}

RobovacWorld::RemoveQueryCb::RemoveQueryCb(RobovacWorld* _world)
{
	world = _world;
}

bool RobovacWorld::RemoveQueryCb::ReportBody(Physic2DBody* body)
{
	world->RemoveBody(body);
	return (true); // Continue to remove all bodies
}

RobovacWorld::LocationAvailableQueryCb::LocationAvailableQueryCb()
{
	_locationIsAvailable = true;
}

bool RobovacWorld::LocationAvailableQueryCb::LocationIsAvailable()
{
	return (_locationIsAvailable);
}

bool RobovacWorld::LocationAvailableQueryCb::ReportBody(Physic2DBody* body)
{
	_locationIsAvailable = false;
	return (false);
}
