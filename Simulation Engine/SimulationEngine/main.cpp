

#include "QuadTree.hpp"

#include "Graphic/WindowSDL.h"
#include "Graphic/Drawer2DSDL.h"
#include "Events/EventHandlerSDL.h"

void DebugDrawQuadTreeAABB(const QuadTreeAABB2::Node* node, IWindow* window, IDrawer2D* drawer, Color color)
{
	size_t x, y;
	size_t w, h;


	DList<QuadTreeAABB2::Node::Index*>::Node* cursor;

	if (node->GetIndexes().GetSize())
	{
		for (cursor = node->GetIndexes().GetFront(); cursor; cursor = cursor->next)
		{

			x = cursor->data->GetAABB().center.x;
			y = cursor->data->GetAABB().center.y;
			w = cursor->data->GetAABB().halfSize.x * 2;
			h = cursor->data->GetAABB().halfSize.y * 2;
			x -= w / 2;
			y -= h / 2;

			drawer->DrawBorder(x, y, w, h, Color::RED(), Color::RED());
		}
	}

	x = node->GetBoundary().center.x;
	y = node->GetBoundary().center.y;
	w = node->GetBoundary().halfSize.x * 2;
	h = node->GetBoundary().halfSize.y * 2;
	x -= w / 2;
	y -= h / 2;

	//if (w < 5 || h < 5)
	//	return;
	if (node->GetBoundary().max.x < 0.1f || node->GetBoundary().max.y < 0.1f)
		return;
	if (node->GetBoundary().min.x >= window->GetWidth() || node->GetBoundary().min.y >= window->GetHeight())
		return;

	if (node->GetChildren()[0] != NULL)
	{
		DebugDrawQuadTreeAABB(node->GetChildren()[0], window, drawer, color);
		DebugDrawQuadTreeAABB(node->GetChildren()[1], window, drawer, color);
		DebugDrawQuadTreeAABB(node->GetChildren()[2], window, drawer, color);
		DebugDrawQuadTreeAABB(node->GetChildren()[3], window, drawer, color);
	}

	if (node->GetBoundary().max.x >= window->GetWidth() || node->GetBoundary().max.y >= window->GetHeight())
		return;



	drawer->DrawBorder(x, y, w, h, color, color);
}

#undef main // Because Of fckg SDL
//
//int main()
//{
//	AABB2			maxTree(Vec2(500, 500), Vec2(1000, 10000));
//	QuadTreeAABB2	quadTree(maxTree, 20);
//	AABB2			lol(Vec2(110, 110), Vec2(20, 20));
//
//	IEventHandler*	eventHandler = new EventHandlerSDL();
//	IWindow*		window = new WindowSDL(eventHandler, 1800, 1000, "TEST");
//	IDrawer2D*		drawer = new Drawer2DSDL();
//	drawer->SetCurrentImage(window->GetBackBuffer());
//
//
//	QuadTreeAABB2::Node::Index* index;
//
//
//
//	index = quadTree.InsertData(lol, lol);
//	for (size_t i = 0; i < 20; i += 1)
//	{
//		drawer->ClearImage();
//		lol.center.x += 100;
//		lol.ComputeMinMax();
//		index->RelocateFromBot();
//		DebugDrawQuadTreeAABB(quadTree.GetRoot(), window, drawer, Color::BLUE());
//		window->Refresh();
//		std::cout << "Refresh" << std::endl;
//	}
//
//
//
//
//	return (0);
//}




#include "RobovacSimulation.h"

int main()
{
	RobovacSimulation rs;

	rs.Launch();

	return (0);
}
















///////////////////////////////////////////////////////////////
//// TESSSSSSSSSSSSSSSSSSSSSSSSTTTTTTTTTTTTTTTTTTTTTTT MAIN //

//#include "Tools/XMLParser.h" // TMP FOR TESTING
//
//#include <Windows.h>
//#include <iostream>
//
////#include "Tools/LinkedList.hpp"
//
////#include "Tools/MyTimer.h"
//
//#include "Maths/XML/AABBXML.h"

//AABB2	box(Vec2(500, 500), Vec2(500, 500));
//
//XMLNode* node = AABB2TXML::Export(box);
//std::cout << node->ToString() << std::endl;
//
//system("pause");
//
//
////Timer		timer;
////XMLParser	xmlParser;
////XMLNode*	xmlTree;
////std::string file = "world.xml";
//
////timer.SetElapsedTime();
////xmlParser.Parse(file);
////xmlTree = xmlParser.GetTree();
////timer.SetElapsedTime();
////std::cout << "File (" << file << ") Parsed in (" << timer.GetElapsedSecond() << ")s" << std::endl;
////system("pause");
////std::cout << xmlTree->ToString() << std::endl;
//
//////system("pause");
//
////unsigned short nth = 0;
////unsigned short depthMax = 400;
//
////std::string name = "Vec2";
//
////XMLNode*	xmlNode = xmlTree->GetNodeNamed(name, nth);
//
////timer.SetElapsedTime();
////while (xmlNode)
////{
//
////	std::cout << "Found Node (" << name << ") at Nth (" << nth << ") with depthMax of (" << depthMax << ")" << std::endl << xmlNode->ToString() << std::endl;
////	//nth += 1;
////	xmlNode->Remove();
////	xmlNode = xmlTree->GetNodeNamed(name);
//
////}
//
////timer.SetElapsedTime();
////std::cout << "File (" << file << ") Searched  in (" << timer.GetElapsedSecond() << ")s" << std::endl;
////system("pause");
//
///*LinkedList<size_t>	list;
//size_t sList = 400;
//
//for (size_t i = 0; i < sList; i += 1)
//{
//	list.PushBack(i);
//}
//
//for (size_t e = 0; e < 20; e += 1)
//{
//
//	LinkedList<size_t>::Node* cursor = list.GetFront();
//	std::cout << "List Size (" << list.GetSize() << ")" << std::endl;
//	std::cout << "Cursor (" << cursor << ")" << std::endl;
//	while (cursor)
//	{
//		std::cout << cursor->GetData() << std::endl;
//		if (rand() % 2 && cursor)
//			cursor = cursor->Remove();
//		if (cursor)
//			cursor = cursor->GetNext();
//	}
//
//}
//*/
//system("pause");
