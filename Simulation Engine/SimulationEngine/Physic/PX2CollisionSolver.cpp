#include "PX2CollisionSolver.h"

#include "../Maths/ValueTools.h"
#include "../Maths/VectorTransformer.h"
#include "../Maths/IntersectionComputer.h"


#include <algorithm>



#include "PX2DebugDrawer.h"

#include <sstream> // TMP
#include <chrono> // TMP
#include <thread> // TMP

PX2CollisionSolver::PX2CollisionSolver(PX2CollisionListener * collisionListener)
{
	// !! Collision Listener !!
	_collisionListener = collisionListener;

	// Type Move
	_testTypeMoveFunctions[0] = &PX2CollisionSolver::TestSS;
	_testTypeMoveFunctions[1] = &PX2CollisionSolver::TestDS;
	_testTypeMoveFunctions[2] = &PX2CollisionSolver::TestDD;

	_resolveTypeMoveFunctions[1] = &PX2CollisionSolver::ResolveDS;
	_resolveTypeMoveFunctions[2] = &PX2CollisionSolver::ResolveDD;

	// Dynamic VS Dynamic
	/// Fast
	_testFastDD[PX2Shape::TYPE::CIRCLE][PX2Shape::TYPE::CIRCLE] = &PX2CollisionSolver::TestFastCDCD;
	_resolveFastDD[PX2Shape::TYPE::CIRCLE][PX2Shape::TYPE::CIRCLE] = &PX2CollisionSolver::ResolveFastCDCD;
	_testFastDD[PX2Shape::TYPE::CIRCLE][PX2Shape::TYPE::BOX] = &PX2CollisionSolver::TestFastCDBD;
	_testFastDD[PX2Shape::TYPE::BOX][PX2Shape::TYPE::CIRCLE] = &PX2CollisionSolver::TestFastCDBD;
	_testFastDD[PX2Shape::TYPE::BOX][PX2Shape::TYPE::BOX] = &PX2CollisionSolver::TestFastBDBD;
	/// Slow
	_testSlowDD[PX2Shape::TYPE::CIRCLE][PX2Shape::TYPE::CIRCLE] = &PX2CollisionSolver::TestSlowCDCD;
	_resolveSlowDD[PX2Shape::TYPE::CIRCLE][PX2Shape::TYPE::CIRCLE] = &PX2CollisionSolver::ResolveSlowCDCD;
	_testSlowDD[PX2Shape::TYPE::CIRCLE][PX2Shape::TYPE::BOX] = &PX2CollisionSolver::TestSlowCDBD;
	_testSlowDD[PX2Shape::TYPE::BOX][PX2Shape::TYPE::CIRCLE] = &PX2CollisionSolver::TestSlowCDBD;
	_testSlowDD[PX2Shape::TYPE::BOX][PX2Shape::TYPE::BOX] = &PX2CollisionSolver::TestSlowBDBD;

	// Dynamic VS Static
	/// Fast
	_testFastDS[PX2Shape::TYPE::CIRCLE][PX2Shape::TYPE::CIRCLE] = &PX2CollisionSolver::TestFastCDCS;
	_testFastDS[PX2Shape::TYPE::CIRCLE][PX2Shape::TYPE::BOX] = &PX2CollisionSolver::TestSlowCDBS;
	_resolveFastDS[PX2Shape::TYPE::CIRCLE][PX2Shape::TYPE::BOX] = &PX2CollisionSolver::ResolveSlowCDBS;
	_testFastDS[PX2Shape::TYPE::BOX][PX2Shape::TYPE::CIRCLE] = &PX2CollisionSolver::TestFastBDCS;
	_testFastDS[PX2Shape::TYPE::BOX][PX2Shape::TYPE::BOX] = &PX2CollisionSolver::TestFastBDBS;
	/// Slow
	_testSlowDS[PX2Shape::TYPE::CIRCLE][PX2Shape::TYPE::CIRCLE] = &PX2CollisionSolver::TestSlowCDCS;
	_testSlowDS[PX2Shape::TYPE::CIRCLE][PX2Shape::TYPE::BOX] = &PX2CollisionSolver::TestSlowCDBS;
	_resolveSlowDS[PX2Shape::TYPE::CIRCLE][PX2Shape::TYPE::BOX] = &PX2CollisionSolver::ResolveSlowCDBS;
	_testSlowDS[PX2Shape::TYPE::BOX][PX2Shape::TYPE::CIRCLE] = &PX2CollisionSolver::TestSlowBDCS;
	_testSlowDS[PX2Shape::TYPE::BOX][PX2Shape::TYPE::BOX] = &PX2CollisionSolver::TestSlowBDBS;
}

PX2CollisionSolver::~PX2CollisionSolver()
{
}
// Debug Fonction
std::string GetDescriptionBody(PX2Body* body)
{
	std::string desc;

	if (body->GetShape()->GetType() == PX2Shape::TYPE::CIRCLE)
		desc = "C";
	else if (body->GetShape()->GetType() == PX2Shape::TYPE::BOX)
		desc = "B";
	else
		desc = "U"; // U For Unknown

	if (body->GetType() == PX2Body::TYPE::DYNAMIC)
		desc += "D";
	else if (body->GetType() == PX2Body::TYPE::STATIC)
		desc += "S";
	return (desc);
}

std::string GetDescriptionMove(PX2Manifold * manifold)
{
	if (manifold->A()->IsHighVelocity() || manifold->B()->IsHighVelocity())
		return ("Fast");
	return ("Slow");
}

void PX2CollisionSolver::Solve(const PX2ManifoldList& manifoldList, const float & dt)
{
	_collisionListener->Reset();
	const std::vector<PX2Manifold*>&	manifolds = manifoldList.GetManifoldSorted();

	if (!manifolds.size())
		return;

	////std::cout << "\n\n-- PX2CollisionSolver::Solve() :) --" << std::endl;

	// ////std::cout << "Possible Intersections: (" << manifolds.size() << ") For (" << _bodies.size() << ") Objects" << std::endl;

	size_t i = 0;
	/// NARROW PHASE (Set TOIs Of Collision) ///
	for (; i < manifolds.size(); i += 1)
	{
		/*if (manifolds.at(i)->Type())
	  //std::cout << "Test (" << GetDescriptionMove(manifolds.at(i)) << ") (" << GetDescriptionBody(manifolds.at(i)->A()) << ") vs (" << GetDescriptionBody(manifolds.at(i)->B()) << ")" << std::endl;*/
		(this->*_testTypeMoveFunctions[manifolds.at(i)->Type()])(manifolds.at(i), dt);
	}
	/// RESOLVE Collisions
	ResolveCollisions(dt);


	i = 0;
	for (; i < manifolds.size(); i += 1)
	{
		manifolds.at(i)->Release();
	}

}

void PX2CollisionSolver::ResolveCollisions(const float & dt)
{
	PX2ManifoldList::Node*	cursor;

	if (_collisionListener->GetCollisionCount())
	{

		////std::cout << "\n-ALL COLLISIONS-\n";
		const std::vector<PX2CollisionListener::CollisionInfo>& collisions = _collisionListener->GetCollisions();

		for (size_t i = 0; i < _collisionListener->GetBodyCountMax(); i += 1)
		{
			if (collisions.at(i).list.GetSize())
			{
				cursor = collisions.at(i).list.GetFront();

				////std::cout << "Body (" << i << ") Collide With" << std::endl;
				// Resolve At List First
				if (!cursor->data->resolved)
				{
					cursor->data->resolved = true; // RESOLVED //
					(this->*_resolveTypeMoveFunctions[cursor->data->Type()])(cursor->data, dt);
					// CORRECT POSITION
					//////std::cout << " A Pos (" << cursor->data->A()->GetPositionOld().ToString() << ") Vel (" << cursor->data->A()->GetVelocityLinear().ToString() << ")";
					//////std::cout << " B Pos (" << cursor->data->B()->GetPositionOld().ToString() << ") Vel (" << cursor->data->B()->GetVelocityLinear().ToString() << ")";
				}
				//cursor = cursor->next;
				//while (cursor && cursor->data->Type() < 2) // Not Dynamic / Dynamic (For which Collision Point is less important (because in constant movement)
				//{
				//	if (!cursor->data->resolved)
				//	{
				//		cursor->data->resolved = true; // RESOLVED //
				//		(this->*_resolveTypeMoveFunctions[cursor->data->Type()])(cursor->data, dt);
				//	}
				//	cursor = cursor->next;
				//}
			}


		}
		/*PX2Window->Refresh();

		////std::cout << "OkI" << std::endl;*/
	}
}

//void PX2CollisionSolver::Solve(PX2Manifold * manifold, const float & dt)
//{
//	(this->*_testTypeMoveFunctions[manifold->Type()])(manifold, dt);
//}

#include <iostream> // TMP




// MAIN HANDLE TYPE MOVE FUNCTIONS //
void PX2CollisionSolver::TestDD(PX2Manifold * manifold, const float & dt)
{
	//////std::cout << "Test Dynamic vs Dynamic" << std::endl
	SwapSleepingToA(manifold);

	if (manifold->A()->IsHighVelocity() || manifold->B()->IsHighVelocity())
		(this->*_testFastDD[manifold->A()->GetShape()->GetType()][manifold->B()->GetShape()->GetType()])(manifold, dt);
	else
		(this->*_testSlowDD[manifold->A()->GetShape()->GetType()][manifold->B()->GetShape()->GetType()])(manifold, dt);
}

void PX2CollisionSolver::TestDS(PX2Manifold * manifold, const float & dt)
{
	//////std::cout << "Test Dynamic vs Static" << std::endl;
	SwapDynamicToA(manifold);

	if (manifold->A()->IsHighVelocity()) // Only A is Dynamic
		(this->*_testFastDS[manifold->A()->GetShape()->GetType()][manifold->B()->GetShape()->GetType()])(manifold, dt);
	else
		(this->*_testSlowDS[manifold->A()->GetShape()->GetType()][manifold->B()->GetShape()->GetType()])(manifold, dt);
}

void PX2CollisionSolver::ResolveDD(PX2Manifold * manifold, const float & dt)
{
	//////std::cout << "Test Dynamic vs Dynamic" << std::endl
	SwapSleepingToA(manifold);

	if (manifold->A()->IsHighVelocity() || manifold->B()->IsHighVelocity())
		(this->*_resolveFastDD[manifold->A()->GetShape()->GetType()][manifold->B()->GetShape()->GetType()])(manifold, dt);
	else
		(this->*_resolveSlowDD[manifold->A()->GetShape()->GetType()][manifold->B()->GetShape()->GetType()])(manifold, dt);
}

void PX2CollisionSolver::ResolveDS(PX2Manifold * manifold, const float & dt)
{
	//////std::cout << "Test Dynamic vs Static" << std::endl;
	SwapDynamicToA(manifold);

	if (manifold->A()->IsHighVelocity()) // Only A is Dynamic
		(this->*_resolveFastDS[manifold->A()->GetShape()->GetType()][manifold->B()->GetShape()->GetType()])(manifold, dt);
	else
		(this->*_resolveSlowDS[manifold->A()->GetShape()->GetType()][manifold->B()->GetShape()->GetType()])(manifold, dt);
}

/////////////////////////////////////

void PX2CollisionSolver::TestFastCDCD(PX2Manifold * manifold, const float & dt)
{
	//////std::cout << "Test Fast CD - CD" << std::endl;
	TestSlowCDCD(manifold, dt);
}

void PX2CollisionSolver::ResolveFastCDCD(PX2Manifold * m, const float & dt)
{
	return;
	/*PX2Drawer->DrawCircle(m->A()->GetPositionOld().x, m->A()->GetPositionOld().y, m->A()->GetShape()->GetRadius() * 2, Color(255, 0, 255));
	PX2Drawer->DrawCircle(m->A()->GetPosition().x, m->A()->GetPosition().y, m->A()->GetShape()->GetRadius() * 2, Color(200, 0, 200));


	PX2Drawer->DrawCircle(m->B()->GetPositionOld().x, m->B()->GetPositionOld().y, m->B()->GetShape()->GetRadius() * 2, Color(0, 255, 255));
	PX2Drawer->DrawCircle(m->B()->GetPosition().x, m->B()->GetPosition().y, m->B()->GetShape()->GetRadius() * 2, Color(0, 200, 200));

	PX2Window->Refresh();*/

	////std::cout << "RESOLVE FAST CD CD MANIFOLD" << std::endl;
	/*_correction = (m->normal * (m->penetration)) * 0.5f;
	m->A()->SetPosition(m->A()->GetAABB().center + _correction);
	m->B()->SetPosition(m->B()->GetAABB().center - _correction);*/


	/*PX2Drawer->DrawCircle(m->A()->GetPosition().x, m->A()->GetPosition().y, m->A()->GetShape()->GetRadius() * 2, Color(200, 200, 200));
	PX2Drawer->DrawCircle(m->B()->GetPosition().x, m->B()->GetPosition().y, m->B()->GetShape()->GetRadius() * 2, Color(200, 200, 200));
	PX2Window->Refresh();*/

	////std::cout << "RESOLVED FAST CD CD MANIFOLD" << std::endl;

	_bFlt[0] = (1.0f / _bFlt[0]);

	_bFlt[2] = (1.0f / (m->A()->GetMass() + m->B()->GetMass()));

	_bFlt[0] = (m->A()->GetMass() - m->B()->GetMass());
	_bFlt[0] *= _bFlt[2];

	_bFlt[1] = (m->B()->GetMass() - m->A()->GetMass());
	_bFlt[1] *= _bFlt[2];




	//_e = std::min(m->A()->GetRestitution(), m->B()->GetRestitution());
	///*newVelX1 = (firstBall.speed.x * (firstBall.mass – secondBall.mass) + (2 * secondBall.mass * secondBall.speed.x)) / (firstBall.mass + secondBall.mass);
	//newVelY1 = (firstBall.speed.y * (firstBall.mass – secondBall.mass) + (2 * secondBall.mass * secondBall.speed.y)) / (firstBall.mass + secondBall.mass);
	//newVelX2 = (secondBall.speed.x * (secondBall.mass – firstBall.mass) + (2 * firstBall.mass * firstBall.speed.x)) / (firstBall.mass + secondBall.mass);
	//newVelY2 = (secondBall.speed.y * (secondBall.mass – firstBall.mass) + (2 * firstBall.mass * firstBall.speed.y)) / (firstBall.mass + secondBall.mass);*/

	_e = std::min(m->A()->GetRestitution(), m->B()->GetRestitution());

	_nVelA = (m->A()->GetVelocityLinear() * _bFlt[0]) + (m->B()->GetVelocityLinear() * (2.0f * m->B()->GetMass() * _bFlt[2]));
	_nVelA *= _e;

	_nVelB = (m->B()->GetVelocityLinear() * _bFlt[0]) + (m->A()->GetVelocityLinear() * (2.0f * m->A()->GetMass() * _bFlt[2]));
	_nVelB *= _e;


	m->A()->SetVelocityLinear(_nVelA);
	m->B()->SetVelocityLinear(_nVelB);
}

void PX2CollisionSolver::TestFastCDBD(PX2Manifold * manifold, const float & dt)
{
	//////std::cout << "Test Fast CD - BD" << std::endl;
}

void PX2CollisionSolver::TestFastBDBD(PX2Manifold * manifold, const float & dt)
{
	//////std::cout << "Test Fast BD - BD" << std::endl;

}

// SLOW - Circle Dynamic / Circle Dynamic
void PX2CollisionSolver::TestSlowCDCD(PX2Manifold * m, const float & dt)
{
	return;
	_normal = (m->A()->GetAABB().center - m->B()->GetAABB().center);
	_bFlt[0] = (m->A()->GetShape()->GetRadius() + m->B()->GetShape()->GetRadius());
	_bFlt[1] = _bFlt[0] * _bFlt[0];

	if (VectorTransformer::LengthSquared(_normal) > _bFlt[1])
		return;

	VectorTransformer::Normalize(_normal, m->normal);
	_bFlt[1] = VectorTransformer::Length(_normal);

	// m->dist = 0.0001f; // Is Colliding

	if (_bFlt[1] != 0)
	{
		// Distance is difference between radius and distance
		m->penetration = fabs(_bFlt[0] - _bFlt[1]);
		// Utilize our d since we performed sqrt on it already within Length( )
		// Points from A to B, and is a unit vector

	}
	// Circles are on same position
	else
	{
		// Choose random (but consistent) values
		////std::cout << "Rand Lol " << std::endl;
		m->penetration = _bFlt[0];
		m->normal = -m->normal;
	}

	m->toi = 2.0f;

	// HERE NORMAL AND PENETRATION IS SET FOR RESOLVING COLLISION POSITION
	_collisionListener->NotifyCollisionAB(m);
}

void PX2CollisionSolver::ResolveSlowCDCD(PX2Manifold * m, const float & dt)
{
	return;
	/*PX2Drawer->DrawCircle(m->A()->GetPositionOld().x, m->A()->GetPositionOld().y, m->A()->GetShape()->GetRadius() * 2, Color(255, 0, 255));
	PX2Drawer->DrawCircle(m->A()->GetPosition().x, m->A()->GetPosition().y, m->A()->GetShape()->GetRadius() * 2, Color(200, 0, 200));


	PX2Drawer->DrawCircle(m->B()->GetPositionOld().x, m->B()->GetPositionOld().y, m->B()->GetShape()->GetRadius() * 2, Color(0, 255, 255));
	PX2Drawer->DrawCircle(m->B()->GetPosition().x, m->B()->GetPosition().y, m->B()->GetShape()->GetRadius() * 2, Color(0, 200, 200));

	PX2Window->Refresh();*/

	////std::cout << "RESOLVE SLOW CD CD MANIFOLD" << std::endl;
	//_correction = (m->normal * (m->penetration));
	//m->A()->SetPosition(m->A()->GetAABB().center + _correction);
	//m->B()->SetPosition(m->B()->GetAABB().center - _correction);


	/* PX2Drawer->DrawCircle(m->A()->GetPosition().x, m->A()->GetPosition().y, m->A()->GetShape()->GetRadius() * 2, Color(200, 200, 200));
	PX2Drawer->DrawCircle(m->B()->GetPosition().x, m->B()->GetPosition().y, m->B()->GetShape()->GetRadius() * 2, Color(200, 200, 200));
	PX2Window->Refresh(); */
	//////std::cout << "RESOLVED SLOW CD CD MANIFOLD" << std::endl;

	//// And Resolve Collision From CorrectPos


	////////std::cout << "Correct CDCD (" << m->normal.ToString() << ") Pene (" << m->penetration << ")" << std::endl;
	//// Correst Position




	///*
	_bFlt[0] = (1.0f / _bFlt[0]);

	_bFlt[2] = (1.0f / (m->A()->GetMass() + m->B()->GetMass()));

	_bFlt[0] = (m->A()->GetMass() - m->B()->GetMass());
	_bFlt[0] *= _bFlt[2];

	_bFlt[1] = (m->B()->GetMass() - m->A()->GetMass());
	_bFlt[1] *= _bFlt[2];




	//_e = std::min(m->A()->GetRestitution(), m->B()->GetRestitution());
	///*newVelX1 = (firstBall.speed.x * (firstBall.mass – secondBall.mass) + (2 * secondBall.mass * secondBall.speed.x)) / (firstBall.mass + secondBall.mass);
	//newVelY1 = (firstBall.speed.y * (firstBall.mass – secondBall.mass) + (2 * secondBall.mass * secondBall.speed.y)) / (firstBall.mass + secondBall.mass);
	//newVelX2 = (secondBall.speed.x * (secondBall.mass – firstBall.mass) + (2 * firstBall.mass * firstBall.speed.x)) / (firstBall.mass + secondBall.mass);
	//newVelY2 = (secondBall.speed.y * (secondBall.mass – firstBall.mass) + (2 * firstBall.mass * firstBall.speed.y)) / (firstBall.mass + secondBall.mass);*/


	_e = std::min(m->A()->GetRestitution(), m->B()->GetRestitution());

	_nVelA = (m->A()->GetVelocityLinear() * _bFlt[0]) + (m->B()->GetVelocityLinear() * (2.0f * m->B()->GetMass() * _bFlt[2]));
	_nVelA *= _e;

	_nVelB = (m->B()->GetVelocityLinear() * _bFlt[0]) + (m->A()->GetVelocityLinear() * (2.0f * m->A()->GetMass() * _bFlt[2]));
	_nVelB *= _e;


	m->A()->SetVelocityLinear(_nVelA);
	m->B()->SetVelocityLinear(_nVelB);
	////m->A()->ApplyForce(_nVelA);
	////m->B()->ApplyForce(_nVelB);



	//m->A()->SetColliding(true);
	//m->B()->SetColliding(true);
}

void PX2CollisionSolver::TestSlowCDBD(PX2Manifold * manifold, const float & dt)
{
	//////std::cout << "Test Slow CD - BD" << std::endl;
}

void PX2CollisionSolver::TestSlowBDBD(PX2Manifold * manifold, const float & dt)
{
	//////std::cout << "Test Slow BD - BD" << std::endl;
}

////////
// DS //
////////

void PX2CollisionSolver::SwapDynamicToA(PX2Manifold * manifold)
{
	// Swap B to A if B is the dynamic One
	if (manifold->A()->GetType() < manifold->B()->GetType())
	{
		manifold->SwapAB(_swapBuffer);
	}
}

void PX2CollisionSolver::SwapSleepingToA(PX2Manifold * manifold)
{
	if (manifold->B()->IsSleeping())
	{
		manifold->SwapAB(_swapBuffer);
	}
}



void PX2CollisionSolver::TestFastCDCS(PX2Manifold * manifold, const float & dt)
{
	//////std::cout << "Test Fast CD - CS" << std::endl;
}

void PX2CollisionSolver::ResolveFastCDCS(PX2Manifold * manifold, const float & dt)
{
}

// FAST - Circle D / Box S
void PX2CollisionSolver::TestFastCDBS(PX2Manifold * manifold, const float & dt)
{
	////std::cout << "Test Fast CD - BS" << std::endl;

	return (TestSlowCDBS(manifold, dt)); // TMPPPP ??

	return;
}

void PX2CollisionSolver::ResolveFastCDBS(PX2Manifold * manifold, const float & dt)
{
	return (ResolveSlowCDBS(manifold, dt));

	std::cout << "Resolve Fast CD - BS" << std::endl;
	manifold->A()->SetPosition(manifold->positionCorrectionA);
	_nVelA = manifold->A()->GetVelocityLinear() - (manifold->normal * 2.0f * manifold->A()->GetVelocityLinear().Dot(manifold->normal));
	_nVelA *= std::min(manifold->A()->GetRestitution(), manifold->B()->GetRestitution());

	manifold->A()->SetVelocityLinear(_nVelA);
}

void PX2CollisionSolver::TestFastBDCS(PX2Manifold * manifold, const float & dt)
{
	//////std::cout << "Test Fast BD - CS" << std::endl;
}

void PX2CollisionSolver::TestFastBDBS(PX2Manifold * manifold, const float & dt)
{
	//////std::cout << "Test Fast BD - BS" << std::endl;
}

// SLOW - Circle Dynamic / Circle Static
void PX2CollisionSolver::TestSlowCDCS(PX2Manifold * manifold, const float & dt)
{
	//////std::cout << "Test Slow CD - CS" << std::endl;
}

#include <algorithm>

// Support function that returns the AABB2 ertex with index n
Vec2 Corner(AABB2 b, int n)
{
	Vec2 p;
	p.y = ((n & 0x01) ? b.min.y : b.max.y);
	p.x = ((n & 0x02) ? b.max.x : b.min.x);
	return (p);
}

int PX2CollisionSolver::TestSlowCDBSEdge(PX2Manifold * manifold, const float & dt, const Vec2& a, const Vec2& b, const Vec2& edgeA, const Vec2& edgeB, const Vec2& edgeNormal, Vec2& inter)
{
	/*PX2DebugDrawer::DrawVec2Vec2(a, b, Color(55, 55, 255));
	PX2DebugDrawer::DrawVec2Vec2(edgeA, edgeB, Color(55, 255, 55));
	PX2Window->Refresh();
	system("pause");*/
	
	/*//std::cout << "Test A(" << a.ToString() << ") B(" << b.ToString() << ")" << std::endl;
	//std::cout << "angle (" << angle << ")" << std::endl;*/
	if (manifold->A()->GetDirNorm().Dot(edgeNormal) <= -0.0f)
	{
		////std::cout << "VS Edge A(" << edgeA.ToString() << ") B(" << edgeB.ToString() << ")" << std::endl;
		if (IntersectionComputer::SegmentSegmentC(a, b, edgeA, edgeB, inter))
		{

			//std::cout << "Inter (" << inter.ToString() << ")" << std::endl;

			inter -= a;

			if (std::fabs(manifold->A()->GetDir().x) > std::fabs(manifold->A()->GetDir().y))
				_bFlt[0] = inter.x / manifold->A()->GetDir().x;
			else
				_bFlt[0] = inter.y / manifold->A()->GetDir().y;

			if (_bFlt[0] < manifold->toi)
			{

				manifold->positionCorrectionA = (inter + a);
				manifold->normal = edgeNormal;
				manifold->toi = _bFlt[0];

				//std::cout << "Inter (" << inter.ToString() << ")" << std::endl;
				//std::cout << "Toi (" << toi << ")" << std::endl;

				//PX2Drawer->DrawCircleFill(manifold->positionCorrectionA.x, manifold->positionCorrectionA.y, 4, Color(255, 55, 255));

				//PX2Window->Refresh();

				////std::cout << "Intersect" << std::endl;
				//system("pause");

			}
			return (2); // Intersect -> Stop
		}

		//std::cout << "Not Leaving" << std::endl;
		return (1); // No Intersect But not Leaving 
	}
	//std::cout << "Leaving ! Stop ! " << std::endl;
	return (0);
}

// SLOW - Circle Dynamic / Box Static (Mass Of Static Doesnt Matter ! Static IS STATIC (unmoveable))
void PX2CollisionSolver::TestSlowCDBS(PX2Manifold * manifold, const float & dt)
{
	AABB2		e = manifold->B()->GetAABB();
	float		r = manifold->A()->GetShape()->GetRadius();

	//std::cout << "Dir Object (" << manifold->A()->GetVelocityLinear().ToString() << ")" << std::endl;

	e.min.x -= r;
	e.min.y -= r;
	e.max.x += r;
	e.max.y += r;

//	PX2DebugDrawer::DrawAABB(manifold->A()->GetAABB(), Color::RED());
//	PX2DebugDrawer::DrawAABB(manifold->B()->GetAABB(), Color::WHITE());

	const AABB2&	abox = manifold->B()->GetAABB();
	PX2ShapeCircle*	circle = (PX2ShapeCircle*)(manifold->A()->GetShape());
	PX2ShapeBox*	box = (PX2ShapeBox*)(manifold->B()->GetShape());

	const Vec2&		a = manifold->A()->GetPositionOld();
	const Vec2&		b = manifold->A()->GetPosition();

	Vec2			inter;

	manifold->toi = FLT_MAX;

	//// Closest point on A to center of B
	Vec2 center = manifold->A()->GetPositionOld();

	if (center.y < abox.max.y)
	{
		if (center.y < abox.min.y)
		{
			//std::cout << "TOP" << std::endl;
			TestSlowCDBSEdge(manifold, dt, a, b, Vec2(abox.min.x, abox.min.y - r), Vec2(abox.max.x, abox.min.y - r), Vec2(0, -1), inter);
		}
		else
		{
			// HCENTER
			//std::cout << "VCENTER" << std::endl;
		}
	}
	else
	{
		//std::cout << "BOT" << std::endl;
		TestSlowCDBSEdge(manifold, dt, a, b, Vec2(abox.min.x, abox.max.y + r), Vec2(abox.max.x, abox.max.y + r), Vec2(0, 1), inter);
	}
	// Locate Circle center Compare To AABB2 
	if (center.x > abox.min.x)
	{
		if (center.x > abox.max.x)
		{
			//std::cout << " RIGHT" << std::endl;
			TestSlowCDBSEdge(manifold, dt, a, b, Vec2(abox.max.x + r, abox.min.y), Vec2(abox.max.x + r, abox.max.y), Vec2(1, 0), inter);

		}
		else
		{
			//std::cout << " HCENTER" << std::endl;
			// HCENTER
		}
	}
	else
	{
		//std::cout << " LEFT" << std::endl;
		TestSlowCDBSEdge(manifold, dt, a, b, Vec2(abox.min.x - r, abox.min.y), Vec2(abox.min.x - r, abox.max.y), Vec2(-1, 0), inter);
	}

	if (manifold->toi != FLT_MAX)
	{
		_collisionListener->NotifyCollisionA(manifold);
		//std::cout << "Return Colliding" << std::endl;
		return;
	}

	int				u = 0;


	float t[2];
	Vec2 point[2];
	Vec2 normal[2];
	// Test Corner At Pos 



	if (a.y < abox.center.y) u |= 1;
	if (a.x > abox.center.x) u |= 2;
	Vec2 corner = Corner(abox, u);


	//std::cout << "U (" << u << ")" << std::endl;
	//std::cout << "Test Circle At Corner (" << corner.ToString() << ")" << std::endl;
	if (IntersectionComputer::LineCircle(a, manifold->A()->GetDir(), corner, r, t, point, normal))
	{
		if (t[0] > 0 && t[0] < 1)
		{
			manifold->positionCorrectionA = point[0];
			manifold->normal = normal[0];
			manifold->toi = t[0];
			VectorTransformer::Normalize(manifold->normal);
			_collisionListener->NotifyCollisionA(manifold);
		}
	}



	// Else IF STILL NO INTERSET MAYBE TRY ANOTHER METHOD ?
	// IF INSIDE Wide Or Narrow BOX(AABB2 )


	// Last Box Resolution Check
	Vec2 nearest = manifold->A()->GetPosition();
	AABB2 narrowBox(abox.center, Vec2(abox.halfSize.x, abox.halfSize.y + r));

	//PX2DebugDrawer::DrawAABB(narrowBox, Color(255, 255, 0));
	//PX2Window->Refresh();
	//system("pause");

	if (!narrowBox.ContainsPoint(nearest))
	{
		AABB2 wideBox(abox.center, Vec2(abox.halfSize.x + r, abox.halfSize.y));

		/*PX2DebugDrawer::DrawAABB(wideBox, Color(255, 255, 0));
		PX2Window->Refresh();
		system("pause");*/
		if (!wideBox.ContainsPoint(nearest))
		{
			//	std::cout << "NOT Insiiiiiiide" << std::endl;
				// Circle Is not Colliding at All with AABB
			return;
		}
	}



	AABB2	wide(abox.center, Vec2(abox.halfSize.x + r + 0.01f, abox.halfSize.y + r + 0.01f));

	//std::cout << "Insiiiiiiide" << std::endl;
	manifold->toi = 0;

	// Could Be Optimize
	Vec2 diffMin = wide.min - nearest;
	Vec2 diffMax = wide.max - nearest;

	//PX2DebugDrawer::DrawAABB(wide, Color(55, 255, 285));

	(-diffMin.x < diffMax.x ? nearest.x = diffMin.x : nearest.x = diffMax.x);
	(-diffMin.y < diffMax.y ? nearest.y = diffMin.y : nearest.y = diffMax.y);

	float distance;



	if (fabs(nearest.x) < fabs(nearest.y))
	{
		distance = fabs(nearest.x);
		nearest.y = manifold->A()->GetPosition().y;
		nearest.x += manifold->A()->GetPosition().x;

	}
	else
	{
		distance = fabs(nearest.y);
		nearest.x = manifold->A()->GetPosition().x;
		nearest.y += manifold->A()->GetPosition().y;
	}

	/*std::cout << "Nearest (" << nearest.ToString() << ")" << std::endl;
	std::cout << "Origin (" << manifold->A()->GetPosition().ToString() << ")" << std::endl;
	PX2Drawer->DrawCircle(nearest.x, nearest.y, 4, Color::RED());
	PX2Window->Refresh();
	system("pause");*/

	manifold->positionCorrectionA = nearest;
	manifold->normal = Vec2(0, 0);
	

	//VectorTransformer::Normalize(manifold->normal);
	_collisionListener->NotifyCollisionA(manifold);

	return;
}

void PX2CollisionSolver::ResolveSlowCDBS(PX2Manifold * manifold, const float & dt)
{
	//std::cout << "Resolve Slow CD - BS" << std::endl;
	manifold->A()->ResetPosition(manifold->positionCorrectionA);
	_nVelA = manifold->A()->GetVelocityLinear() - (manifold->normal * 2.0f * manifold->A()->GetVelocityLinear().Dot(manifold->normal));
	_nVelA *= std::min(manifold->A()->GetRestitution(), manifold->B()->GetRestitution());

	manifold->A()->SetVelocityLinear(_nVelA);
	//

	//Vec2 n = manifold->normal;

	//

	//// get all of relative normal velocity
	//float relNv = (manifold->A()->GetVelocityLinear()).Dot(n);

	//

	//std::cout << "TOI (" << manifold->toi << ")" << std::endl;

	//float dist = VectorTransformer::Length(manifold->A()->GetDir() * manifold->toi);
	//// we want to remove only the amount which leaves them touching next frame
	//std::cout << "Dist To Object (" << dist << ")" << std::endl;

	//std::cout << "RelNv (" << relNv << ")" << std::endl;
	//float remove = relNv + dist / PX2_DT_STEP;

	//std::cout << "Remove (" << remove << ")" << std::endl;
	//if (remove < 0)
	//{
	//	
	//	// compute impulse
	//	float imp = remove / (manifold->A()->GetInvMass() + manifold->B()->GetInvMass());
	//	imp = remove;
	//	
	//	_nVelA = -(manifold->A()->GetDirNorm() + n * imp);
	//	std::cout << "Old Vel: (" << manifold->A()->GetVelocityLinear().ToString() <<  ")" << std::endl;
	//	std::cout << "New Vel: (" << _nVelA.ToString() <<  ")" << std::endl;
	//	// apply impulse
	//	manifold->A()->SetVelocityLinear(_nVelA);
	//}


	//Vec2 nVelANorm;
	//VectorTransformer::Normalize(_nVelA, nVelANorm);

	//PX2DebugDrawer::DrawVec2Vec2(manifold->A()->GetPosition(), manifold->A()->GetPosition() + nVelANorm * 100.0f, Color(255, 0, 255));
	//PX2DebugDrawer::DrawAABB(manifold->B()->GetAABB(), Color(255, 255, 85));

	//PX2Drawer->DrawCircleFill(manifold->positionCorrectionA.x, manifold->positionCorrectionA.y, 5, Color::WHITE());
	//
	//PX2Window->Refresh();

	//std::cout << "Angle Friction (" << 1.0f - nVelANorm.Dot(manifold->normal) << ")" << std::endl;
	//std::cout << "Cross Friction (" << nVelANorm.Cross(manifold->normal) << ")" << std::endl;



	//manifold->A()->SetVelocityLinear(_nVelA);

	//system("pause");
}

// SLOW - Box Dynamic / Circle Static
void PX2CollisionSolver::TestSlowBDCS(PX2Manifold * manifold, const float & dt)
{
	//////std::cout << "Test Slow BD - CS" << std::endl;
}

// SLOW - Box Dynamic / Box Static
void PX2CollisionSolver::TestSlowBDBS(PX2Manifold * manifold, const float & dt)
{
	//////std::cout << "Test Slow BD - BS" << std::endl;
}

/////////////////////////
// SS -> NO COLLISIONS //
/////////////////////////


void PX2CollisionSolver::TestSS(PX2Manifold * manifold, const float & dt)
{
	return;
}

void PX2CollisionSolver::ClassicDDResolveCollision(PX2Manifold * manifold, const float & dt)
{
	// Calculate relative velocity
	_rv = manifold->B()->GetVelocityLinear();
	_rv -= manifold->A()->GetVelocityLinear();

	// Calculate relative velocity in terms of the normal direction
	_velAlongNormal = _rv.Dot(manifold->normal);

	// Do not resolve if velocities are separating
	if (_velAlongNormal <= 0)
	{
		////// CORRECT POSITION ///////
		_correction = (manifold->normal * (manifold->penetration));

		manifold->A()->SetPosition(manifold->A()->GetAABB().center - _correction);
		manifold->B()->SetPosition(manifold->B()->GetAABB().center + _correction);

		////// RESOLVE COLLISION ///////

		// Calculate restitution
		_e = std::min(manifold->A()->GetRestitution(), manifold->B()->GetRestitution());
		// Calculate impulse scalar

		_j = -(1.0f + _e) * _velAlongNormal;
		_j /= (manifold->A()->GetInvMass() + manifold->B()->GetInvMass());

		// Apply impulse
		_impulse = manifold->normal * _j;

		_sumMass = (manifold->A()->GetMass() + manifold->B()->GetMass());

		_ratio = 1.0f;

		//_ratio = (manifold->A()->GetMass() / _sumMass);
		manifold->A()->ApplyForce(-_impulse * _ratio);

		//_ratio = (manifold->B()->GetMass() / _sumMass);
		manifold->B()->ApplyForce(_impulse * _ratio);
	}
}

void PX2CollisionSolver::ClassicDSResolveCollision(PX2Manifold * manifold, const float & dt)
{
	// Calculate relative velocity
	_rv = manifold->A()->GetVelocityLinear();
	//VectorTransformer::Normalize(_rv, _rv);
	// Calculate relative velocity in terms of the normal direction
	_velAlongNormal = _rv.Dot(manifold->normal);
	// Do not resolve if velocities are separating
	if (_velAlongNormal <= 0)
	{
		////// CORRECT POSITION ///////
		_correction = (manifold->normal * (manifold->dist));

		// Only Apply to the dynamic one (A)
		//manifold->A()->SetPosition(manifold->A()->GetPositionOld() - _correction);

		////// RESOLVE COLLISION ///////

		// Calculate restitution
		_e = std::min(manifold->A()->GetRestitution(), manifold->B()->GetRestitution());

		// Calculate impulse scalar
		_j = _e * _velAlongNormal;

		_nVelA = manifold->A()->GetVelocityLinear() - (manifold->normal * 2.0f * manifold->A()->GetVelocityLinear().Dot(manifold->normal));
		_nVelA *= _e;

		// Apply impulse
		//_impulse = manifold->normal * _j;

		//// Only Apply to the dynamic one (A)
		//manifold->A()->ApplyForce(-_impulse);
		manifold->A()->SetVelocityLinear(_nVelA);
	}
	//else
	//{
	//	_nVelA = manifold->A()->GetVelocityLinear() - (manifold->normal * 2.0f * manifold->A()->GetVelocityLinear().Dot(manifold->normal));
	//	manifold->A()->SetVelocityLinear(_nVelA);
	//	//manifold->A()->SetVelocityLinear(Vec2(0, 0));
	//	//////std::cout << "Loupibalaboum" << std::endl;
	//}

}