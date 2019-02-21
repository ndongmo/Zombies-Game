/*!
* \file SpriteBatch.h
* \brief Draws batched quads using indices.
* \author Ndongmo Silatsa Fabrice
* \date 27-10-2016
* \version 1.0
*/

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "Vertex.h"

namespace NS2 
{
	/*!
	* \brief Different kind of Glyp sorting.
	*/
	enum class GlyphSortType { NONE, FRONT_TO_BACK, BACK_TO_FRONT, TEXTURE };

	/*!
	* \brief Glypg : Single quad.
	*/
	class Glyph 
	{
	public:
		Glyph(const glm::vec4& destRet, const glm::vec4& uvRect, 
			GLuint textureID, float depth, const ColorRGBA8& color, float angle);

		Glyph(const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& uvRect,
			GLuint textureID, float depth, const ColorRGBA8& color, float angle);

		Glyph(const glm::vec4& destRet, const glm::vec4& uvRect, 
			GLuint textureID, float depth, const ColorRGBA8& color);

		Glyph(const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& uvRect,
			GLuint textureID, float depth, const ColorRGBA8& color);

		Glyph(const std::vector<glm::vec2>& points, const glm::vec4& uvRect,
			GLuint textureID, float depth, const ColorRGBA8& color);

		GLuint textureID;	/* Glyph texture ID */
		float depth;		/* Glyph deth */
		Vertex topLeft;		/* Glyph top left corner */
		Vertex bottomLeft;	/* Glyph bottom left corner */
		Vertex bottomRight;	/* Glyph bottom right corner */
		Vertex topRight;	/* Glyph top right corner */

	private:
		/*!
		* \brief Rotate a point by specified angle.
		* \param point: point to rotate
		* \return rotated point
		*/
		glm::vec2 rotatePoint(glm::vec2 point, float angle);
	};

	/*!
	* \brief Used to draw a texture. Each render batch is uded for a single draw call.
	*/
	class RenderBatch
	{
	public:
		RenderBatch(GLuint offset, GLuint numVertices, GLuint textureID) :
			offset(offset),
			numVertices(numVertices),
			textureID(textureID)
		{};
		GLuint offset;		/* offset value */
		GLuint numVertices; /* nomber of vertices */
		GLuint textureID;		/* texture ID */
	};

	class SpriteBatch
	{
	public:
		/*!
		* \brief Initialize spriteBatch stuffs.
		*/
		void init();
		/*!
		* \brief Clean buffer and vertex arrays.
		*/
		void dispose();
		/*!
		* \brief Prepare render stuffs before drawing.
		* \param sortType: sort mode
		*/
		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
		/*!
		* \brief Create all needed render batches.
		*/
		void end();
		/*!
		* \brief Create Glyph element.
		* \param destRet: coordinates of quad to draw
		* \param uvRect: texture coordinates
		* \param textureID: texture ID
		* \param depth
		* \param color
		*/
		void draw(const glm::vec4& destRet, const glm::vec4& uvRect, 
			GLuint textureID, float depth, const ColorRGBA8& color);
		void draw(const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& uvRect,
			GLuint textureID, float depth, const ColorRGBA8& color);

		/*!
		* \brief Create Glyph element.
		* \param destRet: coordinates of quad to draw
		* \param uvRect: texture coordinates
		* \param textureID: texture ID
		* \param depth
		* \param color
		* \param angle
		*/
		void draw(const glm::vec4& destRet, const glm::vec4& uvRect, 
			GLuint texture, float depth, const ColorRGBA8& color, float angle);
		void draw(const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& uvRect,
			GLuint texture, float depth, const ColorRGBA8& color, float angle);

		/*!
		* \brief Create Glyph element.
		* \param destRet: coordinates of quad to draw
		* \param uvRect: texture coordinates
		* \param textureID: texture ID
		* \param depth
		* \param color
		* \param direction
		*/
		void draw(const glm::vec4& destRet, const glm::vec4& uvRect, GLuint texture, 
			float depth, const ColorRGBA8& color, const glm::vec2& direction);

		/*!
		* \brief Create Glyph element.
		* \param points: set of 4 points
		* \param uvRect: texture coordinates
		* \param textureID: texture ID
		* \param depth
		* \param color
		*/
		void draw(const std::vector<glm::vec2>& points, const glm::vec4& uvRect,
			GLuint textureID, float depth, const ColorRGBA8& color);

		/*!
		* \brief Apply all render batches.
		*/
		void renderBatch();

	private:
		/*!
		* \brief Create all needed render batches.
		*/
		void createRenderBatches();
		/*!
		* \brief Create vertex array.
		*/
		void createVertexArray();
		/*!
		* \brief Sort Glyph array.
		*/
		void sortGlyph();

		/* COMPARATOR */
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);

		GlyphSortType m_sortType;					/* Current sort mode */
		GLuint m_vbo = 0;							/* vertex buffer object */
		GLuint m_vao = 0;							/* vertex array object */
		std::vector<Glyph*> m_glyphsPointers;		/* Pointers of glyphs for sorting */
		std::vector<Glyph> m_glyphs;				/* Array of glyphs */
		std::vector<RenderBatch> m_renderBatches;	/* Array of render batches */
	};
}