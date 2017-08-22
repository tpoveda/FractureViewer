#include "FractureCubeMap.h"
#include "stb_image.h"

FractureCubeMap::FractureCubeMap(const char* front, const char* back, const char* top, const char* bottom, const char* left, const char* right, double intensity, double contribution) :
m_cubeMapVAO(0),
m_cubeMapVBO(0),
m_cubeMapID(0),
m_cubeMapFBO(0),
m_cubeMapDepthBuffer(0),
m_rotation(0),
m_width(0),
m_height(0)
{
	m_intensity = intensity;

	MakeBigCube();

	// generamos una textura de cubemap para almacenar todos los lados
	glGenTextures(1, &m_cubeMapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapID);
	glActiveTexture(GL_TEXTURE0);

	// cargamos cada imagen y las copiamos al lado del cubemap correcto
	assert(LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front));
	assert(LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back));
	assert(LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top));
	assert(LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom));
	assert(LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left));
	assert(LoadCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_X, right));

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//GenDynamicCubeMapFBO();
}

FractureCubeMap::~FractureCubeMap()
{
	glDeleteTextures(1, &m_cubeMapID);
}

float FractureCubeMap::GetRotation()
{
	return m_rotation;
}

double FractureCubeMap::GetIntensity()
{
	return m_intensity;
}

void FractureCubeMap::SetIntensity(double intensity)
{
	m_intensity = intensity;
}

void FractureCubeMap::SetRotation(float rotation)
{
	m_rotation = rotation;
}

void FractureCubeMap::Render()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapID);

	/*glBindFramebuffer(GL_FRAMEBUFFER, m_cubeMapFBO);

	RenderCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
	RenderCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	RenderCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	RenderCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	RenderCubeMapSide(GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	RenderCubeMapSide(GL_TEXTURE_CUBE_MAP_POSITIVE_X);*/

	glBindVertexArray(m_cubeMapVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//glBindFramebuffer(GL_FRAMEBUFFER, m_cubeMapFBO);
}

void FractureCubeMap::MakeBigCube()
{
	float points[] = {
		-10.0f, 10.0f, -10.0f,
		-10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, 10.0f, -10.0f,
		-10.0f, 10.0f, -10.0f,

		-10.0f, -10.0f, 10.0f,
		-10.0f, -10.0f, -10.0f,
		-10.0f, 10.0f, -10.0f,
		-10.0f, 10.0f, -10.0f,
		-10.0f, 10.0f, 10.0f,
		-10.0f, -10.0f, 10.0f,

		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,

		-10.0f, -10.0f, 10.0f,
		-10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, -10.0f, 10.0f,
		-10.0f, -10.0f, 10.0f,

		-10.0f, 10.0f, -10.0f,
		10.0f, 10.0f, -10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		-10.0f, 10.0f, 10.0f,
		-10.0f, 10.0f, -10.0f,

		-10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f, 10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f, 10.0f,
		10.0f, -10.0f, 10.0f
	};

	// generamos y bindeamos el VBO
	glGenBuffers(1, &m_cubeMapVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_cubeMapVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof (GLfloat), &points, GL_STATIC_DRAW);

	// generamos y bindeamos el VAO
	glGenVertexArrays(1, &m_cubeMapVAO);
	glBindVertexArray(m_cubeMapVAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_cubeMapVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void FractureCubeMap::GenDynamicCubeMapFBO()
{
	// generamos y bindeamos el FBO
	glGenFramebuffers(1, &m_cubeMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_cubeMapFBO);

	// creamos el depth buffer uniforme
	glGenRenderbuffers(1, &m_cubeMapDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_cubeMapDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	
	// enlazamos el frame buffer y el render buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_cubeMapFBO);

	// enlazamos solamente la textura +X del cubemap
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, m_cubeMapID, 0);

	// desbindeamos los buffers
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

bool FractureCubeMap::LoadCubeMapSide(GLenum side_target, const char* file_name)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapID);

	int x, y, n;
	int force_channels = 4;
	unsigned char*  image_data = stbi_load(file_name, &x, &y, &n, force_channels);
	if (!image_data) 
	{
		fprintf(stderr, "ERROR: could not load %s\n", file_name);
		return false;
	}
	// non-power-of-2 dimensions check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0)
	{
		fprintf(
			stderr, "WARNING: image %s is not power-of-2 dimensions\n", file_name
			);
	}

	// copy image data into 'target' side of cube map
	glTexImage2D(side_target,0,GL_RGBA,x,y,0,GL_RGBA,GL_UNSIGNED_BYTE,image_data);
	free(image_data);

	m_width = x;
	m_height = y;

	return true;
}

void FractureCubeMap::RenderCubeMapSide(GLenum side_target)
{
	// enlazamos una nueva textura y el renderbuffer al fbo
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, side_target, m_cubeMapID, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 1, 1, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	switch (side_target)
	{
		case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
			gluLookAt(0, 0, 0, 1, 0, 0, 0, 1, 0);
			break;

		case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
			gluLookAt(0, 0, 0, -1, 0, 0, 0, 1, 0);
			break;

		case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
			gluLookAt(0, 0, 0, 0, 10, 0, 1, 0, 0);
			break;

		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
			gluLookAt(0, 0, 0, 0, -10, 0, 1, 0, 0);
			break;

		case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
			gluLookAt(0, 0, 0, 0, 0, 10, 0, 1, 0);
			break;

		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
			gluLookAt(0, 0, 0, 0, 0, -10, 0, 1, 0);
			break;

		default:
			break;
	}
}