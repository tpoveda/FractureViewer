#ifndef FractureShader_H
#define FractureShader_H

#include <GL/glew.h>
#include <string>
#include <map>
#include "glm.hpp"
#include <gtc/type_ptr.hpp>
#include "FractureMaterial.h"

//! @class FractureShader Clase que implementa todas las funcionaliades necesarios para trabajar con shaders en nuestro motor
class FractureShader
{
public:
	//! @brief Permite crear un shader e inicializar sus atributos
	FractureShader();

	//! @brief Elimina el shader
	virtual ~FractureShader();

	//! @biref Permite cargar un shader desde un string
	//! @param type Tipo de shader a cargar
	//! @param source que contiene el texto del shader a cargar
	void LoadFromString(GLenum type, const std::string& source);

	// !@brief Permite cargar un shader desde un archivo externo
	// !@param type Tipo de shader a cargar
	//! @param filename Archivo externo a cargar
	void LoadFromFile(GLenum type, const std::string& filename);

	//! @brief Crea y linkea los shaders a un nuevo programa
	void CreateAndLinkProgram();

	//! @brief Permite enlazar (bindear) el programa al contexto OpenGL
	void Use();

	//! @brief Permite desenalzar el programa del contexto OpenGL
	void UnUse();

	//! @brief Permite anadir un atributo al shader
	//! @param attribute Atributo a anadir
	void AddAttribute(const std::string& attribute);

	//! @brief Permite anadir un uniforme al shader
	//! @param attribute Uniforme a anadir
	void AddUniform(const std::string& uniform);

	//! @brief Permite acceder un atributo del shader
	//! @param attribute Nombre del atributo al que queremos acceder
	GLuint operator[] (const std::string& attribute);

	//! @brief Permite acceder un uniforme del shader
	//! @param attribute Nombre del uniforme al que queremos acceder
	GLuint operator() (const std::string& uniform);

	virtual void UpdateUniforms(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, FractureMaterial& material, const glm::mat4& modelMatix = glm::mat4(1));

	void SetUniform(const std::string& uniform, int value);
	void SetUniform(const std::string& uniform, float value);
	void SetUniform(const std::string& uniform, double value);
	void SetUniform(const std::string& uniform, const glm::vec3& value);
	void SetUniform(const std::string& uniform, const glm::dvec3& value);
	void SetUniform(const std::string& uniform, const glm::mat4& value);

	//! @brief Borra el programa del shader
	void DeleteShaderProgram();

private:
	//! @brief Tipo de shaders que podemos crear
	enum ShaderType{ VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER };

	//! @brief Programa al que se linkean los shaders
	GLuint m_program;

	//! @brief Numero total de shaders creados (como maximo podemos tener 3 creados)
	int m_totalShaders;

	//! @brief Array de shaders creados
	GLuint m_shaders[3];

	//! Lista de atributos del shader
	std::map<std::string, GLuint> m_attributeList;

	//! Lista de uniformes del shader
	std::map<std::string, GLuint > m_uniformLocationList;

};

#endif