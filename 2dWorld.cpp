#include <Box2D/Box2D.h>
#include <vector>
#include "2dWorld.h"
#include "Game.h"
#include <cmath>
#include <iostream>

constexpr auto PI = 3.14159265359;
constexpr auto SQR2 = 1.41421356237;

//funkcja zderzen dla klasy myListener (lacznik pomiedzy listenerem a worldem)
void WorldClass::myListener::BeginContact(b2Contact* contact) {
    Game::begin_ball_collision(contact->GetFixtureA()->GetBody(), contact->GetFixtureB()->GetBody());
} 
void WorldClass::myListener::EndContact(b2Contact* contact) {
    Game::end_ball_collision(contact->GetFixtureA()->GetBody(), contact->GetFixtureB()->GetBody());
}
void WorldClass::myListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
}
void WorldClass::myListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
}

//constructor
WorldClass::WorldClass(float RATIO)
{
    this->RATIO = RATIO;
    this->world.SetContactListener(&listener);
}
//destructor
WorldClass::~WorldClass()
{
}

void WorldClass::destroyBody(b2Body* body)
{
    this->world.DestroyBody(body);
}
void WorldClass::ReinitBall(float s_X, float s_Y, float R)
{
    this->destroyBody(ball);
    this->init_ball(s_X, s_Y, R);
}
void WorldClass::create_board(float Window_X, float Window_Y, float L_flipper_X, float R_flipper_X, float flipper_Y, float R)
{
    Window_X /= RATIO;
    Window_Y /= RATIO;
    L_flipper_X /= RATIO;
    R_flipper_X /= RATIO;
    flipper_Y /= RATIO;
    R /= RATIO;
    
    b2PolygonShape BodyShape;
    b2Body* Body=nullptr;
    b2BodyDef BodyDef;

    b2ChainShape ChainShape;

    //end point-> srodek pilki + R

    //najpierw lejki, potem krawedzie

    BodyDef.type = b2_staticBody;

    //Left exit:
        
    // L_flip_X-3*R (podstawa) * sqrt2
    //srodek prostokata: 3*R + podstawa/2


    b2Vec2 Vertices[3] =
    { b2Vec2(L_flipper_X,flipper_Y + 4 * R),
        b2Vec2(3 * R,flipper_Y - 4 * R),
        b2Vec2(3 * R,Window_Y)
    };

    ChainShape.CreateChain(Vertices, 3);

    Body = this->world.CreateBody(&BodyDef);
    Body->CreateFixture(&ChainShape, 0.0f);
    this->boundries.push_back(Body);

    ChainShape.Clear();

    //tunelik wychodzacy nad lewy fliper
    
    Vertices[0] = b2Vec2(L_flipper_X,flipper_Y - R-1.0f);  // 1 m zeby sie nie stykaly
    Vertices[1] = b2Vec2(3 * R,flipper_Y - 11 * R);
    Vertices[2] = b2Vec2(3 * R, flipper_Y - 23 * R);

    ChainShape.CreateChain(Vertices, 3);

    Body = this->world.CreateBody(&BodyDef);
    Body->CreateFixture(&ChainShape, 0.0f);
    this->boundries.push_back(Body);

    ChainShape.Clear();

    //Right exit
    Vertices[0] = b2Vec2(R_flipper_X ,flipper_Y + 4 * R);  // 1 m zeby sie nie stykaly
    Vertices[1] = b2Vec2(Window_X - 6 * R, flipper_Y - 4 * R);
    Vertices[2] = b2Vec2(Window_X - 6 * R, Window_Y);

    ChainShape.CreateChain(Vertices, 3);

    Body = this->world.CreateBody(&BodyDef);
    Body->CreateFixture(&ChainShape, 0.0f);
    this->boundries.push_back(Body);

    ChainShape.Clear();

    //tunelik wychodzacy nad prawy fliper
    Vertices[0] = b2Vec2(R_flipper_X , flipper_Y - R - 1.0f);  // 1 m zeby sie nie stykaly
    Vertices[1] = b2Vec2(Window_X - 6 * R, flipper_Y - 11 * R);
    Vertices[2] = b2Vec2(Window_X - 6 * R, flipper_Y - 23 * R);

    ChainShape.CreateChain(Vertices, 3);

    Body = this->world.CreateBody(&BodyDef);
    Body->CreateFixture(&ChainShape, 0.0f);
    this->boundries.push_back(Body);

    ChainShape.Clear();

    //przepraszam za to, co znajdzie sie ponizej
    //lewy bumper
    b2Vec2 BumpVertices[9] = {
        //down left corner
        b2Vec2(7 * R, flipper_Y - 13 * R),
        b2Vec2(7 * R + SQR2 * R / 4, flipper_Y - 13 * R + SQR2 * R / 4),
        b2Vec2(7 * R + R / 2, flipper_Y - 13 * R + R / 2),

        //right corner
        b2Vec2(L_flipper_X - 9 * R, flipper_Y - 9 * R),
        b2Vec2(L_flipper_X - 9 * R + SQR2 * R / 4, flipper_Y - 9 * R + (SQR2 * R / 4 - R / 2)),
        b2Vec2(L_flipper_X - 9 * R + R / 2, flipper_Y - 9 * R - R / 2),

        //top left corner
        b2Vec2(7 * R + R / 2, flipper_Y - 22 * R - R / 2),
        b2Vec2(7 * R + SQR2 * R / 4, flipper_Y - 13 * R - SQR2 * R / 4),
        b2Vec2(7 * R, flipper_Y - 22 * R)
    };

    ChainShape.CreateChain(BumpVertices, 9);

    
    Body = this->world.CreateBody(&BodyDef);
    Body->CreateFixture(&ChainShape, 100.0f);
    this->boundries.push_back(Body);

    ChainShape.Clear();

    //prawy
    b2Vec2 BumpVertices2[9] = {
        //down right corner
        b2Vec2(Window_X - 10 * R, flipper_Y - 13 * R),
        b2Vec2(Window_X - 10 * R - SQR2 * R / 4, flipper_Y - 13 * R + SQR2 * R / 4),
        b2Vec2(Window_X - 10 * R - R / 2, flipper_Y - 13 * R + R / 2),

        //left corner
        b2Vec2(R_flipper_X + 6 * R, flipper_Y - 9 * R),
        b2Vec2(R_flipper_X + 6 * R - SQR2 * R / 4, flipper_Y - 9 * R + (SQR2 * R / 4 - R / 2)),
        b2Vec2(R_flipper_X + 6 * R - R / 2, flipper_Y - 9 * R - R / 2),

        //top Right corner
        b2Vec2(Window_X - 10 * R - R / 2, flipper_Y - 22 * R - R / 2),
        b2Vec2(Window_X - 10 * R - SQR2 * R / 4, flipper_Y - 13 * R - SQR2 * R / 4),
        b2Vec2(Window_X - 10 * R, flipper_Y - 22 * R)
    };

    ChainShape.CreateChain(BumpVertices2, 9);


    Body = this->world.CreateBody(&BodyDef);
    Body->CreateFixture(&ChainShape, 100.0f);
    this->boundries.push_back(Body);
}
void WorldClass::create_boundries(float X, float Y, float R)
{
    R /= this->RATIO;
    X /= this->RATIO;
    Y /= this->RATIO;

    b2PolygonShape BoundBox;
    b2Body* Bound=nullptr;
    b2BodyDef BoundDef;

    BoundDef.type = b2_staticBody;

    //brzegi:
    
    //TOP
    BoundDef.position.Set(0.0f, 0.0f);
    Bound = this->world.CreateBody(&BoundDef);
    BoundBox.SetAsBox(X, 0.f);
    Bound->CreateFixture(&BoundBox, 0.0f);

    this->boundries.push_back(Bound);

    
    //DOWN
    BoundDef.position.Set(X-R, Y);
    Bound = this->world.CreateBody(&BoundDef);
    BoundBox.SetAsBox(2*R, 0.f);
    
    Bound->CreateFixture(&BoundBox, 0.0f);

    this->boundries.push_back(Bound);

    //LEFT
    BoundDef.position.Set(0.0f, 0.0f);
    Bound = this->world.CreateBody(&BoundDef);
    BoundBox.SetAsBox(0.f, Y);
    Bound->CreateFixture(&BoundBox, 0.0f);

    this->boundries.push_back(Bound);

    //RIGHT
    BoundDef.position.Set(X, 0.0f);
    Bound = this->world.CreateBody(&BoundDef);
    //same size
    Bound->CreateFixture(&BoundBox, 0.0f);

    this->boundries.push_back(Bound);

    
    //LEFT WALL OF THE TUNNEL
    BoundDef.position.Set(X-3*R, Y/2+X/4);
    Bound = this->world.CreateBody(&BoundDef);
    BoundBox.SetAsBox(0.f, (Y-X/2)/2);
    Bound->CreateFixture(&BoundBox, 0.0f);

    this->boundries.push_back(Bound);


    //complex boundries
    b2ChainShape ChainShape;
    b2BodyDef ChainDef;
    ChainDef.type = b2_staticBody;

    //PRZPERASZAM ZA TE ABOMINACJE, ALE MUSI BYC C-ARRAY :^)
    b2Vec2 VerticesOut[91];
    b2Vec2 VerticesIn[91];
    b2Vec2 el;
    float RADIUS = X / 2;
    float el_x(0);
    float el_y(0);
    for (int i(0); i <= 90; i++) {
        el_x = RADIUS + RADIUS * cos(i * PI / 180.f);
        el_y = RADIUS - RADIUS * sin(i * PI / 180.f);
        
        el.Set(el_x, el_y);
        VerticesOut[i] = el;
        
        el_x = RADIUS + (RADIUS - 3 * R) * cos(i * PI / 180.f);
        el_y = RADIUS - (RADIUS-3*R) * sin(i * PI / 180.f);
        el.Set(el_x, el_y); //tunnel

        VerticesIn[i] = el;
    }
    ChainShape.CreateChain(VerticesOut, 91);

    Bound = this->world.CreateBody(&ChainDef);
    Bound->CreateFixture(&ChainShape, 0.0f);
    this->boundries.push_back(Bound);
    
    ChainShape.Clear();
    ChainShape.CreateChain(VerticesIn, 91);
    
    Bound = this->world.CreateBody(&ChainDef);
    Bound->CreateFixture(&ChainShape, 0.0f);
    this->boundries.push_back(Bound);

}
void WorldClass::init_ball(float s_X, float s_Y, float R)
{
    s_X /= this->RATIO;
    s_Y /= this->RATIO;
    R /= this->RATIO;
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(s_X, s_Y);
    this-> ball = world.CreateBody(&bodyDef);

    b2CircleShape bodyShape;
    bodyShape.m_radius = (R);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &bodyShape;

    //do recznego dostosowania
    fixtureDef.density = 1.f;
    fixtureDef.friction = 0.1f;
    fixtureDef.restitution = 0.8f;

    this->ball->CreateFixture(&fixtureDef);
}
void WorldClass::init_round_bumper(float X, float Y,float R)
{
    X /= this->RATIO;
    Y /= this->RATIO;
    R /= this->RATIO;

    b2CircleShape bumperShape;
    b2BodyDef bodyDef;
    b2Body* bumper = nullptr;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &bumperShape;

    bodyDef.position.Set(X, Y);
    bodyDef.userData = this;
    bumper = world.CreateBody(&bodyDef);

    //promien bumpera
    bumperShape.m_radius = R;
    bumper->CreateFixture(&fixtureDef);

    bumper->SetUserData(this);
    this->bumpers.push_back(bumper);
}
void WorldClass::world_step()
{
    this->world.Step(timeStep, velocityIterations, positionIterations);
}
void WorldClass::create_flippers(float Left_X, float Right_X, float Y, float R)
{
    Left_X/= this->RATIO;
    Right_X/= this->RATIO;
    Y/= this->RATIO;
    R/= this->RATIO;
    b2PolygonShape RectangleShape;
    b2CircleShape CircleShape;
    CircleShape.m_radius = R;

    b2BodyDef RectangleDef;
    b2BodyDef Sta_CircleDef;
    RectangleDef.type = b2_dynamicBody;
   
    b2Body* Rectangle;
    b2Body* Sta_Circle;

    //LEFT:
    RectangleDef.position.Set(Left_X + (2 * R), Y);
    Rectangle = this->world.CreateBody(&RectangleDef);
    RectangleShape.SetAsBox(3*R, R);
    Rectangle->CreateFixture(&RectangleShape, 30.f);    //gestosc do ustawienia recznego

    

    Sta_CircleDef.position.Set(Left_X, Y);
    Sta_Circle = this->world.CreateBody(&Sta_CircleDef);
    Sta_Circle->CreateFixture(&CircleShape,0.f);
   
    b2RevoluteJointDef jointDef;
    jointDef.enableLimit = true;
    //0.523599f 30
    //0.785398f 45
    jointDef.lowerAngle = -0.523599f;
    jointDef.upperAngle = 0.523599f;
    jointDef.Initialize(Sta_Circle, Rectangle, Sta_Circle->GetPosition());  //laczy dynamiczne obiekty, akotwiczone w srodku kola
    this->LeftFlipper = (b2RevoluteJoint*)this->world.CreateJoint(&jointDef);
    
    //RIGHT:

    RectangleDef.position.Set(Right_X - (2 * R), Y);
    Rectangle = this->world.CreateBody(&RectangleDef);
    RectangleShape.SetAsBox(3 * R, R);
    Rectangle->CreateFixture(&RectangleShape, 30.f);    //gestosc do ustawienia recznego

    Sta_CircleDef.position.Set(Right_X, Y);
    Sta_Circle = this->world.CreateBody(&Sta_CircleDef);
    Sta_Circle->CreateFixture(&CircleShape, 0.f);

    jointDef.enableLimit = true;
    jointDef.lowerAngle = -0.523599f;
    jointDef.upperAngle = 0.523599f;
    jointDef.Initialize(Sta_Circle, Rectangle, Sta_Circle->GetPosition());  //laczy dynamiczne obiekty, akotwiczone w srodku kola
    this->RightFlipper = (b2RevoluteJoint*)this->world.CreateJoint(&jointDef);
}

float WorldClass::GetBodyPositionX(b2Body*body)
{
    return body->GetPosition().x * this->RATIO;
}
float WorldClass::GetBodyPositionX(b2RevoluteJoint*joint, char body)
{
    if (body == 'A' || body == 'a')
        return joint->GetBodyA()->GetPosition().x * this->RATIO;
    else if (body == 'B' || body == 'b')
        return joint->GetBodyB()->GetPosition().x * this->RATIO;
}
float WorldClass::GetBodyPositionY(b2Body*body)
{
    return body->GetPosition().y * this->RATIO;
}
float WorldClass::GetBodyPositionY(b2RevoluteJoint* joint, char body)
{
    if (body == 'A' || body == 'a')
        return joint->GetBodyA()->GetPosition().y * this->RATIO;
    else if (body == 'B' || body == 'b')
        return joint->GetBodyB()->GetPosition().y * this->RATIO;
}
b2World* WorldClass::GetWorldPtr()
{
    return this->world_ptr;
}