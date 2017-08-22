#include "FractureTexture.h"
#include <map>
#include <string>

using namespace std;
using namespace Fracture;

namespace Fracture
{
	class FractureResourceManager
	{
	public:
		static FractureResourceManager* GetInstance();
		~FractureResourceManager();
		FractureTexture* GetTexture(int id, string path);
		void DropTexture(int id);
		void DeleteTextureMap();

		protected:
			FractureResourceManager();
			FractureResourceManager(const FractureResourceManager &g);
			FractureResourceManager &operator= (const FractureResourceManager &);

		private:
			map <int, FractureTexture*> m_textureMap;
			map<int, FractureTexture*>::iterator it;
			static FractureResourceManager* m_pInstance;
	};
}