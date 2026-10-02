#include <SFML/Graphics.hpp>

#include <imgui.h>
#include <imgui-SFML.h>
#include "ConcurrentInventory.h"

int main()
{
    ConcurrentInventory inventory;

    std::thread t1([&inventory]() {
        for (int i = 0; i < 1000; i++)
            inventory.AddItem("Sword");
    });

    std::thread t2([&inventory]() {
        for (int i = 0; i < 1000; i++)
            inventory.AddItem("Shield");
    });

    t1.join();
    t2.join();

    inventory.DisplayAllItems();

    //unimportant imgui stuff
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

        ImGui::Begin("Test Window");

        ImGui::Text("SFML + ImGui is working!");

        if (ImGui::Button("Click Me"))
        {

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