#pragma once

#define BIT_SET(a, pos) ((a) |= (1ULL << (pos)))
#define BIT_CLEAR(a,pos) ((a) &= ~(1ULL<<(pos)))
#define BIT_FLIP(a,pos) ((a) ^= (1ULL<<(pos)))
#define BIT_CHECK(a,pos) (!!((a) & (1ULL<<(pos)))) 

class BoolArray
{
public:

	BoolArray(const uint8_t& width, const uint8_t& height, bool valueInit = false) {
		_width = width;
		_height = height;
		Allocate();
		Init(valueInit);
	}
#ifdef _WIN32

	std::string	ToString() {
		std::stringstream	ss;
		bool				state;
		for (uint8_t h = 0; h < _height; h += 1) {
			for (uint8_t w = 0; w < _width; w += 1) {

				state = Get(w, h);
				if (state) {
					ss << "1";
				}
				else {
					ss << "0";
				}
				if (w < (_width - 1))
					ss << " ";
			}
			ss << "\n";
		}
		return (ss.str());
	}

#endif // _WIN32

	void	Set(const uint8_t& x, const uint8_t& y, bool state) {
		if (!PosIsValid(x, y))
			return;
		float		posByteF = std::roundf(((float)((float)y * (float)_width) + (float)x));
		uint16_t	posByte = (uint16_t)posByteF;
		uint8_t		posBit = (uint8_t)(posByte % 8);
		posByte = (uint16_t)(posByteF / 8.0f);
		if (state)
			BIT_SET(_array[posByte], posBit);
		else
			BIT_CLEAR(_array[posByte], posBit);
	}

	bool	Get(const uint8_t& x, const uint8_t& y) {
		if (!PosIsValid(x, y))
			return (false);
		float		posByteF = std::roundf(((float)((float)y * (float)_width) + (float)x));
		uint16_t	posByte = (uint16_t)posByteF;
		uint8_t		posBit = (uint8_t)(posByte % 8);
		posByte = (uint16_t)(posByteF / 8.0f);
		return (BIT_CHECK(_array[posByte], posBit));
	}

	const uint8_t& GetWidth() const { return (_width); };
	const uint8_t& GetHeight() const { return (_height); };

	bool			PosIsValid(const uint8_t& x, const uint8_t& y)
	{
		if (x >= _width || y >= _height)
			return (false);
		return (true);
	}

	void			Set(bool value)
	{
		for (uint8_t h = 0; h < _height; h += 1) {
			for (uint8_t w = 0; w < _width; w += 1) {
				Set(w, h, value);
			}
		}
	}

private:

	void	Allocate() {
		uint16_t	size = (uint16_t)((uint16_t)_width * (uint16_t)_height);
		float		fsize = std::ceilf((float)size / 8.0f); // To be replaced on Arduino (std::ceilf)
		size = (uint16_t)fsize;
		_array = new uint8_t[size];
	}

	void	Init(const bool& valueInit) {
		Set(valueInit);
	}

	uint8_t* _array;
	uint8_t		_width;
	uint8_t		_height;
};
