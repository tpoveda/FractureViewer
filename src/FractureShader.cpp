#include "FractureShader.h"
#include <iostream>
#include <fstream>
#include "Frac3D_Debug.h"

FractureShader::FractureShader()
{
	m_totalShaders = 0;
	m_shaders[VERTEX_SHADER] = 0;
	m_shaders[FRAGMENT_SHADER] = 0;
	m_shaders[GEOMETRY_SHADER] = 0;
	m_attributeList.clear();
	m_uniformLocationList.clear();
}

FractureShader::~FractureShader()
{
	m_attributeList.clear();
	m_uniformLocationList.clear();
}

void FractureShader::LoadFromString(GLenum type, std::string const& source)
{
	GLuint shader = glCreateShader(type);

	const char* ptmp = source.c_str();
	glShaderSource(shader, 1, &ptmp, nullptr);

	// comprobamos que el shader se ha cargado correctamente
	GLint status;
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		Frac3D_Debug::Frac3D_ErrLog("ERROR: El shader con indice %i no se pudo compilar\n", shader);
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* infoLog = new GLchar[infoLogLength];
		glGetShaderInfoLog(shader, infoLogLength, nullptr, infoLog);
		fprintf(stderr, "Log de compilacion para el shader %i:\n%s\n", shader, infoLog);
		Frac3D_Debug::Frac3D_Log("Log de compilacion para el shader %i:\n%s\n", shader, infoLog);
		delete[]infoLog;
	}
	Frac3D_Debug::Frac3D_Log("El shader con indice %i se ha compilado con exito\n", shader);
	m_shaders[m_totalShaders++] = shader;
}

void FractureShader::LoadFromFile(GLenum type, std::string const& filename)
{
	Frac3D_Debug::Frac3D_Log("Creando shader desde %s\n", filename.c_str());
	std::ifstream file;
	file.open(filename.c_str(), std::ios_base::in);
	if (file)
	{
		std::string line;
		std::string buffer;
		while (getline(file, line))
		{
			buffer.append(line);
			buffer.append("\r\n");
		}

		// copiamos a la fuente
		LoadFromString(type, buffer);
	}
	else
	{
		fprintf(stderr, "ERROR: No se pudo cargar el shader %s\n", filename);
		Frac3D_Debug::Frac3D_ErrLog("ERROR: No se pudo cargar el shader %s\n", filename);
	}
}

void FractureShader::CreateAndLinkProgram()
{
	m_program = glCreateProgram();
	Frac3D_Debug::Frac3D_Log("Creado programa %u. Enlazando shaders %u y %u ...\n", m_program, m_shaders[VERTEX_SHADER], m_shaders[FRAGMENT_SHADER]);
	if (m_shaders[VERTEX_SHADER] != 0) glAttachShader(m_program, m_shaders[VERTEX_SHADER]);
	if (m_shaders[FRAGMENT_SHADER] != 0) glAttachShader(m_program, m_shaders[FRAGMENT_SHADER]);
	if (m_shaders[GEOMETRY_SHADER] != 0) glAttachShader(m_program, m_shaders[GEOMETRY_SHADER]);

	// linkeamos los shaders y comprobamos que el linkeo es correcto
	GLint status;
	glLinkProgram(m_program);
	glGetProgramiv(m_program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		Frac3D_Debug::Frac3D_ErrLog("ERROR: El programa con indice %i no se pudo linkear\n", m_program);
		GLint infoLogLength;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* infoLog = new GLchar[infoLogLength];
		glGetProgramInfoLog(m_program, infoLogLength, nullptr, infoLog);
		fprintf(stderr, "Log de linkeo para el programa %i:\n%s\n", m_program, infoLog);
		Frac3D_Debug::Frac3D_Log("Log de linkeo para el programa %i:\n%s\n", m_program, infoLog);
		delete[]infoLog;
	}
	else Frac3D_Debug::Frac3D_Log("El programa con indice %i se ha linkeado con exito\n", m_program);

	glValidateProgram(m_program);
	GLint params = -1;
	glGetProgramiv(m_program, GL_VALIDATE_STATUS, &params);
	if (GL_TRUE != params)
	{
		Frac3D_Debug::Frac3D_ErrLog("ERROR: El programa con indice %i no se ha validado con exito\n\n", m_program);
		GLint infoLogLength;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* infoLog = new GLchar[infoLogLength];
		glGetProgramInfoLog(m_program, infoLogLength, nullptr, infoLog);
		fprintf(stderr, "Log de validacion del programa %i:\n%s\n", m_program, infoLog);
		Frac3D_Debug::Frac3D_Log("Log de validacion del programa %i:\n%s\n", m_program, infoLog);
		delete[]infoLog;
	}
	else Frac3D_Debug::Frac3D_Log("El programa con indice %i se ha validado con exito\n\n", m_program);

	glDeleteShader(m_shaders[VERTEX_SHADER]);
	glDeleteShader(m_shaders[FRAGMENT_SHADER]);
	glDeleteShader(m_shaders[GEOMETRY_SHADER]);
}

void FractureShader::Use()
{
	glUseProgram(m_program);
}

void FractureShader::UnUse()
{
	glUseProgram(0);
}

void FractureShader::AddAttribute(std::string const& attribute)
{
	m_attributeList[attribute] = glGetAttribLocation(m_program, attribute.c_str());
}

void FractureShader::AddUniform(std::string const& uniform)
{
	m_uniformLocationList[uniform] = glGetUniformLocation(m_program, uniform.c_str());
}

GLuint FractureShader::operator[](std::string const& attribute)
{
	return m_attributeList[attribute];
}

GLuint FractureShader::operator()(std::string const& uniform)
{
	return m_uniformLocationList[uniform];
}

void FractureShader::DeleteShaderProgram()
{
	glDeleteProgram(m_program);
}

void FractureShader::UpdateUniforms(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, FractureMaterial& material, const glm::mat4& modelMatrix)
{
}

void FractureShader::SetUniform(const std::string& uniform, int value)
{
	glUniform1i(m_uniformLocationList.at(uniform), value);
}

void FractureShader::SetUniform(const std::string& uniform, float value)
{
	glUniform1f(m_uniformLocationList.at(uniform), value);
}

void FractureShader::SetUniform(const std::string& uniform, double value)
{
	glUniform1f(m_uniformLocationList.at(uniform), value);
}

void FractureShader::SetUniform(const std::string& uniform, const glm::vec3& value)
{
	glUniform3f(m_uniformLocationList.at(uniform), value.x, value.y, value.z);
}

void FractureShader::SetUniform(const std::string& uniform, const glm::dvec3& value)
{
	glUniform3f(m_uniformLocationList.at(uniform), value.x, value.y, value.z);
}

void FractureShader::SetUniform(const std::string& uniform, const glm::mat4& value)
{
	glUniformMatrix4fv(m_uniformLocationList.at(uniform), 1, GL_FALSE, glm::value_ptr(value));
}