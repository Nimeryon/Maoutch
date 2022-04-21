#include <SFML/Graphics.hpp>

#include "GameObjectHandler.h"
#include "GameObject.h"

namespace maoutch
{
	GameObjectHandler* GameObjectHandler::_instance = nullptr;
	std::mutex GameObjectHandler::_mutex;

	GameObjectHandler::GameObjectHandler() : _neeedObjectSorting(false), _needObjectDeleting(false), _needObjectAdding(false) {}
	GameObjectHandler::~GameObjectHandler() { Clear(); }

	GameObjectHandler* GameObjectHandler::GetInstance()
	{
		std::lock_guard lock(_mutex);

		if (!_instance) _instance = new GameObjectHandler();
		return _instance;
	}

	void GameObjectHandler::AddObject(GameObject* object)
	{
		_needObjectAdding = true;

		_objectsToAdd.push_back(object);
	}
	void GameObjectHandler::Destroy(GameObject* object)
	{
		_needObjectDeleting = true;

		object->OnDestroy();
		object->RemoveFromParent();
		for (GameObject* childObject : object->childrens)
			Destroy(childObject);

		_objectsToDestroy.push_back(object);
	}

	void GameObjectHandler::ProcessObjectsAdding()
	{
		if (_needObjectAdding)
		{
			_needObjectAdding = false;
			_AddObjectsToAdd();
		}
	}

	void GameObjectHandler::ProcessInputs()
	{
		for (GameObject* object : _objects)
			if (object->IsActive())
				object->ProcessInputs();
	}

	void GameObjectHandler::EarlyUpdate(float dt)
	{
		for (GameObject* object : _objects)
		{
			object->TransformUpdate();
			if (object->IsActive())
				object->EarlyUpdate(dt);
		}
	}
	void GameObjectHandler::Update(float dt)
	{
		for (GameObject* object : _objects)
			if (object->IsActive())
				object->Update(dt);
	}
	void GameObjectHandler::FixedUpdate(float dt)
	{
		for (GameObject* object : _objects)
			if (object->IsActive())
				object->FixedUpdate(dt);
	}
	void GameObjectHandler::LateUpdate(float dt)
	{
		for (GameObject* object : _objects)
			if (object->IsActive())
				object->LateUpdate(dt);
	}

	void GameObjectHandler::Draw(sf::RenderWindow& window)
	{
		if (_needObjectDeleting) 
		{
			_ClearObjectsToDestroy();
			_needObjectDeleting = false;
		}

		if (_neeedObjectSorting)
		{
			std::sort(_objects.begin(), _objects.end(), [](GameObject* object1, GameObject* object2) { return object1->GetZIndex() < object2->GetZIndex(); });
			_neeedObjectSorting = false;
		}
		
		for (GameObject* object : _objects)
			if (object->IsActive())
				object->DrawCall(window);
	}

	void GameObjectHandler::NeedUpdateSorting()	{ _neeedObjectSorting = true; }
	void GameObjectHandler::Clear()
	{
		std::vector<GameObject*> objects = _objects;
		for (GameObject* object : objects)
			delete object;

		_objects.clear();
		_objectsToAdd.clear();
		_objectsToDestroy.clear();
	}

	GameObject* GameObjectHandler::GetObject(const std::string& name)
	{
		for (auto& object : _objects)
			if (object->GetName() == name) return object;

		return nullptr;
	}

	void GameObjectHandler::_ClearObjectsToDestroy()
	{
		for (GameObject* object : _objectsToDestroy)
		{
			delete object;
			_objects.erase(std::find(_objects.begin(), _objects.end(), object));
		}

		_objectsToDestroy.clear();
		NeedUpdateSorting();
	}
	void GameObjectHandler::_AddObjectsToAdd()
	{
		for (GameObject* object : _objectsToAdd)
			_objects.push_back(object);

		_objectsToAdd.clear();
		NeedUpdateSorting();
	}
}
