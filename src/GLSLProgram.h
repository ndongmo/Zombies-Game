/*!
* \file GLSLProgram.h
* \brief openGL shading language(GLSL) program manager.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

#include<string>
#include<GL/glew.h>

namespace NS2 
{
	class GLSLProgram
	{
	public:

		/*!
		* \brief Compile shader from vertex and fragment shader file program (GLSL).
		* \param vertexShaderFilePath : vertex shader file path
		* \param fragmentShaderFilePath : fragment shader file path
		*/
		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
		/*!
		* \brief Compile shader from vertex and fragment shader program array of char.
		* \param vertexSource : vertex shader program char array
		* \param fragmentSource : fragment shader program char array
		*/
		void compileShadersFromSource(const char* vertexSource, const char* fragmentSource);
		/*!
		* \brief Add attribute to program.
		* \param attributeName : name of attribute to add
		*/
		void addAttribute(const std::string& attributeName);
		/*!
		* \brief Get variable location of desired attribute.
		* \param uniformName : name of variable
		*/
		GLint getUniformLocation(const std::string& uniformName);
		/*!
		* \brief Link shaders to program.
		*/
		void linkShaders();
		/*!
		* \brief Prepare program for use.
		*/
		void use();
		/*!
		* \brief Disable the program.
		*/
		void unuse();
		/*!
		* \brief Delete created program.
		*/
		void dispose();

	private:
		/*!
		* \brief Compile shader from created shader ID and source code.
		* \param source : shader source code
		* \param name : name of shader
		* \param id : id of created shader
		*/
		void compileShaders(const char* source, const std::string& name, GLuint& id);

		int m_numAttributtes = 0;		/* Number of attributes */
		GLuint m_programID = 0;			/* Program ID */
		GLuint m_vertexShaderID = 0;	/* Vertex shader ID */
		GLuint m_fragmentShaderID = 0;	/* Fragment shader ID */
	};
}