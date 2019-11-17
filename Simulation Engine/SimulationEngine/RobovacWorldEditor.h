#pragma once

#include "RobovacWorld.h"


#include "Graphic\IDrawer2D.h"
#include "Events\IEventHandler.h"

class RobovacWorldEditor
{
public:
	RobovacWorldEditor(RobovacWorld* roombaWorld);
	virtual ~RobovacWorldEditor();

	void	SetEvents(IEventHandler* eventHandler);

	void	Draw(IDrawer2D* drawer);

private:

	void	MouseLeftPressed(const Mouse* mouse);
	void	MouseLeftReleased(const Mouse* mouse);

	void	MouseRightReleased(const Mouse* mouse);

	void	MouseMotion(const Mouse* mouse);


	bool	_mouseLeftPressedRemain;



	Vec2			_pressed;
	Vec2			_released;

	RobovacWorld*	_roombaWorld;
};
