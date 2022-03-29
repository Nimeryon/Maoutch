#pragma once
namespace sf { class RenderWindow; }
#include "vector"

namespace maoutch
{
	class GameObject;

	class GameObjectHandler
	{
	public:
		GameObjectHandler();
		~GameObjectHandler();

		GameObjectHandler(GameObjectHandler const&) = delete;
		void operator=(GameObjectHandler const&) = delete;

		void AddObject(GameObject* object);
		void Destroy(GameObject* object);

		void ProcessInputs();

		void EarlyUpdate(float dt);
		void Update(float dt);
		void FixedUpdate(float dt);
		void LateUpdate(float dt);

		void Draw(sf::RenderWindow& window);

		void NeedUpdateSorting();

		static GameObjectHandler* instance;

	private:
		bool _neeedObjectSorting;
		bool _needObjectDeleting;
		std::vector<GameObject*> _objects;
		std::vector<GameObject*> _objectsToDestroy;

		void _ClearObjectsToDestroy();
	};
}
