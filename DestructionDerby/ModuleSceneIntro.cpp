#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	SceneMap();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	phys_cubes.Clear();
	cubes.Clear();

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	char title[80];
	sprintf_s(title, "Player 1 lifes: %i    Player 2 lifes: %i     Player 1 wins: %i   Player 2 wins: %i", 
		App->player->lifes,	App->player2->lifes, App->player->wins, App->player2->wins);
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1->type == physType::PLAYER1 && body2->type == physType::BULLET)
		App->player->RespawnCar();
	if (body1->type == physType::PLAYER2 && body2->type == physType::BULLET)
		App->player2->RespawnCar();
}

void ModuleSceneIntro::Draw(float dt) {
	for (int i = 0; i < phys_cubes.Count(); i++)
	{
		phys_cubes[i]->GetTransform(&cubes[i].transform);
		cubes[i].Render();
	}
}

void ModuleSceneIntro::SceneMap()
{
	//FLOOR
	Cube floor;
	floor = { 400, 0, 400};
	floor.color.Set(0, 1, 1, 1);
	
	PhysBody3D* p = App->physics->AddBody(floor, 0);
	p->type = physType::WALL;
	
	phys_cubes.PushBack(p);
	cubes.PushBack(floor);

	//WALLS
	Cube wall1(400, 300, 0);
	wall1.color.Set(0, 0, 1, 1);
	wall1.SetPos(0, 150, -200);
	p = App->physics->AddBody(wall1, 0);
	p->type = physType::WALL;
	phys_cubes.PushBack(p);
	cubes.PushBack(wall1);

	wall1.color.Set(0, 0, 1, 1);
	wall1.SetPos(0, 150, 200);
	p = App->physics->AddBody(wall1, 0);
	p->type = physType::WALL;
	phys_cubes.PushBack(p);
	cubes.PushBack(wall1);

	Cube wall2(0, 300, 400);
	wall2.color.Set(0, 0, 1, 1);
	wall2.SetPos(200, 150, 0);
	p = App->physics->AddBody(wall2, 0);
	p->type = physType::WALL;
	phys_cubes.PushBack(p);
	cubes.PushBack(wall2);

	wall2.color.Set(0, 0, 1, 1);
	wall2.SetPos(-200, 150, 0);
	p = App->physics->AddBody(wall2, 0);
	p->type = physType::WALL;
	phys_cubes.PushBack(p);
	cubes.PushBack(wall2);

	//BUILDINGS
	Cube building(100, 50, 100);
	building.color.Set(1, 0, 1, 1.F);
	building.SetPos(-75, 25, -75);
	p = App->physics->AddBody(building, 0);
	p->type = physType::WALL;
	phys_cubes.PushBack(p);
	cubes.PushBack(building);

	building.color.Set(1, 0, 1, 1.F);
	building.SetPos(75, 25, 75);
	p = App->physics->AddBody(building, 0);
	p->type = physType::WALL;
	phys_cubes.PushBack(p);
	cubes.PushBack(building);

	building.color.Set(1, 0, 1, 1.F);
	building.SetPos(-75, 25, 75);
	p = App->physics->AddBody(building, 0);
	p->type = physType::WALL;
	phys_cubes.PushBack(p);
	cubes.PushBack(building);

	building.color.Set(1, 0, 1, 1.F);
	building.SetPos(75, 25, -75);
	p = App->physics->AddBody(building, 0);
	p->type = physType::WALL;
	phys_cubes.PushBack(p);
	cubes.PushBack(building);

	//CONSTRAIN
	Cube constrain1(1, 1, 1);
	constrain1.SetPos(0, 80, 0);
	PhysBody3D* const1 = App->physics->AddBody(constrain1, 0);

	Cube constrain2(45, 5, 5);
	constrain2.color.Set(1, 0, 0, 1.F);
	constrain2.SetPos(0, 40, 0);
	PhysBody3D* const2 = App->physics->AddBody(constrain2, 50);
	const2->type = physType::BULLET;
	phys_cubes.PushBack(const2);
	cubes.PushBack(constrain2);

	App->physics->AddConstraintHinge(*const1, *const2, { 0,0,0 }, { 0,70,30 }, { 0,0,1 }, { 1,0,0 }, false);


}
