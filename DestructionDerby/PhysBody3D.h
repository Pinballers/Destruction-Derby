#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"

class btRigidBody;
class Module;
class btQuaternion;

enum physType
{
	PLAYER1,
	PLAYER2,
	BULLET,
	WALL,
	NO_TYPE,
};

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	void SetRotation(btQuaternion rotation) const;
	

private:
	btRigidBody* body = nullptr;

public:
	p2List<Module*> collision_listeners;
	physType type = NO_TYPE;
	
};

#endif // __PhysBody3D_H__