#include <SFML/Graphics.hpp>
#include <cmath>
#include <sstream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Balle rebondissante avec obstacle incliné");
    sf::CircleShape shape(20.f);
    shape.setFillColor(sf::Color::Green);

    sf::RectangleShape obstacle1(sf::Vector2f(200, 10));
    obstacle1.setPosition(300, 300);
    obstacle1.setRotation(45.f); // Incline l'obstacle de 45 degrés
    obstacle1.setFillColor(sf::Color(255, 0, 255));

    sf::RectangleShape obstacle2(sf::Vector2f(200, 10));
    obstacle2.setPosition(500, 400);
    obstacle2.setRotation(-45.f); // Incline l'obstacle de 45 degrés
    obstacle2.setFillColor(sf::Color(255, 255, 0));

    sf::RectangleShape obstacles[2] = { obstacle1, obstacle2 };

    float x = window.getSize().x / 2;
    float y = window.getSize().y / 2;
    float vx = 400;
    float vy = -400;

    const float gravity = 0.5f;
    const float bounce = 0.7f;

    bool isHoldingBall = false; // Indique si l'utilisateur tient la balle


    //sf::Font font;
    //if (!font.loadFromFile("arial.ttf")) // Assurez-vous d'avoir ce fichier dans votre répertoire
    //    return -1;

    //sf::Text fpsText;
    //fpsText.setFont(font);
    //fpsText.setCharacterSize(14);
    //fpsText.setFillColor(sf::Color::White);
    //fpsText.setPosition(10, 10);

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds();

        // Calculez les FPS en utilisant le delta de temps
        //float fps = 1.f / dt;
        //std::stringstream ss;
        //ss << "FPS: " << fps;
        //fpsText.setString(ss.str());

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            float distance = std::sqrt(std::pow(x - mousePos.x, 2) + std::pow(y - mousePos.y, 2));
            if (distance <= shape.getRadius() && !isHoldingBall)
            {
                isHoldingBall = true; // Attraper la balle
            }
        }
        else
        {
            isHoldingBall = false; // Relâcher la balle
        }

        if (isHoldingBall) // Si l'utilisateur tient la balle
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            x = static_cast<float>(mousePos.x);
            y = static_cast<float>(mousePos.y);
            vx = 0;
            vy = 0;
        }

        else
        {
            vy += gravity;

            x += vx * dt;
            y += vy * dt;

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

                // Projection de la position de la balle sur la ligne de l'obstacle
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

        shape.setPosition(x - shape.getRadius(), y - shape.getRadius());

        window.clear();
        window.draw(shape);
        for (const auto& obstacle : obstacles)
            window.draw(obstacle);

        // Dessinez le texte FPS
        //window.draw(fpsText);

        window.display();
    }

    return 0;
}
