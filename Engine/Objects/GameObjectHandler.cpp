#include <SFML/Graphics.hpp>

#include "GameObjectHandler.h"
#include "GameObject.h"

namespace maoutch
{
	GameObjectHandler* GameObjectHandler::_instance = nullptr;
	std::mutex GameObjectHandler::_mutex;

	GameObjectHandler::GameObjectHandler() : _neeedObjectSorting(false), _needObjectDeleting(false), _needObjectAdding(false) {}
	GameObjectHandler::~GameObjectHandler() { Clear(); }

	GameObjectHandler* GameObjectHandler::Instance()
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
	void GameObjectHandler::AddTranform(ITransformable* transform)
	{
		_transformables.push_back(std::shared_ptr<ITransformable>(transform));
	}
	void GameObjectHandler::Destroy(GameObject* object)
	{
		_needObjectDeleting = true;

		object->RemoveFromParent();
		for (GameObject* childObject : object->childrens)
			Destroy(childObject);

		_objectsToDestroy.push_back(object);
	}
	void GameObjectHandler::NotifyDestroy(ITransformable* iTransformable)
	{
		onITransformableDestroy(iTransformable);
		_transformables.erase(std::find(_transformables.begin(), _transformables.end(), iTransformable->shared_from_this()));
	}

	void GameObjectHandler::ProcessObjectsAdding()
	{
		if (_needObjectAdding)
		{
			_needObjectAdding = false;
			_AddObjectsToAdd();
		}
	}
	void GameObjectHandler::ProcessObjectsDestroy()
	{
		if (_needObjectDeleting)
		{
			_needObjectDeleting = false;
			_ClearObjectsToDestroy();
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
		for (GameObject* object : _objects)
			delete object;

		_objects.clear();
		_objectsToAdd.clear();
		_objectsToDestroy.clear();
	}

	std::vector<GameObject*> GameObjectHandler::GetObjects(const std::string& name)
	{
		std::vector<GameObject*> objects = {};
		for (auto& object : _objects)
			if (object->GetName() == name)
				objects.emplace_back(object);

		return objects;
	}

	void GameObjectHandler::_OnStateChange()
	{
		Clear();
	}

	void GameObjectHandler::_ClearObjectsToDestroy()
	{
		for (GameObject* object : _objectsToDestroy)
		{
			object->OnDestroy();
			_objects.erase(std::find(_objects.begin(), _objects.end(), object));

			NotifyDestroy(object);
		}

		_objectsToDestroy.clear();
		NeedUpdateSorting();
	}
	void GameObjectHandler::_AddObjectsToAdd()
	{
		for (GameObject* object : _objectsToAdd)
			_objects.push_back(object);

		for (GameObject* object : _objectsToAdd)
			object->Init();

		_objectsToAdd.clear();
		NeedUpdateSorting();
	}
}
