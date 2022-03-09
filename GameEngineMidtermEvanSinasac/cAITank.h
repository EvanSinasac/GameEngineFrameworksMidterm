#pragma once

#include "cTank.h"
#include "cBulletShell.h"
#include "cLaser.h"

class cAITank : public cTank
{
private:
	bool waiting;
	bool moving;
	bool blocked;
	float waitToMove;
	bool firing;
	bool charging;
	float chargeTimer;
	bool firingLaser;
	cBulletShell* shellMade;
	cLaser* laserMade;
	int weaponType;
public:
	cAITank(int type);
	virtual ~cAITank();


	virtual void MoveUp();
	virtual void MoveDown();
	virtual void MoveLeft();
	virtual void MoveRight();

	void updateTank(float deltaTime);
	void setMovingToTrue();

	cBulletShell* FireBullet();
	cLaser* FireLaser(glm::vec3 pos);

	int getWeaponType();

	void SetFiring(bool fire);
	bool GetFiring();

	void SetFiringAndChargingLaser(bool fire, bool chargi, float charge);
	bool GetCharging();
	bool GetFiringLaser();
};