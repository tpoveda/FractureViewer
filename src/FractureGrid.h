#ifndef FractureGrid_H
#define FractureGrid_H

#include "FractureShader.h"

class FractureGrid
{
public:
	FractureGrid(int width = 10, int depth = 10);
	virtual ~FractureGrid(void);

	void Render(const float* MVP);

protected:
	GLuint vaoID;
	GLuint vboVerticesID;
	GLuint vboIndicesID;
	int total_indices;
	FractureShader shader;
};




#endif