#ifndef Window_H
#define Window_H

#include "SDL2/SDL.h"
#include "GL/glew.h"
#include <string>
#include <assert.h>
#include "Frac3D_Debug.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//! @class Window Clase que permite crear una ventana OpenGL
class Window
{
public:
	//! @brief Inicializa el sistema de debug, crea la ventana SDL y el contexto OpenGL
	//! @param width Anchura de la ventana
	//! @param height Altura de la ventana
	//! @param title Titulo de la ventana
	//! @param fullscreen Modo ventana. 0 = modo normal ; 1 = modo pantalla completa
	//! @param debug Activar debug de la pantalla
	Window(int width, int height, const std::string& title, bool fullscreen, bool debug);

	//! @brief Elimina el contexto OpenGL, destruye la ventana SDL y cierra todos los subsistemas SDL
	virtual ~Window();
	
	//! @brief Actualiza el loop central del juego
	void Update();

	//! @brief Cambie los buffes frontal y trase (renderiza)
	void SwapBuffers();

	//! @brief Devuelve si el la ventana del motor debe ser cerrada o no
	//! @return True si debemos cerrar la ventana o Fals en caso contrario
	bool IsCloseRequested();

	//! @brief Devuelve si la ventana de render se debe ejecutar en modo debug o no
	//! @return True si debemos ejecutar la ventana en modo debug o False en caso contrario
	bool IsDebugued();

	//! @brief Devuelve la anchura de la ventana de render
	//! @return La anchura de la ventana de render
	int GetWidth() const;

	//! @brief Devuelve la altura de la ventana de render
	//! @return La altura de la ventana de render
	int GetHeight() const;

	//! @brief Devuelve el ratio de aspecto de la ventana de render
	//! @return El ratio de aspecto (anchura/altura) de la ventana de render
	float GetAspect() const;

	//! @brief Devuelve el titulo de la ventana de render
	//! @return El titulo de la ventana de render
	const std::string& GetTitle();

	//! @brief Fija el titulo de la ventana
	//! @param Titulo de la ventana
	void SetTitle(const char* title);

	//! @brief Fija si la ventana tiene que mostrarse en modo pantalla completa o no
	//! @param value Booleano indicando si la ventana tiene que mostrarse en pantalla completa o no
	void SetFullscreen(bool flag);

	//! @brief Devuelve si la pantalla esta en modo de pantalla completa o no
	//! @return Booleano indicando si la ventana esta en modo de pantalla completa o no
	bool GetFullscreen();

	//! @brief Devuelve la ventana SDL
	//! @return Ventana SDL
	SDL_Window* GetSDLWindow();

	//! @brief Actualiza el tamano de la ventana al redimensionarla
	//! @param width Nueva anchura de la ventana
	//! @param height Nueva altura de la ventana
	void OnResize(int width, int height, glm::mat4 &projectionMatrix);

private:
	//! @brief Anchura de la ventana
	int m_width;

	//! @brief Altura de la ventana
	int m_height;

	//! @brief Titulo de la ventana
	std::string m_title;

	//! @brief Ventana SDL
	SDL_Window* m_window;

	//! @brief Contexto OpenGL
	SDL_GLContext m_glContext;

	//! @brief Controla si la ventana ha sido o no cerrada
	bool m_isCloseRequested;

	//! @brief Booleano indicando si la ventana tiene que ejecutarse en modo debugging o no
	bool m_isDebuged;

	//! @brief Booleano indicando si la ventana esta o no en modo pantalla completa
	bool m_isFullscreen;
};

#endif