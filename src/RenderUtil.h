#ifndef RenderUtil_H
#define RenderUtil_H

//! @class RenderUtil Clase que recoge algunas funciones utiles para la inicializacion del sistema de render
class RenderUtil
{
public:
	//! @brief Configura los buffers a usar por la ventana de render
	static void ClearScreen();

	//! @brief Inicializa la ventana grafica
	static void InitGraphics();
};

#endif
