#include "Frac3D_Debug.h"
#include "Time.h"

Frac3D_Debug::Frac3D_Debug()
{
}

Frac3D_Debug::~Frac3D_Debug()
{
}

bool Frac3D_Debug::Frac3D_RestartLog()
{
	time_t now;
	char* date;
	FILE* file = fopen(GL_LOG_FILE, "w");

	if (!file)
	{
		fprintf(stderr, "ERROR: no se pudo abrir el archivo de log GL_LOG_FILE %s para escritura\n", GL_LOG_FILE);
		return false;
	}
	now = time(nullptr);	// Almacenamos el la hora
	date = ctime(&now);		// Almacenamos la fecha
	fprintf(file, "GL_LOG_FILE log. tiempo local %s\n", date);	// Imprimimos la fecha y la hora al inicio del log
	fprintf(file, "build version: %s %s\n\n", __DATE__, __TIME__);
	fclose(file);			// Cerramos el archivo
	return true;
}

bool Frac3D_Debug::Frac3D_Log(const char* message, ...)
{
	va_list argptr;
	FILE* file = fopen(GL_LOG_FILE, "a");	// Abrimos el archivo en modo Append
	if (!file)
	{
		fprintf(stderr, "ERROR: no se pudo abrir el archivo de log GL_LOG_FILE %s para anadir\n", GL_LOG_FILE);
		return false;
	}
	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);
	fclose(file);
	return true;
}

bool Frac3D_Debug::Frac3D_ErrLog(const char* message, ...)
{
	va_list argptr;
	FILE* file = fopen(GL_LOG_FILE, "a");	// Abrimos el archivo en modo Append
	if (!file)
	{
		fprintf(stderr, "ERROR:no se pudo abrir el archivo de log GL_LOG_FILE %s para anadir\n", GL_LOG_FILE);
		return false;
	}
	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);
	va_start(argptr, message);
	vfprintf(stderr, message, argptr);
	va_end(argptr);
	fclose(file);
	return true;
}

void Frac3D_Debug::Fra3D_GLParams()
{
	int i;
	int v[2];
	unsigned char s = 0;
	GLenum params[] =
	{
		GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
		GL_MAX_CUBE_MAP_TEXTURE_SIZE,
		GL_MAX_DRAW_BUFFERS,
		GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
		GL_MAX_TEXTURE_IMAGE_UNITS,
		GL_MAX_TEXTURE_SIZE,
		GL_MAX_VARYING_FLOATS,
		GL_MAX_VERTEX_ATTRIBS,
		GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
		GL_MAX_VERTEX_UNIFORM_COMPONENTS,
		GL_MAX_VIEWPORT_DIMS,
		GL_STEREO,
	};
	const char* names[] =
	{
		"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
		"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
		"GL_MAX_DRAW_BUFFERS",
		"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
		"GL_MAX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_TEXTURE_SIZE",
		"GL_MAX_VARYING_FLOATS",
		"GL_MAX_VERTEX_ATTRIBS",
		"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_VERTEX_UNIFORM_COMPONENTS",
		"GL_MAX_VIEWPORT_DIMS",
		"GL_STEREO",
	};
	Frac3D_Log("Parametros del contexto OpenGL: \n");
	for (i = 0; i < 10; i++)
	{
		int v = 0;
		glGetIntegerv(params[i], &v);
		Frac3D_Log("%s %i\n", names[i], v);
	}
	v[0] = v[1] = 0;
	glGetIntegerv(params[10], v);
	Frac3D_Log("%s %i %i\n", names[10], v[0], v[1]);
	glGetBooleanv(params[11], &s);
	Frac3D_Log("%s %i\n", names[11], static_cast<unsigned int>(s));
	Frac3D_Log("-----------------------------\n");
}