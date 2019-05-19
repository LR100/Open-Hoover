#ifndef _IIMAGE_H_
#define _IIMAGE_H_

#define IIMAGE_NB_FORMAT 6

#include <string>
#include <functional>

class IImage
{
public:
	virtual ~IImage() {};

	enum Format
	{
		DEFAULT = 0,
		RGBA = 1,
		ARGB = 2,
		ABGR = 3,
		BGRA = 4,
		RGB
	};

	static std::string			FormatToString(const IImage::Format& format);
	// With Format, Image functions should be set in an array of pointer 
	// Ex: If image is RGBA for SetPixel(x, y, color);
	/*  SetPixel(x, y, color)
	{
	_setPixel[_format](x, y, color);
	}
	--> To Avoid Switch of if case
	*/

	virtual void				Init(const unsigned int& width, const unsigned int& height, const Format& format) = 0;

	virtual const unsigned int&	GetWidth() const = 0;
	virtual const unsigned int&	GetHeight() const = 0;
	virtual void *				GetData() const = 0;

	virtual void				Clear() = 0;
	virtual void				SetPixel(const unsigned int& x, const unsigned int& y, const unsigned int& color) = 0;
	virtual void				SetPixel(const unsigned int& x, const unsigned int& y, const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a) = 0;
	virtual void				GetPixel(const unsigned int& x, const unsigned int& y, unsigned char& r, unsigned char& g, unsigned char& b) const = 0;
	virtual void				GetPixel(const unsigned int& x, const unsigned int& y, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) const = 0;
	virtual const Format&		GetFormat() = 0;

	virtual bool				Export(const std::string& path) const = 0;
};

class AImage : public IImage
{
public:

	AImage();
	virtual ~AImage();

	virtual void *				GetData() const { return (_data); };

	virtual void				Clear() override;
	virtual void				SetPixel(const unsigned int& x, const unsigned int& y, const unsigned int& color) override;
	virtual void				SetPixel(const unsigned int& x, const unsigned int& y, const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a) override;

	virtual void				GetPixel(const unsigned int& x, const unsigned int& y, unsigned char& r, unsigned char& g, unsigned char& b) const override;
	virtual void				GetPixel(const unsigned int& x, const unsigned int& y, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) const override;
	virtual const Format&		GetFormat() override;

protected:

	// SetPixel By Format
	/// RGBA
	static void SetAPixelRGBA(unsigned char *p, const unsigned int& color);
	static void SetBPixelRGBA(unsigned char *p, const unsigned char& r,  const unsigned char& g, const unsigned char& b, const unsigned char& a);
	/// RGB
	static void SetAPixelRGB(unsigned char *p, const unsigned int& color);
	static void SetBPixelRGB(unsigned char *p, const unsigned char& r,  const unsigned char& g, const unsigned char& b, const unsigned char& a);

	/// ARGB
	static void SetAPixelARGB(unsigned char *p, const unsigned int& color);
	static void SetBPixelARGB(unsigned char *p, const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a);

	// GetPixel By Format
	static void GetAPixel(unsigned char *p, unsigned char& r, unsigned char& g, unsigned char& b, const unsigned int& pr, const unsigned int& pg, const unsigned int& pb);
	static void GetBPixel(unsigned char *p, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a, const unsigned int& pr, const unsigned int& pg, const unsigned int& pb, const unsigned int& pa);
	
	/// RGBA
	static void GetAPixelRGBA(unsigned char *p, unsigned char& r, unsigned char& g, unsigned char& b);
	static void GetBPixelRGBA(unsigned char *p, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a);
	/// RGB
	static void GetAPixelRGB(unsigned char *p, unsigned char& r, unsigned char& g, unsigned char& b);
	static void GetBPixelRGB(unsigned char *p, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a);

	/// ARGB
	static void GetAPixelARGB(unsigned char *p, unsigned char& r, unsigned char& g, unsigned char& b);
	static void GetBPixelARGB(unsigned char *p, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a);

	/// BGRA
	static void GetAPixelBGRA(unsigned char *p, unsigned char& r, unsigned char& g, unsigned char& b);
	static void GetBPixelBGRA(unsigned char *p, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a);

	// Table Of SetPixel By Format
	typedef void (*SetAPix)(unsigned char *, const unsigned int&);
	typedef void (*SetBPix)(unsigned char *, const unsigned char&, const unsigned char&, const unsigned char&, const unsigned char&);

	static const SetAPix	_setAPix[IIMAGE_NB_FORMAT];
	static const SetBPix	_setBPix[IIMAGE_NB_FORMAT];

	typedef void (*GetAPix)(unsigned char *, unsigned char&, unsigned char&, unsigned char&);
	typedef void (*GetBPix)(unsigned char *, unsigned char&, unsigned char&, unsigned char&, unsigned char&);
	static const GetAPix	_getAPix[IIMAGE_NB_FORMAT];
	static const GetBPix	_getBPix[IIMAGE_NB_FORMAT];

	// Init By Format (!! SHOULD BE CALL AT INIT)
	void InitByFormat();
	// Format
	unsigned char			*_data;
	IImage::Format			_format;
	unsigned int			_size;
	unsigned int			_sizeLine;
	unsigned int			_bpp;
	unsigned int			_width, _height;
};

#endif /* !_IIMAGE_H_ */

