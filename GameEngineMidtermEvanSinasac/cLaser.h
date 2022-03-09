#pragma once
#include "cMesh.h"
#include "GLCommon.h"
#include "GLMCommon.h"
#include "lookDirection.h"

class cLaser
{
private:
	cMesh* m_pMesh;
	glm::vec3 position;
	bool alive;
	float timeAlive;
public:
	cLaser();
	virtual ~cLaser();

	void updatePos(float deltaTime);

	cMesh* makeMesh(glm::vec3 position, float scale, glm::vec4 colour);
	cMesh* getMesh();

	bool getAlive();

};