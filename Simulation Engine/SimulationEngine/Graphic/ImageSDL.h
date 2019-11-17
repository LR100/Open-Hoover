#ifndef _IMAGESDL_H_
#define _IMAGESDL_H_

#include <SDL.h>
#include "IImage.h"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN

#define SDL_RED_FLAG 0xff000000
#define SDL_GREEN_FLAG  0x00ff0000
#define SDL_BLUE_FLAG  0x0000ff00
#define SDL_ALPHA_FLAG 0x000000ff

#else

#define SDL_RED_FLAG 0x000000ff
#define SDL_GREEN_FLAG  0x0000ff00
#define SDL_BLUE_FLAG 0x00ff0000
#define SDL_ALPHA_FLAG 0xff000000

#endif

class ImageSDL : public AImage
{
public:
	ImageSDL();
	virtual ~ImageSDL();

	virtual void	Init(const unsigned int & width, const unsigned int & height, const Format & format) override;

	virtual const unsigned int& GetWidth() const override;
	virtual const unsigned int& GetHeight() const override;

	// OTHERS
	static IImage::Format	GetFormatBySDLFormat(SDL_PixelFormat* format);
	static IImage::Format	GetFormatBySDLFormat(Uint32 f);
	void					InitFormatBySDLFormat(SDL_PixelFormat* format);
	void					InitFromSurface(SDL_Surface *surface);
	SDL_Surface*			GetSurface() const { return (_surface); };
	
protected:
	SDL_Surface*			_surface;

	// Hérité via AImage
	virtual bool Export(const std::string & path) const override;
};

#endif /* !_IMAGESDL_H_ */