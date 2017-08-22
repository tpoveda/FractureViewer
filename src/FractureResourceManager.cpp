#include "FractureResourceManager.h"

FractureResourceManager* FractureResourceManager::m_pInstance = 0;

FractureResourceManager::FractureResourceManager()
{
	
}

FractureResourceManager::~FractureResourceManager()
{
	DeleteTextureMap();

	m_pInstance = 0;
}

FractureTexture* FractureResourceManager::GetTexture(int id, string path)
{
	FractureTexture *texture;
	it = m_textureMap.find(id);

	if (it == m_textureMap.end())
	{
		texture = new FractureTexture();

		if (!texture->LoadTextureFromFile(path))
		{
			cout << "Error al cargar la textura" << endl;
		}

		m_textureMap[id] = texture;
	
	}

	else
	{
	
		texture = it->second;
	}

	texture->Grab();

	return texture;
}

void FractureResourceManager::DropTexture(int id)
{
	it = m_textureMap.find(id);

	if (it != m_textureMap.end())
	{
		if(m_textureMap[id]->Drop()) m_textureMap.erase(id);
	}
	
}

FractureResourceManager* FractureResourceManager::GetInstance()
{
	if (m_pInstance == 0)
		m_pInstance = new FractureResourceManager();

	return m_pInstance;
}

void FractureResourceManager::DeleteTextureMap()
{
	for (it = m_textureMap.begin(); it != m_textureMap.end(); ++it)
	{
		FractureTexture* Value = (*it).second;
		delete Value;
	}

	m_textureMap.clear();
}