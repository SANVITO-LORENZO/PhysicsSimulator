#pragma once
#include "Particle.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <filesystem> 
#include <iostream>

class Exporter {
public:
    // Genera il dataset CSV utile per il training di reti neurali orientate alla fisica (PINNs)
    static void exportToCSV(const std::vector<Particle>& particles, int frameCount) {
        std::string folderName = "data";
        
        // Creo la cartella "data" automaticamente se non esiste nel sistema
        if (!std::filesystem::exists(folderName)) {
            std::filesystem::create_directory(folderName);
        }

        std::string filename = folderName + "/sim_frame_" + std::to_string(frameCount) + ".csv";
        std::ofstream file(filename);

        if (file.is_open()) {
            // Intestazione delle colonne
            file << "ID,PosX,PosY,VelX,VelY,Mass\n";
            
            // Scrivo lo stato vettoriale di ogni particella
            for (size_t i = 0; i < particles.size(); ++i) {
                const auto& p = particles[i];
                file << i << "," 
                     << p.position.x << "," << p.position.y << ","
                     << p.velocity.x << "," << p.velocity.y << "," 
                     << p.mass << "\n";
            }
            file.close();
            
            // Feedback nella console per sapere che sta funzionando
            std::cout << "[EXPORT] Salvato frame " << frameCount << " in " << filename << "\n";
        } else {
            std::cout << "[ERRORE] Impossibile creare il file CSV!\n";
        }
    }
};