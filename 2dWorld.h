#pragma once
#include <Box2D/Box2D.h>
#include <vector>


class WorldClass {

private:
	//tworzenie swiata, grawitacji i jego parametrow
	const float timeStep = 1.0f / 60.0f;
	const int32 velocityIterations = 10;
	const int32 positionIterations = 60;

	b2Vec2 gravity = b2Vec2(0.0f, 10.f);
	float RATIO;
	b2World world = gravity;
	b2World* world_ptr = &world;
	class myListener : public b2ContactListener {
		void BeginContact(b2Contact* contact);
		void EndContact(b2Contact* contact);

		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
		void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
	};
	myListener listener;

public:
	
	//boundries
	std::vector<b2Body*> boundries;
	//ball
	b2Body* ball = nullptr;
	//flippers
	b2RevoluteJoint* LeftFlipper = nullptr;
	b2RevoluteJoint* RightFlipper = nullptr;
	std::vector<b2Body*> bumpers;

	//constructor
	WorldClass(float RATIO);

	void destroyBody(b2Body*body);
	void ReinitBall(float s_X, float s_Y, float R);
	virtual ~WorldClass();
	void create_boundries(float X, float Y, float R);
	void init_ball(float s_X, float s_Y, float R);
	void init_round_bumper(float X, float Y, float R);
	void world_step();
	void create_flippers(float Left_X, float Right_X, float s_Y, float R);
	void create_board(float Window_X, float Window_Y, float L_flipper_X, float R_flipper_X, float flipper_Y, float R);

	//get functions
	float GetBodyPositionX(b2Body*body);
	float GetBodyPositionX(b2RevoluteJoint* joint,char body);
	float GetBodyPositionY(b2Body*body);
	float GetBodyPositionY(b2RevoluteJoint*joint,char body);

	b2World* GetWorldPtr();
};