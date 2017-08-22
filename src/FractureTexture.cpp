#include "FractureTexture.h"
#include <IL/IL.h>
#include <IL/ilu.h>

using namespace Fracture;

FractureTexture::FractureTexture()
{
	m_textureID = 0;
	m_textureWidth = 0;
	m_textureHeight = 0;
}

FractureTexture::~FractureTexture()
{
	FreeTexture();
}

bool FractureTexture::LoadTextureFromFile(std::string path)
{
	bool textureLoaded = false;
	
	ILuint imgID = 0;
	ilGenImages(1, &imgID);
	ilBindImage(imgID);
	
	ILboolean correct = ilLoadImage(path.c_str());

	if (correct == IL_TRUE)
	{
		correct = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (correct == IL_TRUE)
		{
			textureLoaded = LoadTextureFromPixels((GLuint*)ilGetData(), (GLuint)ilGetInteger(IL_IMAGE_WIDTH), (GLuint)ilGetInteger(IL_IMAGE_HEIGHT));
		}

		ilDeleteImages(1, &imgID);
	}

	return textureLoaded;
}

bool FractureTexture::LoadTextureFromPixels(GLuint* pixels, GLuint width, GLuint height)
{
	FreeTexture();

	m_textureWidth = width;
	m_textureHeight = height;
	
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		return false;
	}

	return true;
}

void FractureTexture::FreeTexture()
{
	if (m_textureID != 0)
	{
		glDeleteTextures(1, &m_textureID);
		m_textureID = 0;
	}

	m_textureWidth = 0;
	m_textureHeight = 0;
}

void FractureTexture::Render(GLfloat x, GLfloat y)
{
	if (m_textureID != 0)
	{
		glLoadIdentity();	

		glTranslatef(x, y, 0.f);
	
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glBegin(GL_QUADS);
		glTexCoord2f(0.f, 0.f); glVertex2f(0.f, 0.f);
		glTexCoord2f(1.f, 0.f); glVertex2f(m_textureWidth, 0.f);
		glTexCoord2f(1.f, 1.f); glVertex2f(m_textureWidth, m_textureHeight);
		glTexCoord2f(0.f, 1.f); glVertex2f(0.f, m_textureHeight);
		glEnd();
		
	}
}


GLuint FractureTexture::GetTextureID()
{
	return m_textureID;
}

GLuint FractureTexture::GetTextureWidth()
{
	return m_textureWidth;
}

GLuint FractureTexture::GetTextureHeight()
{
	return m_textureHeight;
}

void FractureTexture::Use(GLenum activeTexture)
{
	glActiveTexture(activeTexture);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

