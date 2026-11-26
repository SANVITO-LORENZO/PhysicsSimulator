#pragma once
#include <SFML/Graphics.hpp>

// Struttura base per i corpi del simulatore
struct Particle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    
    float radius;
    float mass;
    sf::Color color;

    // Costruttore per inizializzare velocemente una particella
    Particle(float x, float y, float r, sf::Color c) 
        : position(x, y), velocity(0, 0), acceleration(0, 0), radius(r), mass(r * r), color(c) {}
};