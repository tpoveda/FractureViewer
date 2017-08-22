#ifndef Mesh_H
#define Mesh_H

#include <GL/glew.h>
#include <scene.h>
#include <mesh.h>
#include <vector>
#include <string>

class Mesh
{
public :
	struct MeshData
	{
		static enum BUFFERS
		{
			VERTEX_BUFFER,
			TEXCOORD_BUFFER,
			NORMAL_BUFFER,
			INDEX_BUFFER,
			TANGENT_BUFFER

		};
		GLuint vao;
		GLuint vbo[5];

		unsigned int elementCount;

		MeshData(aiMesh *mesh);
		~MeshData();

		int m_numVertices;
		int m_numFaces;

		void Load(aiMesh *mesh);
		void Render();
		int GetNumVertices() const;
		int GetNumFaces() const;
	};

	std::vector<MeshData*> meshEntries;
	const aiScene* scene;

public:
	Mesh(const std::string& fileName);
	virtual ~Mesh();

	void Render() const;
	const aiScene* GetScene() const;
};

#endif