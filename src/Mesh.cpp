#include "Mesh.h"
#include <iostream>
#include <Windows.h>
#include <SDL2/SDL.h>
#include <Importer.hpp>
#include "Frac3D_Debug.h"
#include <postprocess.h>
#include "glm.hpp"

using namespace std;

Mesh::MeshData::MeshData(aiMesh *mesh)
{
	// Inicializamos los buffers
	vbo[VERTEX_BUFFER] = NULL;
	vbo[TEXCOORD_BUFFER] = NULL;
	vbo[NORMAL_BUFFER] = NULL;
	vbo[INDEX_BUFFER] = NULL;
	vbo[TANGENT_BUFFER] = NULL;

	// Calculamos la cantidad de elementos que vamos a renderizar
	elementCount = mesh->mNumFaces * 3;

	// Generamos el Vertex Attribute Object (VAO) de la malla
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Guardamos el numero de vertices y caras
	m_numVertices = mesh->mNumVertices;
	m_numFaces = mesh->mNumFaces;

	/*************************************************************/
	/****** LECTURA DE POSICION DE LOS VERTICES DE LA MALLA ******/
	/*************************************************************/

	// Si la malla tiene posiciones, por lo tanto, tiene vertices
	if(mesh->HasPositions())
	{
		// Total de vertices = vertices * 3 (x, y, z) --> Cada vertice tiene 3 componentes para la posicion
		float *vertices = new float[mesh->mNumVertices * 3];

		// Por cada posicion, calculamos sus componentes x, y, z
		for(int i = 0; i < mesh->mNumVertices; ++i)
		{
			vertices[i * 3] = mesh->mVertices[i].x;			// Posicion X
			vertices[i * 3 + 1] = mesh->mVertices[i].y;		// Posicion Y = Posicion X + 1
			vertices[i * 3 + 2] = mesh->mVertices[i].z;		// Posicion Z = Posicion X + 2
		}

		// Generamos, Bindeamos y Rellenamos un Vertex Buffer Object (VBO) con los vertices de la malla
		glGenBuffers(1, &vbo[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

		// Atributo 0 --> Posicion de los vertices de la malla
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray (0);

		delete vertices;
	}

	/********************************************************/
	/****** LECTURA DE UVS DE LOS VERTICES DE LA MALLA ******/
	/********************************************************/

	// Si la malla tiene coordenadas de texturas (UVs)
	if(mesh->HasTextureCoords(0))
	{
		// Total UVs = vertices * 2 (x, y) --> Cada vertice tiene 2 componentes para las UVs
		float *texCoords = new float[mesh->mNumVertices * 2];

		// Por cada UV, calculamos sus componentes x, y
		for(int i = 0; i < mesh->mNumVertices; ++i)
		{
			texCoords[i * 2] = mesh->mTextureCoords[0][i].x;
			texCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
		}

		// Generamos, Bindeamos y Rellenamos un Vertex Buffer Object (VBO) con las UVs de la malla
		glGenBuffers(1, &vbo[TEXCOORD_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXCOORD_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);

		// Atributo 1 --> UVs de la malla
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray (1);

		delete texCoords;
	}

	/*************************************************************/
	/****** LECTURA DE NORMALES DE LOS VERTICES DE LA MALLA ******/
	/*************************************************************/

	// Si la malla tiene normales (incluido el calculo de tangentes y bitangentes)
	if(mesh->HasNormals())
	{
		// Total de normales = vertices * 3 (x, y, z) --> Cada vertice tiene 3 componentes para las normales
		float *normals = new float[mesh->mNumVertices * 3];

		// Por cada normal, calculamos sus componentes x, y
		for(int i = 0; i < mesh->mNumVertices; ++i)
		{
			normals[i * 3] = mesh->mNormals[i].x;
			normals[i * 3 + 1] = mesh->mNormals[i].y;
			normals[i * 3 + 2] = mesh->mNormals[i].z;
		}

		if (mesh->HasTangentsAndBitangents())
		{
			// Total de tangentes y bitangentes = vertices * 3 (x, y, z) --> Cada vertice tiene 3 componentes para las tangentes
			float *tangents = new float[mesh->mNumVertices * 3];
			float *bitangents = new float[mesh->mNumVertices * 3];
			float *g_vtans = new float[mesh->mNumVertices * 4];

			// Por cada tangente y bitangente calculamos sus componenes
			for (int i = 0; i < mesh->mNumVertices; ++i)
			{
				tangents[i * 3] = mesh->mTangents[i].x;
				tangents[i * 3 + 1] = mesh->mTangents[i].y;
				tangents[i * 3 + 2] = mesh->mTangents[i].z;
				bitangents[i * 3] = mesh->mBitangents[i].x;
				bitangents[i * 3 + 1] = mesh->mBitangents[i].y;
				bitangents[i * 3 + 2] = mesh->mBitangents[i].z;

				glm::vec3 t = glm::vec3(tangents[i * 3], tangents[i * 3 + 1], tangents[i * 3 + 2]);
				glm::vec3 n = glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]);
				glm::vec3 b = glm::vec3(bitangents[i * 3], bitangents[i * 3 + 1], bitangents[i * 3 + 2]);

				// ortogonalizamos y normalizamos la tangente
				glm::vec3 t_i = glm::normalize(t - n * glm::dot(n, t));
				
				// calculamos el determinante de T, B, N matriz 3x3 por el metodo del producto escalar
				float det = (glm::dot(glm::cross(n, t), b));
				if (det < 0.0f) det = -1.0f;
				else det = 1.0f;

				// cremaos un vector 4d para el calculo de la tangente inversa
				g_vtans[i * 4] = t_i[0];
				g_vtans[i * 4 + 1] = t_i[1];
				g_vtans[i * 4 + 2] = t_i[2];
				g_vtans[i * 4 + 3] = det;	
			}

			// Generamos, Bindeamos y Rellenamos un Vertex Buffer Object (VBO) con las tangentes
			glGenBuffers(1, &vbo[TANGENT_BUFFER]);
			glBindBuffer(GL_ARRAY_BUFFER, vbo[TANGENT_BUFFER]);
			glBufferData(GL_ARRAY_BUFFER, 4 * mesh->mNumVertices * sizeof(GLfloat), g_vtans, GL_STATIC_DRAW);

			// Atributo 3 --> Tangentes de la malla
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
			glEnableVertexAttribArray(3);
		}

		// Generamos, Bindeamos y Rellenamos un Vertex Buffer Object (VBO) con las normales de la malla
		glGenBuffers(1, &vbo[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), normals, GL_STATIC_DRAW);

		// Atributo 2 --> Normales de la malla
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray (2);

		delete normals;
	}
	
	/******************************************/
	/****** LECTURA DE CARAS DE LA MALLA ******/
	/******************************************/

	// Si la malla tiene caras
	if(mesh->HasFaces())
	{
		// Total de indices = caras * 3 (x, y, z) --> Cada cada esta formada por 3 indices
		unsigned int *indices = new unsigned int[mesh->mNumFaces * 3];

		// Por cada cada, calculamos sus indices
		for(int i = 0; i < mesh->mNumFaces; ++i)
		{
			indices[i * 3] = mesh->mFaces[i].mIndices[0];
			indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
		}

		// Generamos, Bindeamos y Rellenamos un Vertex Buffer Object (VBO) con las indices de las caras de la malla
		glGenBuffers(1, &vbo[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->mNumFaces * sizeof(GLuint), indices, GL_STATIC_DRAW);

		// Atributo 4 --> Indices de la malla
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray (4);

		delete indices;
	}
	
	// Desbindeamos los buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Mesh::MeshData::~MeshData()
{
	// Al eliminar la malla, eliminamos los buffers creados
	if(vbo[VERTEX_BUFFER])
	{
		glDeleteBuffers(1, &vbo[VERTEX_BUFFER]);
		vbo[VERTEX_BUFFER] = NULL;
	}

	if(vbo[TEXCOORD_BUFFER])
	{
		glDeleteBuffers(1, &vbo[TEXCOORD_BUFFER]);
		vbo[TEXCOORD_BUFFER] = NULL;
	}

	if(vbo[NORMAL_BUFFER])
	{
		glDeleteBuffers(1, &vbo[NORMAL_BUFFER]);
		vbo[NORMAL_BUFFER] = NULL;
	}

	if(vbo[INDEX_BUFFER])
	{
		glDeleteBuffers(1, &vbo[INDEX_BUFFER]);
		vbo[INDEX_BUFFER] = NULL;
	}

	glDeleteVertexArrays(1, &vao);
}

void Mesh::MeshData::Render()
{
	// Bindeamos el VAO creado anteriormente (Especifica el layout con los atributos de la malla)
	glBindVertexArray(vao);		
	glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

Mesh::Mesh(const std::string& filename)
{

	Assimp::Importer importer;
	
	scene = importer.ReadFile(filename, aiProcess_CalcTangentSpace);		// añadido calculos de tangentes
	
	if(!scene)
	{
		Frac3D_Debug::Frac3D_Log("ERROR: No se pudo cargar la malla\n");
		cout<<("ERROR: No se pudo cargar la malla: %s\n", importer.GetErrorString());
		return;
	}

	cout << scene->mNumAnimations << " animaciones" << endl;
	cout << scene->mNumCameras << " camaras" << endl;
	cout << scene->mNumLights << " luces" << endl;
	cout << scene->mNumMaterials << " materiales" << endl;
	cout << scene->mNumMeshes << " mallas" << endl;
	cout << scene->mNumTextures << " texturas" << endl;

	// Cargamos las mallas de nuestra archivo a la escena
	for (int i = 0; i < scene->mNumMeshes; ++i)
	{
		meshEntries.push_back(new Mesh::MeshData(scene->mMeshes[i]));
	}

}

Mesh::~Mesh(void)
{
	// Eliminamos las mallas cargadas en la escena
	for(int i = 0; i < meshEntries.size(); ++i)
	{
		delete meshEntries.at(i);
	}
	meshEntries.clear();
}

void Mesh::Render() const
{
	// Renderizamos las malals cargadas en la escena
	for (int i = 0; i < meshEntries.size(); ++i)
	{
			meshEntries.at(i)->Render();
	}
}

const aiScene* Mesh::GetScene() const
{
	return scene;
}

int Mesh::MeshData::GetNumVertices() const
{
	return m_numVertices;
}

int Mesh::MeshData::GetNumFaces() const
{
	return m_numFaces;
}