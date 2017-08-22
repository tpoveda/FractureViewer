#include "bgui.h"
#include <map>

class FracGUI
{
public:
	//! @brief Crea una instancia de la clase 
	//! @return Devuelve una instancia de la clase FracGUI
	static FracGUI* Instance();

	//! @brief Inicializa los recursos de la interfaz grafica
	static void InitGUI();

	//! @brief Permite anadir un contenedor a la interfaz
	static void AddContainer(std::string id, int x, int y, int width, int height);

	//! @brief Permite anadir una ventana a la interfaz
	static void AddWindow(std::string id, int x, int y, int width, int height, std::string title);

	//! @brief Permite anadir una ventana modal
	static void AddModalWindow(std::string id, int x, int y, int width, int height, std::string title);

	//! @brief Permite mostrar una ventana modal
	static void ShowModalWindow(std::string parent, std::string modal);

	//! @brief Permite cerrar una ventana modal
	static void CloseModalWindow(std::string parent);

	//! @brief Permite anadir un componente de la interfaz dentro de de una ventana
	static void AddComponentToWindow(std::string parent, std::string child);

	//! @brief Permite anadir un componente de la interfaz dentro de una ventana modal
	static void AddComponentToModalWindow(std::string parent, std::string child);

	//! @brief Permite anadir un componente de la interfaz dentro de un contenedor
	static void AddComponentToContainer(std::string parent, std::string child);

	//! @brief Permite anadir un boton a la interfaz
	static void AddButton(std::string id, int x, int y, std::string title, void(*callback)(int));

	static void OnMouseMove(std::string containerId, int x, int y, int prevx, int prevy);
	static void OnLeftMouseButtonDown(std::string containerId, int x, int y);
	static void OnRightMouseButtonDown(std::string containerId, int x, int y);
	static void OnLeftMouseButtonUp(std::string containerId, int x, int y);
	static void OnRightMouseButtonUp(std::string containerId, int x, int y);

	//! @brief Permite anadir una lista
	static void AddList(std::string id, int x, int y, int width, int height, void(*callback)(int));

	//! @brief Permite anadir items a una lista
	static void AddItemToList(std::string list, std::string item);

	//! @Devuelve un elemento de la interfaz
	static begui::Component* GetGUIElement(std::string id);

	//! @Devuelve el id

	//! @Renderiza un contenedor
	static void RenderWindow(std::string id);

	//! @Updatea un contenedor
	static void UpdateWindow(std::string id);
private:
	FracGUI();
	~FracGUI();

	static FracGUI* instance;
	static std::map<std::string, begui::Component*> guiElements;

};