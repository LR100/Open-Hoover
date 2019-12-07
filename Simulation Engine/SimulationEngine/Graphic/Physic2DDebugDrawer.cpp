#include "Physic2DDebugDrawer.h"

Physic2DDebugDrawer::Physic2DDebugDrawer(IDrawer2D* drawer)
{
	_drawer = drawer;
	_objectsCount = 0;
	_colorCenter = Color(255, 255, 255);
}

Physic2DDebugDrawer::~Physic2DDebugDrawer()
{
}

void Physic2DDebugDrawer::RegisterBody(Physic2DBody* body)
{
	const std::vector<Physic2DFixture*>& fixtures = body->GetFixtures();

	if (_bodiesSprites.count(body->GetID())) {
		UnregisterBodyNoCheck(body->GetID());
	}

	ColorDef		transparency(0, 0, 0);
	Physic2DShape* shape;
	IImage* image = NULL;
	unsigned int	w, h;
	unsigned int	x, y;
	unsigned int	r;
	Color			colorCircle(155, 20, 10);
	Color			colorBox(0, 155, 24);
	SpriteCentered	spriteCentered;
	IImage*			initialImage = _drawer->GetCurrentImage();
	
	std::vector<SpriteCentered>& sprites = _bodiesSprites[body->GetID()];
	for (size_t i = 0; i < fixtures.size(); i += 1) {
		if (fixtures.at(i)->GetShape() != NULL) {
			shape = fixtures.at(i)->GetShape();
			
			if (shape->GetType() == Physic2DShape::BOX) {
				Physic2DShapeBox* box = (Physic2DShapeBox*)shape;
				w = (unsigned int)box->GetWidth();
				h = (unsigned int)box->GetHeight();
				image = _drawer->CreateImage("Physic2DDebugDrawer::Image", (w + 1), (h + 1));
				_drawer->SetCurrentImage(image);
				_drawer->DrawBorder(0, 0, w, h, colorBox, colorBox);
				spriteCentered.offset.x = -(float)w; // Should add fixtures position if setted
				spriteCentered.offset.x *= 0.5f;
				spriteCentered.offset.y = -(float)h;
				spriteCentered.offset.y *= 0.5f;
			} else if (shape->GetType() == Physic2DShape::CIRCLE) {
				Physic2DShapeCircle* circle = (Physic2DShapeCircle*)shape;
				w = (unsigned int)(circle->GetRadius() * 2.0f);
				h = w;
				x = (w / 2);
				y = (w / 2);
				r = (w - 2);
				image = _drawer->CreateImage("Physic2DDebugDrawer::Image", (w + 1), (h + 1));
				_drawer->SetCurrentImage(image);
				_drawer->DrawCircle(x, y, r, colorCircle);
				spriteCentered.offset.x = -(float)w; // Should add fixtures position if setted
				spriteCentered.offset.x *= 0.5f;
				spriteCentered.offset.y = -(float)h;
				spriteCentered.offset.y *= 0.5f;
			} else if (shape->GetType() == Physic2DShape::POLYGON) {

			}

			if (image) {
				spriteCentered.sprite = new Sprite(image, transparency);
				
				_drawer->RemoveImage("Physic2DDebugDrawer::Image");
				sprites.push_back(SpriteCentered());
				sprites.back().sprite = spriteCentered.sprite;
				sprites.back().offset = spriteCentered.offset;
				// std::cout << "Sprite Line Count :" << sprites.back().sprite->GetLines().size() << std::endl;
				delete (image);
				image = NULL;
			}
		}
	}
	_drawer->SetCurrentImage(initialImage);
}

void Physic2DDebugDrawer::UnregisterBody(const size_t& bodyID)
{
	if (_bodiesSprites.count(bodyID)) {
		UnregisterBodyNoCheck(bodyID);
	}
}

void Physic2DDebugDrawer::ResetObjectsCount()
{
	_objectsCount = 0;
}

const size_t& Physic2DDebugDrawer::GetObjectsCount() const
{
	return (_objectsCount);
}

void Physic2DDebugDrawer::UnregisterBodyNoCheck(const size_t& bodyID)
{
	std::vector<SpriteCentered>& sprites = _bodiesSprites.at(bodyID);

	// Free Sprites
	for (size_t i = 0; i < sprites.size(); i += 1) {
		if (sprites.at(i).sprite != NULL) {
			delete (sprites.at(i).sprite);
		}
	}
	_bodiesSprites.erase(bodyID);
}

bool Physic2DDebugDrawer::ReportBody(Physic2DBody* body)
{
	if (_bodiesSprites.count(body->GetID())) {
		//std::cout << "Body Exist" << std::endl;
		std::vector<SpriteCentered>& sprites = _bodiesSprites.at(body->GetID());
		for (size_t i = 0; i < sprites.size(); i += 1) {
			_posDraw.x = (int)(body->GetPosition().x + sprites.at(i).offset.x);
			_posDraw.y = (int)(body->GetPosition().y + sprites.at(i).offset.y);
			//std::cout << "Draw Sprite At Pos (" << _posDraw.ToString() << ")" << std::endl;
			_drawer->DrawSprite(_posDraw.x, _posDraw.y, sprites.at(i).sprite);
			_posDraw.x = (int)(body->GetPosition().x);
			_posDraw.y = (int)(body->GetPosition().y);
			_drawer->DrawCircle(_posDraw.x, _posDraw.y, 1, _colorCenter);
		}
	}
	_objectsCount += 1;
	return (true);
}
