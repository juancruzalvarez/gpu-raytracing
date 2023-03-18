#ifndef FP_CAMERA_H
#define FP_CAMERA_H

#include <glm.hpp>

class FPCamera {
public:

FPCamera() = default;
FPCamera(glm::vec3 position, glm::vec3 forwards = {0,0,-1}, glm::vec3 up = {0,1,0});
void Move(glm::vec3 ammount);
void Rotate(glm::vec3 ammount);
glm::vec3 Position();
glm::vec3 Up();
glm::vec3 Forwards();
glm::vec3 Right();

private:

// Position of the camera in the world.
glm::vec3 position_;
// Orientation vectors of the camera
// Right can be calculated from forwards and up vectors.
glm::vec3 forwards_, up_; 


glm::vec3 RotateVector(glm::vec3 vector_to_rotate, glm::vec3 axis, float ammount_in_radians);

};


#endif