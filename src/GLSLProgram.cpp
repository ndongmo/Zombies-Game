#include "GLSLProgram.h"
#include "IOManager.h"
#include "Logger.h"

#include <fstream>
#include <vector>

namespace NS2 
{
	void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {

		std::string vertexSource;
		std::string fragSource;

		IOManager::readFileToBuffer(vertexShaderFilePath, vertexSource);
		IOManager::readFileToBuffer(fragmentShaderFilePath, fragSource);

		compileShadersFromSource(vertexSource.c_str(), fragSource.c_str());
	}

	void GLSLProgram::compileShadersFromSource(const char* vertexSource, const char* fragmentSource)
	{
		//Vertex and fragment shaders are successfully compiled.
		//Now time to link them together into a program.
		//Get a program object.
		m_programID = glCreateProgram();

		m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (m_vertexShaderID == 0) {
			logE << "Vertex shader failed to be created!" << std::endl;
		}

		m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (m_fragmentShaderID == 0) {
			logE << "Fragment shader failed to be created!" << std::endl;
		}

		compileShaders(vertexSource, "Vertex shader", m_vertexShaderID);
		compileShaders(fragmentSource, "Fragment shader", m_fragmentShaderID);
	}

	void GLSLProgram::linkShaders() {
		//Attach our shaders to our program
		glAttachShader(m_programID, m_vertexShaderID);
		glAttachShader(m_programID, m_fragmentShaderID);

		//Link our program
		glLinkProgram(m_programID);

		//Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_programID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);

			//We don't need the program anymore.
			glDeleteProgram(m_programID);
			//Don't leak shaders either.
			glDeleteShader(m_vertexShaderID);
			glDeleteShader(m_fragmentShaderID);

			logE << &(errorLog[0]) << std::endl;
			logE << "Shader failed to link!" << std::endl;
			Logger::exitGame();
		}

		//Always detach shaders after a successful link.
		glDetachShader(m_programID, m_vertexShaderID);
		glDetachShader(m_programID, m_fragmentShaderID);
		glDeleteShader(m_vertexShaderID);
		glDeleteShader(m_fragmentShaderID);
	}

	void GLSLProgram::addAttribute(const std::string& attributeName) {
		glBindAttribLocation(m_programID, m_numAttributtes++, attributeName.c_str());
	}

	GLint GLSLProgram::getUniformLocation(const std::string& uniformName) {
		GLint location = glGetUniformLocation(m_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX) {
			logE << "Uniform " << uniformName << " not found in shader!" << std::endl;
			Logger::exitGame();
		}
		return location;
	}

	void GLSLProgram::use() {
		glUseProgram(m_programID);
		for (int i = 0; i < m_numAttributtes; i++) {
			glEnableVertexAttribArray(i);
		}
	}

	void GLSLProgram::unuse(){
		glUseProgram(0);
		for (int i = 0; i < m_numAttributtes; i++) {
			glDisableVertexAttribArray(i);
		}
	}

	void GLSLProgram::dispose()
	{
		if (m_programID)
		{
			glDeleteProgram(m_programID);
		}
		m_numAttributtes = 0;
	}

	void GLSLProgram::compileShaders(const char* source, const std::string& name, GLuint& id) {

		glShaderSource(id, 1, &source, nullptr);

		glCompileShader(id);

		GLint isCompiled = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(id); // Don't leak the shader.

			logE << &(errorLog[0]) << std::endl;
			logE << "Shader " << name << " failed to compile!" << std::endl;
			Logger::exitGame();
		}
	}
}