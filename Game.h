#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <vector>
#include <SFML/Audio.hpp>

class Game
{
private:

	//window
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	//Game objects
	sf::CircleShape Ball;
		//Flippers:
	sf::CircleShape LeftFlipperCircle;
	sf::CircleShape RightFlipperCircle;
	sf::RectangleShape LeftFlipperRectangle;
	sf::RectangleShape RightFlipperRectangle;

	//sound
	sf::SoundBuffer playground_buf;
	sf::SoundBuffer flipper_sound_buf;
	sf::SoundBuffer bumper_sound_buf;

	sf::Sound playground;
	sf::Sound flipper_sound;

	//private methods
	void initVariables();
	void initWindow();
	void GameOver();
	void restart();

	//SFLM
	void initRoundBumpers();
	void initBall();
	void updateBall();
	void initFlippers();
	void updateFlippers();
	void initText();
	void updateText();

	void initMusic();

	//High score
	unsigned read_highscore();
	void write_highscore(unsigned hs);

	sf::Sprite BallSprite;
	sf::Texture texture;

	sf::Sprite Background;
	sf::Texture Background_texture;

	sf::Sprite Sidebar;
	sf::Texture Sidebar_texture;

	sf::Sprite Gover;
	sf::Texture Gover_texture;

	sf::Sprite Pause;
	sf::Texture Pause_texture;

public:
	//construct
	Game();
	//destruct
	virtual ~Game();

	//Accessors
	const bool running() const;
	//methods:
	void pollEvents();
	void update();
	void render();

	static void begin_ball_collision(b2Body* bodyA, b2Body* bodyB);
	static void end_ball_collision(b2Body* bodyA, b2Body* bodyB);
};

