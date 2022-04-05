#include <SFML/Graphics.hpp>

#include "InputHandler.h"

namespace maoutch
{
	InputHandler* InputHandler::_instance = nullptr;
	std::mutex InputHandler::_mutex;

	InputHandler::InputHandler() : _windowHasFocus(true), _keyCtrl(false), _keyAlt(false), _keyShift(false) {}
	InputHandler::~InputHandler() = default;

	InputHandler* InputHandler::GetInstance()
	{
		std::lock_guard lock(_mutex);

		if (_instance == nullptr) _instance = new InputHandler();
		return _instance;
	}
	
	void InputHandler::SetWindow(sf::RenderWindow* window) { _window = window; }
	void InputHandler::ClearLastFrameEvents()
	{
		_oldKeyboardState = _keyboardState;
		_oldMouseState = _mouseState;
	}
	void InputHandler::ProcessEvent(const sf::Event& event)
	{
		switch (event.type)
		{
			// Window Focus Events
			case sf::Event::LostFocus:
			{
				_keyboardState.fill(false);
				_mouseState.fill(false);
				_keyCtrl = false;
				_keyAlt = false;
				_keyShift = false;

				_windowHasFocus = false;
			}
			break;
			case sf::Event::GainedFocus:
				_windowHasFocus = true;
				break;
		}

		if (_windowHasFocus)
		{
			switch (event.type)
			{
				// Keyboard Events
			case sf::Event::KeyPressed:
			case sf::Event::KeyReleased:
			{
				int keyCode = event.key.code;
				if (keyCode >= 0 && keyCode < sf::Keyboard::KeyCount)
					_keyboardState[keyCode] = event.type == sf::Event::KeyPressed;
				_keyCtrl = event.key.control;
				_keyAlt = event.key.alt;
				_keyShift = event.key.shift;
			}
			break;

			// Mouse Button Events
			case sf::Event::MouseButtonPressed:
			case::sf::Event::MouseButtonReleased:
			{
				int buttonCode = event.key.code;
				_mouseState[buttonCode] = event.type == sf::Event::MouseButtonPressed;
			}
			break;

			// Mouse Move Event
			case sf::Event::MouseMoved:
			{
				sf::Vector2i mousePos = { event.mouseMove.x, event.mouseMove.y };
				_mousePosition = Vector2::From(_window->mapPixelToCoords(mousePos));
			}
			break;
			}
		}
	}

	// Window Properties
	
	bool InputHandler::IsWindowFocused() const { return _windowHasFocus; }

	// KeyBoard Properties
	
	bool InputHandler::IsKey(sf::Keyboard::Key key) const { return _keyboardState[key]; }
	bool InputHandler::IsKeyDown(sf::Keyboard::Key key) const { return _keyboardState[key] && !_oldKeyboardState[key]; }
	bool InputHandler::IsKeyUp(sf::Keyboard::Key key) const { return !_keyboardState[key] && _oldKeyboardState[key]; }
	bool InputHandler::Ctrl() const { return _keyCtrl; }
	bool InputHandler::Alt() const { return _keyAlt; }
	bool InputHandler::Shift() const { return _keyShift; }

	// Mouse Properties
	
	Vector2 InputHandler::GetMousePosition() const { return _mousePosition; }

	bool InputHandler::IsPointer(sf::Mouse::Button button) const { return _mouseState[button]; }
	bool InputHandler::IsPointerDown(sf::Mouse::Button button) const { return _mouseState[button] && !_oldMouseState[button]; }
	bool InputHandler::IsPointerUp(sf::Mouse::Button button) const { return !_mouseState[button] && _oldMouseState[button]; }
}
