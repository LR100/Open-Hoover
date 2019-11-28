#include "ColorTools.h"

void ColorTools::GenColorPalette(std::vector<ColorDef>& colors, const unsigned int & min, const unsigned int & max, const unsigned int & marge)
{
	unsigned int mi = min, ma = max;

	if (mi < 330)
		mi = 330;
	if (ma > 780)
		ma = 780;

	for (double lambda = mi; lambda < ma; lambda += marge)
	{
		ColorDef	color;
		//std::cout << "lambda:" << lambda << std::endl;
		WaveLengthToColor(lambda, color);
		//std::cout << color.ToString() << std::endl;
		colors.push_back(color);
	}
}

void ColorTools::WaveLengthToColor(const double & l, ColorDef& color)
{
	double t;

	if (l >= 330 && l < 380)
	{
		color.r = (unsigned char)((l - 330) / (380. - 330.) * 90.);
		color.b = (unsigned char)((l - 330.) / (380. - 330.) * 185.) + 70;
	}
	else if (l >= 380 && l < 440)
	{
		color.r = (unsigned char)(-(l - 440.) / (440. - 380.) * 90.);
		color.b = 255;
	}
	else if (l >= 440 && l < 490)
	{
		color.g = (unsigned char)((l - 440.) / (490. - 440.) * 255.);
		color.b = 255;
	}
	else if (l >= 490 && l < 510 )
	{
		color.g = 255;
		color.b = (unsigned char)(-(l - 510.) / (510. - 490.) * 255.);
	}
	else if (l >= 510 && l < 580)
	{
		color.r = (unsigned char)((l - 510.) / (580. - 510.) * 255.);
		color.g = 255;
	}
	else if (l >= 580 && l < 645)
	{
		color.r = 255;
		color.g = (unsigned char)(-(l - 645.) / (645. - 580.) * 255.);
	}
	else if (l >= 645 && l < 780)
		color.r = (unsigned char)(-(l - 780.) / (780. - 645.) * 255.);

}

