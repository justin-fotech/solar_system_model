#pragma once

#include <vector>
#include <string>
#include <tuple>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "GLErrors.h"
#include "OrbitalEllipse.h"

#define PI 3.1415926

class StellarObject;

// takes in meshes and creates all the StellarObjects (sun, planets, satellites) with correct parameters
std::vector<StellarObject> initStellarObjects(std::vector<std::unique_ptr<Mesh>>& meshes);

// helper function for initStellarObjects to link each object to the object it rotates around
StellarObject* getPtrToStellarObject(std::vector<StellarObject>& stellarObjects, std::string orbitalFocus);

class StellarObject
{
private:
	double m_axialTilt, m_rotationSpeed, m_objectRadius, m_lengthOfYear;
	double m_a, m_b;
	
	double m_curObjectRotation; // rotation about itself
	double m_curOrbitalRotation; // orbit around orbital focus
	
	std::unique_ptr<Mesh> m_mesh;

public:
	std::string m_name;

	// current location matrix
	glm::mat4 m_locMat;
	
	// mesh for orbital trajectory (not triangles, but line segments)
	std::unique_ptr<OrbitalEllipse> m_orbitalEllipse;

	// object around which this one orbits
	std::string m_orbitalFocus;

	// using raw ptr, objects were created with std::make_unique, are destructed elsewhere so no clean up
	StellarObject* m_orbitalFocusPtr;

	StellarObject(std::string name, std::string orbitalFocus, double axialTilt, double rotationSpeed, double objectRadius,
		double lengthOfYear, double startingAngle, double a, double b, std::unique_ptr<Mesh> mesh);
	~StellarObject();

	// move constructor takes care of unique_ptr to mesh
	StellarObject(StellarObject&& other) noexcept;

	// update rotation and position
	void updateModel(double timeElapsed);
	
	void updateRotation(double timeElapsed);
	void updatePosition(double timeElapsed);

	// exports model uniform to the shader
	void exportToShader(Shader& shader, const char* uniform);

	void draw(Shader& shader);
};