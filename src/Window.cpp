#include "Window.h"
#include <iostream>
#include "InputManager.h"

using namespace std;

Window::Window(int width, int height, std::string const& title, bool fullscreen, bool debug) :
m_width(width),
m_height(height),
m_title(title),
m_isFullscreen(fullscreen),
m_isCloseRequested(false)
{
	//Debug Log
	if (debug)
	{
		assert(Frac3D_Debug::Frac3D_RestartLog());
		SDL_version compiled;
		SDL_version linked;
		SDL_VERSION(&compiled);
		SDL_VERSION(&linked);
		Frac3D_Debug::Frac3D_Log("SDL iniciado: \nVersion de compilado: %d.%d.%d\nVersion de enlazado: %d.%d.%d\n-----------------------------\n\n", compiled.major, compiled.minor, compiled.patch, linked.major, linked.minor, linked.patch);
	}

	//Inicializamos SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		if (debug) Frac3D_Debug::Frac3D_ErrLog("ERROR SDL: %s\n", SDL_GetError());
		else fprintf(stderr, "ERROR: SDL no se ha podido inicializar. Error SDL: %s\n", SDL_GetError());
		return;
	}

	//Activamos el double buffering
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	if ( m_window == 0)
	{
		if (debug) Frac3D_Debug::Frac3D_ErrLog("ERROR No se ha podido crear la ventana SDL. Error SDL: %s\n", SDL_GetError());
		else fprintf(stderr, "ERROR: No se ha podido crear la ventana SDL. Error SDL: %s\n", SDL_GetError());
		return;
	}
	if (fullscreen) SetFullscreen(true);

	m_glContext = SDL_GL_CreateContext(m_window);
	if (m_glContext == 0)
	{
		if (debug) Frac3D_Debug::Frac3D_ErrLog("ERROR: No se ha podido crear el contexto OpenGL. Erro SDL: %s\n", SDL_GetError());
		else fprintf(stderr, "ERROR: No se ha podido crear el contexto OpenGL. Error SDL: %s\n", SDL_GetError());
		return;
	}

	SDL_GL_SetSwapInterval(1);

	glewExperimental = true;
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		if (debug) Frac3D_Debug::Frac3D_ErrLog("ERROR SDL: %s\n", SDL_GetError());
		else fprintf(stderr, "ERROR: Glew no se ha posido iniciar %s\n", glewGetErrorString(res));
		return;
	}

	// Comprobamos funcionalidades disponibles de OpenGL
	if (GLEW_ARB_uniform_buffer_object) cout << "\tGLEW_ARB_uniform_buffer_object = Disponible\n" << endl;
	else cout << "\tGLEW_ARB_uniform_buffer_object = No disponible\n" << endl;

	if (debug)
	{
		cout << "\tUsando GLEW" << glewGetString(GLEW_VERSION) << endl;
		cout << "\tVendedor: " << glGetString(GL_VENDOR) << endl;
		cout << "\tRenderizador: " << glGetString(GL_RENDERER) << endl;
		cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
		cout << "\tGLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
		Frac3D_Debug::Fra3D_GLParams();
		Frac3D_Debug::Frac3D_Log("\n\n==================== SUBSITEMA DE CONTROL DE VENTANAS INICIALIZADO (SSCV) ====================\n\n");
	}
}

Window::~Window()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Window::Update()
{
	InputManager::Update();
}

void Window::SwapBuffers()
{
	SDL_GL_SwapWindow(m_window);
}

bool Window::IsCloseRequested()
{
	return m_isCloseRequested;
}

bool Window::IsDebugued()
{
	return m_isDebuged;
}

int Window::GetWidth() const
{
	return m_width;
}

int Window::GetHeight() const
{
	return m_height;
}

float Window::GetAspect() const
{
	return static_cast<float>(m_width) / static_cast<float>(m_height);
}

const std::string& Window::GetTitle()
{
	return m_title;
}

void Window::SetTitle(const char* title)
{
	SDL_SetWindowTitle(m_window, title);
}

void Window::SetFullscreen(bool flag)
{
	int mode = 0;
	if (flag)
	{
		mode = SDL_WINDOW_FULLSCREEN;
		m_isFullscreen = true;
	}
	else
	{
		m_isFullscreen = false;
	}

	SDL_SetWindowFullscreen(m_window, mode);
}

bool Window::GetFullscreen()
{
	return m_isFullscreen;
}

SDL_Window* Window::GetSDLWindow()
{
	return m_window;
}

void Window::OnResize(int width, int height, glm::mat4& projectionMatrix)
{
	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	projectionMatrix = glm::perspective(45.0f, static_cast<GLfloat>(width) / height, 1.f, 1000.f);
}
