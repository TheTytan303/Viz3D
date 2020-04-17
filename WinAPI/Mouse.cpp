#pragma once
#include <bitset>
#include <queue>
#include <Windows.h>
#include "Mouse.h"


//current mouse:
std::pair<int, int> Mouse::GetPos() const noexcept {
	return { x,y };
};
int  Mouse::GetX() const noexcept {
	return x;
};
int  Mouse::GetY() const noexcept {
	return y;
};
int  Mouse::GetWheelDelta() const noexcept {
	return wheelDeltaAcc;
};
bool Mouse::LeftIsPressed() const noexcept {
	return leftIsPressed;
}
bool Mouse::RightIsPressed() const noexcept {
	return rightIsPressed;
};
void Mouse::flush() noexcept {
	buffer = std::queue<Event>();
};
Mouse::Event Mouse::Read() noexcept {
	if (buffer.size() > 0u) {
		Mouse::Event returnVale = buffer.front();
		buffer.pop();
		return returnVale;
	}
	else {
		return Mouse::Event();
	}
};
void Mouse::OnMouseMove(int x, int y) noexcept {
	this->x = x;
	this->y = y;
	//Mouse::Event event(Mouse::Event::Type::Move, *this);
	//event.GetPosX();
	//event.GetType();
	buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	TrimBuffer();
};
void Mouse::OnLeftPressed(int x, int y) noexcept {
	leftIsPressed = true;
	buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
	TrimBuffer();
};
void Mouse::OnLeftRelease(int x, int y) noexcept{
	leftIsPressed = false;
	buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	TrimBuffer();
};
void Mouse::OnRightPressed(int x, int y) noexcept{
	rightIsPressed = true;
	buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
	TrimBuffer();
};
void Mouse::OnRightRelease(int x, int y) noexcept {
	rightIsPressed = false;
	buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	TrimBuffer();
}
void Mouse::OnMouseLeave() noexcept{
	isInWindow = false;
	buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
}
void Mouse::OnMouseEnter() noexcept{
	isInWindow = true;
	buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));

}
void Mouse::OnWheelUp(int x, int y) noexcept {
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	TrimBuffer();
};
void Mouse::OnWheelDown(int x, int y) noexcept {
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	TrimBuffer();
}
void Mouse::OnWheelDelta(int x, int y, int delta) noexcept {
	wheelDeltaAcc += delta;
	while (wheelDeltaAcc >= WHEEL_DELTA) {
		wheelDeltaAcc -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (wheelDeltaAcc <= -WHEEL_DELTA) {
		wheelDeltaAcc += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
};

void Mouse::TrimBuffer()  noexcept {
	while (buffer.size() > Mouse::bufferSize) {
		buffer.pop();
	}
};