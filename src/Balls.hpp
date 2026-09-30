#pragma once

#include <vector>
#include <random>
#include <cmath>


#include <SFML/Graphics.hpp>

#include "SpatialHash.h"
#include "Ball.hpp"

class BallGame {
private:
    SpatialHash spatialHash;
    // Create balls
    std::vector<Ball> balls;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> posDist;
    std::uniform_real_distribution<float> velDist;
    std::uniform_int_distribution<int> colorDist;
    std::uniform_real_distribution<float> radiusDist;
    sf::Vector2u windowSize;

public:
    BallGame(sf::Vector2u windowSize) : spatialHash(windowSize,50.0f) {
        gen = std::mt19937(rd());
        posDist = std::uniform_real_distribution<float>(5.0f, 795.0f);
        velDist = std::uniform_real_distribution<float>(-200.0f, 200.0f);
        colorDist = std::uniform_int_distribution<int>(0, 255);
        radiusDist = std::uniform_real_distribution<float>(2.5f, 2.5f);
        this->windowSize = windowSize;

        // Generate random balls
        for (int i = 0; i < 2500; ++i) {
            sf::Color randomColor(colorDist(gen), colorDist(gen), colorDist(gen));
            balls.emplace_back(
                posDist(gen), posDist(gen), // position
                radiusDist(gen), // radius
                randomColor, // color
                velDist(gen), velDist(gen) // velocity
            );
        }
    }

    void updateBalls(float deltaTime) {

        spatialHash.clearHash();

        for (auto& ball : balls) {
            ball.shape.move(ball.velocity * deltaTime);
            spatialHash.insertObject(&ball);
        }

        spatialHash.collisionHandeling();
        spatialHash.wallCollisionHandeling();



    }

    void drawBalls( sf::RenderWindow& window ) const
    {
        for (const auto& ball : balls) {
            window.draw(ball.shape);
        }
    }
};