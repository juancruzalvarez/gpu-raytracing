#ifndef APP_H
#define APP_H

#include "window.h"
#include "shader.h"
#include "fp_camera.h"

class App {

public:

App();
~App();

void Run();

private:


const double kPi = 3.1415926535897932;

//Field of view, expressed in radians.
const double kFOV = kPi / 6.0;

const glm::ivec2 kImageResolution = glm::vec2{1000, 600};


Window *window_;

//Compute shader, responsable of rendering the scene to the img_2d.
Shader *raytracing_shader_;

//Regular shader, responsable of rendering the img_2d to the screen.
Shader *render_img_shader_;

//OpenGL img2d object id.
//The id of the image where the scene gets rendered on the compute shader,
//and its then displayed to the screen by the regular shader.
uint32_t img_id_;

FPCamera camera_;

};


#endif