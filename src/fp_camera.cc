#include "fp_camera.h"

#include <gtx/rotate_vector.hpp>

FPCamera::FPCamera(glm::vec3 position, glm::vec3 forwards, glm::vec3 up) {
	position_ = position;
	forwards_ = forwards;
	up_ = up;
}

void FPCamera::Move(glm::vec3 ammount) {
	auto right = glm::cross(forwards_, up_);
	position_ += right * ammount.x;
	position_ += up_ * ammount.y;
	position_ += forwards_ * ammount.z;
}

void FPCamera::Rotate(glm::vec3 ammount) {
	auto right = glm::cross(forwards_, up_);
	auto up = up_;
	auto forwards = forwards_;
	up_ = RotateVector(up_, right, ammount.x);
	forwards_ = RotateVector(forwards_, right, ammount.x);

	up_ = RotateVector(up_, forwards, ammount.z);
	forwards_ = RotateVector(forwards_, up, ammount.y);
}

glm::vec3 FPCamera::RotateVector(glm::vec3 vector_to_rotate, glm::vec3 axis, float ammount_in_radians) {
	return glm::rotate(vector_to_rotate, ammount_in_radians, axis);
}

glm::vec3 FPCamera::Position() { return position_; }
glm::vec3 FPCamera::Up() { return up_; }
glm::vec3 FPCamera::Forwards() { return forwards_; }
glm::vec3 FPCamera::Right() { return glm::cross(forwards_, up_); }

