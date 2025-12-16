#include "Analyzer.hpp"
#include "MMapLoader.hpp"

#include <chrono>  // Pour le chronomètre haute précision
#include <iomanip> // Pour std::setprecision (formatage des nombres)
#include <iostream>
#include <vector>

// convertir la taille en unités lisibles
std::string formatSize(size_t bytes) {
    const char *suffixes[] = {"B", "KB", "MB", "GB", "TB"};
    int i = 0;
    double dblBytes = bytes;
    if (bytes > 1024) {
        for (i = 0; (bytes / 1024) > 0 && i < 4; i++, bytes /= 1024)
            dblBytes = bytes / 1024.0;
    }

    char output[50];
    sprintf(output, "%.2f %s", dblBytes, suffixes[i]);
    return std::string(output);
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_log_file>" << std::endl;
        return EXIT_FAILURE;
    }

    // désactive la synchronisation avec C stdio pour accélérer cout
    std::ios_base::sync_with_stdio(false);

    try {
        const char *filepath = argv[1];

        // Chargement IO
        std::cout << "[INFO] Loading file: " << filepath << "..." << std::endl;

        // Timer global
        MMapLoader loader(filepath);
        std::string_view view = loader.getView();

        std::cout << "[INFO] File mapped into memory. Size: " << formatSize(view.size()) << std::endl;

        // Analyse AVX-512
        Analyzer analyzer;

        std::cout << "[INFO] Starting high-performance analysis..." << std::endl;

        // Démarrage du chronomètre
        auto start = std::chrono::high_resolution_clock::now();

        // fonction core
        analyzer.analyze(view);

        // Arrêt du chronomètre
        auto end = std::chrono::high_resolution_clock::now();

        // Reporting & Benchmarks
        std::chrono::duration<double> elapsed = end - start;
        double seconds = elapsed.count();
        size_t lines = analyzer.getLineCount();

        // Calcul du débit (Throughput) : Quantité de données / Temps
        // convertion de la taille en Gigaoctets par seconde (GB/s)
        double gigabytes = static_cast<double>(view.size()) / (1024.0 * 1024.0 * 1024.0);
        double throughput = gigabytes / seconds;

        std::cout << "\n" << std::string(40, '-') << "\n";
        std::cout << " RESULTATS \n";
        std::cout << std::string(40, '-') << "\n";
        std::cout << " Lignes trouvées : " << lines << "\n";
        std::cout << " Temps CPU       : " << std::fixed << std::setprecision(4) << seconds << " sec\n";
        std::cout << " Bande passante  : " << std::fixed << std::setprecision(2) << throughput << " GB/s\n";
        std::cout << std::string(40, '-') << std::endl;

    } catch (const std::exception &e) {
        std::cerr << "[CRITICAL ERROR] " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
