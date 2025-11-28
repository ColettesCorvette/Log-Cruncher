#pragma once // permet au fichier d'être inclus au plus une fois

#include <string_view> // std::string_view
#include <cstddef> // size_t

class MMapLoader {
public:
    
    // constructeur
    explicit MMapLoader(std::string_view file);

    // destructeur
    ~MMapLoader();

    
    MMapLoader (const MMapLoader&) = delete; // interdiction de la copie
    MMapLoader& operator = (const MMapLoader&) = delete; // interdiction de l'assignation


    std::string_view getView() const; // retourne une vue en lecture seule

private:

    void* m_addr; // adresse du pointer de mmap
    size_t m_length; // taille du mapping dans la mémoire
    int m_fd;  


};
