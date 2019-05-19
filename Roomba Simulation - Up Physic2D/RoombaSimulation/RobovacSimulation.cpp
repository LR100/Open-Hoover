// SDL
#include "Events\EventHandlerSDL.h"
#include "Graphic\WindowSDL.h"
#include "Graphic\Drawer2DSDL.h"


#include "RobovacSimulation.h"

IDrawer2D*	PX2Drawer;
IWindow*	PX2Window;
IDrawer2D*	QuadtreeDrawer;
IWindow*	QuadtreeWindow;

RobovacSimulation::RobovacSimulation()
{
	// TMP DEBUG
	_treeMode = false;

	// END TMP DEBUG

	_isLaunch = false;
	_isRunning = false;
	_switchMode = false;


	_editMode = true;

	_fpsAimed = 100.0f;
	_timeToDraw = 1.0f / _fpsAimed;

	_timeToTick = .3f;
	_timeCountToTick = 0.0f;

	_timeCountToDraw = 0.0f;
}

void RobovacSimulation::Launch()
{
	if (!_isLaunch)
	{
		Init();
		_isRunning = true;
		_isLaunch = true;
		Loop();
	}
}

void RobovacSimulation::Init()
{
	InitWorld(); // FIRST !!!!
	InitEvents();
	InitGraphic();
}

void RobovacSimulation::InitEvents()
{
	_eventHandler = new EventHandlerSDL();

	SetEvents();
}

void RobovacSimulation::InitGraphic()
{
	_window = new WindowSDL(_eventHandler, 800, 800, "Robovac Simulation");
	_drawer = new Drawer2DSDL(_window->GetBackBuffer()->GetFormat());
	std::cout << "DRAWER 2D ADDR (" << _drawer << ")" << std::endl;
	std::cout << "DRAWER DEFAULT FORMAT (" << _drawer->GetDefaultFormatImage() << ")" << std::endl;
	std::cout << "Set Window Back BUFFER" << std::endl;
	_drawer->SetCurrentImage(_window->GetBackBuffer());


	PX2Window = _window;
	PX2Drawer = _drawer; // TMP (INIT PX2DRAWER !!!!!)

	QuadtreeWindow = _window;
	QuadtreeDrawer = _drawer; // TMP (INIT QuadTreeDRAWER !!!!!)

	// Init Drawer In World;
	_roombaWorld->InitDrawer(_drawer);
}

void RobovacSimulation::InitWorld()
{
	_roombaWorld = new RobovacWorld();
	_roombaWorldEditor = new RobovacWorldEditor(_roombaWorld);
}

void RobovacSimulation::SetEvents()
{
	if (_editMode)
		SetEventsEdit();
	else
		SetEventsPlay();
}

void RobovacSimulation::SetEventsEdit()
{
	// Clear
	_eventHandler->Clear();
	// Then Generic FIRST !
	SetEventsGeneric();

	_roombaWorldEditor->SetEvents(_eventHandler);
}

void RobovacSimulation::SetEventsPlay()
{
	// Clear
	_eventHandler->Clear();
	// Then Generic FIRST !
	SetEventsGeneric();

	_roombaWorld->SetEvents(_eventHandler);
}

void RobovacSimulation::SetEventsGeneric()
{
	_eventHandler->AddHandlerToEvent(ControlKey::WINDOW_CLOSE, &RobovacSimulation::Stop, this);
	_eventHandler->AddHandlerToEvent(ControlKey::KEY_ESCAPE, ControlKeyState::RELEASED, &RobovacSimulation::Stop, this);
	_eventHandler->AddHandlerToEvent(ControlKey::KEY_E, ControlKeyState::RELEASED, &RobovacSimulation::SwitchModeNotify, this);
	_eventHandler->AddHandlerToEvent(ControlKey::KEY_C, ControlKeyState::RELEASED, &RobovacSimulation::ClearWorld, this);
	_eventHandler->AddHandlerToEvent(ControlKey::KEY_S, ControlKeyState::RELEASED, &RobovacSimulation::SaveWorld, this);
	_eventHandler->AddHandlerToEvent(ControlKey::KEY_R, ControlKeyState::RELEASED, &RobovacSimulation::ReloadWorld, this);


	// TMP DEBUG
	_eventHandler->AddHandlerToEvent(ControlKey::KEY_LCTRL, &RobovacWorld::AddRobovac, _roombaWorld, Vec2(300, 300));
	_eventHandler->AddHandlerToEvent(ControlKey::KEY_T, ControlKeyState::RELEASED, &RobovacSimulation::SwitchTreeMode, this);
	// END TMP DEBUG

}

#include <chrono>
#include <thread>

void RobovacSimulation::Loop()
{
	std::chrono::microseconds microsleep(50);

	_timer.SetElapsedTime();

	_fpsC.AddFrame();

	while (_isRunning)
	{
		std::this_thread::sleep_for(microsleep); // Too avoid Using CPU too Much

		_timer.SetElapsedTime();
		_eventHandler->UpdateEvents();

		_fpsC.ShowFps();
		if (_isRunning)
		{
			_timeCountToDraw += _timer.GetElapsedSecond();
			_timeCountToTick += _timer.GetElapsedSecond();


			if (_timeCountToDraw >= _timeToDraw)
			{
				if (_switchMode)
				{
					SwitchMode();
				}

				Draw();
				_fpsC.AddFrame();
				_timeCountToDraw = 0;
			}

			if (_timeCountToTick >= _timeToTick)
			{
				//_roombaWorld->AddRobovac(Vec2(300, 300), 40.0f);
				_timeCountToTick = 0;
			}
		}
		else
		{
			// DO BEFORE LEAVE ACTION
		}
	}
}

void RobovacSimulation::Stop()
{
	if (_isRunning)
	{
		_window->Close();
		_isRunning = false;
	}
}

void RobovacSimulation::Draw()
{

	// Update Physics
	UpdatePhysic();

	
	// Clear Screen
	_drawer->ClearImage();

	// TMP DEBUG
	if (_treeMode)
	{
		DrawTree();
	}


	// END TMP DEBUG


	_roombaWorld->Draw(_drawer);


	Color				colorText;
	std::stringstream	ss;

	if (_editMode)
	{
		ss << "Edit Mode (Press E To Play)";
		colorText = Color(25, 255, 50);
		_roombaWorldEditor->Draw(_drawer);
	}
	else
	{
		colorText = Color(155, 5, 10);
		ss << "Play Mode (press E to Edit)";
	}


	_drawer->LoadText(ss.str(), 14, "pixel.ttf", colorText);
	_drawer->DrawText(10, 10);


	_window->Refresh();
}

void RobovacSimulation::UpdatePhysic()
{
	_roombaWorld->Update(_timeCountToDraw);
}

void RobovacSimulation::SwitchMode()
{
	_switchMode = false;
	_editMode = !_editMode;
	SetEvents();
	if (!_editMode)
	{
		_roombaWorld->AddRobovac(Vec2(250, 300));
	}
}

void RobovacSimulation::SwitchModeNotify()
{
	_switchMode = true;
}

void RobovacSimulation::ClearWorld()
{
	_roombaWorld->Clear();
}

void RobovacSimulation::ReloadWorld()
{
	_roombaWorld->Clear();
	_roombaWorld->Import("robovacWorld.xml");
}

#include "Physic/XML/PX2WorldXML.h"

void RobovacSimulation::SaveWorld()
{
	XMLNode* world = PX2WorldXML::Export(_roombaWorld->_pxWorld);
	
	world->Export("robovacWorld.xml");

}

#include "Physic/PX2CollisionQuadTree.h"

void DebugDrawQuadTreeAABB(const PX2CollisionQuadTree::Node* node, IWindow* window, IDrawer2D* drawer, Color color)
{
	size_t x, y;
	size_t w, h;


	//for (size_t i = 0; i < node->GetIndexes().size(); i += 1)
	//{
	//	
	//	x = node->GetIndexes().at(i)->GetAABB().center.x;
	//	y = node->GetIndexes().at(i)->GetAABB().center.y;
	//	w = node->GetIndexes().at(i)->GetAABB().halfSize.x * 2;
	//	h = node->GetIndexes().at(i)->GetAABB().halfSize.y * 2;
	//	x -= w / 2;
	//	y -= h / 2;

	//	drawer->DrawBorder(x, y, w, h, Color(0, 35, 125), Color(0, 35, 125));
	//}

	x = node->GetBoundary().center.x;
	y = node->GetBoundary().center.y;
	w = node->GetBoundary().halfSize.x * 2;
	h = node->GetBoundary().halfSize.y * 2;
	x -= w / 2;
	y -= h / 2;

	//if (w < 5 || h < 5)
	//	return;

	if (node->GetBoundary().max.x < 0.1f || node->GetBoundary().max.y < 0.1f)
		return;

	if (node->GetBoundary().min.x >= window->GetWidth() || node->GetBoundary().min.y >= window->GetHeight())
		return;

	if (node->GetChildren()[0] != NULL)
	{
		DebugDrawQuadTreeAABB(node->GetChildren()[0], window, drawer, color);
		DebugDrawQuadTreeAABB(node->GetChildren()[1], window, drawer, color);
		DebugDrawQuadTreeAABB(node->GetChildren()[2], window, drawer, color);
		DebugDrawQuadTreeAABB(node->GetChildren()[3], window, drawer, color);
	}

	if (node->GetBoundary().max.x >= window->GetWidth() || node->GetBoundary().max.y >= window->GetHeight())
		return;


	drawer->DrawBorder(x, y, w, h, color, color);
}

#include "Physic/PX2BodyManager.h" // TMP

Vec2* dirs = NULL;
size_t NB_DIRS = 90;

void RobovacSimulation::DrawTree()
{
	DebugDrawQuadTreeAABB(_roombaWorld->_pxWorld->GetBodyManager()->GetTreeCollision()->GetRoot(), _window, _drawer, Color(150, 150, 150));

	Vec2	pos(500, 500);

	float goodRotate = 360.0f / (float)(NB_DIRS);

	if (dirs == NULL)
	{
		Vec2	odir(1, 0);
		Vec2	dir;
		float	rotate = 0;

		dirs = new Vec2[NB_DIRS];

		for (size_t i = 0; i < NB_DIRS; i += 1)
		{
			dir = odir;
			VectorTransformer::Rotate(dir, rotate);
			VectorTransformer::Normalize(dir, dirs[i]);
			rotate += goodRotate;
		}
	}

	for (size_t i = 0; i < NB_DIRS; i += 1)
	{
		VectorTransformer::Rotate(dirs[i], 0.05f);
		VectorTransformer::Normalize(dirs[i]);
		float distance;
		_roombaWorld->_pxWorld->GetBodyManager()->GetTreeCollision()->GetFirstIndexInRay(pos, dirs[i], distance);
	}

	////VectorTransformer::Rotate(dir, 0.2f);

	//VectorTransformer::Normalize(dir);
	//float distance;
	//_roombaWorld->_pxWorld->GetBodyManager()->GetTreeCollision()->GetFirstIndexInRay(pos, dir, distance);
}

void RobovacSimulation::SwitchTreeMode()
{
	_treeMode = !_treeMode;
}


