#include "FracGUI.h"

FracGUI* FracGUI::instance = nullptr;
std::map<std::string, begui::Component*> FracGUI::guiElements;

FracGUI::FracGUI()
{
}

FracGUI::~FracGUI()
{
}

FracGUI* FracGUI::Instance()
{
	if (instance == NULL)
	{
		instance = new FracGUI();
		return instance;
	}
	return instance;
}

void FracGUI::InitGUI()
{
	// reiniciamos la lista de elementos del GUI
	guiElements.clear();

	// nos guardamos el directorio de Windows
	char win_dirc[MAX_PATH + 1];
	GetWindowsDirectory(win_dirc, MAX_PATH);
	char win_dir[MAX_PATH + 1] = "c:\\windows";

	// inicializamos los subsistemas de las fuentes y recursos
	if (!begui::FontManager::initialize()) throw std::exception("ERROR: No se pudo inicializar el subsistema de fuentes\n");
	if (!begui::FontManager::setFont(strcat(win_dir, "\\Fonts\\tahoma.ttf"), 11)) throw std::exception("ERROR: No se pudo cargar la fuente por defecto\n");

	// cargamos los recursos de la interfaz
	begui::ResourceManager::inst()->setResourceDir("./res/gui/");
	begui::ResourceManager::inst()->loadResources();
}

void FracGUI::AddModalWindow(std::string id, int x, int y, int width, int height, std::string title)
{
	begui::Dialog* dialog = new begui::Dialog();
	dialog->create(x, y, width, height, title);

	guiElements.insert(std::pair<std::string, begui::Component*>(id, dialog));
}

void FracGUI::ShowModalWindow(std::string parent, std::string modal)
{
	begui::Container* container = static_cast<begui::Container*>(GetGUIElement(parent));
	container->showModal(GetGUIElement(modal));
}

void FracGUI::CloseModalWindow(std::string parent)
{
	begui::Container* container = static_cast<begui::Container*>(GetGUIElement(parent));
	container->hideModal();
}

void FracGUI::AddComponentToModalWindow(std::string parent, std::string child)
{
	begui::Dialog* dialog = static_cast<begui::Dialog*>(GetGUIElement(parent));

	if (dialog != nullptr) dialog->addComponent(GetGUIElement(child));
}

void FracGUI::AddWindow(std::string id, int x, int y, int width, int height, std::string title)
{
	begui::Window* window = new begui::Window();
	window->create(x, y, width, height, title);

	guiElements.insert(std::pair<std::string, begui::Component*>(id, window));
}

void FracGUI::AddComponentToWindow(std::string parent, std::string child)
{
	begui::Window* window = static_cast<begui::Window*>(GetGUIElement(parent));

	if(window != nullptr) window->addComponent(GetGUIElement(child));
}

void FracGUI::AddComponentToContainer(std::string parent, std::string child)
{
	begui::Window* window = static_cast<begui::Window*>(GetGUIElement(parent));

	if (window != nullptr) window->addComponent(GetGUIElement(child));
}

void FracGUI::AddButton(std::string id, int x, int y, std::string title, void(*callback)(int))
{
	begui::Button* button = new begui::Button();
	button->create(x, y, title, -15);
	button->handleClick(makeFunctor((Functor1<int>*)0, callback));
	guiElements.insert(std::pair <std::string, begui::Component*> (id, button));
}


void FracGUI::OnLeftMouseButtonDown(std::string containerId, int x, int y)
{
	begui::Container* container = static_cast<begui::Container*>(GetGUIElement(containerId));
	begui::Window* window = static_cast<begui::Window*>(GetGUIElement("ventana1"));

	if (container != nullptr) container->onMouseDown(x, y, MOUSE_BUTTON_LEFT);
	if (window != nullptr) window->onMouseDown(x, y, MOUSE_BUTTON_LEFT);
}

void FracGUI::OnRightMouseButtonDown(std::string containerId, int x, int y)
{
	begui::Container* container = static_cast<begui::Container*>(GetGUIElement(containerId));

	if (container != nullptr) container->onMouseDown(x, y, MOUSE_BUTTON_RIGHT);
}

void FracGUI::OnLeftMouseButtonUp(std::string containerId, int x, int y)
{
	begui::Container* container = static_cast<begui::Container*>(GetGUIElement(containerId));

	if (container != nullptr) container->onMouseUp(x, y, MOUSE_BUTTON_LEFT);
}

void FracGUI::OnRightMouseButtonUp(std::string containerId, int x, int y)
{
	begui::Container* container = static_cast<begui::Container*>(GetGUIElement(containerId));

	if (container != nullptr) container->onMouseUp(x, y, MOUSE_BUTTON_RIGHT);
}

void FracGUI::OnMouseMove(std::string containerId, int x, int y, int prevx, int prevy)
{
	begui::Container* container = static_cast<begui::Container*>(GetGUIElement(containerId));

	if (container != nullptr) container->onMouseMove(x, y, prevx, prevy);
}

void FracGUI::AddList(std::string id, int x, int y, int width, int height, void(*callback)(int))
{
	begui::ListBox* list = new begui::ListBox();
	list->create(x, y, width, height, begui::ListBox::SINGLE_SELECT);
	list->handleOnItemSelect(makeFunctor((Functor1<int>*)0, callback));

	guiElements.insert(std::pair<std::string, begui::Component*>(id, list));
}

void FracGUI::AddItemToList(std::string list, std::string item)
{
	begui::ListBox* listBox = static_cast<begui::ListBox*>(GetGUIElement(list));

	if (listBox != nullptr) listBox->addItem(item);
}

void FracGUI::RenderWindow(std::string id)
{
	begui::Window* window = static_cast<begui::Window*>(GetGUIElement(id));

	if (window != nullptr) window->frameRender();
}

void FracGUI::UpdateWindow(std::string id)
{
	begui::Window* window = static_cast<begui::Window*>(GetGUIElement(id));

	if (window != nullptr) window->frameUpdate();
}

begui::Component* FracGUI::GetGUIElement(std::string id)
{
	std::map<std::string, begui::Component*>::const_iterator it = guiElements.find(id);

	if (it != guiElements.end())
	{
		return guiElements[id];
	}

	return nullptr;
}

void FracGUI::AddContainer(std::string id, int x, int y, int width, int height)
{
	begui::Container* container = new begui::Container();
	container->setPos(x, y);
	container->setSize(width, height);

	guiElements.insert(std::pair<std::string, begui::Component*>(id, container));
}
