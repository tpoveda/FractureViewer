#ifndef Time_H
#define Time_H

#include <time.h>
#include <chrono>
#include <thread>

//! @class Time Clase que permite conocer el tiempo de juego
class Time
{
public:
	//! @brief Devuelve el tiempo de juego
	//! @return El tiempo de juego
	static double GetTime();

	static double GetDelta();
	static void SetDelta(double value);

private:
	static double m_delta;
};

#endif 