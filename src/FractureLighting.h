#ifndef FractureLighting_H
#define FractureLighting_H

#include <glm.hpp>

struct BaseLight
{
	glm::dvec3 color;
	glm::dvec3 colorSpec;
	double intensity;

	BaseLight(glm::dvec3 color = glm::dvec3(0, 0, 0), glm::dvec3 colorSpec = glm::dvec3(0, 0, 0), double intensity = 0)
	{
		this->color = color;
		this->colorSpec = colorSpec;
		this->intensity = intensity;
	}
};

struct DirectionalLight
{
	BaseLight base;
	glm::dvec3 direction;

	DirectionalLight(BaseLight base = BaseLight(), glm::dvec3 direction = glm::dvec3(0, 0, 0))
	{
		this->base = base;
		this->direction = direction;
	}
};

struct Attenuation
{
	double constant;
	double linear;
	double exponent;

	Attenuation(double constant = 0, double linear = 0, double exponent = 1)
	{
		this->constant = constant;
		this->linear = linear;
		this->exponent = exponent;
	}
};

struct PointLight
{
	BaseLight base;
	Attenuation atten;
	glm::dvec3 position;
	double range;

	PointLight(BaseLight base = BaseLight(), Attenuation atten = Attenuation(), glm::dvec3 position = glm::dvec3(), double range = 0.0)
	{
		this->base = base;
		this->atten = atten;
		this->position = position;
		this->range = range;
	}
};

struct SpotLight
{
	PointLight pointLight;
	glm::dvec3 direction;
	double cutoff;

	SpotLight(PointLight pointLight = PointLight(), glm::dvec3 direction = glm::dvec3(), double cutoff = 0)
	{
		this->pointLight = pointLight;
		this->direction = direction;
		this->cutoff = cutoff;
	}
};

#endif