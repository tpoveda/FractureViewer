#ifndef FractureObject_H
#define FractureObject_H

#include <vector>
class FractureComponent;

class FractureObject
{
public:
	FractureObject();
	virtual ~FractureObject();

	FractureObject* AddChild(FractureObject* child);
	FractureObject* AddComponent(FractureComponent* component);

private:
	void Input();
	void Update();
	void Render();

	std::vector<FractureObject*> m_children;
	std::vector<FractureComponent*> m_components;
};

#endif