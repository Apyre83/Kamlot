#include "Kamlot.h"

std::vector<sf::RectangleShape> parseConfFile(const std::string &file) {
    std::ifstream confFile(file);
    std::string line;
    std::vector<sf::RectangleShape> rectangles;

    if (!confFile) {
        std::cout << "Unable to open file" << std::endl;
        exit(1);
    }

    while (getline(confFile, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "rectangle") {
            float x, y, width, height, rotation;
            int r, g, b;
            iss >> x >> y >> width >> height >> rotation >> r >> g >> b;

			std::cout << "x: " << x << " y: " << y << " width: " << width << " height: " << height << " rotation: " << rotation << " r: " << r << " g: " << g << " b: " << b << std::endl;

            sf::RectangleShape rectangle(sf::Vector2f(width, height));
            rectangle.setPosition(x, y);
            rectangle.setRotation(rotation);
            rectangle.setFillColor(sf::Color(r, g, b));

            rectangles.push_back(rectangle);
        }
    }
    return rectangles;
}
int main(void)
{
    std::vector<sf::RectangleShape> obstacles = parseConfFile("conf");

    sf::RenderWindow window(sf::VideoMode(800, 600), "Kamlot");

    float x = window.getSize().x / 2;
    float y = window.getSize().y / 2;
    const float gravity = 2.0f;
    const float bounce = 0.8f;

	std::vector<Ball> balls;
	for (int i = 0; i < 50; i++)
		balls.push_back(Ball(x - 200 + i * 4, y, 20.f, gravity, bounce, sf::Color(4 * i, 255 - 4 * i, 0)));



    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds();

		for (auto &ball : balls) {

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				float distance = std::sqrt(std::pow(ball.getX() - mousePos.x, 2) + std::pow(ball.getY() - mousePos.y, 2));
				if (distance <= ball.getRadius() && !ball.getIsHoldingBall()) {
					ball.setIsHoldingBall(true);
				}
			} else {
				ball.setIsHoldingBall(false);
			}

			if (ball.getIsHoldingBall()) // Si l'utilisateur tient la balle
			{
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				x = static_cast<float>(mousePos.x);
				y = static_cast<float>(mousePos.y);
				ball.setVelocity(0, 0);
				ball.setPosition(x, y);
			}
			else
			{
				ball.calculateVelocity(obstacles, window);
				ball.update(dt);
			}
		}

        window.clear();
		for (auto &ball : balls)
			ball.draw(window);
        for (const auto &obstacle : obstacles) {
            window.draw(obstacle);
        }

        window.display();
    }

    return 0;
}

