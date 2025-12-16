#include "../benchmarks/timer.hpp"
#include "Analyzer.hpp"
#include "MMapLoader.hpp"

#include <chrono>  // chronomètre haute précision
#include <iomanip> // std::setprecision (formatage des nombres)
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_log_file>" << std::endl;
        return EXIT_FAILURE;
    }

    // désactive la synchronisation avec C stdio pour accélérer cout
    std::ios_base::sync_with_stdio(false);

    try {
        const char *filepath = argv[1];

        std::cout << "[INFO] Loading file: " << filepath << "..." << std::endl;
        MMapLoader loader(filepath);
        std::string_view view = loader.getView();
        std::cout << "[INFO] File mapped. Size: " << formatSize(view.size()) << std::endl;

        Analyzer analyzer;

        // --- WARMUP ---
        // force l'OS à mapper physiquement toutes les pages en RAM.
        // met les lignes de cache CPU en état "Ready".
        std::cout << "[INFO] Warming up cache & Page Tables..." << std::endl;
        analyzer.analyze(view);

        // --- BENCHMARK ---
        std::cout << "[INFO] Starting Benchmark run..." << std::endl;

        auto start = std::chrono::high_resolution_clock::now();

        // processing function
        analyzer.analyze(view);

        auto end = std::chrono::high_resolution_clock::now();

        // --- RESULTATS ---
        std::chrono::duration<double> elapsed = end - start;
        double seconds = elapsed.count();
        size_t lines = analyzer.getLineCount();

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
