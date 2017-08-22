#include "FractureMesh.h"
#include <iostream>
#include <Windows.h>
#include <SDL2/SDL.h>
#include <Importer.hpp>
#include "Frac3D_Debug.h"
#include <postprocess.h>
#include <cimport.h>

using namespace std;

FractureMesh::FractureMesh(const std::string& filename)
{
	Assimp::Importer importer;

	const aiScene* scene = aiImportFile(filename.c_str(), aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	if (!scene)
	{
		Frac3D_Debug::Frac3D_Log("ERROR: No se pudo cargar la malla\n");
		cout << ("ERROR: No se pudo cargar la malla: %s\n", importer.GetErrorString());
		return;
	}

	cout << scene->mNumAnimations << " animaciones" << endl;
	cout << scene->mNumCameras << " camaras" << endl;
	cout << scene->mNumLights << " luces" << endl;
	cout << scene->mNumMaterials << " materiales" << endl;
	cout << scene->mNumMeshes << " mallas" << endl;
	cout << scene->mNumTextures << " texturas" << endl;
	cout << scene->mMeshes[0]->mNumBones << " huesos" << endl;

	for (int i = 0; i < scene->mNumMeshes; ++i)
	{
		meshEntries.push_back(new FractureMesh::MeshData(scene->mMeshes[i]));
		m_numMeshes++;
	}
	//mesh = new FractureMesh::MeshData(scene->mMeshes[0]);
}

FractureMesh::MeshData::MeshData(aiMesh* mesh)
{
	// Inicializamos los buffers
	vbo[VERTEX_BUFFER] = NULL;
	vbo[TEXCOORD_BUFFER] = NULL;
	vbo[NORMAL_BUFFER] = NULL;
	vbo[INDEX_BUFFER] = NULL;
	vbo[TANGENT_BUFFER] = NULL;
	vbo[BONES_BUFFER] = NULL;

	pointsBounding = new GLfloat[6];
	HullShape = new btConvexHullShape();

	for (int i = 0; i < 6; i++)
	{
		pointsBounding[i] = 0;
	}

	// Calculamos la cantidad de elementos que vamos a renderizar
	elementCount = mesh->mNumFaces * 3;

	// Generamos el Vertex Attribute Object (VAO) de la malla
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	g_point_count = mesh->mNumVertices;
	m_numVertices = mesh->mNumVertices;
	m_numFaces = mesh->mNumFaces;
	g_bone_count = mesh->mNumBones;

	cout << mesh->mNumVertices << " vertices en mesh[0]" << endl;

	// Reservamos memoria para los vertices
	if (mesh->HasPositions())
	{
		printf("La malla tiene posiciones\n");
		g_vp = (GLfloat*)malloc(g_point_count * 3 * sizeof (GLfloat));
	}
	if (mesh->HasNormals())
	{
		printf("La malla tiene normales\n");
		g_vn = (GLfloat*)malloc(g_point_count * 3 * sizeof (GLfloat));
	}
	if (mesh->HasTextureCoords(0))
	{
		printf("La malla tiene coordenadas de texturas\n");
		g_vt = (GLfloat*)malloc(g_point_count * 2 * sizeof (GLfloat));
	}
	if (mesh->HasTangentsAndBitangents())
	{
		printf("La malla tiene tangentes\n");
		g_vtans = (GLfloat*)malloc(g_point_count * 4 * sizeof (GLfloat));
	}
	if (mesh->HasBones())
	{
		printf("La malla tiene huesos\n");
		g_bone_count = (int)mesh->mNumBones;

		// array con los nombres de los huesos. Maximo = 256 huesos, longitud de nombre maxima = 64
		char bone_names[256][64];

		// aqui almaceno un array per-verte bone ID. Cada vertice debe saber que huesos le afectan, 
		// teniendo en cuenta que un vertice solamente puede ser afectado por un hueso.
		bone_ids = (int*)malloc(g_point_count * sizeof(int));

		for (int i = 0; i < g_bone_count; i++)
		{
			const aiBone* bone = mesh->mBones[i];

			// nos guardamos el nombre de los huesos
			strcpy(bone_names[i], bone->mName.data);
			printf("bone_names[%i]=%s\n", i, bone_names[i]);

			// almacenamos la matriz de desplazamiento (offset) inversa para cada hueso
			bone_offset_matrices[i] = ConvetAssimpMatrix(bone->mOffsetMatrix);

			// nos guardamos el peso de los huesos: asumimos que, por defecto, el peso es 1.0
			// porque unicamente usamos 1 hueso por vertice por lo que cualquie hueso que afecte
			// a un vertice se asignara el bone_id
			int num_weights = (int)bone->mNumWeights;

			for (int j = 0; j < num_weights; j++)
			{
				aiVertexWeight weight = bone->mWeights[j];
				int vertex_id = (int)weight.mVertexId;

				// ignoramos el peso si es menor que un factor de 0.5
				if (weight.mWeight >= 0.5f) bone_ids[vertex_id] = i;
			}
		}

	}

	for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++)
	{
		if (mesh->HasPositions())
		{
			const aiVector3D* vp = &(mesh->mVertices[v_i]);
			g_vp[v_i * 3] = (GLfloat)vp->x;
			g_vp[v_i * 3 + 1] = (GLfloat)vp->y;
			g_vp[v_i * 3 + 2] = (GLfloat)vp->z;

			CreateHullShape(vp);

			if (v_i == 0)
			{
				pointsBounding[0] = mesh->mVertices[v_i].x;
				pointsBounding[1] = mesh->mVertices[v_i].x;
				pointsBounding[2] = mesh->mVertices[v_i].y;
				pointsBounding[3] = mesh->mVertices[v_i].y;
				pointsBounding[4] = mesh->mVertices[v_i].z;
				pointsBounding[5] = mesh->mVertices[v_i].z;
			}

			else
			{
				if (mesh->mVertices[v_i].x < pointsBounding[0]) pointsBounding[0] = mesh->mVertices[v_i].x;
				if (mesh->mVertices[v_i].x > pointsBounding[1]) pointsBounding[1] = mesh->mVertices[v_i].x;
				if (mesh->mVertices[v_i].y < pointsBounding[2]) pointsBounding[2] = mesh->mVertices[v_i].y;
				if (mesh->mVertices[v_i].y > pointsBounding[3]) pointsBounding[3] = mesh->mVertices[v_i].y;
				if (mesh->mVertices[v_i].z < pointsBounding[4]) pointsBounding[4] = mesh->mVertices[v_i].z;
				if (mesh->mVertices[v_i].z > pointsBounding[5]) pointsBounding[5] = mesh->mVertices[v_i].z;
			}
		}
		if (mesh->HasNormals())
		{
			const aiVector3D* vn = &(mesh->mNormals[v_i]);
			g_vn[v_i * 3] = (GLfloat)vn->x;
			g_vn[v_i * 3 + 1] = (GLfloat)vn->y;
			g_vn[v_i * 3 + 2] = (GLfloat)vn->z;
		}
		if (mesh->HasTextureCoords(0))
		{
			const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
			g_vt[v_i * 2] = (GLfloat)vt->x;
			g_vt[v_i * 2 + 1] = (GLfloat)vt->y;
		}
		if (mesh->HasTangentsAndBitangents())
		{
			const aiVector3D* tangent = &(mesh->mTangents[v_i]);
			const aiVector3D* bitangent = &(mesh->mBitangents[v_i]);
			const aiVector3D* normal = &(mesh->mNormals[v_i]);

			glm::vec3 t = glm::vec3(tangent->x, tangent->y, tangent->z);
			glm::vec3 n = glm::vec3(normal->x, normal->y, normal->z);
			glm::vec3 b = glm::vec3(bitangent->x, bitangent->y, bitangent->z);

			// ortogonalizamos y normalizamos la tangente
			glm::vec3 t_i = glm::normalize(t - n * glm::dot(n, t));

			// calculamos el determinante de T, B, N matriz 3x3 por el metodo del producto escalar
			float det = (glm::dot(glm::cross(n, t), b));
			if (det < 0.0f) det = -1.0f;
			else det = 1.0f;

			// push back 4d vector for inverse tangent with determinant
			g_vtans[v_i * 4] = t_i[0];
			g_vtans[v_i * 4 + 1] = t_i[1];
			g_vtans[v_i * 4 + 2] = t_i[2];
			g_vtans[v_i * 4 + 3] = det;
		}
	}

	/*************************************************************/
	/****** LECTURA DE POSICION DE LOS VERTICES DE LA MALLA ******/
	/*************************************************************/

	// Si la malla tiene posiciones, por lo tanto, tiene vertices
	if (mesh->HasPositions() && NULL != g_vp)
	{
		// Generamos, Bindeamos y Rellenamos un Vertex Buffer Object (VBO) con los vertices de la malla
		glGenBuffers(1, &vbo[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), g_vp, GL_STATIC_DRAW);

		// Atributo 0 --> Posicion de los vertices de la malla
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(0);

		delete g_vp;
	}

	/********************************************************/
	/****** LECTURA DE UVS DE LOS VERTICES DE LA MALLA ******/
	/********************************************************/

	// Si la malla tiene coordenadas de texturas (UVs)
	if (mesh->HasTextureCoords(0))
	{
		// Generamos, Bindeamos y Rellenamos un Vertex Buffer Object (VBO) con las UVs de la malla
		glGenBuffers(1, &vbo[TEXCOORD_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXCOORD_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * sizeof(GLfloat), g_vt, GL_STATIC_DRAW);

		// Atributo 1 --> UVs de la malla
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(1);

		delete g_vt;
	}

	/*************************************************************/
	/****** LECTURA DE NORMALES DE LOS VERTICES DE LA MALLA ******/
	/*************************************************************/

	// Si la malla tiene normales (incluido el calculo de tangentes y bitangentes)
	if (mesh->HasNormals())
	{
		// Generamos, Bindeamos y Rellenamos un Vertex Buffer Object (VBO) con las normales de la malla
		glGenBuffers(1, &vbo[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), g_vn, GL_STATIC_DRAW);

		// Atributo 2 --> Normales de la malla
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(2);

		delete g_vn;
	}

	if (mesh->HasTangentsAndBitangents())
	{
		// Generamos, Bindeamos y Rellenamos un Vertex Buffer Object (VBO) con las tangentes
		glGenBuffers(1, &vbo[TANGENT_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[TANGENT_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 4 * mesh->mNumVertices * sizeof(GLfloat), g_vtans, GL_STATIC_DRAW);

		// Atributo 3 --> Tangentes de la malla
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(3);

		delete g_vtans;
	}


	/*******************************************/
	/****** LECTURA DEL ID DE LOS HUESOS ******/
	/*******************************************/
	if (mesh->HasBones())
	{
		// Gneramos, Bindeamos y Rellenamos un Vertex Buffer Object (VBO) con los pesos de los vertices de la malla
		glGenBuffers(1, &vbo[BONES_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[BONES_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(GLint), bone_ids, GL_STATIC_DRAW);

		// Atributo 4 --> Huesos de la malla
		glVertexAttribIPointer(4, 1, GL_INT, GL_FALSE, 0);
		glEnableVertexAttribArray(4);

		delete bone_ids;
	}

	// Si la malla tiene normales (incluido el calculo de tangentes y bitangentes)

	/******************************************/
	/****** LECTURA DE CARAS DE LA MALLA ******/
	/******************************************/

	// Si la malla tiene caras
	if (mesh->HasFaces())
	{
		// Total de indices = caras * 3 (x, y, z) --> Cada cada esta formada por 3 indices
		unsigned int *indices = new unsigned int[mesh->mNumFaces * 3];

		// Por cada cada, calculamos sus indices
		for (int i = 0; i < mesh->mNumFaces; ++i)
		{
			indices[i * 3] = mesh->mFaces[i].mIndices[0];
			indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
		}

		// Generamos, Bindeamos y Rellenamos un Vertex Buffer Object (VBO) con las indices de las caras de la malla
		glGenBuffers(1, &vbo[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->mNumFaces * sizeof(GLuint), indices, GL_STATIC_DRAW);

		// Atributo 5 --> Indices de la malla
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(5);

		delete indices;
	}

	// Desbindeamos los buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

FractureMesh::~FractureMesh()
{
	// Eliminamos las mallas cargadas en la escena
	for (int i = 0; i < meshEntries.size(); ++i)
	{
		delete meshEntries.at(i);
	}
	meshEntries.clear();

	RemoveRigidBody();

	//delete mesh;
}

FractureMesh::MeshData::~MeshData()
{
	// Al eliminar la malla, eliminamos los buffers creados
	if (vbo[VERTEX_BUFFER])
	{
		glDeleteBuffers(1, &vbo[VERTEX_BUFFER]);
		vbo[VERTEX_BUFFER] = NULL;
	}

	if (vbo[TEXCOORD_BUFFER])
	{
		glDeleteBuffers(1, &vbo[TEXCOORD_BUFFER]);
		vbo[TEXCOORD_BUFFER] = NULL;
	}

	if (vbo[NORMAL_BUFFER])
	{
		glDeleteBuffers(1, &vbo[NORMAL_BUFFER]);
		vbo[NORMAL_BUFFER] = NULL;
	}

	if (vbo[INDEX_BUFFER])
	{
		glDeleteBuffers(1, &vbo[INDEX_BUFFER]);
		vbo[INDEX_BUFFER] = NULL;
	}

	if (vbo[BONES_BUFFER])
	{
		glDeleteBuffers(1, &vbo[BONES_BUFFER]);
		vbo[BONES_BUFFER] = NULL;
	}

	glDeleteVertexArrays(1, &vao);
}

void FractureMesh::SetRigidBody(btRigidBody *Rigid)
{
	RigidBody = Rigid;
}

void FractureMesh::RemoveRigidBody()
{
	if (RigidBody != NULL)
	{
		delete RigidBody;
		RigidBody = NULL;
	}
}

btRigidBody* FractureMesh::GetRigidBody()
{
	return RigidBody;
}

void FractureMesh::MeshData::CreateHullShape(const aiVector3D *vp)
{
	((btConvexHullShape*)HullShape)->addPoint(btVector3(vp->x, vp->y, vp->z));

}

btConvexHullShape* FractureMesh::MeshData::getHullShape()
{
	return HullShape;
}

GLfloat* FractureMesh::MeshData::getPointsBounding()
{
	return pointsBounding;
}

int FractureMesh::MeshData::GetNumVertices() const
{
	return m_numVertices;
}

int FractureMesh::MeshData::GetNumFaces() const
{
	return m_numFaces;
}

glm::mat4 FractureMesh::MeshData::ConvetAssimpMatrix(aiMatrix4x4 m)
{
	// NOTA: matriz en columnas
	return glm::mat4
		(
		1.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		m.a4, m.b4, m.c4, m.d4
		);
}

int FractureMesh::GetNumMeshes() const
{
	return m_numMeshes;
}

FractureMesh::MeshData* FractureMesh::GetMesh() const
{
	//return mesh;
	return meshEntries.at(0);
}

void FractureMesh::MeshData::Render()
{
	// Bindeamos el VAO creado anteriormente (Especifica el layout con los atributos de la malla)
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void FractureMesh::Render(bool boundingBox) const
{
	// Renderizamos las malals cargadas en la escena
	for (int i = 0; i < meshEntries.size(); ++i)
	{
	meshEntries.at(i)->Render();
	}

	//mesh->Render();
}