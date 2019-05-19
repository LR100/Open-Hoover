#pragma once

#include "IBGContext.h"

#include <stdint.h>

// IS A SINGLETON
class BGContextSDL : public IBGContext
{
public:
	static BGContextSDL& Get()
	{
		static BGContextSDL nonDynSingleton;
		nonDynSingleton.Init();
		return (nonDynSingleton);
	}

	virtual bool Init() override;
	virtual bool Quit() override;

	void NotifyCreatedItem();
	void NotifyDeletedItem();

	
protected:

	
	BGContextSDL();
	~BGContextSDL();

private:
	BGContextSDL & operator=(const BGContextSDL&) {};
	BGContextSDL(const BGContextSDL&);

// Attributes
	unsigned int		_items;
	bool				_isInit;
};

