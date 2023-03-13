#ifndef INPUT_H
#define INPUT_H

namespace input {

	enum class Action {
		kPressed,
		kReleased,
		kRepeat,
	};

	enum Modifiers {
		kShift = 1,
		kCtrl = 2,
		kAlt = 4,
		kSuper = 8,
		kCapsLock = 16,
		kNumLock = 32,
	};

	typedef std::function<void(int glfw_key_code, int scancode, Action action, int modifiers)> KeyCallbackFn;
	typedef std::function<void(uint32_t utf32_character)> CharCallbackFn;
	typedef std::function<void(glm::vec2 pos)> MouseMoveCallbackFn;
}

#endif