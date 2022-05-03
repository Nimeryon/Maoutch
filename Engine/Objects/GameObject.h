#pragma once
namespace sf
{
	class Drawable;
	class RenderWindow;
}
#include <SFML/Graphics/Transformable.hpp>
#include <vector>
#include <string>
#include <cassert>

#include "../../Types/Vector2.h"
#include "../../Interfaces/ITransformable.h"

namespace maoutch
{
	class GameObject : public ITransformable
	{
	public:
		explicit GameObject(const std::string& name, const int& zIndex = 0, const bool& isVisible = true, const bool& isActive = true);
		virtual ~GameObject();
		
		// Function called when object destroyed
		virtual void OnDestroy() {}

		// Function called for processing events
		virtual void ProcessInputs() {}

		// Function called at the first frame the object is loaded
		virtual void Init() {}

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

		template <typename T>
		T* GetChildren(const int& index)
		{
			assert(index >= 0 && index < childrens.size());
			return (T*)childrens[index];
		}
		template <typename T>
		T* GetChildren(const std::string& name)
		{
			for (GameObject* object : childrens)
				if (object->GetName() == name) return (T*)object;

			return nullptr;
		}

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

		Vector2 GetPosition() const override;
		Vector2 GetScale() const override;
		Vector2 GetOrigin() const override;
		float GetRotation() const override;

		// Transform

		Vector2 GetGlobalPosition() const;
		Vector2 GetGlobalScale() const;
		float GetGlobalRotation() const;

		void SetPosition(const Vector2& position) override;
		void SetRotation(float angle) override;
		void SetScale(const Vector2& factor) override;
		void SetOrigin(const Vector2& origin) override;
		void Move(const Vector2& offset) override;
		void Rotate(float angle) override;
		void Scale(const Vector2& factor) override;

		GameObject* parent = nullptr;
		std::vector<GameObject*> childrens = {};

	protected:
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