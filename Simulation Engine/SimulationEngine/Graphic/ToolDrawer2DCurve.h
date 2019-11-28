#ifndef _TOOLDRAWER2CURVE_H_
#define _TOOLDRAWER2CURVE_H_

#include "IDrawer2D.h"
//#include "../Maths/MyMathRandom.h" 
//#include "../Maths/ValueTools.h"


#include <vector>
#include <iomanip>
#include <sstream>
#include <map>
#include <unordered_map>

class ToolDrawer2DCurve
{
public:

	typedef enum CurveDrawMode : uint16_t
	{
		NORMAL = 0,
		DEGRADE = 1,
		WITHPOINT = 2,
		ENDMODE
	};

	ToolDrawer2DCurve(const size_t& width, const size_t& height, std::string title = "");
	virtual ~ToolDrawer2DCurve() {};

	void Draw(IDrawer2D *drawer, const size_t& xPos, const size_t& yPos);
	void Resize(const size_t& width, const size_t& height);

	void Reset();
	void SetPoints(const std::vector<Vec2>& points);
	void SetTitle(const std::string& title);

	void					SetDrawMode(CurveDrawMode mode);
	const CurveDrawMode&	GetDrawMode() const;
	void SetMinMaxY(const float& minY, const float& maxY);


	Color*						colorCurve;
	Color*						colorLegend;
	Color*						colorTitle;

protected:

	void DrawModeNormal(IDrawer2D* drawer, const size_t& xB, const size_t& yB, const size_t& xE, const size_t& yE);
	void DrawModeDegrade(IDrawer2D* drawer, const size_t& xB, const size_t& yB, const size_t& xE, const size_t& yE);
	void DrawModeWithPoint(IDrawer2D* drawer, const size_t& xB, const size_t& yB, const size_t& xE, const size_t& yE);
	void DrawLegend(IDrawer2D *drawer, const size_t& xPos, const size_t& yPos);

	Vec2						_min;
	Vec2						_max;
	Vec2						_scale;
	size_t						_padX, _padY;
	std::vector<Vec2>			_points;
	size_t						_width, _height;
	std::string					_title;

	CurveDrawMode				_drawMode;

	typedef void (ToolDrawer2DCurve::*DrawModeFunction)(IDrawer2D* drawer, const size_t&, const size_t&, const size_t&, const size_t&);
	std::unordered_map<CurveDrawMode, DrawModeFunction>	_drawModeFunctions;
};
#endif /* !_TOOLDRAWER2CURVE_H_ */
