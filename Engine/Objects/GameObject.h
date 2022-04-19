#pragma once
namespace sf
{
	class Drawable;
	class RenderWindow;
}
#include <SFML/Graphics/Transformable.hpp>
#include <vector>
#include <string>

#include "../../Types/Vector2.h"

namespace maoutch
{
	class GameObject
	{
	public:
		explicit GameObject(const std::string& name, const int& zIndex = 0, const bool& isVisible = true, const bool& isActive = true);
		virtual ~GameObject();
		
		// Function called when object destroyed
		virtual void OnDestroy() {}

		// Function called for processing events
		virtual void ProcessInputs() {}

		// Function called at the beginning of a frame
		virtual void EarlyUpdate(float dt) {}
		// Function called each frame
		virtual void Update(float dt) {}
		// Function called at a constant rate 60 times per seconds
		virtual void FixedUpdate(float dt) {}
		// Function called at the end of a frame after draw calls
		virtual void LateUpdate(float dt) {}

		void TransformUpdate();
		void DrawCall(sf::RenderWindow& window);

		GameObject* GetChildren(const int& index);
		GameObject* GetChildren(const std::string& name);
		void SetParent(GameObject* object);
		void AddChildren(GameObject* object);
		void RemoveChildren(GameObject* object);
		void RemoveFromParent();

		std::string GetName() const;
		void SetName(const std::string& name);

		bool IsVisible() const;
		bool IsActive() const;
		virtual void SetVisible(const bool& isVisible);
		virtual void SetActive(const bool& isActive);
		void NeedUpdate();

		virtual void SetZIndex(const int& zIndex);
		int GetZIndex() const;

		// LocalTransform

		Vector2 GetPosition() const;
		Vector2 GetScale() const;
		Vector2 GetOrigin() const;
		float GetRotation() const;

		// Transform

		Vector2 GetGlobalPosition() const;
		Vector2 GetGlobalScale() const;
		float GetGlobalRotation() const;

		virtual void SetPosition(const Vector2& position);
		virtual void SetRotation(float angle);
		virtual void SetScale(const Vector2& factor);
		virtual void SetOrigin(const Vector2& origin);
		virtual void Move(const Vector2& offset);
		virtual void Rotate(float angle);
		virtual void Scale(const Vector2& factor);

		GameObject* parent = nullptr;
		std::vector<GameObject*> childrens = {};

	protected:
		sf::Transformable _localTransform;
		sf::Transformable _transform;

	private:
		std::string _name;
		bool _isVisible;
		bool _isActive;
		bool _needUpdate;
		int _zIndex;
		
		virtual void _OnDraw(sf::RenderWindow& window, const sf::Transform& transform) {}
	};
}