#include "GameObject.h"

#include <cassert>
#include <iostream>

#include "../../Tools/Transform.h"
#include "GameObjectHandler.h"

namespace maoutch
{
	GameObject::GameObject(const std::string& name, const int& zIndex, const bool& isVisible) : _name(name), _zIndex(zIndex), _isVisible(isVisible), _needUpdate(true)
	{
		GameObjectHandler::instance->AddObject(this);
	}
	GameObject::~GameObject() = default;

	void GameObject::DrawCall(sf::RenderWindow& window)
	{
		if (!IsVisible()) return;

		if (_needUpdate)
		{
			if (parent == nullptr) _transform = _localTransform;
			else _transform = transform::Compose(_localTransform, parent->_transform);

			for (GameObject* child : childrens)
				child->NeedUpdate();

			_needUpdate = false;
		}

		_OnDraw(window, _transform.getTransform());
	}

	GameObject* GameObject::GetChildren(const int& index)
	{
		assert(index >= 0 && index < childrens.size());
		return childrens[index];
	}
	GameObject* GameObject::GetChildren(const std::string& name)
	{
		for (GameObject* object : childrens)
			if (object->GetName() == name) return object;

		return nullptr;
	}
	void GameObject::SetParent(GameObject* object)
	{
		if (parent == object) return;
		if (parent != nullptr)
			parent->RemoveChildren(this);

		if (object == nullptr)
		{
			parent = object;
			return;
		}
		object->AddChildren(this);
	}
	void GameObject::AddChildren(GameObject* object)
	{
		if (object->parent == this) return;
		childrens.push_back(object);

		object->parent = this;
		object->NeedUpdate();
	}
	void GameObject::RemoveChildren(GameObject* object)
	{
		if (object->parent != this) return;
		childrens.erase(std::find(childrens.begin(), childrens.end(), object));

		object->parent = nullptr;
		object->NeedUpdate();
	}
	void GameObject::RemoveFromParent()
	{
		if (parent == nullptr) return;
		parent->RemoveChildren(this);
	}

	std::string GameObject::GetName() const { return _name; }
	void GameObject::SetName(const std::string& name) { _name = name; }

	bool GameObject::IsVisible() const { return _isVisible; }
	void GameObject::SetVisible(const bool& isVisible) { _isVisible = isVisible; }
	void GameObject::NeedUpdate() { _needUpdate = true; }

	Vector2 GameObject::GetPosition() const { return Vector2::From(_localTransform.getPosition()); }
	Vector2 GameObject::GetScale() const { return Vector2::From(_localTransform.getScale()); }
	Vector2 GameObject::GetOrigin() const { return Vector2::From(_localTransform.getOrigin()); }
	float GameObject::GetRotation() const { return _localTransform.getRotation(); }

	Vector2 GameObject::GetGlobalPosition() const { return Vector2::From(_transform.getPosition()); }
	Vector2 GameObject::GetGlobalScale() const { return Vector2::From(_transform.getScale()); }
	float GameObject::GetGlobalRotation() const { return _transform.getRotation(); }

	void GameObject::SetZIndex(const int& zIndex)
	{
		_zIndex = zIndex;
		GameObjectHandler::instance->NeedUpdateSorting();
	}
	int GameObject::GetZIndex() const { return _zIndex; }

	void GameObject::SetPosition(const Vector2& position)
	{
		_localTransform.setPosition(position);
		NeedUpdate();
	}
	void GameObject::SetRotation(float angle)
	{
		_localTransform.setRotation(angle);
		NeedUpdate();
	}
	void GameObject::SetScale(const Vector2& factor)
	{
		_localTransform.setScale(factor);
		NeedUpdate();
	}
	void GameObject::SetOrigin(const Vector2& origin)
	{
		_localTransform.setOrigin(origin);
		NeedUpdate();
	}
	void GameObject::Move(const Vector2& offset)
	{
		_localTransform.move(offset);
		NeedUpdate();
	}
	void GameObject::Rotate(float angle)
	{
		_localTransform.rotate(angle);
		NeedUpdate();
	}
	void GameObject::Scale(const Vector2& factor)
	{
		_localTransform.scale(factor);
		NeedUpdate();
	}
}