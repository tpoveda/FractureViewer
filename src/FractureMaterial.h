#ifndef FractureMaterial_H
#define FractureMaterial_H

#include "FractureTexture.h"
#include "glm.hpp"

using namespace Fracture;

struct FractureMaterial
{
	FractureTexture* diffuseTexture;
	FractureTexture* specularTexture;
	FractureTexture* ambientTexture;
	FractureTexture* emissiveTexture;
	FractureTexture* normalTexture;
	FractureTexture* alphaTexture;
	glm::dvec3 diffuseColor;
	glm::dvec3 specularColor;
	double specularIntensity;
	double specularPower;
	double emissionContribution;
	double ambientContribution;
	double diffuseContribution;
	double specularContribution;
	double reflectionContribution;
	double textureTiling;

	FractureMaterial(FractureTexture* diffuseTexture = nullptr,
		FractureTexture* specularTexture = nullptr,
		FractureTexture* ambientTexture = nullptr,
		FractureTexture* emissiveTexture = nullptr,
		FractureTexture* normalTexture = nullptr,
		FractureTexture* alphaTexture = nullptr,
		glm::dvec3 diffuseColor = glm::dvec3(1.0, 1.0, 1.0),
		glm::dvec3 specularColor = glm::dvec3(1.0, 1.0, 1.0),
		double specularIntensity = 2,
		double specularPower = 32,
		double emissionContribution = 0.02,
		double ambientContribution = 0.0,
		double diffuseContribution = 1.0,
		double specularContribution = 0.38,
		double reflectionContribution = 1.0,
		double textureTiling = 1.0)
	{
		this->diffuseTexture = diffuseTexture;
		this->specularTexture = specularTexture;
		this->ambientTexture = ambientTexture;
		this->emissiveTexture = emissiveTexture;
		this->normalTexture = normalTexture;
		this->alphaTexture = alphaTexture;
		this->diffuseColor = diffuseColor;
		this->specularColor = specularColor;
		this->specularIntensity = specularIntensity;
		this->specularPower = specularPower;
		this->emissionContribution = emissionContribution;
		this->ambientContribution = ambientContribution;
		this->diffuseContribution = diffuseContribution;
		this->specularContribution = specularContribution;
		this->reflectionContribution = reflectionContribution;
		this->textureTiling = textureTiling;
	}
};

#endif