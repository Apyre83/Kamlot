
#ifndef BALL_HPP
#define BALL_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

class Ball {
private:
    float x, y;
    float radius;
    float gravity;
    float bounce;
    bool isHoldingBall;
    sf::Vector2f velocity;
    sf::CircleShape shape;

public:
    Ball(float x, float y, float radius, float gravity, float bounce, sf::Color color);

    void setPosition(float x, float y);
    void setVelocity(float vx, float vy);
    void setIsHoldingBall(bool holding);
    void calculateVelocity(const std::vector<sf::RectangleShape> &obstacles, const sf::RenderWindow &window);
    void update(float dt);
    void draw(sf::RenderWindow &window);

    float getX() const;
    float getY() const;
    float getRadius() const;
    bool getIsHoldingBall() const;
};

#endif // BALL_HPP

