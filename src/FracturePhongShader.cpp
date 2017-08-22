#include "FracturePhongShader.h"

FracturePhongShader* FracturePhongShader::m_pInstance = nullptr;
glm::dvec3 FracturePhongShader::m_ambientLight = glm::dvec3(0.5f, 0.5f, 0.5f);
DirectionalLight FracturePhongShader::m_directionalLight = DirectionalLight(BaseLight(glm::dvec3(0, 0, 0), glm::dvec3(0, 0, 0), 0), glm::dvec3(0, 0, 0));
PointLight* FracturePhongShader::m_pointLights = nullptr;
SpotLight* FracturePhongShader::m_spotLights = nullptr;
int FracturePhongShader::m_numPointLights = 0;
int FracturePhongShader::m_numSpotLights = 0;
bool FracturePhongShader::m_bAmbientLightActive = true;
bool FracturePhongShader::m_bDirectionalLightActive = true;
bool FracturePhongShader::m_bPointLightActive = true;
bool FracturePhongShader::m_bSpotLightActive = true;
bool FracturePhongShader::m_bNormalMapActive = false;
bool FracturePhongShader::m_bSkyLightReflections = false;

FracturePhongShader::FracturePhongShader()
{
	// añadimos los shaders
	LoadFromFile(GL_VERTEX_SHADER, "./res/shaders/ReflectShaderV2Vert.glsl");
	LoadFromFile(GL_FRAGMENT_SHADER, "./res/shaders/ReflectShaderV2Frag.glsl");
	CreateAndLinkProgram();

	AddUniform("model_mat");
	AddUniform("view_mat");
	AddUniform("projection_mat");
	AddUniform("material.diffuseColor");
	AddUniform("material.emissionContribution");
	AddUniform("material.ambientContribution");
	AddUniform("material.diffuseContribution");
	AddUniform("material.specularContribution");
	AddUniform("ambientLightActive");
	AddUniform("directionalLightActive");
	AddUniform("normalMapActive");
	AddUniform("ambientLight");
	AddUniform("material.specularIntensity");
	AddUniform("material.specularPower");
	AddUniform("directionalLight.base.color");
	AddUniform("directionalLight.base.colorSpec");
	AddUniform("directionalLight.base.intensity");
	AddUniform("directionalLight.direction");
	AddUniform("material.textureTiling");

	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		std::string pointLightName = "pointLights[" + std::to_string(i) + "]";
		AddUniform(pointLightName + ".base.color");
		AddUniform(pointLightName + ".base.intensity");
		AddUniform(pointLightName + ".atten.constant");
		AddUniform(pointLightName + ".atten.linear");
		AddUniform(pointLightName + ".atten.exponent");
		AddUniform(pointLightName + ".position");
		AddUniform(pointLightName + ".range");
	}

	AddUniform("skyReflections");
	AddUniform("skyContribution");
	AddUniform("skyIntensity");
}

FracturePhongShader::~FracturePhongShader()
{
}

void FracturePhongShader::UpdateUniforms(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, FractureMaterial& material, const glm::mat4& modelMatrix)
{
	SetUniform("model_mat", modelMatrix);
	SetUniform("view_mat", viewMatrix);
	SetUniform("projection_mat", projectionMatrix);
	SetUniform("material.diffuseColor", material.diffuseColor);
	SetUniform("material.emissionContribution", material.emissionContribution);
	SetUniform("material.ambientContribution", material.ambientContribution);
	SetUniform("material.diffuseContribution", material.diffuseContribution);
	SetUniform("material.specularContribution", material.specularContribution);
	SetUniform("ambientLightActive", m_bAmbientLightActive);
	SetUniform("normalMapActive", m_bNormalMapActive);
	SetUniform("ambientLight", m_ambientLight);
	SetUniform("material.specularIntensity", material.specularIntensity);
	SetUniform("material.specularPower", material.specularPower);
	SetUniform("directionalLightActive", m_bDirectionalLightActive);
	SetUniform("directionalLight.base.color", m_directionalLight.base.color);
	SetUniform("directionalLight.base.colorSpec", m_directionalLight.base.colorSpec);
	SetUniform("directionalLight.base.intensity", m_directionalLight.base.intensity);
	SetUniform("directionalLight.direction", m_directionalLight.direction);
	SetUniform("material.textureTiling", material.textureTiling);

	for (int i = 0; i < m_numPointLights; i++)
	{
		std::string pointLightName = "pointLights[" + std::to_string(i) + "]";
		SetUniform(pointLightName + ".base.color", m_pointLights[i].base.color);
		SetUniform(pointLightName + ".base.intensity", m_pointLights[i].base.intensity);
		SetUniform(pointLightName + ".atten.constant", m_pointLights[i].atten.constant);
		SetUniform(pointLightName + ".atten.linear", m_pointLights[i].atten.linear);
		SetUniform(pointLightName + ".atten.exponent", m_pointLights[i].atten.exponent);
		SetUniform(pointLightName + ".position", m_pointLights[i].position);
		SetUniform(pointLightName + ".range", m_pointLights[i].range);
	}

	SetUniform("skyReflections", m_bSkyLightReflections);
	SetUniform("skyContribution", material.reflectionContribution);

	if (material.diffuseTexture != nullptr) material.diffuseTexture->Use(GL_TEXTURE1);
	if (material.specularTexture != nullptr) material.specularTexture->Use(GL_TEXTURE2);
	if (material.ambientTexture != nullptr) material.ambientTexture->Use(GL_TEXTURE3);
	if (material.emissiveTexture != nullptr)material.emissiveTexture->Use(GL_TEXTURE4);
	if (material.normalTexture != nullptr)material.normalTexture->Use(GL_TEXTURE5);
	if (material.alphaTexture != nullptr) material.alphaTexture->Use(GL_TEXTURE6);
}

glm::dvec3 FracturePhongShader::GetAmbientLight()
{
	return m_ambientLight;
}

bool FracturePhongShader::GetAmbientLightActive()
{
	return m_bAmbientLightActive;
}

bool FracturePhongShader::GetDirectionalLightActive()
{
	return m_bDirectionalLightActive;
}

void FracturePhongShader::SetAmbientLightActive(bool flag)
{
	m_bAmbientLightActive = flag;
}

void FracturePhongShader::SetDirectionalLightActive(bool flag)
{
	m_bDirectionalLightActive = flag;
}

void FracturePhongShader::SetNormalActive(bool flag)
{
	m_bNormalMapActive = flag;
}

void FracturePhongShader::SetSkyLightReflections(bool flag)
{
	m_bSkyLightReflections = flag;
}

DirectionalLight& FracturePhongShader::GetDirectionalLight()
{
	return m_directionalLight;
}

void FracturePhongShader::SetAmbientLight(const glm::dvec3& ambientLight)
{
	FracturePhongShader::m_ambientLight = ambientLight;
}

void FracturePhongShader::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	FracturePhongShader::m_directionalLight = directionalLight;
}

void FracturePhongShader::SetPointLights(PointLight* pointLights, int arraySize)
{
	if (arraySize > MAX_POINT_LIGHTS) printf("ERROR: Demasiadas luces puntuales en la escena\n");
	else
	{
		FracturePhongShader::m_numPointLights = arraySize;
		FracturePhongShader::m_pointLights = pointLights;
	}
}

void FracturePhongShader::SetSpotLights(SpotLight* spotLights, int arraySize)
{
	if (arraySize > MAX_SPOT_LIGHTS) printf("ERROR: Demasiadas luces focales en la escena\n");
	else
	{
		FracturePhongShader::m_numSpotLights = arraySize;
		FracturePhongShader::m_spotLights = spotLights;
	}
}