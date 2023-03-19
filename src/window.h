#ifndef WINDOW_H_
#define WINDOW_H_

#include <string>
#include <functional>
#include <chrono>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "input.h"

class Window {

public:
	enum class ScreenMode {
		kNormal,
		kMaximized,
		kFullscreen,
	};

	enum class MouseMode {
		kNormal,
		kHidden,
		kHiddenCenter,
	};


	Window(std::string title, int width, int height, bool fullscreen = false);

	// Sets functions to be called when a char is inputed.
	void SetCharCallback(input::CharCallbackFn callback);

	// Sets function to be called when a key is pressed or released.
	void SetKeyCallback(input::KeyCallbackFn callback);

	// Sets the screen mode of the window.
	void SetScreenMode(ScreenMode screen_mode);

	// Makes the current context be that of the window.
	void MakeContextCurrent();

	// Makes the window be visible or invisible.
	void SetVisible(bool visible);

	//Returns the size of the framebuffer of the window, in pixels.
	glm::vec2 FramebufferSize();

	// Returns the size of the window. In general its equal to the framebuffer size, but in some mac devices it differs.
	glm::vec2 WindowSize();

	// Returns true when user events or OS events determinate that the window should be closed.
	bool ShouldClose();

	// Returns true the frame the window is resized. After Update gets called in a frame, the variable this function returns becomes false.
	bool HasResized();

	// Updates the window, polling for events and swaping the framebuffers.
	void Update();

	// Returns the cached state of the key asked about.
	bool IsKeyPressed(int glfw_key_code);

	void SetMouseMode(MouseMode mode);

	MouseMode CurrentMouseMode();

	// Returns the time between two frames, in miliseconds.
	float FrameTime();
	
	// Returns the current position of the mouse, in screen coordinates. relative to bottom left corner.
	glm::vec2 MousePosition();

	// Returns the movement of the mouse, scaled to a -1:1 rangue relative to size of the window.
	glm::vec2 MouseMovement();

private:
	GLFWwindow *window_handle_;

	ScreenMode screen_mode_;
	glm::vec2 framebuffer_size_;
	glm::vec2 window_size_;
	bool has_resized_;

	MouseMode mouse_mode_;
	glm::dvec2 mouse_position_;
	glm::vec2 mouse_movement_;

	input::CharCallbackFn char_callback_;
	input::KeyCallbackFn key_callback_;

	std::chrono::steady_clock::time_point last_frame_time_;
	float frame_elapsed_time_;

};

#endif