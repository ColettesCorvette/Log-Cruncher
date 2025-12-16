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

    // parallélisme
    uint64_t count1 = 0, count2 = 0, count3 = 0, count4 = 0;

    // Boucle principale (AVX), tant qu'il reste au moins 4 * 64 = 256 octets à lire
    while (cursor + 256 <= end) {

        // Charge 256 octets depuis le curseur vers un registre CPU
        // loadu, sécurité si l'adresse n'est pas un multiple de 64
        // Chargement parallèle pour saturer le pipeline
        __m512i chunk1 = _mm512_loadu_si512(reinterpret_cast<const void *>(cursor));
        __m512i chunk2 = _mm512_loadu_si512(reinterpret_cast<const void *>(cursor + 64));
        __m512i chunk3 = _mm512_loadu_si512(reinterpret_cast<const void *>(cursor + 128));
        __m512i chunk4 = _mm512_loadu_si512(reinterpret_cast<const void *>(cursor + 192));

        // compare 'chunk' avec 'target' octet par octet (epi8)
        // renvoie un masque de 64 bits
        // chaque bit à 1 signifie qu'il y a un '\n' à cette position
        __mmask64 mask1 = _mm512_cmpeq_epi8_mask(chunk1, target);
        __mmask64 mask2 = _mm512_cmpeq_epi8_mask(chunk2, target);
        __mmask64 mask3 = _mm512_cmpeq_epi8_mask(chunk3, target);
        __mmask64 mask4 = _mm512_cmpeq_epi8_mask(chunk4, target);

        // _popcnt64 est une instruction hardware qui compte le nombre de bits à 1
        count1 += _popcnt64(mask1);
        count2 += _popcnt64(mask2);
        count3 += _popcnt64(mask3);
        count4 += _popcnt64(mask4);

        // Avance de 256
        cursor += 256;
    }

    m_lineCount += count1 + count2 + count3 + count4;

    // Fallback pour les blocs restants compris entre 64 et 255 octets
    while (cursor + 64 <= end) {
        __m512i chunk = _mm512_loadu_si512(reinterpret_cast<const void *>(cursor));
        __mmask64 mask = _mm512_cmpeq_epi8_mask(chunk, target);
        m_lineCount += _popcnt64(mask);
        cursor += 64;
    }

    // cleanup
    // traite les quelques octets restants entre 0 et 63 à la fin du fichier avec std::count
    // AVX ne peut pas les prendre sans dépasser la mémoire
    m_lineCount += std::count(cursor, end, '\n');
}

size_t Analyzer::getLineCount() const { return m_lineCount; }

Analyzer::~Analyzer() {}
