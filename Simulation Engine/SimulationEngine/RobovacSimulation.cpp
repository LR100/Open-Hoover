// SDL
#include "Events\EventHandlerSDL.h"
#include "Graphic\WindowSDL.h"
#include "Graphic\Drawer2DSDL.h"


#include "RobovacWorld.h"
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


	_calibrateMode = false;
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
	std::cout << "RobovacSimulation::InitGraphic()" << std::endl;
	_window = new WindowSDL(_eventHandler, 800, 800, "Robovac Simulation");
	_drawer = new Drawer2DSDL(_window->GetBackBuffer()->GetFormat());

	/*_windowMap = new WindowSDL(_eventHandler, 800, 800, "Robovac Simulation Mapping");
	_drawerMap = new Drawer2DSDL(_windowMap->GetBackBuffer()->GetFormat());
	_drawerMap->SetCurrentImage(_windowMap->GetBackBuffer());
*/

	std::cout << "DRAWER 2D ADDR (" << _drawer << ")" << std::endl;
	std::cout << "DRAWER DEFAULT FORMAT (" << _drawer->GetDefaultFormatImage() << ")" << std::endl;
	std::cout << "Set Window Back BUFFER" << std::endl;

	_drawer->SetCurrentImage(_window->GetBackBuffer());
	

	//PX2Window = _window;
	PX2Drawer = _drawer; // TMP (INIT PX2DRAWER !!!!!)

	ROBOVACDRAWER = _drawer;
	ROBOVACWINDOW = _window;

	ROBOVACMAPDRAWER = _drawerMap;
	ROBOVACMAPWINDOW = _windowMap;

	QuadtreeWindow = _window;
	QuadtreeDrawer = _drawer; // TMP (INIT QuadTreeDRAWER !!!!!)

	// Init Drawer In World;
	_robovacWorld->InitDrawer(_drawer);
	

	//_roombaWorld->InitDrawer(_drawer);
	// And Set Window Back Buffer again
	_drawer->SetCurrentImage(_window->GetBackBuffer());
}

void RobovacSimulation::InitWorld()
{
	_robovacWorld = new RobovacWorld();
	_roombaWorldEditor = new RobovacWorldEditor(_robovacWorld);
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

	_robovacWorld->SetEvents(_eventHandler);
}

void RobovacSimulation::SetEventsGeneric()
{
	_eventHandler->AddHandlerToEvent(ControlKey::WINDOW_CLOSE, &RobovacSimulation::Stop, this);
	_eventHandler->AddHandlerToEvent(ControlKey::KEY_ESCAPE, ControlKeyState::RELEASED, &RobovacSimulation::Stop, this);
	_eventHandler->AddHandlerToEvent(ControlKey::KEY_E, ControlKeyState::RELEASED, &RobovacSimulation::SwitchModeNotify, this);
	_eventHandler->AddHandlerToEvent(ControlKey::KEY_RETURN, ControlKeyState::RELEASED, &RobovacSimulation::ClearWorld, this);
	_eventHandler->AddHandlerToEvent(ControlKey::KEY_S, ControlKeyState::RELEASED, &RobovacSimulation::SaveWorld, this);
	_eventHandler->AddHandlerToEvent(ControlKey::KEY_R, ControlKeyState::RELEASED, &RobovacSimulation::ReloadWorld, this);


	// TMP DEBUG
	_eventHandler->AddHandlerToEvent(ControlKey::KEY_A, &RobovacSimulation::AddRobovacRandomly, this);
	//_eventHandler->AddHandlerToEvent(ControlKey::KEY_LCTRL, &RobovacWorld::AddRobovac, //_roombaWorld, Vec2(300, 300));
	_eventHandler->AddHandlerToEvent(ControlKey::KEY_T, ControlKeyState::RELEASED, &RobovacSimulation::SwitchTreeMode, this);
	_eventHandler->AddHandlerToEvent(ControlKey::KEY_C, ControlKeyState::RELEASED, &RobovacSimulation::SwitchCalibrateMode, this);
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


				//_robovacWorld->AddRobovac(Vec2(300, 300), 40.0f);
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

	_robovacWorld->Draw(_drawer);

	// TMP DEBUG
	/// DRaw Over The World
	if (_treeMode)
	{
		DrawTree();
	}
	if (_calibrateMode)
	{
		DrawCalibrate();
	}


	// END TMP DEBUG


	


	Color				colorText;
	std::stringstream	ss;

	if (_editMode)
	{
		ss << "Edit Mode (Press E To Play)";
		colorText = Color(25, 255, 50);
		////_roombaWorldEditor->Draw(_drawer);
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
	_robovacWorld->Update(_timeCountToDraw);
}

void RobovacSimulation::AddRobovacRandomly()
{
	Vec2 pos;
	pos.x = ((rand() % 850) + 400);
	pos.y = ((rand() % 300) + 300);
	
	_robovacWorld->AddRobovac(pos);
}

void RobovacSimulation::SwitchMode()
{
	_switchMode = false;
	_editMode = !_editMode;
	SetEvents();
	if (!_editMode)
	{
		////_roombaWorld->AddRobovac(Vec2(250, 300));
	}
}

void RobovacSimulation::SwitchModeNotify()
{
	_switchMode = true;
}

void RobovacSimulation::ClearWorld()
{
	_robovacWorld->Clear();
}

void RobovacSimulation::ReloadWorld()
{
	////_roombaWorld->Clear();
	////_roombaWorld->Import("robovacWorld.xml");
}

//#include "Physic/XML/PX2WorldXML.h"

void RobovacSimulation::SaveWorld()
{
	XMLNode* world = Physic2DWorldXML::Export(_robovacWorld->_p2dWorld);
	world->Export("robovacWorld.xml");
	std::cout << "RobovacSimulation World Saved" << std::endl;
}

//#include "Physic/PX2CollisionQuadTree.h"

//void DebugDrawQuadTreeAABB(const PX2CollisionQuadTree::Node* node, IWindow* window, IDrawer2D* drawer, Color color)
//{
//	size_t x, y;
//	size_t w, h;
//
//
//	//for (size_t i = 0; i < node->GetIndexes().size(); i += 1)
//	//{
//	//	
//	//	x = node->GetIndexes().at(i)->GetAABB().center.x;
//	//	y = node->GetIndexes().at(i)->GetAABB().center.y;
//	//	w = node->GetIndexes().at(i)->GetAABB().halfSize.x * 2;
//	//	h = node->GetIndexes().at(i)->GetAABB().halfSize.y * 2;
//	//	x -= w / 2;
//	//	y -= h / 2;
//
//	//	drawer->DrawBorder(x, y, w, h, Color(0, 35, 125), Color(0, 35, 125));
//	//}
//
//	x = node->GetBoundary().center.x;
//	y = node->GetBoundary().center.y;
//	w = node->GetBoundary().halfSize.x * 2;
//	h = node->GetBoundary().halfSize.y * 2;
//	x -= w / 2;
//	y -= h / 2;
//
//	//if (w < 5 || h < 5)
//	//	return;
//
//	if (node->GetBoundary().max.x < 0.1f || node->GetBoundary().max.y < 0.1f)
//		return;
//
//	if (node->GetBoundary().min.x >= window->GetWidth() || node->GetBoundary().min.y >= window->GetHeight())
//		return;
//
//	if (node->GetChildren()[0] != NULL)
//	{
//		//DebugDrawQuadTreeAABB(node->GetChildren()[0], window, drawer, color);
//		//DebugDrawQuadTreeAABB(node->GetChildren()[1], window, drawer, color);
//		//DebugDrawQuadTreeAABB(node->GetChildren()[2], window, drawer, color);
//		//DebugDrawQuadTreeAABB(node->GetChildren()[3], window, drawer, color);
//	}
//
//	if (node->GetBoundary().max.x >= window->GetWidth() || node->GetBoundary().max.y >= window->GetHeight())
//		return;
//
//
//	drawer->DrawBorder(x, y, w, h, color, color);
//}


#include "Physic/PX2BodyManager.h" // TMP

Vec2* dirs = NULL;


bool markersAreEqual(uint16_t* markersA, uint16_t* markersB, uint8_t nbMarkers, float diffAccept = 0.05, float percentAccept = 0.80f)
{
	float percentageToBeOk = percentAccept; // 80 %
	float diffDist = 0;
	float maxDiff = diffAccept; //  % dist diff

	// std::cout << "Nb markers:" << nbMarkers << std::endl;

	uint8_t markerMatchToMatch = (uint8_t)((float)nbMarkers * percentageToBeOk);
	uint8_t markerMatchCount = 0;
	//std::cout << "Nb markers: TO Match" << (float)markerMatchToMatch << std::endl;

	for (uint16_t i = 0; i < nbMarkers; i += 1)
	{
		diffDist = fabsf((float)markersA[i] - (float)markersB[i]);

		if (diffDist < (maxDiff * (float)markersA[i]))
		{
			markerMatchCount += 1;
		}
	}
	//std::cout << "markerMatchCount:" << markerMatchCount << " markerMatchToMatch:" << markerMatchToMatch << std::endl;

	if (markerMatchCount < markerMatchToMatch)
		return (false);

	return (true);
}

#include <chrono> // TMP for Sleep
#include <thread> 


#define DRAW_CALIBRATE 1

float angularVelocityToGuess = 275.0f; // This value should be Guessed




float RobovacSimulation::CalibrateAngularRotate(float angularVelocityMin, float angularVelocityMax, uint16_t markersCount, float angularRotateDesired = 5, float diffAccept, float percentAccept)
{
	//float minAngularRotateDesired = 2; // Minimum angle between each mesure
	//float maxAngularRotateDesired = 5; // Maximum angle between each mesure
	///float angularRotateDesired = 5;

	float timeMintoMaxAngularRotate = (angularRotateDesired / angularVelocityMax); // Time Min to reach the Max angular rotation per mesure Desired
	float timeMaxtoMinAngularRotate = (angularRotateDesired / angularVelocityMin); // Time Max to reach the Min angular rotation per mesure Desired

	float timeMaxToReachFullRotate = (360.0f / angularVelocityMin);

	//std::cout << "timetoMinAngularRotate (" << delayToAngularMax << ")\n";
	std::cout << "Max time to reach Full Rotate (" << timeMaxToReachFullRotate << ") s\n";
	std::cout << "Time Min to Max Angular Rotate (" << timeMintoMaxAngularRotate << ") s\n";
	std::cout << "Time Max to Min Angular Rotate (" << timeMaxtoMinAngularRotate << ") s\n";

	float timeAverageToRotateDesired = ((timeMintoMaxAngularRotate + timeMaxtoMinAngularRotate) / 2.0f); // Still In Seconds
	float timePassed = 0;


	float angularVelocityStepToGuess = (angularVelocityToGuess * timeAverageToRotateDesired); // This value should be Guessed
	std::cout << "angularVelocityStepToGuess (" << angularVelocityStepToGuess << ") degree/s\n";


	uint16_t  maxMeasure = (720 / angularRotateDesired);
	uint16_t* markers = new uint16_t[markersCount];
	uint16_t* dist = new uint16_t[maxMeasure]; // Fat distance buffers
	


	Vec2	pos(300, 300);
	Vec2	origin(300, 300);

	Vec2i posI;
	posI.x = pos.x;
	posI.y = pos.y;


	//_drawer->DrawCircleFill(posI.x, posI.y, 100, Color::ORANGE());
	float rotatePos = 0;

	ColorDef line(255, 0, 0);
	Vec2 inter;
	uint16_t measureCount = 0;
	float timeStop = timeMaxToReachFullRotate + (timeMaxToReachFullRotate * 0.5f);


	int diffX = 10;
	int diffY = 10; // Simulate non-perfect rotation of robot

					// Distance of sensor from origin //
	float distSensor = 20.0f;

	while (timePassed < timeStop && measureCount < maxMeasure) // To avoid overflow in measure
	{
		Vec2 dir(1, 0);
		VectorTransformer::Rotate(dir, rotatePos);

		VectorTransformer::Normalize(dir);
		float distance = 0;

		rotatePos += (angularVelocityStepToGuess);
		//rotatePos += (((float)((rand() % 1000) - 1000)) / 10000.0f) * angularVelocityStepToGuess; // add error in rotation (from 0 to 10% of normal rotation)

		pos.x = (origin.x + (dir.x * distSensor)) + ((rand() % 10) - 5) + (dir.x * ((rand() % diffX) - (diffX / 2)));
		pos.y = (origin.y + (dir.y * distSensor)) + ((rand() % 10) - 5) + (dir.y * ((rand() % diffY) - (diffY / 2)));

		//_roombaWorld->_pxWorld->GetBodyManager()->GetTreeCollision()->GetFirstIndexInRay(pos, dir, inter);


		distance = VectorTransformer::Distance(inter, pos);
		dist[measureCount] = distance;
		// std::cout << "Distance (" << distance << ")" << std::endl;

		Vec2i posD;
		posI.x = pos.x;
		posI.y = pos.y;


		posD = posI;
		posD.x += dir.x * distance;
		posD.y += dir.y * distance;

		line.b += 20;
		line.g += 10;
		

		if (measureCount < markersCount)
		{
			markers[measureCount] = distance;
#if DRAW_CALIBRATE
			_drawer->DrawLine(posI.x, posI.y, posD.x, posD.y, Color::GREEN());
#endif
		}
		else
		{

			if (measureCount > (markersCount * 1.5f))
			{
				if (markersAreEqual(markers, &dist[measureCount - markersCount], markersCount, diffAccept, percentAccept))
				{
					std::cout << "MARKERS ARE EQUAL !!\n";

#if DRAW_CALIBRATE
					_drawer->DrawLine(posI.x, posI.y, posD.x, posD.y, Color::RED());
					_drawer->DrawCircleFill(posD.x, posD.y, 10, Color::RED());
#endif


					std::cout << "Time Total Passed (" << timePassed << ")\n";
					timePassed -= (markersCount * timeAverageToRotateDesired);
					std::cout << "Time Passed Minus Markers (" << timePassed << ")\n";
					std::cout << "Guessed Angular Velocity (" << 360.0f / timePassed << ")degree/s To Guess (" << angularVelocityToGuess << ")d/s\n";

#if DRAW_CALIBRATE
					_window->Refresh();
#endif
					//system("pause");
					delete(markers);
					delete(dist);

					return ((360.0f / timePassed));

					timePassed = timeStop;

				}
#if DRAW_CALIBRATE
				_drawer->DrawLine(posI.x, posI.y, posD.x, posD.y, line);
#endif
			}
			else
			{
#if DRAW_CALIBRATE
				_drawer->DrawLine(posI.x, posI.y, posD.x, posD.y, Color::YELLOW());
#endif
			}

		}

		std::this_thread::sleep_for(std::chrono::milliseconds((int)(timeAverageToRotateDesired * 10000.0f)));
#if DRAW_CALIBRATE
		_window->Refresh();
#endif

		measureCount += 1;
		timePassed += timeAverageToRotateDesired;
	}

	std::cout << "CALIB FAILED !! Time Total Passed (" << timePassed << ")\n";
	timePassed -= (markersCount * timeAverageToRotateDesired);
	std::cout << "Time Passed Minus Markers (" << timePassed << ")\n";
	std::cout << "Guessed Angular Velocity (" << 360.0f / timePassed << ")degree/s To Guess (" << angularVelocityToGuess << ")d/s\n";

	delete(markers);
	delete(dist);

	return ((angularVelocityMax + angularVelocityMin) / 2.0f);
}


static float percentAcceptGlobal = 1.0f;
static float diffAcceptGlobal = 0.0f;

void RobovacSimulation::DrawCalibrate()
{

	std::cout << "\n--Start Calibrating Angular Velocity--\n";
	float minA = 90;
	float maxA = 360;


	
	float B;
	float A;

	float aV;

	A = CalibrateAngularRotate(minA, maxA, 40, 1, diffAcceptGlobal, percentAcceptGlobal);
	minA = (A - 20);
	maxA = (A + 20);
	B = CalibrateAngularRotate(minA, maxA, 40, 1, diffAcceptGlobal, percentAcceptGlobal);
	aV = ((A + B) * 0.5f);

	

	std::cout << "Angular Velocity (" << aV << ")Degree/Seconds\n";
	if (std::fabs(angularVelocityToGuess - aV) < (angularVelocityToGuess / 90.0f))
	{


		// Go For Validation -> Retry it 15 times 
		const int nbTest = 15;
		int i = 0;

		for (; i < nbTest; i += 1)
		{
			A = CalibrateAngularRotate(minA, maxA, 80, 2, diffAcceptGlobal, percentAcceptGlobal);
			minA = (A - 20);
			maxA = (A + 20);
			B = A;
			aV = ((A + B) * 0.5f);
			if (!(std::fabs(angularVelocityToGuess - aV) < (angularVelocityToGuess / 90.0f)))
			{
				std::cout << "Failed Validation Test";
				i = 666; // Failed
			}
		}
		if (i != 666) // VALIDATE
		{
			std::cout << "OK\n\n";
			std::cout << "PERCENT ACCEPT: (" << percentAcceptGlobal << ")\n";
			std::cout << "DIFF ACCEPT: (" << diffAcceptGlobal << ")\n";
			std::cout << "OK :) :) :) \n\n";
			std::cout << "OK :) :) :) \n\n";

			system("pause");
		}
	}
	else
	{
		percentAcceptGlobal -= 0.02f;
		if (percentAcceptGlobal < 0.6f)
		{
			diffAcceptGlobal += 0.01f;
			percentAcceptGlobal = 1.0f; // 100%
		}
		std::cout << "!!! NOT OK !!!!\n\n";
		std::cout << "PERCENT ACCEPT: (" << percentAcceptGlobal << ")\n";
		std::cout << "DIFF ACCEPT: (" << diffAcceptGlobal << ")\n";
		std::cout << "!!! NOT OK !!!!\n\n";

	}
	//system("pause");
	/*

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
	}*/




	//float velocitityAngularSupposed = 180.0; // In Degree per Second (could be 90 or 360)



	//float angularMinDesiredForMarkers = 20; // Angle Minimum in case of Max Velocity
	//float angularMaxDesiredForMarkers = 180; // Angle Minimum in case of Max Velocity

	//float delayToAngularMin = (angularMinDesiredForMarkers / angularVelocityMin); // TO have at least MarkersCount From The first 180 degree for case with angularVelocityMax
	//float delayToAngularMax = (angularMaxDesiredForMarkers / angularVelocityMax); // TO have at least MarkersCount From The first 180 degree for case with angularVelocityMax
	//



	// CALIBRATION CONTROL INFO

	float angularVelocityMin = 90; // IN degree / s
	float angularVelocityMax = 360;

	//_window->Refresh();

	//float delayForMarkersCount = delayToAngular / markersCount;

	//float angularVelocityStepToGuess = (172.0f; // This value should be Guessed

	//// ROTATE
	//// delay
	//// STOP 
	//// MEASURE 


}

//struct Robovac {
//
//	if (state == WORK)
//	{
//		
//
//	}
//
//	void Work()
//	{
//		distance = 0; // 
//
//		if (distance < 5) // CM
//		{
//
//
//		}
//
//
//	}
//	
//	
//
//
//	Vec2	_positionEstimated;
//};

void RobovacSimulation::DrawTree()
{
	//DebugDrawQuadTreeAABB(//_roombaWorld->_pxWorld->GetBodyManager()->GetTreeCollision()->GetRoot(), _window, _drawer, Color(150, 150, 150));
}

void RobovacSimulation::SwitchTreeMode()
{
	_treeMode = !_treeMode;
}

void RobovacSimulation::SwitchCalibrateMode()
{
	_calibrateMode = !_calibrateMode;
	// Reset Calibrate Values 
	percentAcceptGlobal = 1.0f;
	diffAcceptGlobal = 0.0f;
}


