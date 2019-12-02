#pragma once

#include "IImage.h"
#include "Color.h"

class DImage : public IImage
{
public:

	DImage();
	virtual ~DImage();

	virtual void* GetData() const { return (_data); };

	virtual void					Clear() override;
	virtual void					SetLine(const unsigned int& x, const unsigned int& y, unsigned char* line, const unsigned int& lineSize);
	virtual void					SetPixel(const unsigned int& x, const unsigned int& y, const unsigned int& color) override;
	virtual void					SetPixel(const unsigned int& x, const unsigned int& y, const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a) override;

	virtual void					GetPixel(const unsigned int& x, const unsigned int& y, unsigned char& r, unsigned char& g, unsigned char& b) const override;
	virtual void					GetPixel(const unsigned int& x, const unsigned int& y, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) const override;
	virtual const ColorFormat&		GetFormat() const override;
	virtual const unsigned int&		GetSizeLine() const override;
	virtual const unsigned int&		GetBytesPerPixel() const override;

protected:

	// SetPixel By Format
	/// RGBA
	static void SetAPixelRGBA(unsigned char* p, const unsigned int& color);
	static void SetBPixelRGBA(unsigned char* p, const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a);
	/// RGB
	static void SetAPixelRGB(unsigned char* p, const unsigned int& color);
	static void SetBPixelRGB(unsigned char* p, const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a);

	/// ARGB
	static void SetAPixelARGB(unsigned char* p, const unsigned int& color);
	static void SetBPixelARGB(unsigned char* p, const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a);

	// GetPixel By Format
	static void GetAPixel(unsigned char* p, unsigned char& r, unsigned char& g, unsigned char& b, const unsigned int& pr, const unsigned int& pg, const unsigned int& pb);
	static void GetBPixel(unsigned char* p, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a, const unsigned int& pr, const unsigned int& pg, const unsigned int& pb, const unsigned int& pa);

	/// RGBA
	static void GetAPixelRGBA(unsigned char* p, unsigned char& r, unsigned char& g, unsigned char& b);
	static void GetBPixelRGBA(unsigned char* p, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a);
	/// RGB
	static void GetAPixelRGB(unsigned char* p, unsigned char& r, unsigned char& g, unsigned char& b);
	static void GetBPixelRGB(unsigned char* p, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a);

	/// ARGB
	static void GetAPixelARGB(unsigned char* p, unsigned char& r, unsigned char& g, unsigned char& b);
	static void GetBPixelARGB(unsigned char* p, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a);

	/// BGRA
	static void GetAPixelBGRA(unsigned char* p, unsigned char& r, unsigned char& g, unsigned char& b);
	static void GetBPixelBGRA(unsigned char* p, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a);

	// Table Of SetPixel By Format
	typedef void (*SetAPix)(unsigned char*, const unsigned int&);
	typedef void (*SetBPix)(unsigned char*, const unsigned char&, const unsigned char&, const unsigned char&, const unsigned char&);

	static const SetAPix	_setAPix[COLOR_FORMAT_COUNT];
	static const SetBPix	_setBPix[COLOR_FORMAT_COUNT];

	typedef void (*GetAPix)(unsigned char*, unsigned char&, unsigned char&, unsigned char&);
	typedef void (*GetBPix)(unsigned char*, unsigned char&, unsigned char&, unsigned char&, unsigned char&);
	static const GetAPix	_getAPix[COLOR_FORMAT_COUNT];
	static const GetBPix	_getBPix[COLOR_FORMAT_COUNT];

	// Init By Format (!! SHOULD BE CALL AT INIT)
	void InitByFormat();
	// Format
	unsigned char* _data;
	ColorFormat				_format;
	unsigned int			_size;
	unsigned int			_sizeLine;
	unsigned int			_bpp;
	unsigned int			_width, _height;
};
