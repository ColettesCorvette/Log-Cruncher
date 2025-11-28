#include <iostream>

int main(int argc, char *argv[]) {


    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <fichier>" << std::endl;
        return EXIT_FAILURE;
    }


    try {




    }
    catch(const std::exception& e) {
        std::cerr << "Critical failure : " << e.what() << std::endl;
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}