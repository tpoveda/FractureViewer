#ifndef IFractureReferenceCounter_H
#define IFractureReferenceCounter_H

#include <assert.h>

namespace Fracture
{
	//! Clase base para casi todos los objetos del motor Fracture Engine
	/** 
	Esta clase proporciona un contador de referencias a traves de los metodos Grab() y Drop()
	La mayoria de objetos del motor se derivan de esta clase.

	Cuando creamos un objeto usando un metodo que comienza con create, el objeto se creara y la funcion
	nos devolvera un puntero a ese objeto. Si no necesitamos mas el objeto, deberemos llamar al metodo 
	Drop(). Esto destruira el objeto a no ser que el objeto este usando el objeto y sus referencias
	no llegen a 0.
	**/

	class IFractureReferenceCounter
	{
	public:
		//! Constructor de la clase IFractureReferenceCounter
		IFractureReferenceCounter() : m_referenceCounter(0)
		{
		}

		//! Destructor de la clase IFractureReferenceCounter
		virtual ~IFractureReferenceCounter()
		{
		}

		//! Aumenta el contador de referencias del objeto en uno
		void Grab() const
		{
			++m_referenceCounter;
		}

		//! Disminuye el contador de referencias del objeto en uno
		bool Drop() const
		{
			//assert(m_referenceCounter <= 0);

			--m_referenceCounter;
			if (m_referenceCounter == 0)
			{
				delete this;
				return true;
			}
			
			return false;
		}


	private:
		//! El contador de referencias. Es mutable para permitir la el contador de referncias en objetos constantes (const)
		mutable int m_referenceCounter;
	};
}

#endif