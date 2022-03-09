#pragma once
#include "cTank.h"

class cPlayerTank : public cTank
{
private:
	bool finishMoving;
	int weaponType;
	float moveSpeed;
	bool firingBullet;
	float chargeTimer;
	bool charging;
	bool firingLaser;
public:
	cPlayerTank();
	virtual ~cPlayerTank();

	void updateTank(float deltaTime);

	virtual void MoveUp();
	virtual void MoveDown();
	virtual void MoveLeft();
	virtual void MoveRight();

	void SetFinishMoving(bool setM);

	void SetWeaponType(int type);
	int GetWeaponType();

	void SetFiringBullet(bool fire);
	bool GetFiringBullet();

	void SetFiringAndChargingLaser(bool fire, bool chargi, float charge);
	bool GetCharging();
	bool GetFiringLaser();
};