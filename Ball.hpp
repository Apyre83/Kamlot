#pragma once

#include "Kamlot.h"

class Ball {
private:
    sf::CircleShape shape;
	float x, y;
    float vx, vy;
	float gravity, bounce;
	bool isHoldingBall;

public:
    Ball(float x, float y, float radius, float gravity, float bounce, sf::Color color)
        : x(0), y(0), vx(0), vy(0), gravity(gravity), bounce(bounce), isHoldingBall(false)
	{
        shape.setRadius(radius);
        shape.setPosition(x - radius, y - radius);
        shape.setFillColor(color);
    }

    void setPosition(float x, float y) {
        shape.setPosition(x - shape.getRadius(), y - shape.getRadius());
    }

    void setVelocity(float vx, float vy) {
        this->vx = vx;
        this->vy = vy;
    }

	bool getIsHoldingBall() const { return isHoldingBall; }
    void setIsHoldingBall(bool holding) { isHoldingBall = holding; }
    float getX() const { return shape.getPosition().x + shape.getRadius(); }
    float getY() const { return shape.getPosition().y + shape.getRadius(); }
    float getRadius() const { return shape.getRadius(); }

    void update(float dt) {
		x += vx * dt;
		y += vy * dt;
		shape.setPosition(x - shape.getRadius(), y - shape.getRadius());
    }

    void draw(sf::RenderWindow &window) const {
        window.draw(shape);
    }

    // Autres méthodes utiles (comme la gestion des collisions) peuvent être ajoutées ici
	void	calculateVelocity(std::vector<sf::RectangleShape> obstacles, sf::RenderWindow &window)
	{
            vy += gravity;

            // Gestion des bords de la fenêtre
            if (y >= window.getSize().y - shape.getRadius()) {
                y = window.getSize().y - shape.getRadius();
                vy *= -bounce;
            }

            if (y <= shape.getRadius()) {
                y = shape.getRadius();
                vy *= -bounce;
            }

            if (x >= window.getSize().x - shape.getRadius()) {
                x = window.getSize().x - shape.getRadius();
                vx *= -bounce;
            }

            if (x <= shape.getRadius()) {
                x = shape.getRadius();
                vx *= -bounce;
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
					float dot = vx * nx + vy * ny;
					if (dot < 0) {
						vx -= 2 * dot * nx * bounce;
						vy -= 2 * dot * ny * bounce;
					}
				}
            }
	}
};

