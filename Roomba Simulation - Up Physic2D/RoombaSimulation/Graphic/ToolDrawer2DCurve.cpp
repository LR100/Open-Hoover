#include "ToolDrawer2DCurve.h"

ToolDrawer2DCurve::ToolDrawer2DCurve(const size_t & width, const size_t & height, std::string title)
{
	_drawModeFunctions[NORMAL] = &ToolDrawer2DCurve::DrawModeNormal;
	_drawModeFunctions[DEGRADE] = &ToolDrawer2DCurve::DrawModeDegrade;
	_drawModeFunctions[WITHPOINT] = &ToolDrawer2DCurve::DrawModeWithPoint;
	_drawMode = WITHPOINT;
	_min.x = 0;
	_min.y = 0;
	_max.x = 1;
	_max.y = 1;
	_title = title;
	colorLegend = Color(40, 145, 245);
	colorCurve = Color(170, 25, 95);
	colorTitle = Color(120, 200, 220);
	Resize(width, height);
}

void ToolDrawer2DCurve::Draw(IDrawer2D * drawer, const size_t & xPos, const size_t & yPos)
{
	DrawLegend(drawer, xPos, yPos);
	if (_points.size() == 0)
		return;

	int xB, yB, xE, yE;
	int hpx, hpy;
	hpx = _padX / 2;
	hpy = _padY / 2;
	DrawModeFunction	fct = _drawModeFunctions[_drawMode];

	xB = ((_points.at(0).x - _min.x) * _scale.x) + hpx;
	yB = (_height - _padY) - ((_points.at(0).y - _min.y) * _scale.y) + hpy;

	size_t offsetX = (size_t)((float)_points.size() / (float)_width);
	if (offsetX == 0)
		offsetX = 1;

	for (size_t inc = 1; inc < _points.size(); inc += offsetX)
	{
		xE = ((_points.at(inc).x - _min.x) * _scale.x) + hpx;
		yE = (_height - _padY) - ((_points.at(inc).y - _min.y) * _scale.y) + hpy;

		(this->*fct)(drawer, xB + xPos, yB + yPos, xE + xPos, yE + yPos);
		xB = xE;
		yB = yE;
	}
}

void ToolDrawer2DCurve::Reset()
{
}

void ToolDrawer2DCurve::SetTitle(const std::string & title)
{
	_title = title;
}

void ToolDrawer2DCurve::SetPoints(const std::vector<Vec2>& points)
{
	if (!points.size())
		return;
	_points = points;
	_min.x = points.at(0).x;
	_max.x = points.at(0).x;
	_min.y = points.at(0).y;
	_max.y = points.at(0).y;
	for (size_t inc = 1; inc < points.size(); inc++)
	{
		if (points.at(inc).x < _min.x)
			_min.x = points.at(inc).x;
		else if (points.at(inc).x > _max.x)
			_max.x = points.at(inc).x;

		if (points.at(inc).y < _min.y)
			_min.y = points.at(inc).y;
		else if (points.at(inc).y > _max.y)
			_max.y = points.at(inc).y;
	}
	_scale.x = (float)(_width - _padX) / (_max.x - _min.x);
	_scale.y = (float)(_height - _padY) / (_max.y - _min.y);
}

void ToolDrawer2DCurve::Resize(const size_t & width, const size_t & height)
{
	_width = width;
	_height = height;
	_padX = _width / 8;
	_padY = _height / 8;
	_scale.x = (float)(_width - _padX) / (_max.x - _min.x);
	_scale.y = (float)(_height - _padY) / (_max.y - _min.y);

}

void ToolDrawer2DCurve::SetDrawMode(CurveDrawMode mode)
{
	_drawMode = mode;
}

const ToolDrawer2DCurve::CurveDrawMode & ToolDrawer2DCurve::GetDrawMode() const
{
	return (_drawMode);
}

void ToolDrawer2DCurve::SetMinMaxY(const float & minY, const float & maxY)
{
	_max.y = maxY;
	_min.y = minY;
	_scale.y = (float)(_height - _padY) / (_max.y - _min.y);
}

void ToolDrawer2DCurve::DrawModeNormal(IDrawer2D* drawer, const size_t & xB, const size_t & yB, const size_t & xE, const size_t & yE)
{
	drawer->DrawLine(xB, yB, xE, yE, colorCurve);
}

void ToolDrawer2DCurve::DrawModeDegrade(IDrawer2D* drawer, const size_t & xB, const size_t & yB, const size_t & xE, const size_t & yE)
{
	drawer->DrawLine(xB, yB, xE, yE, Color(colorCurve.r - ((yB - yE) % 255), colorCurve.g - ((yB - yE) % 255), colorCurve.b - ((yB - yE) % 255)));
}

void ToolDrawer2DCurve::DrawModeWithPoint(IDrawer2D * drawer, const size_t & xB, const size_t & yB, const size_t & xE, const size_t & yE)
{
	drawer->DrawLine(xB, yB, xE, yE, colorCurve);
	drawer->DrawCircleFill(xB, yB, 1, Color(200, 200, 255));
	drawer->DrawCircleFill(xE, yE, 1, Color(200, 200, 255));
}

void ToolDrawer2DCurve::DrawLegend(IDrawer2D * drawer, const size_t & xPos, const size_t & yPos)
{
	float hpx, hpy;
	hpx = _padX / 2;
	hpy = _padY / 2;
	size_t sfont = (hpy + hpx) / 10;
	std::stringstream ss;

	drawer->DrawRectFill(xPos, yPos, _width, _height, 1, Color(10, 10, 45));
	drawer->DrawBorder(xPos, yPos, _width, _height, Color(200, 200, 200), Color(150, 150, 190));


	if (_title.size())
	{
		ss.str("");
		ss << std::fixed << _title;
		drawer->LoadText(ss.str(), sfont * 1.5, "pixel.ttf", colorTitle);
		drawer->DrawText(xPos + _width / 3, yPos);
	}

	drawer->DrawLine(hpx + xPos, hpy + yPos, hpx + xPos, (_height - hpy) + yPos, Color(255, 255, 255));
	drawer->DrawLine(hpx + xPos, (_height - hpy) + yPos, (_width - hpx) + xPos, (_height - hpy) + yPos, Color(255, 255, 255));

	
	ss.str("");
	float hpxTextLeft = hpx / 10.0f;

	ss << std::fixed << std::setprecision(1) << _max.y;
	drawer->DrawLine(hpx / 1.5f + xPos, hpy + yPos, hpx + xPos, hpy + yPos, Color(255, 255, 255));
	drawer->LoadText(ss.str(), sfont, "pixel.ttf", colorLegend);
	drawer->DrawText(xPos + hpxTextLeft, hpy / 1.3 + yPos);

	ss.str("");
	ss << std::fixed << std::setprecision(1) << _min.y;
	drawer->DrawLine(hpx / 1.5f + xPos, (_height - hpy) + yPos, hpx + xPos, (_height - hpy) + yPos, Color(255, 255, 255));
	drawer->LoadText(ss.str(), sfont, "pixel.ttf", colorLegend);
	drawer->DrawText(xPos + hpxTextLeft,  (_height - hpy * 1.2) + yPos);

	ss.str("");
	ss << std::fixed << std::setprecision(1) << _min.x;
	drawer->DrawLine(hpx + xPos, (_height - hpy) + yPos, hpx + xPos, (_height - hpy + (hpy / 3.0f)) + yPos, Color(255, 255, 255));
	drawer->LoadText(ss.str(), sfont, "pixel.ttf", colorLegend);
	drawer->DrawText(hpx / 1.5f + xPos, (_height - hpy + (hpy / 3.0f)) + yPos);

	ss.str("");
	ss << std::fixed << std::setprecision(1) << _max.x;
	drawer->DrawLine((_width - hpx) + xPos, (_height - hpy) + yPos, (_width - hpx) + xPos, (_height - hpy + (hpy / 3.0f)) + yPos, Color(255, 255, 255));
	drawer->LoadText(ss.str(), sfont, "pixel.ttf", colorLegend);
	drawer->DrawText((_width - hpx) - (hpx / 1.5f) + xPos, (_height - hpy + (hpy / 3.0f)) + yPos);
}