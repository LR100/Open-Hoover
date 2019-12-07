#ifndef _DRAWER2DSDL_H_
#define _DRAWER2DSDL_H_

#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <sstream>
#include <SDL_image.h> // Using SDL_Image Lib for Loading images files (.jpeg, .png, ...)
#include <SDL_ttf.h> // Using TTF SDL (for text drawing)
#include "ImageSDL.h" // Using Image SDL (override of IImage with SDL)

#include "IDrawer2D.h"
#include "../Maths/ValueTools.h"


#ifdef DrawText
#undef DrawText
#endif

#define TRSPR 255
#define TRSPG 24
#define TRSPB 244

class Drawer2DSDL : public IDrawer2D
{
public:
	Drawer2DSDL(const ColorFormat& format);
	

	// SINGLETON BY CREATING
	void * operator new(size_t size);

	

private:

	Drawer2DSDL(const Drawer2DSDL& other);
	Drawer2DSDL(const Drawer2DSDL&& other);

	virtual ~Drawer2DSDL();
	static Drawer2DSDL*	sptr; // SINGLETON INSTANCE	

public:

	virtual void 					Quit() override;

	// INIT
	virtual void					SetDefaultFont(const std::string& name) override;
	virtual void					SetDefaultFontSize(const unsigned int& size) override;

	// Images
	virtual bool					SetCurrentImage(IImage* image) override;
	virtual IImage*					GetCurrentImage() const override;
	virtual void					SetDefaultImage(const std::string& name) override;
	virtual const std::string&		GetDefaultImageName() override;
	virtual void					UseDefaultImage() override;

	virtual void					SetDefaultFormatImage(const ColorFormat& format) override;
	virtual const ColorFormat&	GetDefaultFormatImage() override;

	virtual IImage*					CreateImage(const std::string& name, const unsigned int& w, const unsigned int& h, const ColorFormat& format) override;
	virtual void					UseImage(const std::string& name) override;
	virtual IImage*					GetImage(const std::string& name) override;
	virtual IImage*					RemoveImage(const std::string& name) override;

	// Tools
	virtual void					ScaleImage(const unsigned int& sW, const unsigned int& sH) override;
	virtual void					ClearImage() override;
	virtual void					CopyImage(const std::string& nameToDraw) override;
	virtual void					DrawImage(const std::string& nameToDraw, const int& xPos, const int& yPos) override;

	// Shapes
	virtual void					DrawPoint(const int& x, const int& y, const Color& color) override;
	virtual void					DrawLine(const int& x, const int& y, const int&xE, const int& yE, const Color& color) override;
	virtual void					DrawLine(const int& x, const int& y, const int&xE, const int& yE, const Color& colorB, const Color& colorE) override;
	
	virtual void					DrawBorder(const int& x, const int& y, const unsigned int& width, const unsigned int& height, const Color& color_l, const Color& color_r) override;
	virtual void					DrawBorder(const int& x, const int& y, const unsigned int& width, const unsigned int& height, float thickness, const Color& color_l, const Color& color_r) override;

	virtual void					DrawRect(const int& x, const int& y, const unsigned int& width, const unsigned int& height, const Color& color) override;
	virtual void					DrawRectFill(const int& x, const int& y, const unsigned int& width, const unsigned int& height, float fillscale, const Color& color) override;

	virtual void					DrawCircleFill(const int& x, const int& y, const unsigned int& diameter, const Color& color) override;
	virtual void					DrawCircleFill(const int& x, const int& y, const unsigned int& diameter, const Color& colora, const Color& colorb) override;
	virtual void					DrawCircle(const int& x, const int& y, const unsigned int& diameter, const Color& color) override;

	// Text Part
	virtual int						LoadText(const std::string& text, const unsigned int& sFont, const std::string& font, const Color& color) override;
	virtual void					GetSizeText(unsigned int& w, unsigned int& h) override;
	virtual void					DrawText(const int& xPos, const int& yPos) override;

	// Sprites
	virtual	void					AddSprite(const std::string& id, Sprite* sprite) override;
	virtual	void					LoadSprite(const std::string& id, const std::string& path) override;
	virtual void					SetSprite(const std::string& id) override; // (Set Current Sprite and use it for next Draw);
	virtual void					SetSpriteColorTransparency(const Color& color) override;
	virtual void					DrawSprite(const int& x, const int& y) override;
	virtual void					DrawSprite(const int& x, const int& y, Sprite* sprite) override;
	///
	// OTHERS -
	///
	// SDL Specification
	void	AddImage(const std::string& name, ImageSDL *img);

	/// //////////// ///
	///  DRAW STRUCT ///
	/// //////////// ///

	// Rectangle 
	struct DrawRectSnakeAttrib
	{
		unsigned int minX;
		unsigned int minY;
		unsigned int maxX;
		unsigned int maxY;
		unsigned int x;
		unsigned int y;
		char dir;
		unsigned int filledRect;
		unsigned int rectToFill;

		void Init(unsigned int width, unsigned int height, unsigned int xPos, unsigned int yPos, float fillScale);
		void IncDrawRectangle(char& dir, unsigned int& filledRect, unsigned int& x, unsigned int& y, unsigned int& minX, unsigned int& minY, unsigned int& maxX, unsigned int& maxY);
	};

	struct PolyEdge
	{
		int		min_y;
		int		max_y;
		int		x_to_min_y;
		int		sign;
		int		dx;
		int		dy;
		float	sum;
	};

private:

	inline void CorrectXY(int & x, int & y);
	inline int	CorrectX(const int & x);
	inline int	CorrectY(const int & y);

	inline void	InternSetPixel(const int& x, const int& y, const unsigned int& color);
	inline void	InternCheckBFSetPixel(const int& x, const int& y, const unsigned int& color);
	inline void InternSetLine(const int& x, const int& y, const int& xe, const Sprite::Line& line);

	struct Font
	{
		TTF_Font	*font;
	};

	struct Text
	{
		SDL_Surface	*text;
	};

	// INITIALISATION
	void		Init();

	// Images
	ColorFormat										_defaultImageFormat;
	ImageSDL*										_currImage;
	std::string										_currImageName;
	ImageSDL*										_defaultImage;
	std::string										_defaultImageName;
	std::unordered_map<std::string, ImageSDL*>		_images;
	// Text
	std::unordered_map<std::string, Font>			_fonts;
	Text											_textActive;
	// Sprites
	std::unordered_map<std::string, Sprite*>		_sprites;
	Sprite*											_spriteActive;
	Color*											_spriteColorTransparency;

	// Drawing Optimization STRUCT //
	struct InfoLine
	{
		int		deltaX, deltaY;
		int		incX, incY;
		int		i;
		int		sum;
		int		xi, yi;
	};
	
	struct InfoRect
	{
		int		xMax, yMax;
		int		xi, yi;
		int		iw, ih;
		int		cX, cY;
		int		cXMax, cYMax;
	};

	struct InfoCircle
	{
		int		xi, yi;
		int		d;
		int		dx, dy;
		int		radius;
	};

	struct InfoInternLine
	{
		int		x;
		int		xmem;
	};

	InfoLine										_iL;
	InfoRect										_iR;
	InfoCircle										_iC;
	InfoInternLine									_iIL;

	// Colors Manipulation attributs
	float											_offsetColor;
	float											_stepColor;
	ColorFDef										_colorFA;
	ColorFDef										_colorFB;
	ColorFDef										_colorFC;

};

#endif /* !_DRAWER2DSDL_H_ */