#include "RobovacWorldEditor.h"

RobovacWorldEditor::RobovacWorldEditor(RobovacWorld * roombaWorld)
{
	_roombaWorld = roombaWorld;
}

RobovacWorldEditor::~RobovacWorldEditor()
{

}

void RobovacWorldEditor::SetEvents(IEventHandler * eventHandler)
{
	_mouseLeftPressedRemain = false;
	_released.x = eventHandler->GetMouse()->x;
	_released.y = eventHandler->GetMouse()->y;

	eventHandler->AddHandlerToEvent(ControlKey::MOUSE_LEFT, ControlKeyState::PRESSED, &RobovacWorldEditor::MouseLeftPressed, this, eventHandler->GetMouse());
	eventHandler->AddHandlerToEvent(ControlKey::MOUSE_LEFT, ControlKeyState::RELEASED, &RobovacWorldEditor::MouseLeftReleased, this, eventHandler->GetMouse());

	eventHandler->AddHandlerToEvent(ControlKey::MOUSE_RIGHT, ControlKeyState::RELEASED, &RobovacWorldEditor::MouseRightReleased, this, eventHandler->GetMouse());

	eventHandler->AddHandlerToEvent(ControlKey::MOUSE_MOTION, &RobovacWorldEditor::MouseMotion, this, eventHandler->GetMouse());
}

void RobovacWorldEditor::Draw(IDrawer2D * drawer)
{
	if (_mouseLeftPressedRemain)
	{
		Color			colorWall(255, 255, 255);
		Vec2i			origin;
		unsigned int	width, height;

		width = (unsigned int)std::fabs(std::fabs(_pressed.x) - std::fabs(_released.x));
		height = (unsigned int)std::fabs(std::fabs(_pressed.y) - std::fabs(_released.y));

		origin.x = (int)std::min(_pressed.x, _released.x);
		origin.y = (int)std::min(_pressed.y, _released.y);


		drawer->DrawBorder(origin.x, origin.y, width, height, colorWall, colorWall);

		origin.x -= 15; // Display Text "Wall" at left top of wall
		origin.y -= 15;
		drawer->LoadText("wall", 12, "pixel.ttf", colorWall);
		drawer->DrawText(origin.x, origin.y);
	}
	else
	{
		Color			colorWall(255, 255, 255);
		Vec2i			origin;

		origin.x = (int)(_released.x);
		origin.y = (int)(_released.y);


		origin.x -= 15; // Display Text "Wall/Delete" at left top of Mouse Cursor
		origin.y -= 15;
		drawer->LoadText("wall/delete", 12, "pixel.ttf", colorWall);
		drawer->DrawText(origin.x, origin.y);
	}
}

void RobovacWorldEditor::MouseLeftPressed(const Mouse* mouse)
{
	if (!_mouseLeftPressedRemain)
	{
		_mouseLeftPressedRemain = true;
		//std::cout << "RobovacWorldEditor::MouseLeftPressed()" << std::endl;
		_pressed.x = mouse->x;
		_pressed.y = mouse->y;
	}
}

void RobovacWorldEditor::MouseLeftReleased(const Mouse* mouse)
{
	if (_mouseLeftPressedRemain)
	{
		//std::cout << "RobovacWorldEditor::MouseLeftReleased()" << std::endl;
		_mouseLeftPressedRemain = false;

		Vec2			origin;
		Vec2			size;
		Vec2			center;

		size.x = std::fabs(std::fabs(_pressed.x) - std::fabs(_released.x)) * 0.5f;
		size.y = std::fabs(std::fabs(_pressed.y) - std::fabs(_released.y)) * 0.5f;

		origin.x = std::min(_pressed.x, _released.x);
		origin.y = std::min(_pressed.y, _released.y);

		center.x = origin.x + size.x;
		center.y = origin.y + size.y;

		AABB2 wall(center, size);

		const float	minSize = 4.0f;

		if (size.x < minSize || size.y < minSize)
		{
			//std::cerr << "Wall is too Small (Minimum size (" << minSize << "))" << std::endl;
			// Create Robovac
			_roombaWorld->AddRobovacNoIA(_released);

		}
		else
		{
			_roombaWorld->AddWall(wall);
		}
	}
}

void RobovacWorldEditor::MouseRightReleased(const Mouse * mouse)
{
	std::cout << "RobovacWorldEditor::MouseRightReleased()" << std::endl;
	Vec2	pos;

	pos = _released;
	// +/- Scale Of Pos Of World In Screen;


	_roombaWorld->RemoveObjectsAtPosition(pos);
}

void RobovacWorldEditor::MouseMotion(const Mouse * mouse)
{
	//std::cout << "RobovacWorldEditor::MouseMotion()" << std::endl;
	_released.x = mouse->x;
	_released.y = mouse->y;
}
