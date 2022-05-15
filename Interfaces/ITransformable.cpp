#include "ITransformable.h"
#include "../../Engine/Objects/GameObjectHandler.h"

namespace maoutch
{
	ITransformable::ITransformable()
	{
		GameObjectHandler::Instance()->AddTranform(this);
	}
	ITransformable::~ITransformable() = default;

	sf::FloatRect ITransformable::GetBounds() const { return sf::FloatRect(); }

	Vector2 ITransformable::GetPosition() const { return Vector2::From(_localTransform.getPosition()); }
	Vector2 ITransformable::GetScale() const { return Vector2::From(_localTransform.getScale()); }
	Vector2 ITransformable::GetOrigin() const { return Vector2::From(_localTransform.getOrigin()); }
	float ITransformable::GetRotation() const { return _localTransform.getRotation(); }

	void ITransformable::SetPosition(const Vector2& position) { _localTransform.setPosition(position); }
	void ITransformable::SetRotation(float angle) { _localTransform.setRotation(angle); }
	void ITransformable::SetScale(const Vector2& factor) { _localTransform.setScale(factor); }
	void ITransformable::SetOrigin(const Vector2& origin) { _localTransform.setOrigin(origin); }
	void ITransformable::Move(const Vector2& offset) { _localTransform.move(offset); }
	void ITransformable::Rotate(float angle) { _localTransform.rotate(angle); }
	void ITransformable::Scale(const Vector2& factor) { _localTransform.scale(factor); }
}
