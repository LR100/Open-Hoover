#ifndef _COLORSTOOLS_H_
#define _COLORSTOOLS_H_


#include <vector>
#include "Color.h"

class ColorTools
{
public:
	virtual ~ColorTools() {};

	static void GenColorPalette(std::vector<Color>& colors, const unsigned int& min, const unsigned int& max, const unsigned int& marge);
private:
	static void WaveLengthToColor(const double& lambda, Color& color);

};


#endif /* !_COLORSTOOLS_H_ */
