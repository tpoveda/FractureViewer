#ifndef FractureCubeMap_H
#define FractureCubeMap_H

#include "GL/glew.h"
#include "FractureTexture.h"

using namespace Fracture;

class FractureCubeMap
{
public:
	FractureCubeMap(const char* front, const char* back, const char* top, const char* bottom, const char* left, const char* right, double intensity = 1.0f, double contribution = 1.0f);
	~FractureCubeMap();
	void Render();
	float GetRotation();
	void SetRotation(float rotation);
	double GetIntensity();
	void SetIntensity(double intensity);
	bool LoadCubeMapSide(GLenum side_target, const char* file_name);

	double m_intensity;

private:
	void MakeBigCube();
	void GenDynamicCubeMapFBO();
	void RenderCubeMapSide(GLenum side_target);

	GLuint m_cubeMapVAO;
	GLuint m_cubeMapVBO;
	GLuint m_cubeMapID;
	GLuint m_cubeMapFBO;			// Framebuffer del cubemap dinamico
	GLuint m_cubeMapDepthBuffer;
	float m_rotation;
	int m_width;
	int m_height;
};


#endif