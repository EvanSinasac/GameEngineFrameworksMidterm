#include "cPlayerTank.h"
#include "globalThings.h"

cPlayerTank::cPlayerTank()
{
	this->finishMoving = false;
	weaponType = 0;
	moveSpeed = 1.0f;
	firingBullet = false;
	chargeTimer = 0.0f;
	charging = false;
	firingLaser = false;
}

cPlayerTank::~cPlayerTank()
{

}

void cPlayerTank::updateTank(float deltaTime)
{
	if (charging)
	{
		chargeTimer -= deltaTime;
		if (chargeTimer <= 0.0f)
		{
			charging = false;
			firingLaser = true;
		}
	}
}


void cPlayerTank::MoveUp()
{
	if (!finishMoving)
	{
		if (!this->CheckUp())
		{
			int x = 100 - ceil(this->GetPosition().x);
			int y = 60 - ceil(this->GetPosition().y);
			::g_Maze[y][x] = false;

			this->SetPosition(this->GetPosition() += glm::vec3(0.0f, moveSpeed, 0.0f));
			this->getMesh()->positionXYZ = this->GetPosition();
			this->SetLookDirection(UP);
			
			x = 100 - ceil(this->GetPosition().x);
			y = 60 - ceil(this->GetPosition().y);
			::g_Maze[y][x] = true;
		}
	}
	else
	{
		if (!(ceil(this->GetPosition().y) == this->GetPosition().y || floor(this->GetPosition().y) == this->GetPosition().y))
		{
			int x = 100 - ceil(this->GetPosition().x);
			int y = 60 - ceil(this->GetPosition().y);
			::g_Maze[y][x] = false;

			this->SetPosition(this->GetPosition() += glm::vec3(0.0f, moveSpeed, 0.0f));
			this->getMesh()->positionXYZ = this->GetPosition();
			this->SetLookDirection(UP);
			
			x = 100 - ceil(this->GetPosition().x);
			y = 60 - ceil(this->GetPosition().y);
			::g_Maze[y][x] = true;
		}
		finishMoving = false;
	}
}
void cPlayerTank::MoveDown()
{
	if (!finishMoving)
	{
		if (!this->CheckDown())
		{
			int x = 100 - floor(this->GetPosition().x);
			int y = 60 - floor(this->GetPosition().y);
			::g_Maze[y][x] = false;

			this->SetPosition(this->GetPosition() -= glm::vec3(0.0f, moveSpeed, 0.0f));
			this->getMesh()->positionXYZ = this->GetPosition();
			this->SetLookDirection(DOWN);
			
			x = 100 - floor(this->GetPosition().x);
			y = 60 - floor(this->GetPosition().y);
			::g_Maze[y][x] = true;
		}
	}
	else
	{
		if (!(ceil(this->GetPosition().y) == this->GetPosition().y || floor(this->GetPosition().y) == this->GetPosition().y))
		{
			int x = 100 - floor(this->GetPosition().x);
			int y = 60 - floor(this->GetPosition().y);
			::g_Maze[y][x] = false;

			this->SetPosition(this->GetPosition() -= glm::vec3(0.0f, moveSpeed, 0.0f));
			this->getMesh()->positionXYZ = this->GetPosition();
			this->SetLookDirection(DOWN);
			
			x = 100 - floor(this->GetPosition().x);
			y = 60 - floor(this->GetPosition().y);
			::g_Maze[y][x] = true;
		}
		finishMoving = false;
	}
}
void cPlayerTank::MoveLeft()
{
	if (!finishMoving)
	{
		if (!this->CheckLeft())
		{
			int x = 100 - (int)this->GetPosition().x;
			int y = 60 - (int)this->GetPosition().y;
			::g_Maze[y][x] = false;

			this->SetPosition(this->GetPosition() += glm::vec3(moveSpeed, 0.0f, 0.0f));
			this->getMesh()->positionXYZ = this->GetPosition();
			this->SetLookDirection(LEFT);
			
			x = 100 - (int)this->GetPosition().x;
			y = 60 - (int)this->GetPosition().y;
			::g_Maze[y][x] = true;
		}
	}
	else
	{
		if (!(ceil(this->GetPosition().y) == this->GetPosition().y || floor(this->GetPosition().y) == this->GetPosition().y))
		{
			int x = 100 - (int)this->GetPosition().x;
			int y = 60 - (int)this->GetPosition().y;
			::g_Maze[y][x] = false;

			this->SetPosition(this->GetPosition() += glm::vec3(moveSpeed, 0.0f, 0.0f));
			this->getMesh()->positionXYZ = this->GetPosition();
			this->SetLookDirection(LEFT);
			
			x = 100 - (int)this->GetPosition().x;
			y = 60 - (int)this->GetPosition().y;
			::g_Maze[y][x] = true;
		}
		finishMoving = false;
	}
}
void cPlayerTank::MoveRight()
{
	if (!finishMoving)
	{
		if (!this->CheckRight())
		{
			int x = 100 - ceil(this->GetPosition().x);
			int y = 60 - ceil(this->GetPosition().y);
			::g_Maze[y][x] = false;

			this->SetPosition(this->GetPosition() -= glm::vec3(moveSpeed, 0.0f, 0.0f));
			this->getMesh()->positionXYZ = this->GetPosition();
			this->SetLookDirection(RIGHT);
			
			x = 100 - ceil(this->GetPosition().x);
			y = 60 - ceil(this->GetPosition().y);
			::g_Maze[y][x] = true;
		}
	}
	else
	{
		if (!(ceil(this->GetPosition().y) == this->GetPosition().y || floor(this->GetPosition().y) == this->GetPosition().y))
		{
			int x = 100 - ceil(this->GetPosition().x);
			int y = 60 - ceil(this->GetPosition().y);
			::g_Maze[y][x] = false;

			this->SetPosition(this->GetPosition() -= glm::vec3(moveSpeed, 0.0f, 0.0f));
			this->getMesh()->positionXYZ = this->GetPosition();
			this->SetLookDirection(RIGHT);
			
			x = 100 - ceil(this->GetPosition().x);
			y = 60 - ceil(this->GetPosition().y);
			::g_Maze[y][x] = true;
		}
		finishMoving = false;
	}
}

void cPlayerTank::SetFinishMoving(bool setM)
{
	this->finishMoving = setM;
}

void cPlayerTank::SetWeaponType(int type)
{
	weaponType = type;
}
int cPlayerTank::GetWeaponType()
{
	return weaponType;
}

void cPlayerTank::SetFiringBullet(bool fire)
{
	firingBullet = fire;
}
bool cPlayerTank::GetFiringBullet()
{
	return firingBullet;
}

void cPlayerTank::SetFiringAndChargingLaser(bool fire, bool chargi, float charge)
{
	firingLaser = fire;
	charging = chargi;
	chargeTimer = charge;
}
bool cPlayerTank::GetCharging()
{
	return charging;
}
bool cPlayerTank::GetFiringLaser()
{
	return firingLaser;
}