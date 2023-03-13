#include "app.h"
#include "window.h"

App::App() {

}

void App::Run() {
	glfwInit();

	window_ = new Window("Raytracing!", 800, 800);
	window_->MakeContextCurrent();

	while (!window_->ShouldClose()) {
		window_->Update();
	}
}

App::~App() {

}