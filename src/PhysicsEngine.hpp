#pragma once
#include "Particle.hpp"
#include <vector>
#include <cmath>

class PhysicsEngine {
public:
    std::vector<Particle> particles;
    sf::Vector2f gravity = {0.0f, 981.0f}; // 9.81 scalato per adattarsi ai pixel dello schermo
    float restitution = 0.8f; // (1.0 = elastico perfetto, <1 perde energia)
    sf::Vector2f constraintSize; // I muri della stanza

    PhysicsEngine(float width, float height) : constraintSize(width, height) {}

    void addParticle(float x, float y, float radius, sf::Color color) {
        particles.emplace_back(x, y, radius, color);
    }

    // Funzione principale chiamata 60 volte al secondo
    void update(float dt) {
        applyGravity();
        applyConstraints();
        solveCollisions();
        updatePositions(dt); // Risoluzione del moto
    }

private:
    void applyGravity() {
        for (auto& p : particles) {
            p.acceleration = gravity;
        }
    }

    // Uso un'integrazione di base per aggiornare le posizioni (Semi-Implicit Euler)
    void updatePositions(float dt) {
        for (auto& p : particles) {
            p.velocity += p.acceleration * dt;
            p.position += p.velocity * dt;
            p.acceleration = {0.0f, 0.0f}; // Resetto l'accelerazione per il frame successivo
        }
    }

    // Evita che le particelle escano dallo schermo
    void applyConstraints() {
        for (auto& p : particles) {
            // Controllo bordi laterali (Muro Sinistro e Destro)
            if (p.position.x - p.radius < 0.0f) {
                p.position.x = p.radius;
                p.velocity.x *= -restitution;
            } else if (p.position.x + p.radius > constraintSize.x) {
                p.position.x = constraintSize.x - p.radius;
                p.velocity.x *= -restitution;
            }

            // Controllo Soffitto e Pavimento
            if (p.position.y - p.radius < 0.0f) {
                p.position.y = p.radius;
                p.velocity.y *= -restitution;
            } else if (p.position.y + p.radius > constraintSize.y) {
                p.position.y = constraintSize.y - p.radius;
                p.velocity.y *= -restitution;
            }
        }
    }

    // Risoluzione delle collisioni
    // TODO: Attualmente è O(N^2). Da ottimizzare con un QuadTree o Spatial Grid se aggiungo migliaia di palline.
    void solveCollisions() {
        const int numParticles = particles.size();
        for (int i = 0; i < numParticles; ++i) {
            for (int j = i + 1; j < numParticles; ++j) {
                Particle& p1 = particles[i];
                Particle& p2 = particles[j];

                sf::Vector2f collisionAxis = p1.position - p2.position;
                float distSquared = (collisionAxis.x * collisionAxis.x) + (collisionAxis.y * collisionAxis.y);
                float minDist = p1.radius + p2.radius;

                // Controllo se si stanno toccando (o compenetrando)
                if (distSquared < minDist * minDist) {
                    float dist = std::sqrt(distSquared);
                    sf::Vector2f n = collisionAxis / dist; // Vettore normale della collisione
                    float delta = minDist - dist;

                    // 1. Correzione Posizionale: le separo per non farle incastrare
                    p1.position += 0.5f * delta * n;
                    p2.position -= 0.5f * delta * n;

                    // 2. Risoluzione dell'Impulso: calcolo le nuove velocità dopo l'urto
                    sf::Vector2f relativeVelocity = p1.velocity - p2.velocity;
                    float velocityAlongNormal = (relativeVelocity.x * n.x) + (relativeVelocity.y * n.y);
                    
                    // Se si stanno già allontanando, non calcolo l'urto
                    if (velocityAlongNormal > 0) continue;

                    // Conservazione della quantità di moto
                    float j_impulse = -(1.0f + restitution) * velocityAlongNormal;
                    j_impulse /= (1.0f / p1.mass) + (1.0f / p2.mass);

                    sf::Vector2f impulse = j_impulse * n;
                    p1.velocity += impulse / p1.mass;
                    p2.velocity -= impulse / p2.mass;
                }
            }
        }
    }
};