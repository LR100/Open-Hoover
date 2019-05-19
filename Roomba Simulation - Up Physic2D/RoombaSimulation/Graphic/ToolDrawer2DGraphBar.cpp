#include <iostream>
#include "ToolDrawer2DGraphBar.h"

ToolDrawer2DGraph::ToolDrawer2DGraph(const size_t & width, const size_t & height)
{
	_width = width;
	_height = height;
	_min = 0;
	_max = 1;
}

void ToolDrawer2DGraph::ResetValues()
{
	_values.clear();
	UpdateResetValues();
}

void ToolDrawer2DGraph::SetDimension(const size_t & width, const size_t & height)
{
	_width = width;
	_height = height;
	if (_width < 1)
		_width = 1;
	if (_height < 1)
		_height = 1;
	UpdateMinMax();
	UpdateValues();
}

void ToolDrawer2DGraph::Draw(IDrawer2D * drawer, const size_t& xPos, const size_t& yPos)
{
	DrawGraph(drawer, xPos, yPos);
	DrawLegend(drawer, xPos, yPos);
}

ToolDrawer2DGraphBar::ToolDrawer2DGraphBar(const size_t & width, const size_t & height) : ToolDrawer2DGraph(width, height)
{
	_needClean = false;
	_diffMM = 1;
	_scaleH = 1;
	_scaleW = 1;
}

ToolDrawer2DGraphBar::~ToolDrawer2DGraphBar()
{
}

void ToolDrawer2DGraphBar::SetColorBars(const size_t & color)
{
	_colorBars = color;
	for (size_t inc = 0; inc < _barsInfo.size(); inc++)
		_barsInfo[inc]._color = color;
}

void ToolDrawer2DGraphBar::SetColorBar(const size_t & pos, const size_t & color)
{
	if (pos >= _barsInfo.size())
		return;
	_barsInfo[pos]._color = color;
	_barsToDraw.push_back(&_barsInfo[pos]);
}

void ToolDrawer2DGraphBar::SetDefaultColorBar(const size_t & pos)
{
	if (pos >= _barsInfo.size())
		return;
	_barsInfo[pos]._color = _colorBars;
	_barsToDraw.push_back(&_barsInfo[pos]);
}

void ToolDrawer2DGraphBar::UpdateMinMax()
{
	_diffMM = ((GRAPHABS(_max)) - (GRAPHABS(_min)));
	_scaleH = (float)((float)(_height) / (float)_diffMM);
}

void ToolDrawer2DGraphBar::UpdateResetValues()
{
	_barsInfo.clear();
	_barsToDraw.clear();
}

void ToolDrawer2DGraphBar::UpdateValues()
{
	BarInfo tmpBar;

	if (_values.size())
		_scaleW = (float)((float)(_width) / (float)(_values.size()));
	else
		_scaleW = 1;
	for (size_t inc = 0; inc < _values.size(); inc++)
	{
		tmpBar._color = _colorBars;
		tmpBar._scaleH = (float)(((float)(_values[inc]) + _min) / _diffMM);
		tmpBar._height = tmpBar._scaleH * (float)(_height);
		tmpBar._startH = (_height - tmpBar._height);
		tmpBar._pos = inc;
		_barsInfo.push_back(tmpBar);
	}
	UpdateBarsToDraw();
}

void ToolDrawer2DGraphBar::UpdateValue(double & value, const size_t & pos)
{
	if (pos >= _barsInfo.size())
		return;
	//std::cout << "UPDATE VALUE: " << value << "At pos:" << pos << std::endl;
	if (value < _min)
	{
		//std::cout << "Update Min" << std::endl;
		//std::cout << "UPDATE VALUE: " << value << "At pos:" << pos << std::endl;
		_min = value;
		UpdateMinMax();
		UpdateScaleBars();
	}
	else if (value > _max)
	{
		//std::cout << "Update Max" << std::endl;
		//std::cout << "UPDATE VALUE: " << value << "At pos:" << pos << std::endl;
		_max = value;
		UpdateMinMax();
		UpdateScaleBars();
	}
	
	_barsInfo.at(pos)._scaleH = (float)(((float)(_values[pos]) + _min) / _diffMM);
	_barsInfo.at(pos)._height = _barsInfo.at(pos)._scaleH * (float)(_height);
	_barsInfo.at(pos)._startH = (_height - _barsInfo.at(pos)._height);
	_barsToDraw.push_back(&_barsInfo.at(pos));
}

void ToolDrawer2DGraphBar::Draw(IDrawer2D * drawer, const size_t & xPos, const size_t & yPos)
{
	if (_needClean)
	{
		drawer->ClearImage();
		_needClean = false;
	}
	ToolDrawer2DGraph::Draw(drawer, xPos, yPos);
	_barsToDraw.clear();
}

void ToolDrawer2DGraphBar::DrawGraph(IDrawer2D * drawer, const size_t& xPos, const size_t& yPos)
{
	
	size_t	incW = 0;
	size_t	x, y;
	size_t	zero = 0;

	for (size_t inc = 0; inc < _barsToDraw.size(); inc++)
	{
		CleanBar(drawer, xPos, yPos, _barsToDraw[inc]);
		DrawBar(drawer, xPos, yPos, _barsToDraw[inc]);
	}
}

void ToolDrawer2DGraphBar::DrawLegend(IDrawer2D * drawer, const size_t& xPos, const size_t& yPos)
{
}

void ToolDrawer2DGraphBar::CleanBar(IDrawer2D * drawer, const size_t & xPos, const size_t & yPos, const BarInfo* bar)
{
	size_t x = (_scaleW * (float)bar->_pos) + xPos;
	size_t y = yPos;
	size_t w = _scaleW;
	size_t zero = 0;

	drawer->DrawRect(x, y, w, _height, zero);
	drawer->DrawBorder(x, y, w, _height, zero, zero);
}

void ToolDrawer2DGraphBar::DrawBar(IDrawer2D * drawer, const size_t & xPos, const size_t & yPos, BarInfo* bar)
{
	size_t	tmpW, color;
	size_t x = (_scaleW * (float)bar->_pos) + xPos;
	size_t y = bar->_startH + yPos;

	tmpW = _scaleW;
	drawer->DrawRect(x, y, tmpW, bar->_height, bar->_color);
	color = 0xF0F0F0F0;
	drawer->DrawBorder(x, y, tmpW, bar->_height, color, color);
}

void ToolDrawer2DGraphBar::UpdateBarsToDraw()
{
	_barsToDraw.clear();
	for (size_t inc = 0; inc < _barsInfo.size(); inc++)
		_barsToDraw.push_back(&_barsInfo[inc]);
}

void ToolDrawer2DGraphBar::UpdateScaleBars()
{
	_needClean = true;
	for (size_t inc = 0; inc < _values.size(); inc++)
	{
		_barsInfo[inc]._scaleH = (float)((float)((float)(_values[inc]) + (float)(_min)) / (float)(_diffMM));
		_barsInfo[inc]._height = _barsInfo[inc]._scaleH * (float)(_height);
		_barsInfo[inc]._startH = (_height - _barsInfo[inc]._height);
	}
}
