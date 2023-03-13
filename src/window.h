#ifndef WINDOW_H_
#define WINDOW_H_

#include <string>
#include <functional>
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "input.h"

class Window {

public:
	enum class ScreenMode {
		kFullscreen,
		kMaximized,
		kNormal,
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

private:
	GLFWwindow *window_handle_;
	ScreenMode screen_mode_;
	glm::vec2 framebuffer_size_;
	glm::vec2 window_size_;
	bool has_resized_;
	input::CharCallbackFn char_callback_;
	input::KeyCallbackFn key_callback_;

};

#endif