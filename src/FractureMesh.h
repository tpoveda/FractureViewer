#ifndef FractureMesh_H
#define FractureMesh_H

#include "FracturePhysics.h"
#include <GL/glew.h>
#include <scene.h>
#include <mesh.h>
#include <vector>
#include <string>
#include <glm.hpp>

#define MAX_BONES 32

class FractureMesh
{
public:
	FractureMesh(const std::string& fileName);
	virtual ~FractureMesh();

	void Render(bool boundingBox) const;

	struct MeshData
	{
		static enum BUFFERS
		{
			VERTEX_BUFFER,
			TEXCOORD_BUFFER,
			NORMAL_BUFFER,
			INDEX_BUFFER,
			TANGENT_BUFFER,
			BONES_BUFFER
		};

		GLuint vao;
		GLuint vbo[5];
		GLfloat* pointsBounding;

		unsigned int elementCount;

		GLfloat* g_vp;
		GLfloat* g_vn;
		GLfloat* g_vt;
		GLfloat* g_vtans;
		GLint* bone_ids;

		glm::mat4 bone_offset_matrices[MAX_BONES];

		int g_point_count;
		int g_bone_count;

		MeshData(aiMesh* mesh);
		~MeshData();

		int m_numVertices;
		int m_numFaces;

		void Load(aiMesh* mesh);
		void Render();
		void CreateHullShape(const aiVector3D *vp);

		int GetNumVertices() const;
		int GetNumFaces() const;
		glm::mat4 ConvetAssimpMatrix(aiMatrix4x4 m);

		GLfloat *getPointsBounding();
		btConvexHullShape* getHullShape();
		btConvexHullShape* HullShape;
	};

	std::vector<MeshData*> meshEntries;
	btRigidBody *RigidBody;

	MeshData* mesh;
	int m_numMeshes = 0;
	int GetNumMeshes() const;
	MeshData* GetMesh() const;
	void SetRigidBody(btRigidBody *Rigid);
	void RemoveRigidBody();
	btRigidBody* GetRigidBody();
};

#endif