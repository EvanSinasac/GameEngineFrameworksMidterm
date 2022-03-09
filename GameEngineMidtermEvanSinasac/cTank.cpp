#include "cTank.h"
#include "globalThings.h"
#include <iostream>

cTank::cTank()
{
	this->m_pMesh = NULL;
	this->m_LastLookDirection = UP;
	this->position = glm::vec3(0.0f);
}

cTank::~cTank()
{

}

void cTank::MoveUp()
{
	if (!CheckUp())
	{
		::g_Maze[(int)this->GetPosition().y][(int)this->GetPosition().x] = false;
		this->position += glm::vec3(0.0f, 1.0f, 0.0f);
		this->getMesh()->positionXYZ = this->position;
		this->SetLookDirection(UP);
		::g_Maze[(int)this->GetPosition().y][(int)this->GetPosition().x] = true;
	}
}
void cTank::MoveDown()
{
	if (!CheckDown())
	{
		::g_Maze[(int)this->GetPosition().y][(int)this->GetPosition().x] = false;
		this->position -= glm::vec3(0.0f, 1.0f, 0.0f);
		this->getMesh()->positionXYZ = this->position;
		this->SetLookDirection(DOWN);
		::g_Maze[(int)this->GetPosition().y][(int)this->GetPosition().x] = true;
	}
}
void cTank::MoveLeft()
{
	if (!CheckLeft())
	{
		::g_Maze[(int)this->GetPosition().y][(int)this->GetPosition().x] = false;
		this->position += glm::vec3(1.0f, 0.0f, 0.0f);
		this->getMesh()->positionXYZ = this->position;
		this->SetLookDirection(LEFT);
		::g_Maze[(int)this->GetPosition().y][(int)this->GetPosition().x] = true;
	}
}
void cTank::MoveRight()
{
	if (!CheckRight())
	{
		::g_Maze[(int)this->GetPosition().y][(int)this->GetPosition().x] = false;
		this->position -= glm::vec3(1.0f, 0.0f, 0.0f);
		this->getMesh()->positionXYZ = this->position;
		this->SetLookDirection(RIGHT);
		::g_Maze[(int)this->GetPosition().y][(int)this->GetPosition().x] = true;
	}
}


bool cTank::CheckUp()
{
	int x = 100 - ceil(this->position.x);
	int y = 60 - ceil(this->position.y);
	if (x < 0 || y < 0)
	{
		return false;
	}
	//std::cout << "tank x: " << this->position.x << " y: " << this->position.y << " ";
	//std::cout << (::g_Maze[y + 1][x] ? "true" : "false") << std::endl;
	return ::g_Maze[y - 1][x];
}
bool cTank::CheckDown()
{
	int x = 100 - floor(this->position.x);
	int y = 60 - floor(this->position.y);
	if (x < 0 || y < 0)
	{
		return false;
	}
	return ::g_Maze[y + 1][x];
}
bool cTank::CheckLeft()
{
	int x = 100 - (int)this->position.x;
	int y = 60 - (int)this->position.y;
	if (x < 0 || y < 0)
	{
		return false;
	}
	return ::g_Maze[y][x-1];
}
bool cTank::CheckRight()
{
	int x = 100 - ceil(this->position.x);
	int y = 60 - ceil(this->position.y);
	if (x < 0 || y < 0)
	{
		return false;
	}
	return ::g_Maze[y][x+1];
}


cMesh* cTank::makeMesh(glm::vec3 position, glm::vec3 orientation, float scale, glm::vec4 colour, LookDirection look)
{
	m_pMesh = new cMesh();
	m_pMesh->meshName = "Low_Poly_Tank_Model_3D_model_xyz_n_rgba.ply";
	m_pMesh->positionXYZ = position;
	this->position = position;
	m_pMesh->orientationXYZ = orientation;
	m_pMesh->scale = scale;// 0.5f;
	m_pMesh->bUseWholeObjectDiffuseColour = true;
	m_pMesh->wholeObjectDiffuseRGBA = colour; // glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);		//Red
	m_LastLookDirection = look;
	return m_pMesh;
}
cMesh* cTank::getMesh()
{
	return m_pMesh;
}

void cTank::SetPosition(glm::vec3 pos)
{
	this->position = pos;
}
glm::vec3 cTank::GetPosition()
{
	return this->position;
}

void cTank::SetLookDirection(LookDirection look)
{
	this->m_LastLookDirection = look;
	this->getMesh()->orientationXYZ = glm::vec3(glm::radians(-90.0f), 0.0f, glm::radians((float)look));
}
LookDirection cTank::GetLookDirection()
{
	return this->m_LastLookDirection;
}