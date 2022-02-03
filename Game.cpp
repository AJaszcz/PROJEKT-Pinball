#include "Game.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include "2dWorld.h"
#include <string>
#include <fstream>

constexpr auto PI = 3.14159265359;
constexpr auto SQR2 = 1.41421356237;

const unsigned WINDOW_HEIGHT = 920;
const unsigned WINDOW_LENGTH = 720;

//ball parameters
float BALL_RADIUS = 11.f;
unsigned starting_x = WINDOW_LENGTH - BALL_RADIUS;
unsigned starting_y = WINDOW_HEIGHT - BALL_RADIUS;

float RATIO=10.f;
WorldClass WORLD(10.f); //tworzenie obiektu klasy WorldClass w zadanych proporcjach (10pixeli-1metr)

float left_flip_pos_x;
float left_flip_pos_y;
float right_flip_pos_x;
float right_flip_pos_y;

//hidden game objects
sf::Texture bumper_texture;
sf::Texture bumper_hit_texture;
std::vector<sf::Sprite> Bumpers;
sf::Font font;
sf::Text lifes_text;
sf::Text score_text;
sf::Text high_score;
std::fstream plik;
sf::Sound bumper_sound;

unsigned points = 0;
int lifes = 3;
bool gameover = false;
bool pause = false;
bool spring = false;
float volume = 50.f;

Game::Game()
{
    Game* game_wsk = this;
    WORLD.create_boundries(WINDOW_LENGTH, WINDOW_HEIGHT, BALL_RADIUS);

    this->initVariables();
    this->initWindow();
    this->initBall();
    this->initFlippers();
    this->initRoundBumpers();
    this->initText();
    this->initMusic();

    //loading textures
    this->Background_texture.loadFromFile("Textures/Background.png");
    this->Background.setTexture(Background_texture, true);

    this->Sidebar_texture.loadFromFile("Textures/Sidebar.png");
    this->Sidebar.setTexture(Sidebar_texture, true);
    this->Sidebar.setPosition(WINDOW_LENGTH, 0.f);

    this->Gover_texture.loadFromFile("Textures/GameOver.png");
    this->Gover.setTexture(Gover_texture, true);

    this->Pause_texture.loadFromFile("Textures/Pause.png");
    this->Pause.setTexture(Pause_texture, true);


}

Game::~Game()
{
    delete this->window;
}
  
//private methods:
void Game::initVariables() {
    this->window = nullptr;
}
void Game::initWindow() {
    this->videoMode.width = WINDOW_LENGTH+360.f;
    this->videoMode.height = WINDOW_HEIGHT;

    this->window = new sf::RenderWindow(this->videoMode, "Arcane Pinball", sf::Style::Titlebar | sf::Style::Close );
    this->window->setFramerateLimit(144);
}
void Game::GameOver()
{
    WORLD.destroyBody(WORLD.ball);
    gameover = true;
}
void Game::restart()
{
    gameover = false;
    lifes = 3;
    this->initBall();
    if (points > this->read_highscore()) {
        //new highscore
        write_highscore(points);
        high_score.setString(std::to_string(this->read_highscore()));
        high_score.setPosition(WINDOW_LENGTH + 180.f - (high_score.getLocalBounds().width / 2.f), WINDOW_HEIGHT - 225.f);
    }
    points = 0;
}
void Game::initRoundBumpers()
{
    bumper_texture.loadFromFile("Textures/bumper.png");
    bumper_hit_texture.loadFromFile("Textures/bumper_hit.png");
    sf::Sprite temp_sprite;
    temp_sprite.setTexture(bumper_texture, true);

    WORLD.init_round_bumper(55.f, 55.f, 3*BALL_RADIUS);
    temp_sprite.setPosition(55.f - 3 * BALL_RADIUS, 55.f - 3 * BALL_RADIUS);
    Bumpers.push_back(temp_sprite);

    WORLD.init_round_bumper(200.f, 95.f, 3 * BALL_RADIUS);
    temp_sprite.setPosition(200.f - 3 * BALL_RADIUS, 95.f - 3 * BALL_RADIUS);
    Bumpers.push_back(temp_sprite);

    WORLD.init_round_bumper(88.f, 160.f, 3 * BALL_RADIUS);
    temp_sprite.setPosition(88.f - 3 * BALL_RADIUS, 160.f - 3 * BALL_RADIUS);
    Bumpers.push_back(temp_sprite);

    WORLD.init_round_bumper(445.f, 190.f, 3 * BALL_RADIUS);
    temp_sprite.setPosition(445.f - 3 * BALL_RADIUS, 190.f - 3 * BALL_RADIUS);
    Bumpers.push_back(temp_sprite);

    WORLD.init_round_bumper(580.f, 280.f, 3 * BALL_RADIUS);
    temp_sprite.setPosition(580.f - 3 * BALL_RADIUS, 280.f - 3 * BALL_RADIUS);
    Bumpers.push_back(temp_sprite);

    WORLD.init_round_bumper(325.f, 260.f, 3 * BALL_RADIUS);
    temp_sprite.setPosition(325.f - 3 * BALL_RADIUS, 260.f - 3 * BALL_RADIUS);
    Bumpers.push_back(temp_sprite);

    WORLD.init_round_bumper(445.f, 325.f, 3 * BALL_RADIUS);
    temp_sprite.setPosition(445.f - 3 * BALL_RADIUS, 325.f - 3 * BALL_RADIUS);
    Bumpers.push_back(temp_sprite);

    WORLD.init_round_bumper(560.f, 410.f, 3 * BALL_RADIUS);
    temp_sprite.setPosition(560.f - 3 * BALL_RADIUS, 410.f - 3 * BALL_RADIUS);
    Bumpers.push_back(temp_sprite);

}
void Game::initBall() {

    WORLD.init_ball(starting_x, starting_y, BALL_RADIUS);

    this->Ball.setRadius(BALL_RADIUS);
    this->Ball.setFillColor(sf::Color(0, 0, 255, 255));
    this->Ball.setPosition(WORLD.GetBodyPositionX(WORLD.ball) - BALL_RADIUS, WORLD.GetBodyPositionY(WORLD.ball) - BALL_RADIUS);


    this->texture.loadFromFile("Textures/BallSmall.png");
    this->BallSprite.setTexture(texture, true);
    this->BallSprite.setPosition(WORLD.GetBodyPositionX(WORLD.ball) - BALL_RADIUS, WORLD.GetBodyPositionY(WORLD.ball) - BALL_RADIUS);
}
void Game::updateBall()
{
    if (WORLD.GetBodyPositionY(WORLD.ball) > WINDOW_HEIGHT) {
        lifes--;
        if (lifes > 0) {
            WORLD.ReinitBall(starting_x, starting_y, BALL_RADIUS);
        }
        else
            this->GameOver();
        
    }
    else{

    this->BallSprite.setPosition(WORLD.GetBodyPositionX(WORLD.ball) - BALL_RADIUS* SQR2 *cos(WORLD.ball->GetAngle()+PI/4), WORLD.GetBodyPositionY(WORLD.ball) - BALL_RADIUS * SQR2 * sin(WORLD.ball->GetAngle() + PI / 4));
    this->Ball.setPosition(WORLD.GetBodyPositionX(WORLD.ball) - BALL_RADIUS, WORLD.GetBodyPositionY(WORLD.ball) - BALL_RADIUS);

    this->BallSprite.setRotation(WORLD.ball->GetAngle() * 57.2958f);
    }

     
}
void Game::initFlippers() {
    
    WORLD.create_flippers(WINDOW_LENGTH / 2 - 8 * BALL_RADIUS, WINDOW_LENGTH / 2 + 8 * BALL_RADIUS, WINDOW_HEIGHT * 0.9f, BALL_RADIUS);
    WORLD.create_board(WINDOW_LENGTH,WINDOW_HEIGHT,WINDOW_LENGTH / 2 - 8 * BALL_RADIUS, WINDOW_LENGTH / 2 + 8 * BALL_RADIUS, WINDOW_HEIGHT * 0.9f, BALL_RADIUS);

    //LEFT:

    this->LeftFlipperCircle.setRadius(BALL_RADIUS);
    this->LeftFlipperCircle.setFillColor(sf::Color(19, 180, 16, 255));
    this->LeftFlipperCircle.setPosition(WORLD.GetBodyPositionX(WORLD.LeftFlipper, 'A')-BALL_RADIUS, WORLD.GetBodyPositionY(WORLD.LeftFlipper, 'A') - BALL_RADIUS);

    this->LeftFlipperRectangle.setSize(sf::Vector2f(6 * BALL_RADIUS, 2 * BALL_RADIUS));
    this->LeftFlipperRectangle.setFillColor(sf::Color(19, 180, 16, 255));
    this->LeftFlipperRectangle.setPosition(WORLD.GetBodyPositionX(WORLD.LeftFlipper, 'B') - 2*BALL_RADIUS, WORLD.GetBodyPositionY(WORLD.LeftFlipper, 'B') - BALL_RADIUS);

    left_flip_pos_x = LeftFlipperRectangle.getPosition().x;
    left_flip_pos_y = LeftFlipperRectangle.getPosition().y;

    //RIGHT:
    this->RightFlipperCircle.setRadius(BALL_RADIUS);
    this->RightFlipperCircle.setFillColor(sf::Color(19, 180, 16, 255));
    this->RightFlipperCircle.setPosition(WORLD.GetBodyPositionX(WORLD.RightFlipper, 'A') - BALL_RADIUS, WORLD.GetBodyPositionY(WORLD.RightFlipper, 'A') - BALL_RADIUS);

    this->RightFlipperRectangle.setSize(sf::Vector2f(-6 * BALL_RADIUS, 2 * BALL_RADIUS));
    this->RightFlipperRectangle.setFillColor(sf::Color(19, 180, 16, 255));
    this->RightFlipperRectangle.setPosition(WORLD.GetBodyPositionX(WORLD.RightFlipper, 'B') + 2 * BALL_RADIUS, WORLD.GetBodyPositionY(WORLD.RightFlipper, 'B') - BALL_RADIUS);

    right_flip_pos_x = RightFlipperRectangle.getPosition().x;
    right_flip_pos_y = RightFlipperRectangle.getPosition().y;
}
void Game::updateFlippers() {
    //left

    this->LeftFlipperCircle.setPosition(WORLD.GetBodyPositionX(WORLD.LeftFlipper, 'A') - BALL_RADIUS, WORLD.GetBodyPositionY(WORLD.LeftFlipper, 'A') - BALL_RADIUS);
    
    this->LeftFlipperRectangle.setPosition(left_flip_pos_x + sin(WORLD.LeftFlipper->GetJointAngle()) * BALL_RADIUS, left_flip_pos_y + cos(WORLD.LeftFlipper->GetJointAngle()) * -BALL_RADIUS + BALL_RADIUS);
    this->LeftFlipperRectangle.setRotation(WORLD.LeftFlipper->GetJointAngle() * 57.2958f);

    //right
    this->RightFlipperCircle.setPosition(WORLD.GetBodyPositionX(WORLD.RightFlipper, 'A') - BALL_RADIUS, WORLD.GetBodyPositionY(WORLD.RightFlipper, 'A') - BALL_RADIUS);

    this->RightFlipperRectangle.setPosition(right_flip_pos_x + sin(WORLD.RightFlipper->GetJointAngle()) * BALL_RADIUS, right_flip_pos_y + cos(WORLD.RightFlipper->GetJointAngle()) * -BALL_RADIUS + BALL_RADIUS);
    this->RightFlipperRectangle.setRotation(WORLD.RightFlipper->GetJointAngle() * 57.2958f);
}
void Game::initText()
{
    font.loadFromFile("Text/Font.otf");
    lifes_text.setFont(font);
    lifes_text.setString(std::to_string(lifes));
    lifes_text.setCharacterSize(48);
    lifes_text.setFillColor(sf::Color(213, 173, 83, 255));
    lifes_text.setPosition(WINDOW_LENGTH+180.f - (lifes_text.getLocalBounds().width / 2.f), 490.f);


    score_text.setFont(font);
    score_text.setString(std::to_string(points));
    score_text.setCharacterSize(48);
    score_text.setFillColor(sf::Color(213,173,83,255));
    score_text.setPosition(WINDOW_LENGTH+180.f - (score_text.getLocalBounds().width / 2.f), 280.f);

    high_score.setFont(font);
    high_score.setCharacterSize(24);
    high_score.setFillColor(sf::Color(213, 173, 83, 255));
    high_score.setString(std::to_string(this->read_highscore()));
    high_score.setPosition(WINDOW_LENGTH + 180.f - (high_score.getLocalBounds().width / 2.f), WINDOW_HEIGHT-225.f);

}
void Game::updateText()
{
    lifes_text.setString(std::to_string(lifes));
    score_text.setString(std::to_string(points));
    lifes_text.setPosition(WINDOW_LENGTH + 180.f - (lifes_text.getLocalBounds().width / 2.f), 490.f);
    score_text.setPosition(WINDOW_LENGTH + 180.f - (score_text.getLocalBounds().width/ 2.f), 280.f);
}
void Game::initMusic()
{
    this->playground_buf.loadFromFile("Sound/Playground.wav");
    this->playground.setBuffer(this->playground_buf);
    this->playground.setLoop(true);
    this->playground.setVolume(volume);
    this->playground.play();

    this->flipper_sound_buf.loadFromFile("Sound/flipper.wav");
    this->flipper_sound.setBuffer(flipper_sound_buf);
    this->flipper_sound.setVolume(volume);

    this->bumper_sound_buf.loadFromFile("Sound/bumper.wav");
    bumper_sound.setBuffer(bumper_sound_buf);
    bumper_sound.setVolume(volume);
}
unsigned Game::read_highscore()
{
    std::string re;
    plik.open("Text/highscore.txt", std::ios_base::in | std::ios_base::binary);
    if (!plik.good()) {
        plik.close();
        this->write_highscore(0);
        plik.open("Text/highscore.txt", std::ios_base::in | std::ios_base::binary);
    }
    if (plik.is_open()) {
        if (!plik.eof())
            plik >> re;
    }
    plik.close();
    return(std::stoul(re));
}
void Game::write_highscore(unsigned hs)
{
    plik.open("Text/highscore.txt", std::ios_base::out | std::ios_base::binary);
    if (plik.is_open()) {
        plik << hs;
    }
    plik.close();
}
const bool Game::running() const
{
    return this->window->isOpen();
}
void Game::pollEvents()
{
    float energy(2.f);

    //Event polling
    while (this->window->pollEvent(this->ev)) {
        switch (this->ev.type) {
            //zamykanie okna
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if (this->ev.key.code == sf::Keyboard::Escape) {
                this->window->close();
                this->playground.stop();
            }
            if (this->ev.key.code == sf::Keyboard::Left) {
                WORLD.LeftFlipper->GetBodyB()->ApplyAngularImpulse(-60000.f, true);
                this->flipper_sound.play();
            }
            if (this->ev.key.code == sf::Keyboard::Right) {
                WORLD.RightFlipper->GetBodyB()->ApplyAngularImpulse(60000.f, true);
                this->flipper_sound.play();
            }
            if (this->ev.key.code == sf::Keyboard::Space) {
                //gdy pilka znajduje sie na dole tunelu
                if (WORLD.GetBodyPositionY(WORLD.ball) >= WINDOW_HEIGHT - 2 * BALL_RADIUS && WORLD.GetBodyPositionX(WORLD.ball) >= WINDOW_LENGTH - 2 * BALL_RADIUS &&!spring) {
                    spring = true;
                    while (spring) {
                        this->window->pollEvent(this->ev);
                        if (ev.type == sf::Event::KeyReleased) {
                            if (this->ev.key.code == sf::Keyboard::Space);
                                spring = false;
                        }
                        if (energy < 15000.f)
                            energy += 0.1f;
                        else
                            energy = 15000.f;
                    }
                    WORLD.ball->ApplyForceToCenter(b2Vec2(0.f, -1 *energy), true);
                }
            }
            //if restart needed
            if (this->ev.key.code == sf::Keyboard::F3 && gameover) {
                this->restart();
            }
            if (this->ev.key.code == sf::Keyboard::P) {
                if (pause) {
                    //resume
                    this->playground.play();
                    pause = false;
                }
                else {
                    //pause
                    this->playground.pause();
                    pause = true;
                }
            }
            break;
        }
    }

}
void Game::update()
{
    while(gameover)
        this->pollEvents();
    this->pollEvents();

    //jesli gra nadal dziala (okno jest otwarte)
    if (this->running()) {
        WORLD.world_step();
        this->updateFlippers();
        this->updateBall();
        this->updateText();
        while (pause) {
            this->pollEvents();
            this->render();
        }
    }
}
void Game::render()
{
    //if game is still running
    if (this->running()) {

        this->window->clear();
        //lower layer
        this->window->draw(this->Background);
        this->window->draw(this->Sidebar);
        this->window->draw(this->LeftFlipperCircle);
        this->window->draw(this->LeftFlipperRectangle);
        this->window->draw(this->RightFlipperCircle);
        this->window->draw(this->RightFlipperRectangle);
        for (auto i : Bumpers)
            this->window->draw(i);
        this->window->draw(this->Ball);
        this->window->draw(this->BallSprite);
        //upper layer
        this->window->draw(lifes_text);
        this->window->draw(score_text);
        this->window->draw(high_score);

        if (gameover)
            this->window->draw(this->Gover);
        if (pause)
            this->window->draw(this->Pause);
        this->window->display();
    }
}
void Game::begin_ball_collision(b2Body* bodyA, b2Body* bodyB)
{
    if (WORLD.ball == bodyA){
        for (int i(0); i < WORLD.bumpers.size();i++) {
            if (bodyB == WORLD.bumpers[i]) {
                points += 10;
                Bumpers[i].setTexture(bumper_hit_texture);
                bumper_sound.play();
            }
        }
    }
}
void Game::end_ball_collision(b2Body* bodyA, b2Body* bodyB)
{
    b2Body* ptr = nullptr;
    if (WORLD.ball == bodyA) {
        for (int i(0); i < WORLD.bumpers.size(); i++) {
            if (bodyB == WORLD.bumpers[i]) {
                Bumpers[i].setTexture(bumper_texture);
            }
        }
    }
}
