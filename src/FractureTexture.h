#ifndef FractureTexture_H
#define FractureTexture_H

#include <Windows.h>
#include <GL/glew.h>
#include <iostream>
#include "IFracReferenceCounter.h"

namespace Fracture
{
	//! @class Texture Clase encargada de cargar, gestionar y visualizar texturas

	class FractureTexture: virtual public IFractureReferenceCounter
	{
		public:
			//! @brief Contructor de la clase
			FractureTexture();
			//! @brief Destructor de la clase
			~FractureTexture();

			//! @brief Creamos una textura del archivo dado
			//! @param path Ruta donde se encuentra la textura
			//! @return True si la textura se ha creado correctamente o False en caso contrario
			bool LoadTextureFromFile(std::string path);

			//! @brief Cargamos una textura del cubemap del archivo dado
			//! @param path Ruta donde se encuentra la textura
			//! @param sideTarget Lado del cubemap al que pertence la textura
			//! @return True si la textura se ha creado correctamente o False en caso contrario
			bool LoadCubeTextureMapFromFile(std::string path, GLenum sideTarget);

			//! @brief Creamos una textura de los pixeles dados
			//! @param pixels Nº de pixels
			//! @param width ancho de la imagen 
			//! @param height alto de la imagen
			//! @return True si la textura se ha creado correctamente o False en caso contrario
			bool LoadTextureFromPixels(GLuint* pixels, GLuint width, GLuint height);

			//! @brief Creamos una textura de los pixeles dados
			//! @param pixels Nº de pixels
			//! @param width ancho de la imagen 
			//! @param height alto de la imagen
			//! @param sideTarget Lado del cubemap al que pertence la textura
			//! @return True si la textura se ha creado correctamente o False en caso contrario
			bool LoadTextureFromPixels(GLuint* pixels, GLuint width, GLuint height, GLenum sideTarget);

			//! @brief Elimina la textura si existe
			void FreeTexture();

			//! @brief Traslada a la posicion dada y renderiza la textura en el quad
			//! @param x posicion x a trasladar
			//! @param y posicion y a trasladar
			void Render(GLfloat x, GLfloat y);

			//! @brief Devuelve el ID de la textura
			GLuint GetTextureID();

			//! @brief Devuelve el ancho de la textura
			GLuint GetTextureWidth();

			//! @brief Devuelve el alto de la textura
			GLuint GetTextureHeight();	

			//! @brief Bindea la textura al contexto OpenGL
			void Use(GLenum activeTexture = GL_TEXTURE0);

		private:
		
			//! @brief Variable que almacenará el ID de la textura
			GLuint m_textureID;

			//! @brief Variable que almacenará el ancho de la textura
			GLuint m_textureWidth;

			//! @brief Variable que almacenará el alto de la textura
			GLuint m_textureHeight;
	};
}

#endif