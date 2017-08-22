#ifndef FracBackend_H
#define FracBackend_H

#include <irrlicht.h>
#include <string>
#include <map>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

//! @class FracBackend Clase que actua de interfaz entre nuestro motor y el motor grafico 3D Irrlicht Engine
class FracBackend
{
public:

	//!@brief Tipos de luces que podemos crear
	enum LightType
	{
		DIRECTIONAL,
		SPOT,
		POINT
	};

	//! @brief Crea una instancia de la clase 
	//! @return Devuelve una instancia de la clase FracBackend
	static FracBackend* Instance();

	//! @brief Crea una ventana
	//! @param width Anchura de la ventana
	//! @param height Altura de la ventana
	//! @param title Titulo de la ventana
	//! @param fullscreen Modo ventana. 0 = modo normal ; 1 = modo pantalla completa
	//! @param stencilbuffer Activar o desactiva el stencilbuffer para dibujar sombras
	//! @param vsync Activa o desactiva la sincronización vertical
	static bool CreateIrrWindow(int width, int height, const char* title, bool fullscreen, bool stencilbuffer, bool vsync);
	
	//! @brief Fija el titulo de la ventana
	//! @param title Titulo de la ventana
	static void SetWindowTitle(const char* title);

	//! @brief Devuelve el device de Irrlicht
	//! @return Device de Irrlicht
	static IrrlichtDevice* GetDevice();

	//! @brief Devuelve el videoDriver de Irrlicht
	//! @return VideoDriver de Irrlicht
	static IVideoDriver* GetVideoDriver();

	//! @brief Devuelve la escena de Irrlicht
	//! @return Escena de Irrlicht
	static ISceneManager* GetScene();

	//! @brief Devuelve el GUIEnvironment de Irrlicht
	//! @return GUIEnvironment de Irrlicht
	static IGUIEnvironment* GetGUI();

	//! @brief Devuelve la camara activa actual
	//! @return Camara activa
	static ICameraSceneNode* GetActiveCamera();

	//! @brief Permite anadir al arbol de la escena un texto estatico
	//! @param text Texto que queremos meter en la escena
	//! @param x Posicion x del texto
	//! @param y Posicion y del texto
	//! @param width Anchura del texturo
	//! @param height Altura del texto
	//! @param border Permite activar o desactivar el borde del texto 
	static void AddStaticText(std::string id, const char* text, int x, int y, int width, int height, bool border, IGUIElement* parent);

	//! @brief Permite mostrar una caja de mensaje
	//! @param caption Titulo de la ventana
	//! @param Texto de la ventana
	static void AddMessageBox(const char* caption, const char* text);

	//! @brief Permite anadir una ventana a la interfaz
	static void AddWindow(std::string id, int x, int y, int width, int height, const char* caption, IGUIElement* parent);

	//! @brief Permite anadir un tabulador de conrol a la interfaz
	static void AddControlTab(std::string id, int x, int y, int width, int height, IGUIElement* parent, bool fillBackground, bool border);

	//! @brief Permite anadir un tabulador a la interfaz
	static void AddTab(std::string id, IGUITabControl* parentTab, const char* caption);

	//! @brief Permite anadir un boton de edicion
	static void AddEditBox(std::string id, const char* caption, int x, int y, int width, int height, bool border, IGUIElement* parent);

	//! @brief Permite anadir un boton
	static void AddButton(std::string id, const char* label, int x, int y, int width, int height, IGUIElement* parent);

	//! @biref Permite anadir una barra de desplazamiento (scrollbar)
	static void AddScrollbar(std::string id, bool horizontal, int x, int y, int width, int height, int max, int min, int pos, int step, IGUIElement* parent);

	//! @brief Permite anadir al arbol de la escena una malla 3D animada
	//! @param meshPath Ruta de la malla
	//! @param texturePath Ruta de la textura de la malla
	static void AddAnimatedMesh(std::string id, const char* meshPath, const char* texturePath);

	//! @brief Permite anadir al arbol de la escena una camara
	//! @param x Posicion x de la camara
	//! @param x Posicion y de la camara
	//! @param x Posicion z de la camara
	//! @param x Posicion en el eje X al punto al que miramos
	//! @param y Posicion en el eje Y al punto al que miramos
	//! @param z Posicion en el eje Z al punto al que miramos
	static void AddCamera(std::string, int posX, int posY, int posZ, int lookAtX, int lookAtY, int lookAtZ);

	//! @brief Permite anadir al arbol de la escena una camara FPS
	//! @param x Posicion x de la camara
	//! @param x Posicion y de la camara
	//! @param x Posicion z de la camara
	//! @param x Posicion en el eje X al punto al que miramos
	//! @param y Posicion en el eje Y al punto al que miramos
	//! @param z Posicion en el eje Z al punto al que miramos
	//! @param rotateVelocity Velocidad de rotacion de la camara
	//! @param moveSpeed Velocidad de movimiento de la camara
	static void AddFPSCamera(std::string id, int posX, int posY, int posZ, int lookAtX, int lookAtY, int lookAtZ, float rotateVelocity, float moveSpeed);

	//! @brief Permite seleccionar la camara que queremos que este activa
	//! @param id Id de la camara a activar
	static void SetActiveCamera(std::string id);

	//! @brief Permite anadir al arbol de la escena un cubo 3D
	//! @param id Id del objeto
	//! @param texturePath Ruta de la textura del cubo
	static void AddCube(std::string id);

	//! @brief Permite anadir al arbol de la escena una esfera 3D
	//! @param id Id del objeto
	//! @param texturePath Ruta de la textura de la esfera
	static void AddSphere(std::string id);

	//!@ brief Permite anadir al arbol de la escena una luz
	//! @param id Id del objeto
	//! @param diffuseR Color difuso del canal rojo
	//! @param diffuseG Color difuso del canal verde
	//! @param diffuseB Color difuso del canal azul
	//! @param diffuseA Color difuso del canal alfa
	//! @param specularR Color especular del canal rojo
	//! @param specularG Color especular del canal verde
	//! @param specularB Color especular del canal azul
	//! @param specularA Color especular del canal alfa
	static void AddLight(std::string id, int diffuseR, int diffuseG, int diffuseB, int diffuseA, int specularR, int specularG, int specularB, int specularA);

	//!@ brief Permite anadir el arbol de la escena una luz
	//! @param id Id del objeto
	static void AddLight(std::string id);

	//! @brief Permite anadir al arbol de la escena un billboard
	//! @param lightID Id de la luz a la que queremo asociar el billboard
	//! @param width Anchura del billboard
	//! @param height Altura del billboard
	//! @param texturePath Ruta de la textura asociada al billboard
	static void AddBillboardSceneNode(std::string lightId, int width, int height, const char* texturePath);

	//! @brief Activa o desactiva la iluminacion de la escena
	//!
	//! Si activas la iluminacion, los objetos dibujados a partir de ahora se iluminaran
	//! y si son ya estaban no se iluminaran
	//! @param flag Activa o desactiva la iluminacion de la escena
	static void SetLighting(bool flag);

	//! @brief Permite ajustar el color de la luz ambiente de la escena
	//! @brief r Color del canal rojo
	//! @brief g Color del canal verde
	//! @brief b Color del canal azul
	//! @brief a Color del canal alfa (transparencia)
	static void SetAmbientLight(int r, int g, int b, int a);

	//! @brief Devuelve el nodo de la escena con el id pasado como parametro
	//! @param id Id del nodo de la escena a devolver
	//! @return El nodo de la escena o nullptr (puntero nulo)
	static ISceneNode* GetSceneNode(const std::string id);

	//!@ brief Devuelve el nodo de la interfaz con el id pasado como parametro
	//! @param id Id del nodo de la interfaz a devolver
	//! @return El nodo de la interfaz o nullptr (puntero nulo)
	static IGUIElement* GetGUINode(const std::string id);

	//! @brief Permite rotar un nodo de la escena
	//! @param x Rotacion en el eje X
	//! @param y Rotacion en el eje Y
	//! @param z Rotacion en el eje Z
	static void SetNodeRotation(std::string id, float x, float y, float z);

	//! @brief Permite posicionar un nodo de la escena
	//! @param x Posicion en el eje X
	//! @param y Posicion en el eje Y
	//! @param z Posicion en el eje Z
	static void SetNodePosition(std::string id, float x, float y, float z);

	//! @brief Permite posicionar un nodo de la escena
	//! @param x Posicion en el eje X
	//! @param y Posicion en el eje Y
	//! @param z Posicion en el eje Z
	static void SetNodeScale(std::string id, float x, float y, float z);

	//! @brief Permite fijar el color del canal difuso de una luz
	//! @param id Id de la luz a modificar
	//! @param r Color del canal rojo
	//! @param g Color del canal verde
	//! @param b Color del canal azul
	//! @param a Color del canal alfa
	static void SetLightDiffuseColor(std::string id, float r, float g, float b, float a);

	//! @brief Permite fijar el color del canal especular de una luz
	//! @param id Id de la luz a modificar
	//! @param r Color del canal rojo
	//! @param g Color del canal verde
	//! @param b Color del canal azul
	//! @param a Color del canal alfa
	static void SetLightSpecularColor(std::string id, float r, float g, float b, float a);

	//! @brief Permite fijar el tipo de luz
	//! @param id Id de la luz a modificar
	//! @param type Tipo de la luz
	static void SetLightType(std::string id, LightType type);

	//! @brief Permite fijar el cono externo de la luz
	//! @param id Id de la luz a modificar
	//! @param value Valor del cono externo de la luz
	static void SetOuterCone(std::string id, int value);

	//! @brief Permite fijar el cono interno de la luz
	//! @param id Id de la luz a modificar
	//! @param value Valor del cono interno de la luz
	static void SetInnerCone(std::string id, int value);

	//! @brief Fija el brillo del material de un objeto
	//! @param materialIndex Indice del material del objeto
	//! @param value Valor del brillo del material
	static void SetMaterialShininess(std::string id, int materialIndex, int value);

	//! @brief Fija la textura de un objeto
	//! @param materialIndex Indice del material del objeto
	//! @param value Valor del brillo del material
	static void SetTexture(std::string id, const char* texturePath);

private:
	FracBackend();
	~FracBackend();
	static FracBackend* irrInstance;
	static IrrlichtDevice* device;
	static IVideoDriver* driver;
	static ISceneManager* smgr;
	static IGUIEnvironment* guienv;

	static std::map<std::string, ISceneNode*> sceneNodesList;
	static std::map<std::string, IGUIElement*> GUIElementsList;

	// Parametros globales de la escena
	static bool lighting;
	static ISceneNode* model;
	static ISceneNode* skybox;
	static bool octree;
	static ICameraSceneNode* camera[2];

};

#endif