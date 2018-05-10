#include "ShaderManager.hpp"

#include <vector>
#include <fstream>
#include <iostream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Parsers/ShaderParser.hpp"

namespace EVA
{
	std::map<FS::path, std::shared_ptr<Shader>> ShaderManager::m_Shaders;

	std::shared_ptr<Shader> ShaderManager::LoadShader(const FS::path& path)
	{
		if (m_Shaders.count(path))
			return m_Shaders[path];

		const auto pathList = ShaderParser::LoadShader(path);
		if (pathList == nullptr)
			return nullptr;

		const auto shaderProgram = glCreateProgram();

		if (!pathList->vertex.empty())
		{
			const auto vertexShader = LoadAndCompileShader(pathList->vertex, GL_VERTEX_SHADER);
			glAttachShader(shaderProgram, vertexShader);
			glDeleteShader(vertexShader);
		}

		if (!pathList->fragment.empty())
		{
			const auto fragmentShader = LoadAndCompileShader(pathList->fragment, GL_FRAGMENT_SHADER);
			glAttachShader(shaderProgram, fragmentShader);
			glDeleteShader(fragmentShader);
		}

		if (!pathList->geometry.empty())
		{
			const auto geometryShader = LoadAndCompileShader(pathList->geometry, GL_GEOMETRY_SHADER);
			glAttachShader(shaderProgram, geometryShader);
			glDeleteShader(geometryShader);
		}

		glLinkProgram(shaderProgram);
		glUseProgram(shaderProgram);

		const auto shader = std::make_shared<Shader>(shaderProgram, pathList);

		m_Shaders[path] = shader;
		return shader;
	}

	void ShaderManager::SaveShader(Shader* shader, const FS::path& path)
	{
		Json::Document d;
		d.SetObject();

		auto& a = d.GetAllocator();

		DataObject data(d, &a);

		if (!shader->paths->vertex.empty())
			data.SetString("vertex", FileSystem::ToString(shader->paths->vertex));

		if (!shader->paths->fragment.empty())
			data.SetString("fragment", FileSystem::ToString(shader->paths->fragment));

		if (!shader->paths->geometry.empty())
			data.SetString("geometry", FileSystem::ToString(shader->paths->geometry));

		Json::Save(&d, path);
	}

	void ShaderManager::ReadShaderSource(const FS::path& path, std::vector<char> &buffer)
	{
		std::ifstream in;
		in.open(FileSystem::ToString(path).c_str(), std::ios::binary);

		if (!in.fail())
		{
			// Get the number of bytes stored in this file
			in.seekg(0, std::ios::end);
			const auto length = (size_t) in.tellg();

			// Go to start of the file
			in.seekg(0, std::ios::beg);

			// Read the content of the file in a buffer
			buffer.resize(length + 1);
			in.read(&buffer[0], length);
			in.close();

			// Add a valid C - string end
			buffer[length] = '\0';
		} 
		else
		{
			std::cerr << "Unable to open " << FileSystem::ToString(path).c_str() << " I'm out!" << std::endl;
			exit(-1);
		}
	}

	unsigned int ShaderManager::LoadAndCompileShader(const FS::path& path, const GLenum shaderType)
	{
		std::cout << "ShaderLoad::LoadAndCompileShader - " << FileSystem::ToString(path).c_str() << "\n";

		// Load a shader from an external file
		std::vector<char> buffer;
		ReadShaderSource(path, buffer);
		const char *src = &buffer[0];

		// Create shaders
		const auto shader = glCreateShader(shaderType);
		//attach the shader source code to the shader objec
		glShaderSource(shader, 1, &src, nullptr);

		// Compile the shader
		glCompileShader(shader);
		// Comile the shader, translates into internal representation and checks for errors.
		GLint compileOk;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileOk);
		if (!compileOk)
		{
			char infolog[1024];;
			glGetShaderInfoLog(shader, 1024, nullptr, infolog);
			std::cout << "The program failed to compile with the error:" << std::endl << infolog << std::endl;
			glfwTerminate();
			getchar();
			exit(-1);
		}
		return shader;
	}
}
