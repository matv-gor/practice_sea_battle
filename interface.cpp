#include <SFML/Graphics.hpp>
#include <optional>

int main(){
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    sf::RenderWindow window(desktop, "Sea Battle", sf::Style::Default);
    window.setVerticalSyncEnabled(true);
    window.setPosition({0, 0});

    sf::RectangleShape field({1300.f, 650.f});
    field.setFillColor(sf::Color(173, 216, 230));
    field.setPosition({80, 95});

    sf::RectangleShape square({55, 55});
    float startPosition{120.f};
    square.setPosition({startPosition, startPosition});

    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
                window.close();
            }

            window.clear();
            window.draw(field);
            float offset{60.f};
            float extraOffset{0};
            for(int i{0}; i < 10; i++){
                for(int j{0}; j < 20; j++){
                    square.setPosition({startPosition+j*offset+extraOffset, startPosition+i*offset});
                    if(j == 9){
                        extraOffset = 30.f;
                    }
                    window.draw(square);
                }
                offset = 60.f;
                extraOffset = 0;
            }
            window.display();
        }
    }

    return 0;
}
