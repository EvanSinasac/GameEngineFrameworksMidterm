#pragma once
#include "cMesh.h"
#include "GLCommon.h"
#include "GLMCommon.h"
#include "lookDirection.h"



class cTank
{
private:
	cMesh* m_pMesh;
	LookDirection m_LastLookDirection;
	glm::vec3 position;
public:
	cTank();
	virtual ~cTank();

	//Move the tank in a direction (will be the same logic whether it's the player, AI or Super Tank moving)
	virtual void MoveUp();
	virtual void MoveDown();
	virtual void MoveLeft();
	virtual void MoveRight();

	//Have to check whether the tank can move in the direction it wants to move in
	bool CheckUp();
	bool CheckDown();
	bool CheckLeft();
	bool CheckRight();

	//Mesh attached to tank
	cMesh* makeMesh(glm::vec3 position, glm::vec3 orientation, float scale, glm::vec4 colour, LookDirection look);
	cMesh* getMesh();

	void SetPosition(glm::vec3 pos);
	glm::vec3 GetPosition();

	void SetLookDirection(LookDirection look);
	LookDirection GetLookDirection();
};