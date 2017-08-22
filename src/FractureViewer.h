#ifndef FractureViewer_H
#define FractureViewer_H

#include "FractureViewerGUI.h"
#include "Window.h"
#include "Time.h"
#include "Util.h"
#include "RenderUtil.h"
#include "FractureShader.h"
#include <iostream>
#include "glm.hpp"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/euler_angles.hpp>
#include <gtx/norm.hpp>
#include "FractureMesh.h"
#include "FractureTexture.h"
#include <consoleapi.h>
#include <iostream>
#include "InputManager.h"
#include "Camera.h"
#include "FracturePhysics.h"
#include "FractureTexture.h"
#include "FracGUI.h"
#include "FractureCubeMap.h"
#include "FractureLighting.h"
#include "FractureMaterial.h"
#include "FracturePhongShader.h"
#include "FractureTargetCamera.h"
#include "FractureFreeCamera.h"
#include "FractureGrid.h"
#include "FractureCamera.h"

using namespace std;
using namespace Fracture;

class FractureViewer
{
public:
	FractureViewer(double frameRate, Window* window, FracturePhysics* physicsEngine);
	~FractureViewer();

	void Start();
	void Stop();
	void ProcessInput();
	void Run();
    void Render();

	void GenImage();

	void SetMesh(std::string name);
	FractureMesh* GetMesh();
	FracturePhysics* GetPhysics();
	void SetWireframe(bool flag);
	double* GetWireframeLineWidth();
	void SetCulling(bool flag);
	void SetSkyLight(bool flag);
	void SetGrid(bool flag);
	void SetBoundingBox(bool flag);
	void SetJoints(bool flag);
	void SetSkinning(bool flag);
	void SetAmbientLight(bool flag);
	void SetDirectionalLight(bool flag);
	void SetPointLight(bool flag);
	void SetSpotLight(bool flag);
	void SetDiffuse(const char* name);
	void SetSpecular(const char* name);
	void SetAmbient(const char* name);
	void SetEmissive(const char* name);
	void SetNormal(const char* name);
	void SetAlpha(const char* name);
	FractureMaterial* GetMaterial();
	glm::dvec3* GetAmbientLight();
	DirectionalLight* GetDirectionalLight();
	FractureCubeMap* GetCubeMap();
	FractureShader* GetPhongShader();
	FractureCamera* GetCamera();

	glm::mat4 model_matrix;
	double tX;
	double tY;
	double tZ;
	FractureCubeMap* cubeMap = nullptr;

private:
	bool m_isRunning;
	double m_frameTime;
	Window* m_window;
	FracturePhysics* m_physicsEngine;
	FractureCamera camera;
	FractureGrid* grid;

	FractureViewerGUI* gui;

	bool m_bWireFrame;
	double m_lineWidth;
	bool m_bCulling;
	bool m_bSkyLight;
	bool m_bGrid;
	bool m_bAmbientLight;
	bool m_bDirectionalLight;
	bool m_bBoundingBox;
	bool m_bJoints;
	bool m_bSkinning;

	FractureTexture* diffuse = nullptr;
	FractureTexture* specular = nullptr;
	FractureTexture* ambient = nullptr;
	FractureTexture* emissive = nullptr;
	FractureTexture* normal = nullptr;
	FractureTexture* alpha = nullptr;
	FractureTexture* GUITexture = nullptr;

	float m_numPointLights, m_numSpotLights;
	glm::dvec3 m_ambientLight;
	DirectionalLight m_directionalLight;
	PointLight* m_pointLights;
	SpotLight* m_spotLights;

	FractureMaterial m_material;
	glm::dvec3 m_materialColor;
	glm::dvec3 m_specularColor;

	FractureShader meshShader;
	FractureShader cubeMapShader;
	FractureShader planeShader;
	FractureShader GUIShader;
	FractureShader simpleShader;
	FractureShader reflectShader;
	FractureShader refractShader;
	FractureShader boneShader;
	FracturePhongShader* phongShader;
	
	// test shaders
	FractureShader renderToTextureShader;

	GLuint vaoID;
	GLuint vboVerticesID;
	GLuint vboIndicesID;
	GLuint textureID;
	GLuint whiteTexID;

	GLuint fboID;
	GLuint renderTex;
	GLuint depthBuffer;

	FractureMesh* mesh;
};

#endif