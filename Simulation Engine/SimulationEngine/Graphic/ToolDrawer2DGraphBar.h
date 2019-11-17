#ifndef _GRAPH_DRAWER_H_
#define _GRAPH_DRAWER_H_

#include <vector>


#define GRAPHABS(x) ((x < 0) ? -(x) : x)
///
// ABSTRACT - DRAWER2D GRAPH - Using IDrawer2D
// The name Drawer2D Graph isnt good cauz it could let think that GraphPainter is a Drawer but its not
///

#include "IDrawer2D.h"

class ToolDrawer2DGraph
{
public:
	ToolDrawer2DGraph(const size_t& width, const size_t& height);
	virtual ~ToolDrawer2DGraph() {};


	virtual void Draw(IDrawer2D *drawer, const size_t& xPos, const size_t& yPos);

	template <typename T>
	void SetValues(T* arr, const size_t& size);
	template <typename T>
	void SetValue(T& value, const size_t& pos);
	void ResetValues();

	void SetDimension(const size_t & width, const size_t & height);
	template <typename T>
	void SetMinMaxValue(T& min, T& max);

protected:

	virtual void UpdateMinMax() = 0;
	virtual void UpdateResetValues() = 0;
	virtual void UpdateValues() = 0;
	virtual void UpdateValue(double& value, const size_t& pos) = 0;

	virtual void DrawGraph(IDrawer2D *drawer, const size_t& xPos, const size_t& yPos) = 0;
	virtual void DrawLegend(IDrawer2D *drawer, const size_t& xPos, const size_t& yPos) = 0;

	std::vector<double>					_values; // Use Double to be sure that values will be handle correctly

	size_t				_width, _height;
	double				_max, _min;
};


///
// - BAR DRAWER
///

class ToolDrawer2DGraphBar : public ToolDrawer2DGraph
{
public:
	ToolDrawer2DGraphBar(const size_t& width, const size_t& height);
	~ToolDrawer2DGraphBar();

	struct BarInfo
	{
		size_t	_pos;
		size_t	_height;
		size_t	_startH;
		float	_scaleH;
		size_t	_color;
	};

	void SetColorBars(const size_t& color);
	void SetColorBar(const size_t& pos, const size_t& color);
	void SetDefaultColorBar(const size_t&pos);

	// Hérité via ToolDrawer2DGraph
	virtual void Draw(IDrawer2D *drawer, const size_t& xPos, const size_t& yPos) override;

private:

	// Hérité via ToolDrawer2DGraph
	virtual void UpdateMinMax() override;
	virtual void UpdateResetValues() override;
	virtual void UpdateValues() override;
	virtual void UpdateValue(double& value, const size_t& pos) override;

	
	virtual void DrawGraph(IDrawer2D *drawer, const size_t& xPos, const size_t& yPos) override;
	virtual void DrawLegend(IDrawer2D *drawer, const size_t& xPos, const size_t& yPos) override;

	virtual void CleanBar(IDrawer2D *drawer, const size_t& xPos, const size_t& yPos, const BarInfo*);
	virtual void DrawBar(IDrawer2D *drawer, const size_t& xPos, const size_t& yPos, BarInfo*);
	virtual void UpdateBarsToDraw();


	void UpdateScaleBars();
	size_t									_colorBars;
	std::vector<BarInfo>					_barsInfo;
	std::vector<BarInfo*>					_barsToDraw;
	bool									_needClean;
	float									_scaleW, _scaleH;
	double									_diffMM;
};

#endif /* ! _GRAPH_DRAWER_H_ */

template<typename T>
inline void ToolDrawer2DGraph::SetValues(T * arr, const size_t & size)
{
	if (size > _width)
		return;
	ResetValues();
	for (size_t inc = 0; inc < size; inc++)
		_values.push_back(double(arr[inc]));
	UpdateValues();
}

template<typename T>
inline void ToolDrawer2DGraph::SetValue(T & value, const size_t & pos)
{
	if (pos < _values.size())
	{
		_values[pos] = (double(value));
		UpdateValue(_values[pos], pos);
	}
}

template<typename T>
inline void ToolDrawer2DGraph::SetMinMaxValue(T & min, T & max)
{
	_max = max;
	_min = min;
	UpdateMinMax();
}

