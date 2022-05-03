#pragma once
namespace sf { class RenderWindow; }
#include <mutex>

#include <vector>
#include "../../Interfaces/IStateDependant.h"

namespace maoutch
{
	class GameObject;

	class GameObjectHandler : IStateDependant
	{
	public:
		static GameObjectHandler* Instance();

		GameObjectHandler(GameObjectHandler const&) = delete;
		void operator=(GameObjectHandler const&) = delete;

		void AddObject(GameObject* object);
		void Destroy(GameObject* object);

		void ProcessObjectsAdding();
		void ProcessObjectsDestroy();

		void ProcessInputs();

		void EarlyUpdate(float dt);
		void Update(float dt);
		void FixedUpdate(float dt);
		void LateUpdate(float dt);

		void Draw(sf::RenderWindow& window);

		void NeedUpdateSorting();
		void Clear();

		template <typename T>
		std::vector<T*> GetObjects()
		{
			std::vector<T*> objects = {};
			for (auto& object : _objects)
				if (dynamic_cast<T*>(object))
					objects.emplace_back((T*)object);

			return objects;
		}
		std::vector<GameObject*> GetObjects(const std::string& name);

	protected:
		GameObjectHandler();
		~GameObjectHandler() override;

		void _OnStateChange() override;

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
