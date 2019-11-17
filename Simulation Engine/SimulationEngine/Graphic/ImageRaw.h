#ifndef _IMAGERAW_H_
#define _IMAGERAW_H_

#include "IImage.h"

class ImageRaw : public	AImage	
{
public:
	ImageRaw();
	~ImageRaw();

	virtual void				Init(const unsigned int & width, const unsigned int & height, const Format & format) override;
	virtual const unsigned int& GetWidth() const override;
	virtual const unsigned int& GetHeight() const override;

private:
	virtual void InitByFormat(const Format & format);

	unsigned int			_bpp;
	unsigned int			_size;
	unsigned int			_sizeLine;
	unsigned int			_width , _height;
};


#endif /* !_IMAGERAW_H_ */
