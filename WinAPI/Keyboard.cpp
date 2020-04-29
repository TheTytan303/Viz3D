#pragma once
#include "Keyboard.h"
#include <bitset>
#include <queue>


/*
	obs³uga klawiatury z poziomu WinAPI
*/

bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept {
	return keystates[keycode];
};
Keyboard::Event Keyboard::ReadKey() noexcept {
	if (keybuffer.size() > 0u) {
		Keyboard::Event e = keybuffer.front();
		keybuffer.pop();
		return e;
	}
	else {
		return Keyboard::Event();
	}
};
bool Keyboard::KeyIsEmpty() const noexcept {
	return keybuffer.empty();
};
void Keyboard::FlushKey() noexcept {
	keybuffer = std::queue<Event>();
};
char Keyboard::ReadChar() noexcept {
	if (keybuffer.size() > 0u) {
		unsigned char returnVale = charbuffer.front();
		charbuffer.pop();
		return returnVale;
	}
	else {
		return 0;
	}
};
bool Keyboard::CharIsEmpty() const noexcept {
	return charbuffer.empty();
};
void Keyboard::FlushChar() noexcept {
	charbuffer = std::queue<char>();
};
void Keyboard::Flush() noexcept {
	FlushKey();
	FlushChar();
};
void Keyboard::EnableAutorepead() noexcept {
	autorepeatEnabled = true;
};
void Keyboard::DisableAutorepead() noexcept {
	autorepeatEnabled = false;
};;
bool Keyboard::AutorepeadisEnabled() const noexcept {
	return autorepeatEnabled;
};

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept {
	keystates[keycode] = true;
	keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
	TrimBuffer(keybuffer);
};
void Keyboard::OnKeyReleased(unsigned char keycode) noexcept {
	keystates[keycode] = false;
	keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
	TrimBuffer(keybuffer);
};
void Keyboard::OnChar(char character)  noexcept {
	charbuffer.push(character);
	TrimBuffer(charbuffer);
};
void Keyboard::ClearState()  noexcept {
	keystates.reset();
};
template<typename T>
static void Keyboard::TrimBuffer(std::queue<T>& buffer)  noexcept {
	while (buffer.size() > Keyboard::bufferSize) {
		buffer.pop();
	}
};