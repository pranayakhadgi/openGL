#ifndef SHADER_H
#define SHADER_H

#include <glad>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int ID; // our program ID

	Shader(const char* vertexPath, const char* fragmentPath);
	void use();


	//the shader utility uniform function generalized!!
	void setBool(const std::string &name, boool value)
		const;
	void setInt(const std::string& name, int value)
		const;
	void setFloat(const std::string& name, float value)
		const;
};
