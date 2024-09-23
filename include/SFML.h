#pragma once

#include <SFML/Graphics.hpp>
#include "color.h"

class Renderer {
public:
    
    Renderer();

    void InIt(int width, int height);
    void setPixel(int x, int y, sf::Color color);
    void display();
    const sf::Image& getImage() const;

private:
    int width, height;
    sf::Image image;
    sf::Image icon;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::RenderWindow window;
    
};
