#include <SFML/Graphics.hpp>

#include <imgui.h>
#include <imgui-SFML.h>
#include "ConcurrentInventory.h"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(800, 600),
        "SFML + ImGui Test"
    );

    sf::Clock deltaClock;

    ImGui::SFML::Init(window);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        // ImGui UI
        ImGui::Begin("Test Window");

        ImGui::Text("SFML + ImGui is working!");

        if (ImGui::Button("Click Me"))
        {
            // Button clicked
        }

        ImGui::End();

        // SFML rendering
        window.clear(sf::Color(30, 30, 30));

        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}