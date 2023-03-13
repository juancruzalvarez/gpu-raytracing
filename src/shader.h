#ifndef SHADER_H_
#define SHADER_H_

#include <stdint.h>
#include <string>

#include <glm.hpp>

// Encapsulates data and provides functions for creating and using OpenGL shaders.
// Before using the shader for rendering, or before setting uniforms, its nescesary to call the Use function.
// NOTE: 
//	currently if there is an error creating the shader, it outputs the err to stdout.
//	maybe make it so Shader class has a valid or correct variable, to determine if the shader was created succesfully.
//	and then the creator of the shader can determinate what to do, and what to do with the err msg.
//	Right now u dont khow if the shader was created succesfully.
class Shader
{

public:
	// NOTE: maybe change const char* to be const std::string&, doesn't really matter.
	Shader(const char* vertex_path, const char* fragment_path, const char* geometry_path = nullptr);
	Shader(const char* compute_path);

	// Activates the shader for use.
	// Subsecuent OpenGL functions that make use of shaders, will use this one.
	void Use();

	// Functions for setting uniform variables in shaders.
	void SetInt  (const std::string& name, int value);
	void SetBool (const std::string& name, bool value);
	void SetFloat(const std::string& name, float value);
	void SetV2f  (const std::string& name, const glm::vec2& value);
	void SetV3f  (const std::string& name, const glm::vec3& value);
	void SetV4f  (const std::string& name, const glm::vec4& value);
	void SetM4x4f(const std::string& name, const glm::mat4& value);

	~Shader();

private:

	uint32_t program_id_;

	// Reads the contents of a file at @path, and returns those contents as a string.
	std::string ReadFileToStr(const char* path);

	// Given the code to a shader, and its type, creates the opengl shader object.
	// Returns the id of the shader object.
	uint32_t CreateShaderProgram(std::string source_code, int shader_type);

};

#endif
