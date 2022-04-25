#pragma once
namespace sf
{
	class RenderWindow;
	class Event;
}
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <mutex>
#include <array>

#include "../Types/Vector2.h"

namespace maoutch
{
	class InputHandler
	{
	public:
		static InputHandler* Instance();

		InputHandler(InputHandler const&) = delete;
		void operator=(InputHandler const&) = delete;

		void SetWindow(sf::RenderWindow* window);
		void ClearLastFrameEvents();
		void ProcessEvent(const sf::Event& event);

		// Window Properties

		bool IsWindowFocused() const;

		// KeyBoard Properties

		bool IsKey(sf::Keyboard::Key key) const;
		bool IsKeyDown(sf::Keyboard::Key key) const;
		bool IsKeyUp(sf::Keyboard::Key key) const;
		bool Ctrl() const;
		bool Alt() const;
		bool Shift() const;

		// Mouse Properties

		Vector2 GetMousePosition() const;

		bool IsPointer(sf::Mouse::Button button) const;
		bool IsPointerDown(sf::Mouse::Button button) const;
		bool IsPointerUp(sf::Mouse::Button button) const;

	protected:
		InputHandler();
		~InputHandler();

	private:
		static InputHandler* _instance;
		static std::mutex _mutex; // For allowing multithreaded use

		sf::RenderWindow* _window = nullptr;

		// Window Properties

		bool _windowHasFocus;

		// KeyBoard

		std::array<bool, sf::Keyboard::KeyCount> _keyboardState = {};
		std::array<bool, sf::Keyboard::KeyCount> _oldKeyboardState = {};
		bool _keyCtrl;
		bool _keyAlt;
		bool _keyShift;

		// Mouse

		Vector2 _mousePosition;
		std::array<bool, sf::Mouse::ButtonCount> _mouseState = {};
		std::array<bool, sf::Mouse::ButtonCount> _oldMouseState = {};
	};
}
