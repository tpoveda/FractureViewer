#ifndef IFractureSceneNode_H
#define IFractureSceneNode_H

#include <string>
#include <list>
#include <glm.hpp>
#include "IFracReferenceCounter.h"

namespace Fracture
{
	//!Enumeracion para todos los tipos de nodos de la escena
	enum NODE_TYPE
	{
		NODE_MESH = 0,
		NODE_UNKNOW = 1
	};

	//! Interfaz de los nodos de la escena
	/**
	Un nodo de la escena es un nodo del grafo de la escena. Cada nodo de la escena
	puede tener hijos, los cuales son tambien nodos de la escena. Los hijos se pueden
	relativamente a la posicion de sus padres. Si un nodo padre no es visible, sus nodos
	hijos tampoco lo son.
	**/
	class IFractureSceneNode : virtual public IFractureReferenceCounter
	{
	public:
		//! Constructor de la clase IFractureSceneNode
		IFractureSceneNode( IFractureSceneNode* parent, int id = 0, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 rotation = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f))
		{
			if (parent) parent->AddChild(this);

			//debemos actualizar la posicion absoluta del nodo
		}

		//! Destructor de la clase IFractureSceneNode
		virtual ~IFractureSceneNode()
		{
			// eliminamos todos los hijos del nodo
			RemoveAllChilds();
		}

		//! Renderiza el nodo
		virtual void Render() = 0;

		//! Devuelve el nombre del nodo
		//! @return Nombre del nodo
		virtual const char* GetName() const
		{
			return m_name.c_str();
		}

		//! Fija el nombre del nodo
		//! @param name Nombre del nodo de la escena
		virtual void SetName(const char* name)
		{
			m_name = name;
		}

		//! Fija el nombre del nodo
		//! @param name Nombre del nodo de la escena
		virtual void SetName(std::string name)
		{
			m_name = name;
		}

		//! Devuelve si un nodo deberia ser visible
		//! @return La visibilidad del nodo
		/** Un nodo sera visible si todos sus padres son visibles **/
		virtual bool IsVisible() const
		{
			if (!m_isVisible) return false;

			if (!m_parent) return false;

			return m_parent->IsVisible();
		}

		//! Fija si un nodo deberia ser visible o no
		//! @param isVisible Si el nodo deberia ser visible
		/** Todos los hijos de este nodo tampoco seran visibles **/
		virtual bool SetVisible(bool isVisible)
		{
			m_isVisible = isVisible;
		}

		//! Devuelve el ID del nodo de la escena
		virtual unsigned int GetID() const
		{
			return m_ID;
		}

		//! Fija el ID del nodo de la escena
		//! @param ID El ID del nodo
		virtual void SetID(unsigned int id)
		{
			m_ID = id;
		}

		//! Anade un hijo al nodo de la escena
		//! @param Puntero al nuevo hijo
		/** Si el nodo a anadir ya tiene un padre primero lo quitamos de ese padre **/
		virtual void AddChild(IFractureSceneNode* child)
		{
			if (child && (child != this))
			{
				child->Grab();
				child->Remove();
				m_children.push_back(child);
				child->m_parent = this;
			}
		}

		//! Elimina un hijo del nodo de la escena
		//! @param child Puntero al hijo que deberia ser eliminado
		//! @return True si el hijo ha sido eliminado, y falso si no ha sido eliminado (porque no se encontro en la lista de nodos hijos)
		virtual bool RemoveChild(IFractureSceneNode* child)
		{
			std::list<IFractureSceneNode*>::iterator it = m_children.begin();
			for (; it != m_children.end(); ++it)
			{
				if ((*it) == child)
				{
					(*it)->m_parent = 0;
					(*it)->Drop();
					m_children.erase(it);
					return true;
				}
			}
			return false;
		}

		//! Elimina todos los hijos de este nodo de la escena
		virtual void RemoveAllChilds()
		{
			std::list<IFractureSceneNode*>::iterator it = m_children.begin();
			for (; it != m_children.end(); ++it)
			{
				(*it)->m_parent = 0;
				(*it)->Drop();
			}
			m_children.clear();
		}

		//! Elimina este nodo, de la escena
		virtual void Remove()
		{
			if (m_parent) m_parent->RemoveChild(this);
		}

		//! Cambia el padre del nodo de la escena
		//! @param newParent Nuevo padre del nodo de la escena
		virtual void SetParent(IFractureSceneNode* newParent)
		{
			Grab();
			Remove();

			m_parent = newParent;

			if (m_parent) m_parent->AddChild(this);

			Drop();
		}

		//! Devuelve el padre de este nodo de la escena
		//! @return Un puntero al padre
		IFractureSceneNode* GetParent() const
		{
			return m_parent;
		}

		//! Devuelve el tipo de este nodo de la escena
		//! @return El tipo de este nodo
		virtual NODE_TYPE GetType() const
		{
			return NODE_UNKNOW;
		}

	protected:
		//! Nombre del nodo de la escena
		std::string m_name;

		//! Transformacion absoluta del nodo
		glm::mat4 m_absoluteTransformation;

		//! Traslacion relativa del nodo de la escena
		glm::vec3 m_relativeTranslation;

		//! Rotacion relativa del nodo de la escena
		glm::vec3 m_relativeRotation;

		//! Escalado relativo del nodo de la escena
		glm::vec3 m_relativeScale;

		//! Puntero al nodo padre
		IFractureSceneNode* m_parent;

		//! Lista de todos los hijos de este nodo
		std::list<IFractureSceneNode*> m_children;

		//! Puntero al gestor de la escena
		//IFractureSceneManager* m_sceneManager;

		//! ID del nodo
		unsigned int m_ID;

		//! Flag que indica si se debe renderizar la informacion relativa al debug del objeto
		bool m_isDebugVisible;

		//! Flag que indica si el nodo es visible
		bool m_isVisible;
	};
}


#endif