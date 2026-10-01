

#include "SpatialHash.h"

#include <algorithm>
#include <cmath>
#include <iostream>

SpatialHash::SpatialHash(sf::Vector2u window, float CellSize){
    windowSize = window;
    cellSize = CellSize;
}

void SpatialHash::insertObject(Ball* ball) {
    sf::Vector2f pos = ball->shape.getPosition();

    int cell_x = pos.x / cellSize;
    int cell_y = pos.y / cellSize;

    std::pair<int, int> key(cell_x, cell_y);
    grid[key].push_back(ball);

}

void SpatialHash::clearHash() {
    grid.clear();
}


void SpatialHash::collisionHandeling(ThreadPool& pool)
{
    using CellKey = std::pair<int, int>;
    using CollisionPair = std::pair<Ball*, Ball*>;

    std::vector<CellKey> cellKeys;
    cellKeys.reserve(grid.size());

    for (const auto& [cellKey, bucket] : grid)
    {
        cellKeys.push_back(cellKey);
    }

    if (cellKeys.empty())
        return;

    const size_t chunkSize = 32;

    const size_t numChunks =
        (cellKeys.size() + chunkSize - 1) / chunkSize;

    std::vector<std::vector<CollisionPair>> results(numChunks);

    const auto& readOnlyGrid = grid;

    for (size_t chunk = 0; chunk < numChunks; ++chunk)
    {
        pool.enqueue([&, chunk]()
        {
            const size_t start = chunk * chunkSize;

            const size_t end = std::min(
                start + chunkSize,
                cellKeys.size()
            );

            auto& localResults = results[chunk];

            for (size_t i = start; i < end; ++i)
            {
                const CellKey& cellKey = cellKeys[i];

                const auto& bucket = readOnlyGrid.at(cellKey);

                for (size_t a = 0; a < bucket.size(); ++a)
                {
                    for (size_t b = a + 1; b < bucket.size(); ++b)
                    {
                        localResults.emplace_back(
                            bucket[a],
                            bucket[b]
                        );
                    }
                }

                for (int dx = -1; dx <= 1; ++dx)
                {
                    for (int dy = -1; dy <= 1; ++dy)
                    {
                        if (dx == 0 && dy == 0)
                            continue;

                        CellKey neighborKey(
                            cellKey.first + dx,
                            cellKey.second + dy
                        );

                        if (!(cellKey < neighborKey))
                            continue;

                        auto it = readOnlyGrid.find(neighborKey);

                        if (it == readOnlyGrid.end())
                            continue;

                        const auto& neighborBucket = it->second;

                        for (Ball* ballA : bucket)
                        {
                            for (Ball* ballB : neighborBucket)
                            {
                                localResults.emplace_back(
                                    ballA,
                                    ballB
                                );
                            }
                        }
                    }
                }
            }
        });
    }

    pool.waitUntilFinished();

    for (const auto& localResults : results)
    {
        for (const auto& [ballA, ballB] : localResults)
        {
            handleCollision(ballA, ballB);
        }
    }
}

void SpatialHash::wallCollisionHandeling() {
    for (auto& [cellKey, bucket] : grid) {
        for (auto* ball : bucket) {
            sf::Vector2f pos = ball->shape.getPosition();
            float radius = ball->shape.getRadius();

            if (pos.x - radius <= 0 || pos.x + radius >= windowSize.x) {
                ball->velocity.x = -ball->velocity.x;
                ball->shape.setPosition(
                    sf::Vector2f(std::clamp(pos.x, radius, windowSize.x - radius), pos.y)
                );
            }
            if (pos.y - radius <= 0 || pos.y + radius >= windowSize.y) {
                ball->velocity.y = -ball->velocity.y;
                ball->shape.setPosition(
                    sf::Vector2f(pos.x, std::clamp(pos.y, radius, windowSize.y - radius))
                );
            }
        }
    }
}

void SpatialHash::handleCollision(Ball* ball1, Ball* ball2) {
    sf::Vector2f pos1 = ball1->shape.getPosition();
    sf::Vector2f pos2 = ball2->shape.getPosition();
    float r1 = ball1->shape.getRadius();
    float r2 = ball2->shape.getRadius();

    sf::Vector2f delta = pos2 - pos1;
    float dist = std::sqrt(delta.x*delta.x + delta.y*delta.y);
    float minDist = r1 + r2;

    if (dist < minDist && dist > 0) {
        sf::Vector2f normal = delta / dist;
        float overlap = minDist - dist;
        sf::Vector2f separation = normal * (overlap * 0.5f);

        ball1->shape.setPosition(pos1 - separation);
        ball2->shape.setPosition(pos2 + separation);

        sf::Vector2f relVel = ball2->velocity - ball1->velocity;
        float velAlongNormal = relVel.x * normal.x + relVel.y * normal.y;
        if (velAlongNormal > 0) return;

        float restitution = 0.0f;
        float impulse = -(1 + restitution) * velAlongNormal;

        sf::Vector2f impulseVector = impulse * normal;
        ball1->velocity -= impulseVector;
        ball2->velocity += impulseVector;
    }
}