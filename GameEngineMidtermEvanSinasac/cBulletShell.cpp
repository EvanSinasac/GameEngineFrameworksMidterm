#include "cBulletShell.h"
#include "globalThings.h"

cBulletShell::cBulletShell(LookDirection look)
{
	m_pMesh = NULL;
	moveDir = look;
	position = glm::vec3(1.0f);
	alive = true;
}

cBulletShell::~cBulletShell()
{

}

void cBulletShell::updatePos()
{
	if (alive)
	{
		if (moveDir == UP)
		{
			if (!CheckUp())
			{
				this->position += glm::vec3(0.0f, 2.0f, 0.0f);
				this->getMesh()->positionXYZ = this->position;
			}
			else {
				alive = false;
			}
		}
		else if (moveDir == DOWN)
		{
			if (!CheckDown())
			{
				this->position -= glm::vec3(0.0f, 2.0f, 0.0f);
				this->getMesh()->positionXYZ = this->position;
			}
			else {
				alive = false;
			}
		}
		else if (moveDir == LEFT)
		{
			if (!CheckLeft())
			{
				this->position += glm::vec3(2.0f, 0.0f, 0.0f);
				this->getMesh()->positionXYZ = this->position;
			}
			else {
				alive = false;
			}
		}
		else if (moveDir == RIGHT)
		{
			if (!CheckRight())
			{
				this->position -= glm::vec3(2.0f, 0.0f, 0.0f);
				this->getMesh()->positionXYZ = this->position;
			}
			else {
				alive = false;
			}
		}
	}
}

bool cBulletShell::CheckUp()
{
	int x = 100 - ceil(this->position.x);
	int y = 60 - ceil(this->position.y);
	if (x < 0 || y < 0)
	{
		return false;
	}
	//std::cout << "tank x: " << this->position.x << " y: " << this->position.y << " ";
	//std::cout << (::g_Maze[y + 1][x] ? "true" : "false") << std::endl;
	return ::g_Maze[y - 2][x] || ::g_Maze[y - 1][x];
}
bool cBulletShell::CheckDown()
{
	int x = 100 - floor(this->position.x);
	int y = 60 - floor(this->position.y);
	if (x < 0 || y < 0)
	{
		return false;
	}
	return ::g_Maze[y + 2][x] || ::g_Maze[y + 1][x];
}
bool cBulletShell::CheckLeft()
{
	int x = 100 - (int)this->position.x;
	int y = 60 - (int)this->position.y;
	if (x < 0 || y < 0)
	{
		return false;
	}
	return ::g_Maze[y][x - 2] || ::g_Maze[y][x - 1];
}
bool cBulletShell::CheckRight()
{
	int x = 100 - ceil(this->position.x);
	int y = 60 - ceil(this->position.y);
	if (x < 0 || y < 0)
	{
		return false;
	}
	return ::g_Maze[y][x + 2] || ::g_Maze[y][x + 1];
}

cMesh* cBulletShell::makeMesh(glm::vec3 position, float scale, glm::vec4 colour)
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
cMesh* cBulletShell::getMesh()
{
	return m_pMesh;
}

bool cBulletShell::getAlive()
{
	return alive;
}