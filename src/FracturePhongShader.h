#ifndef FracturePhongShader_H
#define FracturePhongShader_H

#include "FractureShader.h"
#include "FractureLighting.h"
#include "FractureMaterial.h"

class FracturePhongShader : public FractureShader
{
public:
	static FracturePhongShader* GetInstance()
	{
		if (m_pInstance == 0) m_pInstance = new FracturePhongShader();
		
		return m_pInstance;
	}

	virtual void UpdateUniforms(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, FractureMaterial& material, const glm::mat4& modelMatrix = glm::mat4(1)) override;

	static glm::dvec3 GetAmbientLight();
	static DirectionalLight& GetDirectionalLight();

	static bool GetAmbientLightActive();
	static bool GetDirectionalLightActive();

	static void SetAmbientLight(const glm::dvec3& ambientLight);
	static void SetDirectionalLight(const DirectionalLight& directionalLight);
	static void SetPointLights(PointLight* pointLights, int arraySize);
	static void SetSpotLights(SpotLight* spotLights, int arraySize);
	
	static void SetAmbientLightActive(bool flag);
	static void SetDirectionalLightActive(bool flag);

	static void SetNormalActive(bool flag);
	static void SetSkyLightReflections(bool flag);

private:
	static const int MAX_POINT_LIGHTS = 4;
	static const int MAX_SPOT_LIGHTS = 4;
	static glm::dvec3 m_ambientLight;
	static DirectionalLight m_directionalLight;
	static PointLight* m_pointLights;
	static SpotLight* m_spotLights;
	static int m_numPointLights;
	static int m_numSpotLights;
	static FracturePhongShader* m_pInstance;
	static bool m_bAmbientLightActive;
	static bool m_bDirectionalLightActive;
	static bool m_bPointLightActive;
	static bool m_bSpotLightActive;
	static bool m_bNormalMapActive;
	static bool m_bSkyLightReflections;


	FracturePhongShader();
	~FracturePhongShader();
};


#endif