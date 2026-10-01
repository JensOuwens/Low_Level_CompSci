
#ifndef LOW_LEVEL_SETUP_SPATIALHASH_H
#define LOW_LEVEL_SETUP_SPATIALHASH_H

#include <unordered_map>
#include <vector>
#include <utility>

#include <SFML/Graphics.hpp>

#include "Ball.hpp"
#include "ThreadPool.h"

struct PairHash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2;
    }
};

class SpatialHash {

public:
    SpatialHash(sf::Vector2u window, float CellSize);

    void insertObject(Ball* ball);
    void clearHash();

    void collisionHandeling(ThreadPool& pool);

    void wallCollisionHandeling();

private:
    std::unordered_map<
        std::pair<int, int>,
        std::vector<Ball*>,
        PairHash
    > grid;

    sf::Vector2u windowSize;
    float cellSize;

    void handleCollision(Ball* a, Ball* b);
};

#endif