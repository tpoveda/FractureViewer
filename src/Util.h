#ifndef Util_H
#define Util_H

#ifndef WIN32
#include <unistd.h>
#endif

#ifdef WIN32
#define SNPRINTF _snprintf_s
#else
#define SNPRINTF snprintf
#endif

#define ZERO_MEM(a) memset(a,0,sizeof(a))
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
//#define SAFE_DELETE(p) if(p) {delete p; p = NULL;}
#define INVALID_VALUE 0xFFFFFFFF

#define ToRadian(x) (float)(((x) * 3.1415926536f / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / 3.1415926536f))

//! @class Util Clase que recoge algunas definiciones utiles de nuestro motor asi como algunos metodos utiles
namespace Util
{
	//! @brief Permite mantener la ejecucion del motor en un segundo plano
	//! @brief milliseconds Cantidad de tiempo (en milisegundos) en el que el motor se mantendra en segundo plano (1000 ms = 1 s)
	void Sleep(int milliseconds);
};

#endif