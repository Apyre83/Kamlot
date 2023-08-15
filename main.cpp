#include "Kamlot.h"

std::vector<sf::RectangleShape> parseConfFile(const std::string& file) {
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

void initializeGame(sf::RenderWindow& window, std::vector<sf::RectangleShape>& obstacles, std::vector<Ball>& balls, std::mt19937& gen) {
    obstacles = parseConfFile("conf");
    float x = window.getSize().x / 2.0f;
    float y = window.getSize().y / 2.0f;

    std::uniform_int_distribution<> distrib(0, 1920);
    balls.clear();
    for (int i = 0; i < 50; i++) {
        float pos = static_cast<float>(distrib(gen));
        Ball ball(pos, y / 2, 20.f, GRAVITY, BOUNCE, sf::Color(4 * i, 255 - 4 * i, 0));
        ball.setOutline(2, sf::Color::White);
        balls.push_back(ball);
    }
}

int main(void) {
    std::random_device rd;
    std::mt19937 gen(rd());

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "Kamlot", sf::Style::Fullscreen);

    std::vector<sf::RectangleShape> obstacles;
    std::vector<Ball> balls;

    initializeGame(window, obstacles, balls, gen);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                initializeGame(window, obstacles, balls, gen);
            }
        }

        float dt = clock.restart().asSeconds();

        for (auto& ball : balls) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                double distance = std::sqrt(std::pow(ball.getX() - mousePos.x, 2) + std::pow(ball.getY() - mousePos.y, 2));
                if (distance <= ball.getRadius() && !ball.getIsHoldingBall()) {
                    ball.setIsHoldingBall(true);
                }
            }
            else {
                ball.setIsHoldingBall(false);
            }

            if (ball.getIsHoldingBall()) // Si l'utilisateur tient la balle
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                float x = static_cast<float>(mousePos.x);
                float y = static_cast<float>(mousePos.y);
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
        for (auto& ball : balls)
            ball.draw(window);
        for (const auto& obstacle : obstacles) {
            window.draw(obstacle);
        }

        window.display();
    }

    return 0;
}
