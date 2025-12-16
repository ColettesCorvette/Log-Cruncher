#include "Analyzer.hpp"
#include <algorithm>   // std::count (cleanup)
#include <immintrin.h> // AVX-512 (_mm512_...) et popcnt
#include <iostream>

Analyzer::Analyzer() : cursor(nullptr), end(nullptr), m_lineCount(0) {}

void Analyzer::analyze(std::string_view view) {

    cursor = view.data();
    end = view.data() + view.size();
    m_lineCount = 0;

    // préparation du registre cible
    //  vecteur de 512 bits rempli partout avec le caractère '\n'
    const __m512i target = _mm512_set1_epi8('\n');

    // Boucle principale (AVX), tant qu'il reste au moins 64 octets à lire
    while (cursor + 64 <= end) {

        // Charge 64 octets (512 bits) depuis le curseur vers un registre CPU
        // loadu (sécurité si l'adresse n'est pas un multiple de 64)
        __m512i chunk = _mm512_loadu_si512(reinterpret_cast<const void *>(cursor));

        // compare 'chunk' avec 'target' octet par octet (epi8)
        // renvoie un masque de 64 bits
        // chaque bit à 1 signifie qu'il y a un '\n' à cette position
        __mmask64 mask = _mm512_cmpeq_epi8_mask(chunk, target);

        // _popcnt64 est une instruction hardware qui compte le nombre de bits à 1
        m_lineCount += _popcnt64(mask);
        // Avance de 64
        cursor += 64;
    }

    // cleanup
    // traite les quelques octets restants entre 0 et 63 à la fin du fichier avec std::count
    // AVX ne peut pas les prendre sans dépasser la mémoire
    m_lineCount += std::count(cursor, end, '\n');
}

size_t Analyzer::getLineCount() const { return m_lineCount; }

Analyzer::~Analyzer() {}
