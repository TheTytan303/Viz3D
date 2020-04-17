#pragma once
#include <bitset>
#include <queue>
class Mouse {

	friend class Window;
public:
	class Event {
	public:
		enum class Type {
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid
		};
	private:
		Type type;
		bool leftIsPressed;
		bool rightIsPressed;
		int x, y;
	public:
		Event() noexcept
			:
			type(Type::Invalid),
			leftIsPressed(false),
			rightIsPressed(false),
			x(0),
			y(0)
		{}
		Event(Type type, const Mouse& parent) noexcept
			:
			type(type),
			leftIsPressed(parent.leftIsPressed),
			rightIsPressed(parent.rightIsPressed),
			x(parent.x),
			y(parent.y)
		{}
		bool IsValid() const noexcept {
			return type != Type::Invalid;
		}
		Type GetType() const noexcept {
			return type;
		}
		std::pair<int, int> GetPos() const noexcept {
			return { x,y };
		}
		int GetPosY() const noexcept {
			return y;
		}
		int GetPosX() const noexcept {
			return x;
		}
		bool LeftIsPressed() const noexcept{
			return leftIsPressed;
		}
		bool RightIsPressed() const noexcept {
			return rightIsPressed;
		}
	};

	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	//current mouse:
	std::pair<int, int> GetPos() const noexcept;
	int GetX() const noexcept;
	int GetY() const noexcept;
	int GetWheelDelta() const noexcept;
	bool LeftIsPressed() const noexcept;
	bool RightIsPressed() const noexcept;
	bool IsInWidnow() const noexcept { return isInWindow; }
	bool IsEmpty()const noexcept { return buffer.empty(); };
	void flush() noexcept;
	Event Read() noexcept;
	
private:
	void OnMouseMove(int x, int y) noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftRelease(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightRelease(int x, int y) noexcept;
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;

	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;

	 void TrimBuffer()  noexcept;

	 static constexpr unsigned int bufferSize = 16u;
	 int x, y;
	 bool leftIsPressed = false;
	 bool rightIsPressed = false;
	 bool isInWindow = false;
	 std::queue<Event> buffer;
	 int wheelDeltaAcc = 0;

};