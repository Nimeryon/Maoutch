#pragma once
namespace sf { class RenderWindow; }
#include "vector"
#include <mutex>

namespace maoutch
{
	class GameObject;

	class GameObjectHandler
	{
	public:
		static GameObjectHandler* GetInstance();

		GameObjectHandler(GameObjectHandler const&) = delete;
		void operator=(GameObjectHandler const&) = delete;

		void AddObject(GameObject* object);
		void Destroy(GameObject* object);

		void ProcessObjectsAdding();

		void ProcessInputs();

		void EarlyUpdate(float dt);
		void Update(float dt);
		void FixedUpdate(float dt);
		void LateUpdate(float dt);

		void Draw(sf::RenderWindow& window);

		void NeedUpdateSorting();
		void Clear();

	protected:
		GameObjectHandler();
		~GameObjectHandler();

	private:
		static GameObjectHandler* _instance;
		static std::mutex _mutex; // For allowing multithreaded use

		bool _neeedObjectSorting;
		bool _needObjectDeleting;
		bool _needObjectAdding;
		std::vector<GameObject*> _objects;
		std::vector<GameObject*> _objectsToDestroy;
		std::vector<GameObject*> _objectsToAdd;

		void _ClearObjectsToDestroy();
		void _AddObjectsToAdd();
	};
}
