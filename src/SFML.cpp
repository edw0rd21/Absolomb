#include "SFML.h"
#include <iostream>

Renderer::Renderer() : width(0), height(0) {}

//Initialize SFML window settings
void Renderer::InIt(int width, int height)
{
    this-> width = width;
    this-> height = height;
    window.create(sf::VideoMode(width, height), "RayTracer");
    image.create(width, height);

    //icon loader
    icon.loadFromFile("icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

}

// Set pixel at (x, y) with the specified sf::Color
void Renderer::setPixel(int x, int y, sf::Color color)
{
    image.setPixel(x, y, color);
}

const sf::Image& Renderer::getImage() const
{
    return image;   
}


// Display the image in the SFML window
void Renderer::display()
{

    texture.loadFromImage(image);
    sprite.setTexture(texture);

    // Main rendering loop
    while (window.isOpen()) 
    {
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        
        window.clear();
        window.draw(sprite);
        window.display();
    }
}
