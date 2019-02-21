/*
This is a modified version of the SpriteFont class from the
Seed Of Andromeda source code.
Use it for any of your projects, commercial or otherwise,
free of charge, but do not remove this disclaimer.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS
ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
OF THIS SOFTWARE.

November 28 2014
Original Author: Cristian Zaloj
Modified By: Benjamin Arnold

October 27 2016
Modified By: Fabrice Ndongmo
*/

#pragma once
#pragma warning(disable : 4316)

#include "Vertex.h"

#include <map>
#include <vector>
#include <SDL/SDL_ttf.h>
#include <glm/glm.hpp>

namespace NS2
{
	struct GLTexture;
	class SpriteBatch;

	/*!
	* \brief Glyph (Quad) of a char.
	*/
	struct CharGlyph
	{
	public:
		char character;
		glm::vec4 uvRect;
		glm::vec2 size;
	};

#define FIRST_PRINTABLE_CHAR ((char)32)
#define LAST_PRINTABLE_CHAR ((char)126)

	/*!
	* \brief Different justification modes.
	*/
	enum class Justification 
	{
		LEFT, MIDDLE, RIGHT, MIDDLEX, RIGHTX, MIDDLEY, RIGHTY
	};

	class SpriteFont {
	public:

		/* INITIALIZERS */
		void init(const char* font, int size);
		void init(const char* font, int size, char cs, char ce);

		/*!
		* \brief Destroys the font resources.
		*/
		void dispose();

		int getFontHeight() const {
			return m_fontHeight;
		}

		/*!
		* \brief Measures the dimensions of the text.
		*/
		glm::vec2 measure(const char* s);
		/*!
		* \brief Applies justification.
		*/
		void justify(const char* s, glm::vec2& position, const glm::vec2& scaling, Justification just);
		void justify(const char* s, glm::vec4& destRect, Justification just);

		/*!
		* \brief Draws using a spritebatch.
		*/
		void draw(SpriteBatch& batch, const char* s, const glm::vec2& position, const glm::vec2& scaling,
			float depth, ColorRGBA8 tint, Justification just = Justification::LEFT);

		void draw(SpriteBatch& batch, const char* s, const glm::vec4& destRect,
			float depth, ColorRGBA8 tint, Justification just = Justification::LEFT);

	private:
		/*!
		* \brief Create rows for a desired text to draw.
		*/
		static std::vector<int>* createRows(glm::ivec4* rects, int rectsLength, int r, int padding, int& w);

		int m_regStart, m_regLength;	/* Rectangle coordinates of text */
		CharGlyph* m_glyphs;			/* Array of char glyphs */
		int m_fontHeight;				/* Height of font */
		unsigned int m_texID;			/* Text ID */
	};
}