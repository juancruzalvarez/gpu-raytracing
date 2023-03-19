#include "window.h"

#include <iostream>
#include <functional>
#include <chrono>

#include <GLFW/glfw3.h>

Window::Window(std::string title, int width, int height, bool fullscreen) {
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window_handle_ = glfwCreateWindow(
		width,
		height,
		title.c_str(),
		fullscreen ? glfwGetPrimaryMonitor() : nullptr,
		nullptr
	);

	glfwSetWindowUserPointer(window_handle_, this);

	glfwSetFramebufferSizeCallback(
		window_handle_,
		[](GLFWwindow *window, int width, int height){
			Window* usr_ptr = (Window*) glfwGetWindowUserPointer(window);
			usr_ptr->framebuffer_size_ = glm::vec2(width, height);
			usr_ptr->has_resized_ = true;
		}
	);

	glfwSetWindowSizeCallback(
		window_handle_,
		[](GLFWwindow* window, int width, int height) {
			Window* usr_ptr = (Window*) glfwGetWindowUserPointer(window);
			usr_ptr->window_size_ = glm::vec2(width, height);
			usr_ptr->has_resized_ = true;
		}
	);

	glfwSetWindowMaximizeCallback(
		window_handle_,
		[](GLFWwindow* window, int maximized) {
			Window* usr_ptr = (Window*) glfwGetWindowUserPointer(window);
			usr_ptr->screen_mode_ = maximized ? ScreenMode::kMaximized : ScreenMode::kNormal;
		}
	);

	framebuffer_size_ = { width, height };
	window_size_ = { width, height };
	has_resized_ = true;
	screen_mode_ = fullscreen ? ScreenMode::kFullscreen : ScreenMode::kNormal;
	frame_elapsed_time_ = 0;
	last_frame_time_ = std::chrono::high_resolution_clock::now();
	glfwGetCursorPos(window_handle_, &mouse_position_.x, &mouse_position_.y);
	mouse_movement_ = { 0,0 };
}

void Window::SetKeyCallback(input::KeyCallbackFn callback) {
	key_callback_ = callback;
	glfwSetKeyCallback(
		window_handle_,
		[](GLFWwindow* window, int key, int scancode, int action, int modifiers) {
			input::Action a;
			switch (action) {
			case GLFW_PRESS: a = input::Action::kPressed; break;
			case GLFW_RELEASE: a = input::Action::kReleased; break;
			case GLFW_REPEAT: a = input::Action::kRepeat; break;
			default: return;
			}
			Window* usr_ptr = (Window*) glfwGetWindowUserPointer(window);
			usr_ptr->key_callback_(key, scancode, a, modifiers);
		}
	);
}

void Window::SetCharCallback(input::CharCallbackFn callback) {
	char_callback_ = callback;
	glfwSetCharCallback(
		window_handle_,
		[](GLFWwindow* window, uint32_t character) {
			Window* usr_ptr = (Window*) glfwGetWindowUserPointer(window);
			usr_ptr->char_callback_(character);
		}
	);
}

void Window::MakeContextCurrent() {
	glfwMakeContextCurrent(window_handle_);
}

bool Window::ShouldClose() {
	return glfwWindowShouldClose(window_handle_);
}

bool Window::HasResized() {
	return has_resized_;
}

void Window::Update() {
	has_resized_ = false;

	glfwPollEvents();
	glfwSwapBuffers(window_handle_);

	glm::dvec2 new_mouse_pos;
	glfwGetCursorPos(window_handle_, &new_mouse_pos.x, &new_mouse_pos.y);
	new_mouse_pos.y = window_size_.y - new_mouse_pos.y;
	mouse_movement_ = new_mouse_pos - mouse_position_;
	mouse_movement_ /= window_size_;
	mouse_position_ = new_mouse_pos;

	auto current_time = std::chrono::high_resolution_clock::now();
	//times a 1000 to convert seconds to ms.
	frame_elapsed_time_ = std::chrono::duration<double>(current_time-last_frame_time_).count()*1000;
	last_frame_time_ = current_time;
}

void Window::SetScreenMode(ScreenMode screen_mode) {
	switch (screen_mode)
	{
		case ScreenMode::kFullscreen: {
			glfwSetWindowMonitor(window_handle_, glfwGetPrimaryMonitor(), 0, 0, window_size_.x, window_size_.y, GLFW_DONT_CARE);
			break;
		}
		case ScreenMode::kMaximized: {
			glfwSetWindowMonitor(window_handle_, nullptr, 0, 0, window_size_.x, window_size_.y, GLFW_DONT_CARE);
			glfwMaximizeWindow(window_handle_);
			break;
		}
		case ScreenMode::kNormal: {
			glfwSetWindowMonitor(window_handle_, nullptr, 0, 0, window_size_.x, window_size_.y, GLFW_DONT_CARE);
			break;
		}
	}
}

void Window::SetVisible(bool visible) {
	visible ? glfwShowWindow(window_handle_) : glfwHideWindow(window_handle_);
}

glm::vec2 Window::FramebufferSize() {
	return framebuffer_size_;
}

glm::vec2 Window::WindowSize() {
	return window_size_;
}

bool Window::IsKeyPressed(int glfw_key_code) {
	return glfwGetKey(window_handle_, glfw_key_code) == GLFW_PRESS;
}

void Window::SetMouseMode(MouseMode mode) {
	mouse_mode_ = mode;
	switch (mode) {

	case MouseMode::kNormal: {
		glfwSetInputMode(window_handle_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;
	}
	case MouseMode::kHiddenCenter: {
		glfwSetInputMode(window_handle_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		break;
	}
	case MouseMode::kHidden: {
		glfwSetInputMode(window_handle_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		break;
	}
	}
}

Window::MouseMode Window::CurrentMouseMode() {
	return mouse_mode_;
}

float Window::FrameTime() { return frame_elapsed_time_; }

// Returns the current position of the mouse, in screen coordinates. relative to bottom left corner.
glm::vec2 Window::MousePosition() {
	return mouse_position_;
}

// Returns the movement of the mouse, scaled to a -1:1 rangue relative to size of the window.
glm::vec2 Window::MouseMovement() {
	return mouse_movement_;
}