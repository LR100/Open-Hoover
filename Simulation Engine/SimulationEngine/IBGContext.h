#pragma once

class IBGContext
{
public:
	virtual ~IBGContext() {};

	virtual bool Init() = 0;
	virtual bool Quit() = 0;
};