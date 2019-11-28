#ifndef ENVSDL_H_INCLUDED
#define ENVSDL_H_INCLUDED

// For map<> type
#include <stdlib.h>
#include <string>
#include <map>

// IWindow Interface
#include "IWindow.h" 


#include <SDL.h>

#include "ImageSDL.h" 

#include "../Events/IEventHandler.h"
#include "../BGContextSDL.h"

// An IEventHandler Can be linked To Window SDL 
// to interact with closing / Resizing event Automatically

class WindowSDL : public IWindow
{
public:

	WindowSDL();
	WindowSDL(unsigned int width, unsigned int height, const std::string & name = "Default Window Name");
	WindowSDL(IEventHandler* eHandler, unsigned int width, unsigned int height, const std::string & name = "Default Window Name");

	~WindowSDL();

	// EventHandler
	void	SetEventHandler(IEventHandler* eHandler);

	/// Hérité via IWindow
	// Basics
	bool						Open(IEventHandler* eHandler, unsigned int width, unsigned int height, const std::string & name = "Default Window Name");
	virtual bool				Open(unsigned int width, unsigned int height, const std::string & name= "Default Window Name") override;
	virtual bool				IsOpen() const override;
	virtual void				Close() override;
	virtual bool				IsFocus() const override;
	virtual void				Focus() override;
	// Screen
	virtual void				Resize(const unsigned int& width, const unsigned int& height) override;
	virtual void				SetPosition(const int& x, const int& y) override;
	virtual void				Refresh() override;

	virtual const unsigned int& GetWidth() const override;
	virtual const unsigned int& GetHeight() const override;

	virtual IImage*				GetBackBuffer() const override;
	virtual const ColorFormat&	GetColorFormat() const override;
	virtual const uint32_t&		GetID() const override;

	// Property
	virtual void				SetBorderless(bool state) override;

	// Mouse
	virtual void				CursorHide(bool state) override;
	virtual void				CursorToCenter() override;

protected:

	// Init For Construction
	void				InitBasics();

	// Event Handlers
	void				EventResize();
	void				EventClose();
	void				EventFocus();
	void				EventUnFocus();

	bool			_open;
	bool			_showCursor;
	bool			_focus;
	bool			_borderless;

	unsigned int	_width, _height;
	uint32_t		_id;

	SDL_Window*		_window;

	SDL_Surface*	_screen;
	ImageSDL*		_buffer;
	
	SDL_Rect		_rect;


	ColorFormat		_format;
	IEventHandler*	_linkedEventHandler;
};


#endif // ENVSDL_H_INCLUDED
