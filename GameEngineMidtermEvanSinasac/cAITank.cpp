#include "cAITank.h"
#include "globalThings.h"

cAITank::cAITank(int type)
{
	waiting = false;
	moving = false;
	blocked = false;
	waitToMove = 0.0f;
	firing = false;
	shellMade = NULL;
	laserMade = NULL;
	int weaponType = type;
	firingLaser = false;
	charging = false;
	chargeTimer = 0.0f;
}
cAITank::~cAITank()
{

}


void cAITank::MoveUp()
{
	if (!this->CheckUp())
	{
		int x = 100 - ceil(this->GetPosition().x);
		int y = 60 - ceil(this->GetPosition().y);
		::g_Maze[y][x] = false;

		this->SetPosition(this->GetPosition() += glm::vec3(0.0f, 1.0f, 0.0f));
		this->getMesh()->positionXYZ = this->GetPosition();
		this->SetLookDirection(UP);
		
		x = 100 - ceil(this->GetPosition().x);
		y = 60 - ceil(this->GetPosition().y);
		::g_Maze[y][x] = true;
	}
	else
	{
		this->moving = false;
		this->waiting = true;
		this->blocked = false;
		waitToMove = 5.0f;
	}
}
void cAITank::MoveDown()
{
	if (!this->CheckDown())
	{
		int x = 100 - floor(this->GetPosition().x);
		int y = 60 - floor(this->GetPosition().y);
		::g_Maze[y][x] = false;

		this->SetPosition(this->GetPosition() -= glm::vec3(0.0f, 1.0f, 0.0f));
		this->getMesh()->positionXYZ = this->GetPosition();
		this->SetLookDirection(DOWN);

		x = 100 - floor(this->GetPosition().x);
		y = 60 - floor(this->GetPosition().y);
		::g_Maze[y][x] = true;
	}
	else
	{
		this->moving = false;
		this->waiting = true;
		this->blocked = false;
		waitToMove = 5.0f;
	}
}
void cAITank::MoveLeft()
{
	if (!this->CheckLeft())
	{
		int x = 100 - (int)this->GetPosition().x;
		int y = 60 - (int)this->GetPosition().y;
		::g_Maze[y][x] = false;

		this->SetPosition(this->GetPosition() += glm::vec3(1.0f, 0.0f, 0.0f));
		this->getMesh()->positionXYZ = this->GetPosition();
		this->SetLookDirection(LEFT);

		x = 100 - (int)this->GetPosition().x;
		y = 60 - (int)this->GetPosition().y;
		::g_Maze[y][x] = true;
	}
	else
	{
		this->moving = false;
		this->waiting = true;
		this->blocked = false;
		waitToMove = 5.0f;
	}
}
void cAITank::MoveRight()
{
	if (!this->CheckRight())
	{
		int x = 100 - ceil(this->GetPosition().x);
		int y = 60 - ceil(this->GetPosition().y);
		::g_Maze[y][x] = false;

		this->SetPosition(this->GetPosition() -= glm::vec3(1.0f, 0.0f, 0.0f));
		this->getMesh()->positionXYZ = this->GetPosition();
		this->SetLookDirection(RIGHT);

		x = 100 - ceil(this->GetPosition().x);
		y = 60 - ceil(this->GetPosition().y);
		::g_Maze[y][x] = true;
	}
	else
	{
		this->moving = false;
		this->waiting = true;
		this->blocked = false;
		waitToMove = 5.0f;
	}
}

void cAITank::updateTank(float deltaTime)
{
	if (!charging)
	{
		if (this->moving)
		{
			if (this->GetLookDirection() == UP)
			{
				this->MoveUp();
			}
			else if (this->GetLookDirection() == DOWN)
			{
				this->MoveDown();
			}
			else if (this->GetLookDirection() == RIGHT)
			{
				this->MoveRight();
			}
			else if (this->GetLookDirection() == LEFT)
			{
				this->MoveLeft();
			}
		}
		else if (this->waiting)
		{
			waitToMove -= deltaTime;
			if (waitToMove <= 0.0f)
			{
				//Choose new direction
				srand(time_t(glfwGetTime));
				int newDir = rand() % 4;
				LookDirection newLookDir;
				switch (newDir)
				{
				case 0:
					newLookDir = UP;
					blocked = this->CheckUp();
					break;
				case 1:
					newLookDir = DOWN;
					blocked = this->CheckDown();
					break;
				case 2:
					newLookDir = LEFT;
					blocked = this->CheckLeft();
					break;
				case 3:
					newLookDir = RIGHT;
					blocked = this->CheckRight();
					break;
				default:
					break;
				}
				//Check if we can move in that direction
				if (!blocked)
				{
					this->SetLookDirection(newLookDir);
					moving = true;
				}
				waiting = false;
			}
		}
		else if (this->blocked)
		{
			srand(time_t(glfwGetTime()));
			int newDir = rand() % 4;
			LookDirection newLookDir;
			switch (newDir)
			{
			case 0:
				newLookDir = UP;
				blocked = this->CheckUp();
				break;
			case 1:
				newLookDir = DOWN;
				blocked = this->CheckDown();
				break;
			case 2:
				newLookDir = LEFT;
				blocked = this->CheckLeft();
				break;
			case 3:
				newLookDir = RIGHT;
				blocked = this->CheckRight();
				break;
			default:
				break;
			}
			//Check if we can move in that direction
			if (!blocked)
			{
				this->SetLookDirection(newLookDir);
				moving = true;
			}
		}
	}
	else
	{
		chargeTimer -= deltaTime;
		if (chargeTimer <= 0.0f)
		{
			charging = false;
			firingLaser = true;
		}
	}
	
	if (laserMade)
	{
		if (!laserMade->getAlive())
		{
			firingLaser = false;
		}
	}
	
	if (shellMade)
	{
		if (!shellMade->getAlive())
		{
			firing = false;
		}
	}
	
} //end of updateTank

void cAITank::setMovingToTrue()
{
	this->moving = true;
}

cBulletShell* cAITank::FireBullet()
{
	shellMade = nullptr;
	shellMade = new cBulletShell(this->GetLookDirection());
	if (this->GetLookDirection() == UP)
	{
		shellMade->makeMesh(glm::vec3(this->GetPosition().x, this->GetPosition().y + 1, this->GetPosition().z), 1.0f, glm::vec4(0.1f, 0.2f, 0.0f, 1.0f));
	}
	else if (this->GetLookDirection() == DOWN)
	{
		shellMade->makeMesh(glm::vec3(this->GetPosition().x, this->GetPosition().y - 1, this->GetPosition().z), 1.0f, glm::vec4(0.1f, 0.2f, 0.0f, 1.0f));
	}
	else if (this->GetLookDirection() == LEFT)
	{
		shellMade->makeMesh(glm::vec3(this->GetPosition().x + 1, this->GetPosition().y, this->GetPosition().z), 1.0f, glm::vec4(0.1f, 0.2f, 0.0f, 1.0f));
	}
	else if (this->GetLookDirection() == RIGHT)
	{
		shellMade->makeMesh(glm::vec3(this->GetPosition().x - 1, this->GetPosition().y, this->GetPosition().z), 1.0f, glm::vec4(0.1f, 0.2f, 0.0f, 1.0f));
	}
	return shellMade;
}

cLaser* cAITank::FireLaser(glm::vec3 pos)
{
	laserMade = nullptr;
	laserMade = new cLaser();
	
	laserMade->makeMesh(pos, 1.0f, glm::vec4(0.1f, 0.2f, 0.0f, 1.0f));//glm::vec3(this->GetPosition().x + 1, this->GetPosition().y, this->GetPosition().z), 1.0f, glm::vec4(0.1f, 0.2f, 0.0f, 1.0f));
	firingLaser = true;
	return laserMade;
}

int cAITank::getWeaponType()
{
	return weaponType;
}

void cAITank::SetFiring(bool fire)
{
	firing = fire;
}
bool cAITank::GetFiring()
{
	return firing;
}


void cAITank::SetFiringAndChargingLaser(bool fire, bool chargi, float charge)
{
	firingLaser = fire;
	charging = chargi;
	chargeTimer = charge;
}
bool cAITank::GetCharging()
{
	return charging;
}
bool cAITank::GetFiringLaser()
{
	return firingLaser;
}