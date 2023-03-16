#include "shader.h"

#include <stdint.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <gtc/type_ptr.hpp>


Shader::Shader(const char* vertex_path, const char* fragment_path, const char* geometry_path)
{
	GLint success;
	GLchar infoLog[1024];
	std::string vertex_code = ReadFileToStr(vertex_path);
	std::string fragment_code = ReadFileToStr(fragment_path);
	uint32_t vertex_program = CreateShaderProgram(vertex_code, GL_VERTEX_SHADER);
	uint32_t fragment_program = CreateShaderProgram(fragment_code, GL_FRAGMENT_SHADER);
	uint32_t geometry_program;
	program_id_ = glCreateProgram();

	glAttachShader(program_id_, vertex_program);
	glAttachShader(program_id_, fragment_program);

	if (geometry_path)
	{
		std::string geometry_code = ReadFileToStr(geometry_path);
		geometry_program = CreateShaderProgram(geometry_code, GL_GEOMETRY_SHADER);
		glAttachShader(program_id_, geometry_program);

	}

	glLinkProgram(program_id_);
	glGetProgramiv(program_id_, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(program_id_, 1024, NULL, infoLog);
		std::cout << "Unable to link shaders. Log:" << "\n" << infoLog << "\n";
	}

	glDeleteShader(vertex_program);
	glDeleteShader(fragment_program);

	if (geometry_path)
	{
		glDeleteShader(geometry_program);
	}
}


Shader::Shader(const char* compute_path) {
	GLint success;
	GLchar infoLog[1024];
	std::string compute_code = ReadFileToStr(compute_path);
	uint32_t compute_program = CreateShaderProgram(compute_code, GL_COMPUTE_SHADER);
	program_id_ = glCreateProgram();
	glAttachShader(program_id_, compute_program);
	glLinkProgram(program_id_);
	glGetProgramiv(program_id_, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(program_id_, 1024, NULL, infoLog);
		std::cout << "Unable to link shaders. Log:" << "\n" << infoLog << "\n";
	}

	glDeleteShader(compute_program);
}

void Shader::Use()
{
	glUseProgram(program_id_);
}

void Shader::SetBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(program_id_, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(program_id_, name.c_str()), (int)value);
}
void Shader::SetFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(program_id_, name.c_str()), value);
}
void Shader::SetV2f(const std::string& name, const glm::vec2& value)
{
	glUniform2fv(glGetUniformLocation(program_id_, name.c_str()), 1, &value[0]);
}
void Shader::SetV3f(const std::string& name, const glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(program_id_, name.c_str()), 1, &value[0]);
}
void Shader::SetV4f(const std::string& name, const glm::vec4& value)
{
	glUniform4fv(glGetUniformLocation(program_id_, name.c_str()), 1, &value[0]);
}
void Shader::SetM4x4f(const std::string& name, const glm::mat4& value)
{
	glUniformMatrix4fv(glGetUniformLocation(program_id_, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

Shader::~Shader() 
{
	glDeleteProgram(program_id_);
}

//Reads the contents of a file at @path, and returns those contents as a string.
std::string Shader::ReadFileToStr(const char* path)
{
	std::ifstream stream(path);
	if (!stream.is_open()) {
		std::cout << "Unable to open file: " << path << "\n";
		return "";
	}
	std::stringstream str_buffer;
	str_buffer << stream.rdbuf();
	std::string ret = str_buffer.str();
	stream.close();
	return ret;
}

//Given the code to a shader, and its type, creates the opengl shader object.
//Returns the id of the shader object.
uint32_t Shader::CreateShaderProgram(std::string source_code, int shader_type) {
	uint32_t program;
	GLint success;
	GLchar infoLog[1024];
	const char* source_code_c = source_code.c_str();
	program = glCreateShader(shader_type);
	glShaderSource(program, 1, &source_code_c, NULL);
	glCompileShader(program);
	glGetShaderiv(program, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(program, 1024, NULL, infoLog);
		std::cout << "Unable to compile shader. Error log:" << "\n" << infoLog << "\n";
	}
	return program;
}