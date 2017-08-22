/***************************************************************
TODOLIST - v1.1 - 06/11/2014
- Dividir en funciones
- Cambiar funciones SDL por funciones Irrlicht (fachada)
- Añadir InputManager
- Añadir Juego
****************************************************************/
#include "FractureViewer.h"
#include "FracturePhysics.h"

const static int WIDTH = 1024;					// anchura de la ventana
const static int HEIGHT = 768;					// altura de la ventana
const static char* TITLE = "Fracture Viewer";	// titulo de la ventana
const static double FRAMERATE = 60.0f;		// frames (updates) por segundo

int main(int argc, char* args[])
{
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	printf("-------------------------------------\n");
	printf("----- Fracture Debugging System -----\n");
	printf("-------------------------------------\n");


	Window window(WIDTH, HEIGHT, "Fracture Viewer", false, true);
	FracturePhysics physicsEngine;
	FractureViewer viewer(FRAMERATE, &window, &physicsEngine);
	viewer.Start();

	return 0;
}