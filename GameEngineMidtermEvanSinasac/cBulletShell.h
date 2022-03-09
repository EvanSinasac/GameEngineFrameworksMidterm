#pragma once
#include "cMesh.h"
#include "GLCommon.h"
#include "GLMCommon.h"
#include "lookDirection.h"

class cBulletShell
{
private:
	cMesh* m_pMesh;
	LookDirection moveDir;
	glm::vec3 position;
	bool alive;
public:
	cBulletShell(LookDirection look);
	virtual ~cBulletShell();

	void updatePos();

	bool CheckUp();
	bool CheckDown();
	bool CheckLeft();
	bool CheckRight();

	cMesh* makeMesh(glm::vec3 position, float scale, glm::vec4 colour);
	cMesh* getMesh();

	bool getAlive();
};