#pragma once

#include <vector>
#include <random>
#include <cmath>


#include <SFML/Graphics.hpp>

#include "SpatialHash.h"
#include "Ball.hpp"
#include "ThreadPool.h"

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
    ThreadPool pool;

public:
    BallGame(sf::Vector2u windowSize) : spatialHash(windowSize,50.0f), pool(4) {
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



    void updateBalls(float deltaTime)
    {
        const size_t ballCount = balls.size();

        if (ballCount == 0)
            return;

        const size_t chunkSize = (ballCount + 3) / 4;

        for (size_t i = 0; i < ballCount; i += chunkSize)
        {
            const size_t start = i;
            const size_t end = std::min(i + chunkSize, ballCount);

            pool.enqueue([this, start, end, deltaTime]()
            {
                for (size_t j = start; j < end; ++j)
                {
                    balls[j].shape.move(
                        balls[j].velocity * deltaTime
                    );
                }
            });
        }

        pool.waitUntilFinished();

        spatialHash.clearHash();

        for (auto& ball : balls)
        {
            spatialHash.insertObject(&ball);
        }

        spatialHash.collisionHandeling(pool);

        spatialHash.wallCollisionHandeling();
    }

    void drawBalls( sf::RenderWindow& window ) const
    {
        for (const auto& ball : balls) {
            window.draw(ball.shape);
        }
    }
};