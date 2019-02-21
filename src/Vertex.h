/*!
* \file Vertex.h
* \brief Contain different basic structures.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

#include <GL/glew.h>

namespace NS2 {

	/*!
	* \brief Position structure.
	*/
	struct Position 
	{
		float x; /* X coordinate */
		float y; /* Y coordinate */
	};

	/*!
	* \brief Color structure.
	*/
	struct  ColorRGBA8 {
		ColorRGBA8() : r(0), g(0), b(0), a(0) {}
		ColorRGBA8(GLubyte r, GLubyte g, GLubyte b, GLubyte a) : r(r), g(g), b(b), a(a) {}
		void setColor(GLubyte rr, GLubyte gg, GLubyte bb, GLubyte aa) {
			r = rr;
			g = gg;
			b = bb;
			a = aa;
		}

		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};

	/*!
	* \brief UV structure for texture position.
	*/
	struct UV 
	{
		float u;
		float v;
	};


	/*!
	* \brief Simple 2D Vertex structure.
	*/
	struct Vertex 
	{
		Position position;	// Vertex position
		ColorRGBA8 color;	// Vertex color
		UV uv;				// UV texture coordinates

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a){
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void setPosition(float x,  float y) {
			position.x = x;
			position.y = y;
		}

		void setUV(float u, float v) {
			uv.u = u;
			uv.v = v;
		}
	};
}