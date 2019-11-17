#pragma once

#include "Events\IEventHandler.h"
#include "Graphic\IWindow.h"
#include "Graphic\IDrawer2D.h"
#include "Graphic\Tools\FPSComputer.h"
#include "Tools\MyTimer.h"


#include "RobovacWorld.h"
//#include "RobovacWorldEditor.h"



class RobovacSimulation
{
public:

	RobovacSimulation();
	virtual ~RobovacSimulation() {};

	void	Launch();

private:

	// Init
	void	Init();
	void	InitEvents();
	void	InitGraphic();
	void	InitWorld();

	void	SetEvents();
	void	SetEventsEdit();
	void	SetEventsPlay();

	void	SetEventsGeneric();


	// Loop
	void	Loop();
	// Stop
	void	Stop();

	// Draw
	void	Draw();
	
	// Play
	void	UpdatePhysic();

	// Edit
	void	AddRobovacRandomly();

	// Others
	void	SwitchMode();
	void	SwitchModeNotify();

	// World Updating

	void	ClearWorld();
	void	ReloadWorld();
	void	SaveWorld();


	// Attributes //

	// TMP DEBUG

	void	DrawTree();

	void	DrawCalibrate();
	float	CalibrateAngularRotate(float angularVelocityMin, float angularVelocityMax, uint16_t markersCount, float angularRotateDesired, float diffAccept = 0.05f, float percentAccept = 0.80f);
	void	SwitchTreeMode();
	void	SwitchCalibrateMode();
	bool	_treeMode;
	bool	_calibrateMode;


	// END TMP DEBUG


	bool			_isLaunch;
	bool			_isRunning;

	bool			_switchMode;
	bool			_editMode;

	Timer			_timer;


	float			_fpsAimed;
	float			_timeToDraw;
	float			_timeCountToDraw;

	float			_timeToTick;
	float			_timeCountToTick;


	RobovacWorld*	 _robovacWorld;

	IEventHandler*	_eventHandler;
	IDrawer2D*		_drawer;
	IWindow*		_window;
	IDrawer2D*		_drawerMap;
	IWindow*		_windowMap;
	FpsComputer		_fpsC;

	//RobovacWorldEditor*	_roombaWorldEditor;
};
