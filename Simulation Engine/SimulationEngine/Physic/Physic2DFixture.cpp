#include "Physic2DFixture.h"

#include "Physic2DBody.h"

Physic2DFixture::Physic2DFixture(Physic2DBody* body, Physic2DFixtureProperties properties)
{
	if (body) {
		_data = new UserData();
		_data->self = this; // Only a Physic2DFixture
		_data->bodyID = body->GetID();
	} else {
		_data = NULL;
	}
}
