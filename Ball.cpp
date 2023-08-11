#include "Ball.hpp"

Ball::Ball(float x, float y, float radius, float gravity, float bounce, sf::Color color)
    : x(x), y(y), radius(radius), gravity(gravity), bounce(bounce), isHoldingBall(false), velocity(0, 0) {
    shape.setRadius(radius);
    shape.setFillColor(color);
    shape.setPosition(x, y);
}

void Ball::setPosition(float x, float y) {
    this->x = x;
    this->y = y;
    shape.setPosition(x, y);
}

void Ball::setVelocity(float vx, float vy) {
    velocity.x = vx;
    velocity.y = vy;
}

void Ball::setIsHoldingBall(bool holding) {
    isHoldingBall = holding;
}

void Ball::calculateVelocity(const std::vector<sf::RectangleShape> &obstacles, const sf::RenderWindow &window) {
	if (isHoldingBall) return;

	velocity.y += gravity;

	// Gestion des bords de la fenêtre
	if (y >= window.getSize().y - shape.getRadius()) {
		y = window.getSize().y - shape.getRadius();
		velocity.y *= -bounce;
	}

	if (y <= shape.getRadius()) {
		y = shape.getRadius();
		velocity.y *= -bounce;
	}

	if (x >= window.getSize().x - shape.getRadius()) {
		x = window.getSize().x - shape.getRadius();
		velocity.x *= -bounce;
	}

	if (x <= shape.getRadius()) {
		x = shape.getRadius();
		velocity.x *= -bounce;
	}

	// Vérification de la collision avec chaque obstacle
	for (const auto& obstacle : obstacles)
	{
		float angle = obstacle.getRotation() * 3.14159265f / 180.f; // Conversion en radians
		float nx = std::sin(angle); // Composante x de la normale
		float ny = -std::cos(angle); // Composante y de la normale

		float dx = x - obstacle.getPosition().x;
		float dy = y - obstacle.getPosition().y;
		float t = dx * std::cos(angle) + dy * std::sin(angle);

		// Vérification de la collision
		float d = dx * nx + dy * ny;
		if (std::abs(d) <= shape.getRadius() && t >= 0 && t <= obstacle.getSize().x)
		{
			// Calcul de la réflexion basée sur la normale
			float dot = velocity.x * nx + velocity.y * ny;
			if (dot < 0) {
				velocity.x -= 2 * dot * nx * bounce;
				velocity.y -= 2 * dot * ny * bounce;
			}
		}
	}
}

void Ball::update(float dt) {
    velocity.y += gravity * dt;
    x += velocity.x * dt;
    y += velocity.y * dt;

    setPosition(x, y);
}

void Ball::draw(sf::RenderWindow &window) {
    window.draw(shape);
}

float Ball::getX() const {
    return x;
}

float Ball::getY() const {
    return y;
}

float Ball::getRadius() const {
    return radius;
}

bool Ball::getIsHoldingBall() const {
    return isHoldingBall;
}

