#include "Physic2DWorldB2D.h"
#include "Physic2DBodyB2D.h"
#include "Physic2DShapeB2D.h"

#define MAX_BODIES 1000

Physic2DWorldB2D::Physic2DWorldB2D(Vec2 gravity) : Physic2DWorld(gravity)
{
	_bodyCountMax = MAX_BODIES;
	_b2World = new b2World(b2Vec2(gravity.x, gravity.y));
	_velocityIterations = 10;  //how strongly to correct velocity
	_positionIterations = 4;
	std::cout << "Box2DWorld is INIT" << std::endl;
	// Init IDS For Bodies
	for (size_t i = 0; i < _bodyCountMax; i += 1)
	{
		// _bodies.push_back(NULL); // All bodies are available (Getting from ID is O(1))
		_bodiesAvailableIDs.push_back(_bodyCountMax - i);
	}
}

void Physic2DWorldB2D::Simulate(float timePassed)
{
	//std::cout << "Physic2DWorldB2D::Simulate" << std::endl;
	_b2World->Step(timePassed, _velocityIterations, _positionIterations);
}


Physic2DBody* Physic2DWorldB2D::CreateBody(Physic2DBodyProperties& properties)
{
	size_t			 bodyID = 0;
	bodyID = GetBodyAvailableID();

	if (bodyID == SIZE_MAX) {
		std::cerr << "Physic2DWorldB2D::CreateBody() ERROR: Cannot create more bodies" << std::endl;
		return (NULL);
	}

	Physic2DBodyB2D* body = new Physic2DBodyB2D(this, properties);

	body->SetBodyB2D(_b2World->CreateBody(&body->GetBodyDefB2D()));
	body->SetIDB2D(bodyID);
	_bodiesMAP.emplace(bodyID, body);

	return (body);
}

std::vector<Physic2DBody*> Physic2DWorldB2D::GetBodies() const
{
	std::unordered_map<size_t, Physic2DBody*>::const_iterator it = _bodiesMAP.begin();
	std::unordered_map<size_t, Physic2DBody*>::const_iterator itEnd = _bodiesMAP.end();
	std::vector<Physic2DBody*>	bodies;

	for (; it != itEnd; it++) {
		bodies.push_back(it->second);
	}

	return (bodies);
}

Physic2DBody* Physic2DWorldB2D::GetBody(const size_t& id) const
{
	if (_bodiesMAP.count(id)) {
		return (_bodiesMAP.at(id));
	}
	return (NULL);
}

void Physic2DWorldB2D::GetBodiesInAABB(Physic2DQueryBodyCb* querybody, const AABB2& aabb) const
{
	std::vector<Physic2DBody*>	bodies;
	AABBQuery					aabbquery(querybody, (Physic2DWorld*)this, _b2World, aabb);

	//std::cout << "Get Bodies in AABB : (" << aabb.ToString() << ")" << std::endl;
	aabbquery.Query();
}

void Physic2DWorldB2D::GetBodiesAtPosition(Physic2DQueryBodyCb* querybody, const Vec2& pos) const
{
	AABB2 aabb;

	aabb.center = pos;
	aabb.halfSize.x = 0.5f;

	aabb.halfSize.y = 0.5f;
	aabb.ComputeMinMax();


	std::vector<Physic2DBody*>	bodies;
	AABBQuery					aabbquery(querybody, (Physic2DWorld*)this, _b2World, aabb);

	aabbquery.Query();
}

void Physic2DWorldB2D::DestroyBody(Physic2DBody* body)
{
	if (_bodiesMAP.count(body->GetID()))
	{
		Physic2DBodyB2D* bodyb2d = (Physic2DBodyB2D*)body;
		_b2World->DestroyBody(bodyb2d->GetBodyB2D());
		_bodiesMAP.erase(body->GetID());
		_bodiesAvailableIDs.push_back(body->GetID());
		delete (body);
	}
}

void Physic2DWorldB2D::DestroyAllBodies()
{
}

void Physic2DWorldB2D::SetGravity(const Vec2& gravity)
{
	_b2World->SetGravity(b2Vec2(gravity.x, gravity.y));
}

Vec2 Physic2DWorldB2D::GetGravity() const
{
	return Vec2(_b2World->GetGravity().x, _b2World->GetGravity().y);
}

Physic2DShapeCircle* Physic2DWorldB2D::CreateShapeCircle()
{
	return (new Physic2DShapeCircleB2D(this));
}

Physic2DShapeBox* Physic2DWorldB2D::CreateShapeBox()
{
	return (new Physic2DShapeBoxB2D(this));
}

Physic2DShapePolygon* Physic2DWorldB2D::CreateShapePolygon()
{
	return (new Physic2DShapePolygonB2D(this));
}

Physic2DWorld::RaytraceOutput Physic2DWorldB2D::Raytrace(const Vec2& origin, const Vec2& dir, const float& maxDist)
{
	RaytraceQuery					rtquery(_b2World, origin, dir, maxDist);
	Physic2DWorld::RaytraceOutput	rtoutput;
	rtoutput = rtquery.Raytrace();
	if (rtoutput.found) {
		// NO way that fails -> Body cannot arrive here by chance
		rtoutput.body = _bodiesMAP.at(rtquery.GetBodyID());
	}
	return (rtoutput);
}

void Physic2DWorldB2D::Clear()
{
}


size_t Physic2DWorldB2D::GetBodyAvailableID()
{
	size_t	id;

	if (_bodiesAvailableIDs.size())
	{
		id = _bodiesAvailableIDs.back();
		_bodiesAvailableIDs.pop_back();
	}
	else
	{
		id = SIZE_MAX;
	}
	return (id);
}

#define RT_QUERY_MAX_LENGTH 20.0f // An AABB of maximum N of width/height

Physic2DWorldB2D::RaytraceQuery::RaytraceQuery(b2World * world, const Vec2 & origin, const Vec2 & dir, const float& maxDist)
{
	_origin = origin;
	_dir = dir;
	_world = world;
	_bodyID = 0;
	_maxDist = maxDist;
	_distStep = RT_QUERY_MAX_LENGTH;
	_found = false;
	_step = 0;
	_move = (_dir * RT_QUERY_MAX_LENGTH);
	_output = RaytraceOutput(); // Reset
}

bool Physic2DWorldB2D::RaytraceQuery::ReportFixture(b2Fixture* fixture)
{
	if (fixture->RayCast(&_b2output, _b2input, 0))
	{
		// std::cout << "Found" << std::endl;
		Physic2DFixture::UserData* userData;

		userData = (Physic2DFixture::UserData*)fixture->GetUserData();
		_bodyID = userData->GetBodyID();
		
		_output.intersection.x = _b2input.p1.x + (_move.x * _b2output.fraction);
		_output.intersection.y = _b2input.p1.y + (_move.y * _b2output.fraction);
		_output.shape = userData->GetFixture()->GetShape();
		_output.found = true;

		//std::cout << "AT FRACTION: (" << _b2output.fraction << ")" << std::endl;
		
		_found = true;
		return (false); // STOP
	}
	// std::cout << "NOT Found" << std::endl;
	return (true);
}

Physic2DWorld::RaytraceOutput Physic2DWorldB2D::RaytraceQuery::Raytrace()
{
	size_t				step = 0;
	b2AABB				aabb;
	
	_b2input.maxFraction = 1; // Always
	// Inputs starting Points
	_b2input.p1.x = _origin.x;
	_b2input.p1.y = _origin.y;
	_b2input.p2.x = _origin.x + _move.x;
	_b2input.p2.y = _origin.y + _move.y;

	// AABB X
	if (_b2input.p1.x > _b2input.p2.x) {
		aabb.lowerBound.x = _b2input.p2.x;
		aabb.upperBound.x = _b2input.p1.x;
	} else {
		aabb.lowerBound.x = _b2input.p1.x;
		aabb.upperBound.x = _b2input.p2.x;
	}
	// AABB Y
	if (_b2input.p1.y > _b2input.p2.y) {
		aabb.lowerBound.y = _b2input.p2.y;
		aabb.upperBound.y = _b2input.p1.y;
	} else {
		aabb.lowerBound.y = _b2input.p1.y;
		aabb.upperBound.y = _b2input.p2.y;
	}

	while (!_found && _maxDist > 0)
	{
		// std::cout << "Query AABB" << std::endl;
		_world->QueryAABB(this, aabb);
		_b2input.p1.x = _b2input.p2.x;
		_b2input.p1.y = _b2input.p2.y;
		_b2input.p2.x += _move.x;
		_b2input.p2.y += _move.y;
		_maxDist -= RT_QUERY_MAX_LENGTH;
		_step += 1;
		aabb.lowerBound.x += _move.x;
		aabb.lowerBound.y += _move.y;
		aabb.upperBound.x += _move.x;
		aabb.upperBound.y += _move.y;
	}

	return (_output);
}

const size_t& Physic2DWorldB2D::RaytraceQuery::GetBodyID() const
{
	return (_bodyID);
}

Physic2DWorldB2D::AABBQuery::AABBQuery(Physic2DQueryBodyCb* querybody, Physic2DWorld* world, b2World* b2world, const AABB2& aabb)
{
	_b2aabb.lowerBound.x = aabb.min.x;
	_b2aabb.lowerBound.y = aabb.min.y;
	_b2aabb.upperBound.x = aabb.max.x;
	_b2aabb.upperBound.y = aabb.max.y;
	_b2World = b2world;
	_world = world;
	_userData = NULL;
	_bodyID = 0;
	_querybody = querybody;
}

bool Physic2DWorldB2D::AABBQuery::ReportFixture(b2Fixture* fixture)
{
	 // std::cout << "Fixture" << std::endl;
	_userData = (Physic2DFixture::UserData*)fixture->GetUserData();
	_bodyID = _userData->GetBodyID();
	if (!_bodiesID.count(_bodyID)) {
		_bodiesID.emplace(_bodyID);		
		return (_querybody->ReportBody(_userData->GetFixture()->GetBody()));
	}
	return (true);
}

void Physic2DWorldB2D::AABBQuery::Query()
{
	// std::cout << "Query" << std::endl;
	_b2World->QueryAABB(this, _b2aabb);
}
