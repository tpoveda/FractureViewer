// ############################### //
// ####### Fracture Viewer ####### //
// ############################### //

#include "FractureViewer.h"
#include "FracturePhysics.h"
#include "FractureResourceManager.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include <math.h>
#include <gtx/string_cast.hpp>
#include <gtx/rotate_vector.hpp>

#define FRONT "./res/textures/negz.jpg"
#define BACK "./res/textures/posz.jpg"
#define TOP "./res/textures/posy.jpg"
#define BOTTOM "./res/textures/negy.jpg"
#define LEFT "./res/textures/negx.jpg"
#define RIGHT "./res/textures/posx.jpg"

static const int MAX_POINT_LIGHTS = 4;
static const int MAX_SPOT_LIGHTS = 4;

//for floating point imprecision
const float EPSILON = 0.001f;
const float EPSILON2 = EPSILON*EPSILON;
float theta = 0.0f;
float rot_speed = 50.0f;		// 50 radianes por segundo
glm::mat4 ear_mat = glm::mat4(1);

// Variables VAO, VBOS de geometria extra
GLuint floorVAO, floorVBO, cubeMapVAO, cubeMapVBO, bonesVAO, bonesVBO;

// Otras variables
const float panelWidth = 100;
const float panelHeight = 100;
glm::mat4 cube_model_mat = glm::mat4(1.0f);
float rot = 0.0f;

class DebugDraw : public btIDebugDraw
{

public:

	DebugDraw() :
		mode(DBG_NoDebug)
	{

	}

	void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &fromColor, const btVector3 &toColor)
	{

		glBegin(GL_LINES);
		glColor3f(fromColor.getX(), fromColor.getY(), fromColor.getZ());
		glVertex3f(from.getX(), from.getY(), from.getZ());
		glColor3f(toColor.getX(), toColor.getY(), toColor.getZ());
		glVertex3f(to.getX(), to.getY(), to.getZ());
		glVertex3f(15, 0, 0);
		glEnd();
	}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		drawLine(from, to, color, color);
	}

	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
	{
		const btVector3 to(PointOnB + normalOnB*distance);
		drawLine(PointOnB, to, color);
	}

	void reportErrorWarning(const char* text)
	{
		std::cout << "Error Bullet: text" << std::endl;
	}

	void draw3dText(const btVector3& location, const char* text)
	{
		if (text != NULL)
		{
			int tam = strlen(text);
			for (int x = 0; x<tam; x++)
			{
				glPushMatrix();
				glTranslatef(location.getX(), location.getY(), location.getZ());
				glColor3f(250, 250, 250);
				glPopMatrix();
			}
		}

	}

	void setDebugMode(int mode) { this->mode = mode; }

	int getDebugMode() const { return this->mode; }

private:

	int mode;

};

FractureViewer::FractureViewer(double frameRate, Window* window, FracturePhysics* physicsEngine) :
m_isRunning(false),
m_frameTime(1.0 / frameRate),
m_window(window),
m_physicsEngine(physicsEngine),
m_bWireFrame(false),
m_bAmbientLight(true),
m_bDirectionalLight(true),
m_bSkyLight(false),
m_bGrid(true),
m_bBoundingBox(false),
m_bJoints(false),
m_bSkinning(false),
m_lineWidth(2),
mesh(nullptr),
m_numPointLights(0),
m_numSpotLights(0),
tX(0),
tY(0),
tZ(0)
{
	InputManager::SetDebug(m_window->IsDebugued());
	gui = new FractureViewerGUI(this);
	gui->SetVisible(false);
}

FractureViewer::~FractureViewer()
{
	delete mesh;
	delete cubeMap;
}

void FractureViewer::Start()
{
	if (m_isRunning) return;

	m_isRunning = true;
	Run();
}

void FractureViewer::Stop()
{
	m_isRunning = false;
}

// Target Camera
void FractureViewer::ProcessInput()
{
	if (InputManager::GetKeyDown(InputManager::KEY::KEY_Z))
	{
		theta += rot_speed * Time::GetDelta();
	}
	if (InputManager::GetKeyDown(InputManager::KEY::KEY_X))
	{
		theta -= rot_speed * Time::GetDelta();
	}
	if (InputManager::GetMouseDown(InputManager::MOUSE::LEFT_MOUSE))
	{
		gui->ProcessMouseLeftButtonDown(InputManager::GetMousePosition()[0], InputManager::GetMousePosition()[1]);
		camera.oldX = InputManager::GetMousePosition()[0];
		camera.oldY = InputManager::GetMousePosition()[1];
	}
	if (InputManager::GetMouseDown(InputManager::MOUSE::RIGHT_MOUSE))
	{
		gui->ProcessMouseRightButtonDown(InputManager::GetMousePosition()[0], InputManager::GetMousePosition()[1]);
	}
	if (InputManager::GetMouseUp(InputManager::MOUSE::LEFT_MOUSE))
	{
		gui->ProcessMouseLeftButtonUp(InputManager::GetMousePosition()[0], InputManager::GetMousePosition()[1]);
	}
	if (InputManager::GetMouseUp(InputManager::MOUSE::RIGHT_MOUSE))
	{
		gui->ProcessMouseRightButtonUp(InputManager::GetMousePosition()[0], InputManager::GetMousePosition()[1]);
	}
	if (InputManager::GetEvent().type == SDL_MOUSEMOTION)
	{
		gui->ProcessMouseMotion(InputManager::GetMousePosition()[0], InputManager::GetMousePosition()[1]);

		if (!gui->IsPointerInside(InputManager::GetMousePosition()[0], InputManager::GetMousePosition()[1]))
		{
			if (camera.GetCameraType() == FractureCamera::CameraType::TARGET)
			{
				if (InputManager::GetEvent().motion.state & InputManager::GetKey(InputManager::KEY::KEY_L) & SDL_BUTTON(1))
				{
					// control temporal para el poscionamiento de la luz direccional
					m_directionalLight.direction.x += float(camera.oldY - InputManager::GetMousePosition()[1]) / 5.0f;
					m_directionalLight.direction.z += (camera.oldX - float(InputManager::GetMousePosition()[0])) / 5.0f;
				}
				else if (InputManager::GetEvent().motion.state & SDL_BUTTON(1))
				{
					camera.rY += float(camera.oldY - InputManager::GetMousePosition()[1]) / 5.0f;
					camera.rX += (camera.oldX - float(InputManager::GetMousePosition()[0])) / 5.0f;
					camera.Rotate(camera.rX, camera.rY, 0.0f);
				}
				if (InputManager::GetEvent().motion.state & SDL_BUTTON(2))
				{
					float dy = float(InputManager::GetMousePosition()[1] - camera.oldY) / 100.0f;
					float dx = float(camera.oldX - InputManager::GetMousePosition()[0]) / 100.0f;
					camera.Pan(dx, dy);
				}
				if (InputManager::GetEvent().motion.state & SDL_BUTTON(3))
				{
					camera.dst = (InputManager::GetMousePosition()[1] - camera.oldY) / 15.0f;
					camera.Zoom(camera.dst);
				}
			}
			else if (camera.GetCameraType() == FractureCamera::CameraType::FREE)
			{
				if (InputManager::GetEvent().motion.state & InputManager::GetKey(InputManager::KEY::KEY_L) & SDL_BUTTON(1))
				{
					// control temporal para el poscionamiento de la luz direccional
					m_directionalLight.direction.x += float(camera.oldY - InputManager::GetMousePosition()[1]) / 5.0f;
					m_directionalLight.direction.z += (camera.oldX - float(InputManager::GetMousePosition()[0])) / 5.0f;
				}
				else if (InputManager::GetEvent().motion.state & SDL_BUTTON(1))
				{
					camera.rY += float(InputManager::GetMousePosition()[1] - camera.oldY) / 5.0f;
					camera.rX += float(camera.oldX - InputManager::GetMousePosition()[0]) / 5.0f;
					camera.Rotate(camera.rX, camera.rY, 0.0f);
				}
				if (InputManager::GetEvent().motion.state & SDL_BUTTON(2))
				{
					float dy = float(InputManager::GetMousePosition()[1] - camera.oldY) / 100.0f;
					float dx = float(camera.oldX - InputManager::GetMousePosition()[0]) / 100.0f;
					camera.Pan(dx, dy);
				}
				if (InputManager::GetEvent().motion.state & SDL_BUTTON(3))
				{
					camera.fov += (InputManager::GetMousePosition()[1] - camera.oldY) / 15.0f;
					camera.SetProjection(camera.fov, camera.GetAspectRatio());
				}
			}
		}

		camera.oldX = InputManager::GetMousePosition()[0];
		camera.oldY = InputManager::GetMousePosition()[1];
	}
	
	if (InputManager::GetEvent().type == SDL_QUIT) Stop();
	if (InputManager::GetKeyDown(InputManager::KEY::KEY_SPACE))
	{
		if (gui->GetVisible()) gui->SetVisible(false);
		else gui->SetVisible(true);
	}
	
	if (camera.GetCameraType() == FractureCamera::CameraType::FREE)
	{
		if (InputManager::GetKey(InputManager::KEY::KEY_W))
		{
			camera.Walk(Time::GetDelta());
		}
		if (InputManager::GetKey(InputManager::KEY::KEY_S))
		{
			camera.Walk(-Time::GetDelta());
		}
		if (InputManager::GetKey(InputManager::KEY::KEY_A))
		{
			camera.Strafe(-Time::GetDelta());
		}
		if (InputManager::GetKey(InputManager::KEY::KEY_D))
		{
			camera.Strafe(Time::GetDelta());
		}
		if (InputManager::GetKey(InputManager::KEY::KEY_Q))
		{
			camera.Lift(Time::GetDelta());
		}
		if (InputManager::GetKey(InputManager::KEY::KEY_Z))
		{
			camera.Lift(Time::GetDelta());
		}

		glm::vec3 t = camera.GetTranslation();
		if (glm::dot(t, t) > EPSILON2)
		{
			camera.SetTranslation(t * 0.95f);
		}
	}

	if (InputManager::GetKeyDown(InputManager::KEY::KEY_P))
	{
		GenImage();
	}
}

void FractureViewer::Run()
{
	if (!m_isRunning) return;

	/*--------------------------------INICIALIZACION DE DEVIL Y FISICAS --------------------------------*/
	// Inicializamos DeVIL
	ilInit();
	//ilClearColour(255, 255, 255, 0);
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	ILenum ilError = ilGetError();

	// Inicializamos el motor de fisicas
	m_physicsEngine->Init();

	/*--------------------------------GRID --------------------------------*/
	grid = new FractureGrid(10, 10);

	/*--------------------------------CUBEMAPS --------------------------------*/
	cubeMap = new FractureCubeMap(FRONT, BACK, TOP, BOTTOM, LEFT, RIGHT);

	/*--------------------------------GEOMETRIA --------------------------------*/
	// Malla visulizada
	//mesh = new FractureMesh("./res/meshes/monkey_with_bones_y_up.dae");
	mesh = new FractureMesh("./res/meshes/sphere.obj");
	m_physicsEngine->createRigid(0, mesh, 11);

	// Huesos
	float bone_positions[3 * 256];
	int c = 0;

	for (int i = 0; i < mesh->GetMesh()->g_bone_count; i++)
	{
		printf(glm::to_string(mesh->GetMesh()->bone_offset_matrices[i]).c_str());

		bone_positions[c++] = -glm::vec3(mesh->GetMesh()->bone_offset_matrices[i][3]).x;
		bone_positions[c++] = -glm::vec3(mesh->GetMesh()->bone_offset_matrices[i][3]).y;
		bone_positions[c++] = -glm::vec3(mesh->GetMesh()->bone_offset_matrices[i][3]).z;
	}

	glGenBuffers(1, &bonesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, bonesVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * mesh->GetMesh()->g_bone_count * sizeof(float), bone_positions, GL_STATIC_DRAW);
	glGenVertexArrays(1, &bonesVAO);
	glBindVertexArray(bonesVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	// Plano suelo
	float planePoints[] =
	{
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f, 1.0f,
		-1.0f, 1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f
	};
	glGenBuffers(1, &floorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof (planePoints), planePoints, GL_STATIC_DRAW);
	glGenVertexArrays(1, &floorVAO);
	glBindVertexArray(floorVAO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	// Cubo CubeMap
	float cubePoints[] = {
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

	glGenBuffers(1, &cubeMapVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeMapVBO);
	glBufferData(
		GL_ARRAY_BUFFER, 3 * 36 * sizeof (GLfloat), &cubePoints, GL_STATIC_DRAW);

	glGenVertexArrays(1, &cubeMapVAO);
	glBindVertexArray(cubeMapVAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, cubeMapVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	/*--------------------------------TEXTURAS --------------------------------*/
	GUITexture = FractureResourceManager::GetInstance()->GetTexture(7, "./res/textures/logo.png");

	/*-------------------------------- CAMARA --------------------------------*/
	glm::vec3 p = glm::vec3(5);
	camera.SetPosition(p);
	camera.SetProjection(45.0f, (GLfloat)m_window->GetWidth() / (GLfloat)m_window->GetHeight());

	if (camera.GetCameraType() == FractureCamera::CameraType::TARGET)
	{
		camera.SetTarget(glm::vec3(0, 0, 0));
		camera.Rotate(camera.rX, camera.rY, 0);
	}
	else if (camera.GetCameraType() == FractureCamera::CameraType::FREE)
	{
		glm::vec3 look = glm::normalize(p);
		float yaw = glm::degrees(float(atan2(look.z, look.x) + M_PI));
		float pitch = glm::degrees(asin(look.y));
		camera.rX = yaw;
		camera.rY = pitch;
		camera.Rotate(camera.rX, camera.rY, 0);

	}

	/*-------------------------------- LUCES --------------------------------*/
	m_ambientLight = glm::vec3(0.5f, 0.5f, 0.5f);
	m_directionalLight = DirectionalLight(BaseLight(glm::dvec3(1, 1, 1), glm::dvec3(1, 1, 1), 1.0f), glm::dvec3(1, 1, 1));
	m_pointLights = new PointLight[1];
	m_pointLights[0] = PointLight(BaseLight(glm::dvec3(1, 0.5f, 1), glm::dvec3(1, 0.5f, 1), 0.8f), Attenuation(0, 0, 1), glm::dvec3(1, 1, 1), 10);

	/*-------------------------------- MATERIALES --------------------------------*/
	m_materialColor = glm::dvec3(1, 1, 1);
	m_specularColor = glm::dvec3(1, 1, 1);
	m_material = FractureMaterial(diffuse, specular, ambient, emissive, normal, alpha, m_materialColor, m_specularColor, 1, 8);
	
	/*--------------------------------SHADERS --------------------------------*/
	simpleShader.LoadFromFile(GL_VERTEX_SHADER, "./res/shaders/SimpleShaderVert.glsl");
	simpleShader.LoadFromFile(GL_FRAGMENT_SHADER, "./res/shaders/SimpleShaderFrag.glsl");
	planeShader.LoadFromFile(GL_VERTEX_SHADER, "./res/shaders/PlaneShaderVert.glsl");
	planeShader.LoadFromFile(GL_FRAGMENT_SHADER, "./res/shaders/PlaneShaderFrag.glsl");
	GUIShader.LoadFromFile(GL_VERTEX_SHADER, "./res/shaders/GUIShaderVert.glsl");
	GUIShader.LoadFromFile(GL_FRAGMENT_SHADER, "./res/shaders/GUIShaderFrag.glsl");
	cubeMapShader.LoadFromFile(GL_VERTEX_SHADER, "./res/shaders/CubeMapShaderVert.glsl");
	cubeMapShader.LoadFromFile(GL_FRAGMENT_SHADER, "./res/shaders/CubeMapShaderFrag.glsl");
	reflectShader.LoadFromFile(GL_VERTEX_SHADER, "./res/shaders/ReflectShaderV2Vert.glsl");
	reflectShader.LoadFromFile(GL_FRAGMENT_SHADER, "./res/shaders/ReflectShaderV2Frag.glsl");
	boneShader.LoadFromFile(GL_VERTEX_SHADER, "./res/shaders/BoneShaderVert.glsl");
	boneShader.LoadFromFile(GL_FRAGMENT_SHADER, "./res/shaders/BoneShaderFrag.glsl");
	renderToTextureShader.LoadFromFile(GL_VERTEX_SHADER, "./res/shaders/RenderToTextureVert.glsl");
	renderToTextureShader.LoadFromFile(GL_FRAGMENT_SHADER, "./res/shaders/RenderToTextureFrag.glsl");

	phongShader = FracturePhongShader::GetInstance();
	FracturePhongShader::SetAmbientLight(m_ambientLight);
	FracturePhongShader::SetDirectionalLight(m_directionalLight);
	FracturePhongShader::SetPointLights(m_pointLights, 1);

	simpleShader.CreateAndLinkProgram();
	planeShader.CreateAndLinkProgram();
	GUIShader.CreateAndLinkProgram();
	cubeMapShader.CreateAndLinkProgram();
	reflectShader.CreateAndLinkProgram();
	boneShader.CreateAndLinkProgram();
	renderToTextureShader.CreateAndLinkProgram();

	simpleShader.Use();
	simpleShader.AddUniform("view_mat");
	simpleShader.AddUniform("projection_mat");
	simpleShader.UnUse();

	reflectShader.Use();
	reflectShader.AddUniform("model_mat");
	reflectShader.AddUniform("view_mat");
	reflectShader.AddUniform("projection_mat");
	reflectShader.UnUse();

	planeShader.Use();
	planeShader.AddUniform("V");
	planeShader.AddUniform("P");
	planeShader.UnUse();

	GUIShader.Use();
	GUIShader.AddUniform("gui_scale");
	GUIShader.UnUse();

	cubeMapShader.Use();
	cubeMapShader.AddUniform("skyLightIntensity");
	cubeMapShader.AddUniform("model_mat");
	cubeMapShader.AddUniform("view_mat");
	cubeMapShader.AddUniform("projection_mat");
	cubeMapShader.UnUse();	

	char name[64];
	boneShader.Use();
	boneShader.AddUniform("model_mat");
	boneShader.AddUniform("view_mat");
	boneShader.AddUniform("projection_mat");

	for (int i = 0; i < 256; i++)
	{
		sprintf(name, "bone_matrices[%i]", i);
		boneShader.AddUniform(name);
	}
	boneShader.UnUse();

	renderToTextureShader.Use();
	renderToTextureShader.AddUniform("renderTex");
	renderToTextureShader.AddUniform("model_mat");
	renderToTextureShader.AddUniform("view_mat");
	renderToTextureShader.AddUniform("projection_mat");
	renderToTextureShader.UnUse();

	/********************** FBOS (EFECTOS) ************************/
	/*// generamos y bindeamos el framebuffer
	glGenFramebuffers(1, &fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);

	// creamos el objeto textura
	glGenTextures(1, &renderTex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// bindeamos la textura al FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);

	// creamos el depth buffer
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);

	// bindeamos el depth buffer al FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	// fijamos el target para las variables de salida del fragment shader
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result == GL_FRAMEBUFFER_COMPLETE) {
		cout << "El framebuffer esta lleno" << endl;
	}
	else {
		cout << "ERROR en el framebuffer: " << result << endl;
	}

	// Desbindeamos el framebuffer, y volvemos al framebuffer por defecto
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// creamos una textura blanca de un pixel de tamaño
	GLubyte whiteTex[] = { 255, 255, 255, 255 };
	glGenTextures(1, &whiteTexID);
	glBindTexture(GL_TEXTURE_2D, whiteTexID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whiteTex);*/

	/*--------------------------------MODELO --------------------------------*/
	model_matrix = glm::mat4(1);

	/*--------------------------------PROPIEDADES DE RENDERIZADO --------------------------------*/
	simpleShader.Use();
	glUniformMatrix4fv(simpleShader("model_mat"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(simpleShader("view_mat"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
	glUniformMatrix4fv(simpleShader("projection_mat"), 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));
	simpleShader.UnUse();

	phongShader->Use();
	FracturePhongShader::SetAmbientLight(m_ambientLight);
	FracturePhongShader::SetAmbientLightActive(m_bAmbientLight);
	FracturePhongShader::SetDirectionalLight(m_directionalLight);
	FracturePhongShader::SetDirectionalLightActive(m_bDirectionalLight);
	phongShader->UpdateUniforms(camera.GetViewMatrix(),camera.GetProjectionMatrix(), m_material);
	phongShader->UnUse();

	cubeMapShader.Use();
	glUniform1f(cubeMapShader("skyLightIntensity"), cubeMap->GetIntensity());
	glUniformMatrix4fv(cubeMapShader("model_mat"), 1, GL_FALSE, glm::value_ptr(cube_model_mat));
	glUniformMatrix4fv(cubeMapShader("view_mat"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
	glUniformMatrix4fv(cubeMapShader("projection_mat"), 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));
	cubeMapShader.UnUse();

	boneShader.Use();
	glUniformMatrix4fv(boneShader("model_mat"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
	glUniformMatrix4fv(boneShader("view_mat"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
	glUniformMatrix4fv(boneShader("projection_mat"), 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));
	
	for (int i = 0; i < 256; i++)
	{
		sprintf(name, "bone_matrices[%i]", i);
		glUniformMatrix4fv(boneShader(name), 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
	}
	boneShader.UnUse();

	/*renderToTextureShader.Use();
	glUniform1f(renderToTextureShader("renderTex"), 1);
	glUniformMatrix4fv(renderToTextureShader("model_mat"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
	glUniformMatrix4fv(renderToTextureShader("view_mat"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
	glUniformMatrix4fv(renderToTextureShader("projection_mat"), 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));
	renderToTextureShader.UnUse();*/

	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());

	/*--------------------------------INICIALIZACION DE INTERFAZ --------------------------------*/
	// Cargamos la interfaz y sus recursos
	if (!gui->Init(m_window)) return;
	gui->Load();

	/*--------------------------------LOOP DE RENDERIZADO --------------------------------*/
	double lastTime = Time::GetTime();
	double frameCounter = 0;
	double unprocessedTime = 0;
	int frames = 0;
	float temp = 0;

	while (m_isRunning)
	{
		bool render = false;

		double startTime = Time::GetTime();
		double passedTime = startTime - lastTime;
		lastTime = startTime;

		unprocessedTime += passedTime;
		frameCounter += passedTime;

		if (frameCounter >= 0)
		{
			double totalTime = ((1000.0f * frameCounter) / ((double)frames));
			//printf("Frames: %d\n", (double)frames);
			frames = 0;
			frameCounter = 0;
		}

		while (unprocessedTime > m_frameTime)
		{

			if (m_window->IsCloseRequested())
			{
				Stop();
			}

			Time::SetDelta(m_frameTime);

			render = true;

			rot++;

			m_window->Update();		// actualiza input manager
			ProcessInput();

			unprocessedTime -= m_frameTime;
		}

		if (render)
		{
			Render();
			m_window->SwapBuffers();
			frames++;
		}
		else
		{
			Util::Sleep(1);
		}
	}

	meshShader.DeleteShaderProgram();

	glDeleteBuffers(1, &vboVerticesID);
	glDeleteBuffers(1, &vboIndicesID);
	glDeleteVertexArrays(1, &vaoID);

	m_physicsEngine->Stop();
}

void FractureViewer::Render()
{
	//Renderizamos la textura
	//glBindFramebuffer(GL_FRAMEBUFFER, fboID);
	//glViewport(0, 0, 512, 512);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*renderToTextureShader.Use();
	glUniform1i(renderToTextureShader("renderTex"), 1);
	glUniformMatrix4fv(renderToTextureShader("projection_mat"), 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));
	glUniformMatrix4fv(renderToTextureShader("view_mat"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
	mesh->Render(false);
	renderToTextureShader.UnUse();*/

	//Volvemos al render por defecto
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Renderizamos el Cube Map
	if (m_bSkyLight)
	{
		glDepthMask(GL_FALSE);
		cubeMapShader.Use();
		glUniform1f(cubeMapShader("skyLightIntensity"), cubeMap->GetIntensity());
		glUniformMatrix4fv(cubeMapShader("model_mat"), 1, GL_FALSE, glm::value_ptr(cube_model_mat));
		glUniformMatrix4fv(cubeMapShader("projection_mat"), 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));
		glUniformMatrix4fv(cubeMapShader("view_mat"), 1, GL_FALSE, glm::value_ptr(glm::inverse(camera.GetRotationMatrix())));
		glActiveTexture(GL_TEXTURE0);
		cubeMap->Render();
		cubeMapShader.UnUse();
		glDepthMask(GL_TRUE);
	}

	if (m_bGrid)
	{
		// Renderizamos el suelo
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		grid->Render(glm::value_ptr(camera.GetProjectionMatrix() * camera.GetViewMatrix()));
	}

	// Renderizamos las malla
	if (!m_bSkinning)
	{
		model_matrix = glm::translate(glm::mat4(1), glm::vec3(tX, tY, tZ));
		if (m_bCulling) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);
		phongShader->Use();
		phongShader->SetUniform("skyIntensity", cubeMap->GetIntensity());
		FracturePhongShader::SetAmbientLight(m_ambientLight);
		FracturePhongShader::SetAmbientLightActive(m_bAmbientLight);
		FracturePhongShader::SetDirectionalLight(m_directionalLight);
		FracturePhongShader::SetDirectionalLightActive(m_bDirectionalLight);
		phongShader->UpdateUniforms(camera.GetViewMatrix(), camera.GetProjectionMatrix(), m_material, model_matrix);
		if (mesh != nullptr) mesh->Render(m_bBoundingBox);

		// Colisiones
		DebugDraw debugDraw;
		debugDraw.setDebugMode(
			btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE |
			btIDebugDraw::DBG_FastWireframe |
			btIDebugDraw::DBG_DrawWireframe |
			btIDebugDraw::DBG_DrawAabb |
			btIDebugDraw::DBG_DrawContactPoints |
			btIDebugDraw::DBG_DrawText |
			btIDebugDraw::DBG_DrawConstraintLimits |
			btIDebugDraw::DBG_DrawConstraints
			);

		m_physicsEngine->getWorld()->setDebugDrawer(&debugDraw);

		if (m_bBoundingBox)
			m_physicsEngine->getWorld()->debugDrawWorld();

		phongShader->UnUse();
	}

	/*renderToTextureShader.Use();
	glUniformMatrix4fv(renderToTextureShader("projection_mat"), 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));
	glUniformMatrix4fv(renderToTextureShader("view_mat"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
	glUniform1i(renderToTextureShader("renderTex"), 0);
	mesh->Render(false);
	renderToTextureShader.UnUse();*/

	if (m_bWireFrame)
	{	
		if (m_lineWidth == 0)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else 
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_CULL_FACE);
			glLineWidth(m_lineWidth);
			simpleShader.Use();
			glUniformMatrix4fv(simpleShader("projection_mat"), 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));
			glUniformMatrix4fv(simpleShader("view_mat"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
			mesh->Render(m_bBoundingBox);
			simpleShader.UnUse();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	if(m_bSkinning)
	{
		boneShader.Use();
		glUniformMatrix4fv(boneShader("model_mat"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
		glUniformMatrix4fv(boneShader("projection_mat"), 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));
		glUniformMatrix4fv(boneShader("view_mat"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

		// Aplicamos la matriz de desplazamiento del hueso en el primer hueso (oreja izquierda) = la posicion del hueso es el origen (y el punto de pivote para la rotacion)
		// Aplicamos la animacion (rotacion en el eje Z)
		// Aplicamos la inversa de la matriz de desplazamiento del hueso = el origen vuelve a su posicion original
		// NOTA: Cuando cargamos animaciones, éstas incorpoan un bit al final que aplica esta inversa por lo que no es necesario hacerlo
		glm::mat4 ear_mat = glm::inverse(mesh->GetMesh()->bone_offset_matrices[0]) * glm::rotate(glm::mat4(1), theta, glm::vec3(0, 0, 1)) * mesh->GetMesh()->bone_offset_matrices[0];

		glUniformMatrix4fv(boneShader("bone_matrices[0]"), 1, GL_FALSE, glm::value_ptr(ear_mat));
		if (mesh != nullptr) mesh->Render(m_bBoundingBox); 
		boneShader.UnUse();
	}

	if (m_bJoints)
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_PROGRAM_POINT_SIZE);
		boneShader.Use();
		glBindVertexArray(bonesVAO);
		glUniformMatrix4fv(boneShader("projection_mat"), 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));
		glUniformMatrix4fv(boneShader("view_mat"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
		glDrawArrays(GL_POINTS, 0, mesh->GetMesh()->g_point_count);
		boneShader.UnUse();
		glDisable(GL_PROGRAM_POINT_SIZE);
	}
	


	// Renderizamos el panel GUI
	glDisable(GL_DEPTH_TEST);
	GUIShader.Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GUITexture->GetTextureID());
	float x_scale = panelWidth / m_window->GetWidth();
	float y_scale = panelHeight / m_window->GetHeight();
	glUniform2f(GUIShader("gui_scale"), x_scale, y_scale);
	glBindVertexArray(floorVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindTexture(GL_TEXTURE_2D, 0);
	GUIShader.UnUse();

	// Renderizamos la interfaz
	glActiveTexture(GL_TEXTURE0);
	gui->Render();
}

void FractureViewer::GenImage()
{
	int width = m_window->GetWidth();
	int height = m_window->GetHeight();
	int bytesPerPixel = 3;
	int sizeOfByte = sizeof(unsigned char);
	int totalSize = width * height * sizeOfByte * bytesPerPixel;

	unsigned char* imData = (unsigned char*)malloc(totalSize);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, imData);

	ILuint imageID = ilGenImage();
	
	ilBindImage(imageID);
	ilTexImage(width, height, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, imData);
	ilEnable(IL_FILE_OVERWRITE);

	// creamos el nombre del archivo
	char name[1024];
	long int t = time(NULL);
	sprintf(name, "captura_%ld.png", t);

	ilSave(IL_PNG, name);
	printf("Imagen renderizada! anchura=%d, altura=%d, bpp=%d\n", ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_BPP));
}

/*--------------------------------FUNCIONES EXTRA/DE PRUEBA --------------------------------*/
void FractureViewer::SetMesh(std::string name)
{
	if (name == "")
	{
		delete mesh;
		mesh = nullptr;
		return;
	}

	if (mesh != nullptr)
	{
		m_physicsEngine->RemoveRigidBody(mesh->GetRigidBody());
		delete mesh;
		mesh = nullptr;
	}
	
	char path[100];
	int n = sprintf(path, "./res/meshes/%s", name.c_str());
	mesh = new FractureMesh(path);
	m_physicsEngine->createRigid(0, mesh, 11);
}

FractureMesh* FractureViewer::GetMesh()
{
	return mesh;
}

FracturePhysics* FractureViewer::GetPhysics()
{
	return m_physicsEngine;
}

void FractureViewer::SetWireframe(bool flag)
{
	m_bWireFrame = flag;
}

void FractureViewer::SetDiffuse(const char* name)
{
	if (name == "")
	{
		FractureResourceManager::GetInstance()->DropTexture(1);		// 0 = diffuse
		diffuse = nullptr;
		return;
	}

	if (diffuse != nullptr)
	{
		FractureResourceManager::GetInstance()->DropTexture(1);
		diffuse = nullptr;
	}

	char path[100];
	int n = sprintf(path, "./res/textures/%s", name);
	diffuse = FractureResourceManager::GetInstance()->GetTexture(1, path);
	m_material.diffuseTexture = diffuse;
}

void FractureViewer::SetSpecular(const char* name)
{
	if (name == "")
	{
		FractureResourceManager::GetInstance()->DropTexture(2);		// 1 = specular
		specular = nullptr;
		return;
	}

	if (specular != nullptr)
	{
		FractureResourceManager::GetInstance()->DropTexture(2);
		specular = nullptr;
	}

	char path[100];
	int n = sprintf(path, "./res/textures/%s", name);
	specular = FractureResourceManager::GetInstance()->GetTexture(2, path);
	m_material.specularTexture = specular;
}

void FractureViewer::SetAmbient(const char* name)
{
	if (name == "")
	{
		FractureResourceManager::GetInstance()->DropTexture(3);		// 2 = ambient
		ambient = nullptr;
		return;
	}

	if (ambient != nullptr)
	{
		FractureResourceManager::GetInstance()->DropTexture(3);		
		ambient = nullptr;
	}

	char path[100];
	int n = sprintf(path, "./res/textures/%s", name);
	ambient = FractureResourceManager::GetInstance()->GetTexture(3, path);
	m_material.ambientTexture = ambient;
}

void FractureViewer::SetEmissive(const char* name)
{
	if (name == "")
	{
		FractureResourceManager::GetInstance()->DropTexture(4);		// 3 = emissive
		emissive = nullptr;
		return;
	}

	if (emissive != nullptr)
	{
		FractureResourceManager::GetInstance()->DropTexture(4);
		emissive = nullptr;
	}

	char path[100];
	int n = sprintf(path, "./res/textures/%s", name);
	emissive = FractureResourceManager::GetInstance()->GetTexture(4, path);
	m_material.emissiveTexture = emissive;
}

void FractureViewer::SetNormal(const char* name)
{
	if (name == "")
	{
		FractureResourceManager::GetInstance()->DropTexture(5);		// 4 = normal
		normal = nullptr;
		return;
	}

	if (normal != nullptr)
	{
		FractureResourceManager::GetInstance()->DropTexture(5);
		normal = nullptr;
	}

	char path[100];
	int n = sprintf(path, "./res/textures/%s", name);
	normal = FractureResourceManager::GetInstance()->GetTexture(5, path);
	m_material.normalTexture = normal;
}

void FractureViewer::SetAlpha(const char* name)
{
	if (name == "")
	{
		FractureResourceManager::GetInstance()->DropTexture(6);		// 5 = alpha
		alpha = nullptr;
		return;
	}

	if (alpha != nullptr)
	{
		FractureResourceManager::GetInstance()->DropTexture(6);
		alpha = nullptr;
	}

	char path[100];
	int n = sprintf(path, "./res/textures/%s", name);
	alpha = FractureResourceManager::GetInstance()->GetTexture(6, path);
	m_material.alphaTexture = alpha;
}

double* FractureViewer::GetWireframeLineWidth()
{
	return &m_lineWidth;
}

FractureMaterial* FractureViewer::GetMaterial()
{
	return &m_material;
}

glm::dvec3* FractureViewer::GetAmbientLight()
{
	return &m_ambientLight;
}

DirectionalLight* FractureViewer::GetDirectionalLight()
{
	return &m_directionalLight;
}

FractureCubeMap* FractureViewer::GetCubeMap()
{
	return cubeMap;
}

FractureShader* FractureViewer::GetPhongShader()
{
	return phongShader;
}

FractureCamera* FractureViewer::GetCamera()
{
	return &camera;
}

void FractureViewer::SetCulling(bool flag)
{
	m_bCulling = flag;
}

void FractureViewer::SetSkyLight(bool flag)
{
	m_bSkyLight = flag;
}

void FractureViewer::SetGrid(bool flag)
{
	m_bGrid = flag;
}

void FractureViewer::SetBoundingBox(bool flag)
{
	m_bBoundingBox = flag;
}

void FractureViewer::SetJoints(bool flag)
{
	m_bJoints = flag;
}

void FractureViewer::SetSkinning(bool flag)
{
	m_bSkinning = flag;
}

void FractureViewer::SetAmbientLight(bool flag)
{
	m_bAmbientLight = flag;
}

void FractureViewer::SetDirectionalLight(bool flag)
{
	m_bDirectionalLight = flag;
}

void FractureViewer::SetPointLight(bool flag)
{
}

void FractureViewer::SetSpotLight(bool flag)
{
}