#include "SDL2/SDL.h"
#include "InputManager.h"
#include "Frac3D_Debug.h"
#include "Window.h"

const static int NUM_KEYS = 512;
const static int NUM_MOUSEBUTTONS = 256;

static SDL_Event e;

static int mouseX = 0;
static int mouseY = 0;

static bool InputManagers[NUM_KEYS];
static bool downKeys[NUM_KEYS];
static bool upKeys[NUM_KEYS];

static bool mouseInputManager[NUM_MOUSEBUTTONS];
static bool downMouse[NUM_MOUSEBUTTONS];
static bool upMouse[NUM_MOUSEBUTTONS];

void ClearMouseButtons();
void ClearKeys();

static bool inputDebuged = false;

InputManager::InputManager()
{
}

InputManager::~InputManager()
{

}

void InputManager::Update()
{
	ClearMouseButtons();
	ClearKeys();

	while (SDL_PollEvent(&e))
	{
		if (inputDebuged)
		{
			if (e.type == SDL_QUIT)
			{
				Frac3D_Debug::Frac3D_Log("SSCV: Has cerrado la ventana");
				Frac3D_Debug::Frac3D_Log("\n\n==================== SUBSITEMA DE CONTROL DE VENTANAS FINALIZADO (SSCV) ====================\n\n\\n\n");
				Frac3D_Debug::Frac3D_Log("\n\n========================= ¡¡¡PROGRAMA FINALIZADO CORRECTAMENTE!!! ===========================\n\n");
			}
			if (e.type == SDL_MOUSEMOTION)
			{
				mouseX = e.motion.x;
				mouseY = e.motion.y;
			}

			if (e.type == SDL_KEYDOWN)
			{
				int value = e.key.keysym.scancode;

				InputManagers[value] = true;
				downKeys[value] = true;
				Frac3D_Debug::Frac3D_Log("SSCV: Has presionado la tecla %i\n", value);
			}
			if (e.type == SDL_KEYUP)
			{
				int value = e.key.keysym.scancode;

				InputManagers[value] = false;
				upKeys[value] = true;
				Frac3D_Debug::Frac3D_Log("SSCV: Has soltado la tecla %i\n", value);
			}
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				int value = e.button.button;

				mouseInputManager[value] = true;
				downMouse[value] = true;
				Frac3D_Debug::Frac3D_Log("SSCV: Has presionado el boton del raton %i\n", value);
			}
			if (e.type == SDL_MOUSEBUTTONUP)
			{
				int value = e.button.button;

				mouseInputManager[value] = false;
				upMouse[value] = true;
				Frac3D_Debug::Frac3D_Log("SSCV: Has soltado el boton del raton %i\n", value);
			}
		}
		else
		{
			if (e.type == SDL_QUIT)
			{
			}
			if (e.type == SDL_MOUSEMOTION)
			{
				mouseX = e.motion.x;
				mouseY = e.motion.y;
			}

			if (e.type == SDL_KEYDOWN)
			{
				int value = e.key.keysym.scancode;

				InputManagers[value] = true;
				downKeys[value] = true;
			}
			if (e.type == SDL_KEYUP)
			{
				int value = e.key.keysym.scancode;

				InputManagers[value] = false;
				upKeys[value] = true;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				int value = e.button.button;

				mouseInputManager[value] = true;
				downMouse[value] = true;
			}
			if (e.type == SDL_MOUSEBUTTONUP)
			{
				int value = e.button.button;

				mouseInputManager[value] = false;
				upMouse[value] = true;
			}
		}
		if (e.type == SDL_WINDOWEVENT)
		{
			if (inputDebuged)
			{
				switch (e.window.event)
				{
				case SDL_WINDOWEVENT_SHOWN:
					Frac3D_Debug::Frac3D_Log("SSCV: Ventana %d mostrada\n", e.window.windowID);
					break;
				case SDL_WINDOWEVENT_HIDDEN:
					Frac3D_Debug::Frac3D_Log("SSCV: Ventana %d escondida\n", e.window.windowID);
					break;
				case SDL_WINDOWEVENT_EXPOSED:
					Frac3D_Debug::Frac3D_Log("SSCV: Ventana %d expuesta\n", e.window.windowID);
					break;
				case SDL_WINDOWEVENT_MOVED:
					Frac3D_Debug::Frac3D_Log("SSCV: Ventana %d movida a %d,%d\n", e.window.windowID, e.window.data1, e.window.data2);
					break;
				case SDL_WINDOWEVENT_RESIZED:
					Frac3D_Debug::Frac3D_Log("SSCV: Ventana %d redimensionada a %dx%d\n", e.window.windowID, e.window.data1, e.window.data2);
					break;
				case SDL_WINDOWEVENT_MINIMIZED:
					Frac3D_Debug::Frac3D_Log("SSCV: Ventana %d minimizada\n", e.window.windowID);
					break;
				case SDL_WINDOWEVENT_MAXIMIZED:
					Frac3D_Debug::Frac3D_Log("SSCV: Ventana %d maximizada\n", e.window.windowID);
					break;
				case SDL_WINDOWEVENT_RESTORED:
					Frac3D_Debug::Frac3D_Log("SSCV: Ventana %d restaurada\n", e.window.windowID);
					break;
				case SDL_WINDOWEVENT_ENTER:
					SDL_Log("SSCV: Raton entrando en ventana %d\n", e.window.windowID);
					break;
				case SDL_WINDOWEVENT_LEAVE:
					Frac3D_Debug::Frac3D_Log("SSCV: Raton saliendo de ventana %d\n", e.window.windowID);
					break;
				case SDL_WINDOWEVENT_FOCUS_GAINED:
					Frac3D_Debug::Frac3D_Log("SSCV: Ventana %d ha recibido el foco del teclado\n", e.window.windowID);
					break;
				case SDL_WINDOWEVENT_FOCUS_LOST:
					Frac3D_Debug::Frac3D_Log("SSCV: Ventana %d ha perdido el foco del teclado\n", e.window.windowID);
					break;
				case SDL_WINDOWEVENT_CLOSE:
					Frac3D_Debug::Frac3D_Log("SSCV: Ventana %d cerrada\n", e.window.windowID);
					break;
				default:
					Frac3D_Debug::Frac3D_Log("SSCV: Ventana %d ha recibido un evento desconocido %d\n", e.window.windowID, e.window.event);
					break;
				}
			}
		}
		if (inputDebuged)
		{
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					Frac3D_Debug::Frac3D_Log("SSCV: Has cerrado la ventana");
					Frac3D_Debug::Frac3D_Log("\n\n==================== SUBSITEMA DE CONTROL DE VENTANAS FINALIZADO (SSCV) ====================\n");
					Frac3D_Debug::Frac3D_Log("\n========================= ¡¡¡PROGRAMA FINALIZADO CORRECTAMENTE!!! ===========================\n");
					break;

				}
			}
		}
		else
		{
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					break;
				}
			}
		}
	}
}

bool InputManager::GetKey(int keyCode)
{
	return InputManagers[keyCode];
}

bool InputManager::GetKeyDown(int keyCode)
{
	return downKeys[keyCode];
}

bool InputManager::GetKeyUp(int keyCode)
{
	return upKeys[keyCode];
}

bool InputManager::GetMouse(int button)
{
	return mouseInputManager[button];
}

bool InputManager::GetMouseDown(int button)
{
	return downMouse[button];
}

bool InputManager::GetMouseUp(int button)
{
	return upMouse[button];
}

void InputManager::SetCursor(bool visible)
{
	if (visible) SDL_ShowCursor(1);
	else SDL_ShowCursor(0);
}

glm::vec2 InputManager::GetMousePosition()
{
	return glm::vec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
}

void InputManager::SetMousePosition(Window& window, glm::vec2 pos)
{
	SDL_WarpMouseInWindow(window.GetSDLWindow(), pos.x, pos.y);
}

void InputManager::SetDebug(bool debug)
{
	inputDebuged = debug;
}

SDL_Event InputManager::GetEvent()
{
	return e;
}

void ClearMouseButtons()
{
	for (int i = 0; i < NUM_MOUSEBUTTONS; i++)
	{
		downMouse[i] = false;
		upMouse[i] = false;
	}
}

void ClearKeys()
{
	for (int i = 0; i < NUM_KEYS; i++)
	{
		downKeys[i] = false;
		upKeys[i] = false;
	}
}