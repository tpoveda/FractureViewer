#include "FracBackend.h"

FracBackend* FracBackend::irrInstance = nullptr;
IrrlichtDevice* FracBackend::device = nullptr;
IVideoDriver* FracBackend::driver = nullptr;
ISceneManager* FracBackend::smgr = nullptr;
IGUIEnvironment* FracBackend::guienv = nullptr;

std::map<std::string, ISceneNode*> FracBackend::sceneNodesList;
std::map<std::string, IGUIElement*> FracBackend::GUIElementsList;

bool FracBackend::lighting = false;
bool FracBackend::octree = false;
ISceneNode* FracBackend::model = nullptr;
ISceneNode* FracBackend::skybox = nullptr;
ICameraSceneNode* FracBackend::camera[2] = {0, 0};

FracBackend::FracBackend()
{
}

FracBackend::~FracBackend()
{
	device->drop();
}

FracBackend* FracBackend::Instance()
{
	if (irrInstance == NULL)
	{
		irrInstance = new FracBackend();
		return irrInstance;
	}
	return irrInstance;
}

bool FracBackend::CreateIrrWindow(int width, int height, const char* title, bool fullscreen, bool stencilbuffer, bool vsync)
{
	device = createDevice(EDT_OPENGL, dimension2d<u32>(width, height), 32, fullscreen, stencilbuffer, vsync, nullptr);
	if (!device) return false;

	SetWindowTitle(title);

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	if (driver != NULL && smgr != NULL && guienv != NULL) return true;
	
	return false;
}

void FracBackend::SetWindowTitle(const char* title)
{
	if (device != NULL)
	{
		wchar_t wsTitle[100];
		swprintf(wsTitle, 100, L"%hs", title);
		device->setWindowCaption(wsTitle);
	}
}

IrrlichtDevice* FracBackend::GetDevice()
{
	return device;
}

IVideoDriver* FracBackend::GetVideoDriver()
{
	return driver;
}

ISceneManager* FracBackend::GetScene()
{
	return smgr;
}

IGUIEnvironment* FracBackend::GetGUI()
{
	return guienv;
}

ICameraSceneNode* FracBackend::GetActiveCamera()
{
	return device->getSceneManager()->getActiveCamera();
}

void FracBackend::AddStaticText(std::string id, const char* text, int x, int y, int width, int height, bool border, IGUIElement* parent)
{
	wchar_t wText[100];
	swprintf(wText, 100, L"%hs", text);
	IGUIStaticText* staticText = guienv->addStaticText(wText, rect<s32>(x, y, width, height), border, false, parent);

	if (staticText != nullptr)
	{
		GUIElementsList.insert(std::pair<std::string, IGUIStaticText*>(id, staticText));
	}
}

void FracBackend::AddWindow(std::string id, int x, int y, int width, int height, const char* caption, IGUIElement* parent)
{
	wchar_t wCaption[50];
	swprintf(wCaption, 50, L"%hs", caption);
	IGUIWindow* window = guienv->addWindow(core::rect<s32>(x, y, width, height), false, wCaption, parent);

	if (window != nullptr)
	{
		GUIElementsList.insert(std::pair<std::string, IGUIWindow*>(id, window));
	}
}

void FracBackend::AddControlTab(std::string id, int x, int y, int width, int height, IGUIElement* parent, bool fillBackground, bool border)
{
	IGUITabControl* controlTab = guienv->addTabControl(core::rect<s32>(x, y, width, height), parent, fillBackground, border);

	if (controlTab != nullptr)
	{
		GUIElementsList.insert(std::pair<std::string, IGUITabControl*>(id, controlTab));
	}
}

void FracBackend::AddTab(std::string id, IGUITabControl* parentTab, char const* caption)
{
	wchar_t wCaption[50];
	swprintf(wCaption, 50, L"%hs", caption);
	IGUITab* tab = parentTab->addTab(wCaption);

	if (tab != nullptr)
	{
		GUIElementsList.insert(std::pair<std::string, IGUITab*>(id, tab));
	}
}

void FracBackend::AddEditBox(std::string id, char const* caption, int x, int y, int width, int height, bool border, IGUIElement* parent)
{
	wchar_t wCaption[50];
	swprintf(wCaption, 50, L"%hs", caption);
	IGUIEditBox* editBox = guienv->addEditBox(wCaption, core::rect<s32>(x, y, width, height), border, parent);

	if (editBox != nullptr)
	{
		GUIElementsList.insert(std::pair<std::string, IGUIEditBox*>(id, editBox));
	}
}

void FracBackend::AddButton(std::string id, char const* label, int x, int y, int width, int height, IGUIElement* parent)
{
	wchar_t wLabel[50];
	swprintf(wLabel, 50, L"%hs", label);
	IGUIButton* button = guienv->addButton(core::rect<s32>(x, y, width, height), parent, 0, wLabel);

	if (button != nullptr)
	{
		GUIElementsList.insert(std::pair<std::string, IGUIButton*>(id, button));
	}
}

void FracBackend::AddScrollbar(std::string id, bool horizontal, int x, int y, int width, int height, int max, int min, int pos, int step, IGUIElement* parent)
{
	IGUIScrollBar* scrollbar = guienv->addScrollBar(horizontal, core::rect<s32>(x, y, width, height), parent);
	if (scrollbar != nullptr)
	{
		scrollbar->setMax(max);
		scrollbar->setMin(min);
		scrollbar->setPos(pos);
		scrollbar->setSmallStep(step);
		GUIElementsList.insert(std::pair<std::string, IGUIScrollBar*>(id, scrollbar));
	}
}

void FracBackend::AddMessageBox(char const* caption, char const* text)
{
	wchar_t wCaption[50];
	wchar_t wText[100];
	swprintf(wCaption, 50, L"%hs", caption);
	swprintf(wText, 100, L"%hs", text);
	guienv->addMessageBox(wCaption, wText);
}

void FracBackend::AddAnimatedMesh(std::string id, char const* meshPath, char const* texturePath)
{
	IAnimatedMesh* mesh = smgr->getMesh(meshPath);
	if (!mesh)
	{
		device->drop();
		return;
	}

	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);

	if (node)
	{
		sceneNodesList.insert(std::pair<std::string, IAnimatedMeshSceneNode*>(id, node));
		node->setMaterialFlag(EMF_LIGHTING, lighting);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture(0, driver->getTexture(texturePath));
	}
}

void FracBackend::AddCamera(std::string id, int posX, int posY, int posZ, int lookAtX, int lookAtY, int lookAtZ)
{
	ICameraSceneNode* camera = smgr->addCameraSceneNode(0, vector3df(posX, posY, posZ), vector3df(lookAtX, lookAtY, lookAtZ));
	if(camera != nullptr) sceneNodesList.insert(std::pair<std::string, ICameraSceneNode*>(id, camera));
}

void FracBackend::AddFPSCamera(std::string id, int posX, int posY, int posZ, int lookAtX, int lookAtY, int lookAtZ, float rotateVelocity, float moveSpeed)
{
	ICameraSceneNode* fpsCamera = smgr->addCameraSceneNodeFPS(0, rotateVelocity, moveSpeed);
	fpsCamera->setPosition(vector3df(posX, posY, posZ));
	fpsCamera->setTarget(vector3df(lookAtX, lookAtY, lookAtZ));
	if(fpsCamera!=nullptr)sceneNodesList.insert(std::pair<std::string, ICameraSceneNode*>(id, fpsCamera));
}

void FracBackend::SetActiveCamera(std::string id)
{
	ICameraSceneNode* newCamera = static_cast<ICameraSceneNode*>(GetSceneNode(id));
	ICameraSceneNode* activeCamera = GetActiveCamera();

	if (newCamera != activeCamera)
	{
		activeCamera->setInputReceiverEnabled(false);
		newCamera->setInputReceiverEnabled(true);
		GetDevice()->getSceneManager()->setActiveCamera(newCamera);
	}
}

void FracBackend::AddCube(std::string id)
{
	IMeshSceneNode* cube = smgr->addCubeSceneNode();

	if (!cube)
	{
		device->drop();
		return;
	}

	if (cube)
	{
		sceneNodesList.insert(std::pair<std::string, IMeshSceneNode*>(id, cube));
		cube->setMaterialFlag(EMF_LIGHTING, lighting);
	}
}

void FracBackend::AddSphere(std::string id)
{
	IMeshSceneNode* sphere = smgr->addSphereSceneNode();

	if (!sphere)
	{
		device->drop();
		return;
	}

	if (sphere)
	{
		sceneNodesList.insert(std::pair<std::string, IMeshSceneNode*>(id, sphere));
		sphere->setMaterialFlag(EMF_LIGHTING, lighting);
	}
}

void FracBackend::AddLight(std::string id)
{
	ILightSceneNode* myLight = smgr->addLightSceneNode();

	if (myLight != nullptr) sceneNodesList.insert(std::pair<std::string, ILightSceneNode*>(id, myLight));
}

void FracBackend::AddLight(std::string id, int diffuseR, int diffuseG, int diffuseB, int diffuseA, int specularR, int specularG, int specularB, int specularA)
{
	ILightSceneNode* myLight = smgr->addLightSceneNode();

	SLight lightData;
	lightData.DiffuseColor = SColor(diffuseA, diffuseR, diffuseG, diffuseB);
	lightData.SpecularColor = SColor(specularA, specularG, specularB, specularB);

	myLight->setLightData(lightData);

	if(myLight != nullptr) sceneNodesList.insert(std::pair<std::string, ILightSceneNode*>(id, myLight));
}

void FracBackend::AddBillboardSceneNode(std::string lightId, int width, int height, const char* texturePath)
{
	ISceneNode* pointLightBill = smgr->addBillboardSceneNode(GetSceneNode(lightId), dimension2d<f32>(width, height));
	
	if (pointLightBill != nullptr)
	{
		pointLightBill->setMaterialFlag(video::EMF_LIGHTING, lighting);
		pointLightBill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		pointLightBill->setMaterialTexture(0, driver->getTexture(texturePath));
	}
}

void FracBackend::SetLighting(bool flag)
{
	lighting = flag;
}

void FracBackend::SetAmbientLight(int r, int g, int b, int a)
{
	smgr->setAmbientLight(SColor(a, r, g, b));
}

ISceneNode* FracBackend::GetSceneNode(std::string const id)
{
	std::map<std::string, ISceneNode*>::const_iterator it = sceneNodesList.find(id);

	if (it != sceneNodesList.end())
	{
		return sceneNodesList[id];
	}

	return nullptr;
}

IGUIElement* FracBackend::GetGUINode(std::string const id)
{
	std::map<std::string, IGUIElement*>::const_iterator it = GUIElementsList.find(id);

	if (it != GUIElementsList.end())
	{
		return GUIElementsList[id];
	}

	return nullptr;
}

void FracBackend::SetNodeRotation(std::string id, float x, float y, float z)
{
	GetSceneNode(id)->setRotation(vector3df(x, y, z));
}

void FracBackend::SetNodePosition(std::string id, float x, float y, float z)
{
	GetSceneNode(id)->setPosition(vector3df(x, y, z));
}

void FracBackend::SetNodeScale(std::string id, float x, float y, float z)
{
	GetSceneNode(id)->setScale(vector3df(x, y, z));
}

void ::FracBackend::SetLightDiffuseColor(std::string id, float r, float g, float b, float a)
{
	SLight lightData;
	lightData.DiffuseColor = SColor(a, r, g, b);

	ILightSceneNode* node = static_cast<ILightSceneNode*>(GetSceneNode(id));

	node->setLightData(lightData);
}

void FracBackend::SetLightType(std::string id, LightType type)
{
	SLight lightData;

	switch (type)
	{
	case LightType::POINT:
		lightData.Type = ELT_POINT;
		break;
	case LightType::SPOT:
		lightData.Type = ELT_SPOT;
		break;
	case LightType::DIRECTIONAL:
		lightData.Type = ELT_DIRECTIONAL;
		break;
	}

	ILightSceneNode* node = static_cast<ILightSceneNode*>(GetSceneNode(id));

	node->setLightData(lightData);
}

void FracBackend::SetOuterCone(std::string id, int value)
{
	SLight lightData;
	lightData.OuterCone = value;

	ILightSceneNode* node = static_cast<ILightSceneNode*>(GetSceneNode(id));

	node->setLightData(lightData);
}

void FracBackend::SetInnerCone(std::string id, int value)
{
	SLight lightData;
	lightData.InnerCone = value;

	ILightSceneNode* node = static_cast<ILightSceneNode*>(GetSceneNode(id));

	node->setLightData(lightData);
}

void FracBackend::SetLightSpecularColor(std::string id, float r, float g, float b, float a)
{
	SLight lightData;
	lightData.SpecularColor = SColor(a, r, g, b);

	ILightSceneNode* node = static_cast<ILightSceneNode*>(GetSceneNode(id));

	node->setLightData(lightData);
}

void FracBackend::SetMaterialShininess(std::string id, int materialIndex, int value)
{
	GetSceneNode(id)->getMaterial(materialIndex).Shininess = value;
}

void FracBackend::SetTexture(std::string id, char const* texturePath)
{
	GetSceneNode(id)->setMaterialTexture(0, driver->getTexture(texturePath));
}