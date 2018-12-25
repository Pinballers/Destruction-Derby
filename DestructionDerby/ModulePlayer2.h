#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 1000.0f

class ModulePlayer2 : public Module
{
public:
	ModulePlayer2(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer2();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	void InitialPos() const;
	void RespawnCar();
	void Lost();
	void  Win();
	void Draw(float dt);

public:

	PhysVehicle3D * vehicle;
	float turn;
	float acceleration;
	float brake;
	uint  lifes = 3;
	uint  wins = 0;
	bool  lost = false;

	p2DynArray<PhysBody3D*> bullets;
	p2DynArray<Sphere> spheres;

};