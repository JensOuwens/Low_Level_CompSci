#include "imgui.h"
#include "imgui-SFML.h"
#include "Profiler.hpp"
#include "Balls.hpp"

#include <SFML/Graphics.hpp>


int main() {
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Profiler Only");
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    sf::Vector2u windowSize = window.getSize();
    BallGame * game = new BallGame(windowSize);

    if (!ImGui::SFML::Init(window))
        return -1;

    Profiler profiler;
    sf::Clock deltaClock;
    int iterations = 100000;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        sf::Time deltaTime = deltaClock.restart();
        float deltatime = deltaTime.asSeconds();
        ImGui::SFML::Update(window, deltaTime);

        {
            //imgui show profiler
            ImGui::Begin("Controls");
            if (ImGui::Button("Clear Profiler History")) {
                profiler.clear();
            }
            ImGui::End();
        }

        profiler.renderImGui();

        {
            PROFILE(profiler, "Physics update");
            game->updateBalls(deltatime);
        }

        {
            PROFILE(profiler, "Rendering");
            window.clear(sf::Color::Black);
            ImGui::SFML::Render(window);
            game->drawBalls(window);
            window.display();
        }
    }

    ImGui::SFML::Shutdown();
    return 0;
}