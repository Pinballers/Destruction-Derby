#include "Globals.h"
#include "Application.h"
#include "ModulePlayer2.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer2::ModulePlayer2(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer2::~ModulePlayer2()
{}

// Load assets
bool ModulePlayer2::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 2, 4);
	car.chassis_offset.Set(0, 1.5, 0);
	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;

	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->type = physType::PLAYER2;
	vehicle->collision_listeners.add(App->scene_intro);
	
	InitialPos();



	return true;
}

// Unload assets
bool ModulePlayer2::CleanUp()
{
	LOG("Unloading player");

	bullets.Clear();
	spheres.Clear();

	return true;
}

// Update: draw background
update_status ModulePlayer2::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		acceleration = MAX_ACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (turn < TURN_DEGREES)
			turn += TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if (turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		acceleration = -MAX_ACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT)
	{
		brake = BRAKE_POWER;
	}

	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
	{
		if (lifes > 0)
			RespawnCar();
		else
			lost = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_RCTRL) == KEY_DOWN)
	{
		Sphere s;
		s.radius = 1;
		s.SetPos(vehicle->vehicle->getChassisWorldTransform().getOrigin().getX() + 5 * vehicle->vehicle->getForwardVector().getX(),
			1,
			vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ() + 5 * vehicle->vehicle->getForwardVector().getZ());

		s.color.r = 255;

		float force = 50.0f;
		PhysBody3D* p = App->physics->AddBody(s);
		p->Push(-(App->camera2->Z.x * force), -(App->camera2->Z.y * force), -(App->camera2->Z.z * force));
		p->type = physType::BULLET;
		bullets.PushBack(p);
		spheres.PushBack(s);

	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	return UPDATE_CONTINUE;
}

void ModulePlayer2::Draw(float dt) {
	for (int i = 0; i < bullets.Count(); i++)
	{
		bullets[i]->GetTransform(&spheres[i].transform);
		spheres[i].Render();
	}
	if (lost)
		Lost();

	vehicle->Render();
}

void ModulePlayer2::InitialPos() const {

	vehicle->SetRotation({ 0,-1,0,1 });
	vehicle->SetPos(150, 5, 0);
}

void ModulePlayer2::RespawnCar() {
	if (lifes <= 0) {
		lost = true;
	}
	else {
		InitialPos();
		vehicle->vehicle->getRigidBody()->setAngularVelocity({ 0, 0, 0 });
		vehicle->vehicle->getRigidBody()->setLinearVelocity({ 0, 0, 0 });
		lifes--;
		App->player->InitialPos();
	}

	App->player->restart = true;
}



void ModulePlayer2::Lost() {
	App->player->Win();
	lost = false;
	lifes = 3;
}

void ModulePlayer2::Win() {
	wins++;
	RespawnCar();
	lifes = 3;
}



