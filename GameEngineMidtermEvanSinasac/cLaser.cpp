#include "cLaser.h"
#include "globalThings.h"

cLaser::cLaser()
{
	m_pMesh = NULL;
	position = glm::vec3(1.0f);
	alive = true;
	timeAlive = 0.0f;
}

cLaser::~cLaser() {

}


void cLaser::updatePos(float deltaTime)
{
	if (alive)
	{
		timeAlive += deltaTime;
		if (timeAlive >= 5.0f)
		{
			alive = false;
		}
	}
}


cMesh* cLaser::makeMesh(glm::vec3 position, float scale, glm::vec4 colour)
{
	m_pMesh = new cMesh();
	m_pMesh->meshName = "Sphere_xyz_n_rgba.ply";
	m_pMesh->positionXYZ = position;
	this->position = position;
	m_pMesh->orientationXYZ = glm::vec3(1.0f);
	m_pMesh->scale = scale;// 0.5f;
	m_pMesh->bUseWholeObjectDiffuseColour = true;
	m_pMesh->wholeObjectDiffuseRGBA = colour; // glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);		//Red
	return m_pMesh;
}
cMesh* cLaser::getMesh()
{
	return m_pMesh;
}

bool cLaser::getAlive()
{
	return alive;
}