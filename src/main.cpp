#include <SFML/Graphics.hpp>
#include "PhysicsEngine.hpp"
#include "Exporter.hpp"
#include <iostream>

int main() {
    // Risoluzione iniziale pensata per portatili
    const int START_WIDTH = 700;
    const int START_HEIGHT = 500;
    
    sf::RenderWindow window(sf::VideoMode(START_WIDTH, START_HEIGHT), "C++ Physics Simulator - AI Data Generator");
    window.setFramerateLimit(60); // FPS fissi per garantire stabilità ai calcoli fisici

    PhysicsEngine engine(START_WIDTH, START_HEIGHT);
    int frameCount = 0;
    bool isExporting = false; // Interruttore per la generazione del dataset

    // Genero un blocco di prova iniziale a griglia
    for(int i = 0; i < 150; i++) {
        float posX = 100 + (i % 20) * 30;
        float posY = 100 + (i / 20) * 30;
        engine.addParticle(posX, posY, 10.0f, sf::Color::Cyan);
    }

    std::cout << "Simulatore avviato. Premi 'E' per registrare i dati fisici.\n";

    // Game Loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            // Adatta la fisica e la telecamera se si ridimensiona la finestra
            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0.0f, 0.0f, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                engine.constraintSize = sf::Vector2f(event.size.width, event.size.height);
            }
            
            // Cliccando creo nuove particelle rosse per testare l'impatto
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                engine.addParticle(worldPos.x, worldPos.y, 15.0f, sf::Color::Red);
            }
            
            // Variabili che posso alterare tenendo premuto il tasto
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) engine.gravity.y -= 100.0f;     
                if (event.key.code == sf::Keyboard::Down) engine.gravity.y += 100.0f;   
                if (event.key.code == sf::Keyboard::Right) engine.restitution += 0.05f; 
                if (event.key.code == sf::Keyboard::Left) engine.restitution -= 0.05f;  
            }

            // Attivazione Data Export (reagisce solo al rilascio per evitare spam di letture)
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::E) {
                    isExporting = !isExporting;
                    if (isExporting) {
                        std::cout << "\n>>> REGISTRAZIONE DATASET AI AVVIATA <<<\n";
                    } else {
                        std::cout << "\n>>> REGISTRAZIONE DATASET AI FERMATA <<<\n";
                    }
                }
            }
        }

        // --- 1. AGGIORNAMENTO FISICO ---
        float dt = 1.0f / 60.0f; // Delta time fisso per calcoli deterministici
        engine.update(dt);

        // --- 2. GENERAZIONE DATASET AI ---
        // Salvo solo 1 frame su 10 per non saturare l'hard disk
        if (isExporting && frameCount % 10 == 0) { 
            Exporter::exportToCSV(engine.particles, frameCount);
        }
        frameCount++;

        // --- 3. RENDERING ---
        window.clear(sf::Color(20, 20, 30)); // Sfondo dark per far risaltare i colori

        sf::CircleShape circle;
        for (const auto& p : engine.particles) {
            circle.setRadius(p.radius);
            circle.setOrigin(p.radius, p.radius);
            circle.setPosition(p.position);
            circle.setFillColor(p.color);
            window.draw(circle);
        }

        window.display();
    }
    return 0;
}