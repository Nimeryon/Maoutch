#include <SFML/Graphics.hpp>

#include "GameObjectHandler.h"
#include "GameObject.h"

namespace maoutch
{
	GameObjectHandler* GameObjectHandler::instance = nullptr;

	GameObjectHandler::GameObjectHandler() : _neeedObjectSorting(false), _needObjectDeleting(false)
	{
		if (instance != nullptr) delete instance;
		instance = this;
	};
	GameObjectHandler::~GameObjectHandler()
	{
		if (_needObjectDeleting)
		{
			_ClearObjectsToDestroy();
			_needObjectDeleting = false;
		}
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
			object->ProcessInputs();
	}

	void GameObjectHandler::EarlyUpdate(float dt)
	{
		for (GameObject* object : _objects)
			object->EarlyUpdate(dt);
	}
	void GameObjectHandler::Update(float dt)
	{
		for (GameObject* object : _objects)
			object->Update(dt);
	}
	void GameObjectHandler::FixedUpdate(float dt)
	{
		for (GameObject* object : _objects)
			object->FixedUpdate(dt);
	}
	void GameObjectHandler::LateUpdate(float dt)
	{
		for (GameObject* object : _objects)
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
			object->DrawCall(window);
	}

	void GameObjectHandler::NeedUpdateSorting()	{ _neeedObjectSorting = true; }

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
