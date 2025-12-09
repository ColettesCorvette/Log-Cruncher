#include "MMapLoader.hpp"
#include <fcntl.h>      // open
#include <iostream>     // logs
#include <string>       // std::string
#include <sys/mman.h>   // mmap, munmap
#include <sys/stat.h>   // fstat
#include <system_error> // std::system_error
#include <unistd.h>     // close

// constructeur
MMapLoader::MMapLoader(std::string_view file) : m_addr(nullptr), m_length(0), m_fd(-1) {

    // ouverture du fichier en lecteur seule
    if ((m_fd = open(std::string(file).c_str(), O_RDONLY)) == -1) {
        throw std::system_error(errno, std::generic_category(), "Failed to open the file");
    }

    // récupération de la taille du fichier
    struct stat sb;
    if (fstat(m_fd, &sb) == -1) {
        close(m_fd);
        throw std::system_error(errno, std::generic_category(), "fstat error");
    }
    m_length = sb.st_size;

    // mapping du fichier dans la mémoire
    m_addr = mmap(nullptr, m_length, PROT_READ, MAP_PRIVATE | MAP_HUGETLB, m_fd, 0);

    if (m_addr == MAP_FAILED) {

        m_addr = mmap(nullptr, m_length, PROT_READ, MAP_PRIVATE, m_fd, 0);

        if (m_addr == MAP_FAILED) {
            close(m_fd);
            throw std::system_error(errno, std::generic_category(), "mmap error");
        }
        madvise(m_addr, m_length, MADV_HUGEPAGE);
    }
    madvise(m_addr, m_length, MADV_SEQUENTIAL);
}

// destructeur
MMapLoader::~MMapLoader() {

    if (m_addr != nullptr && m_addr != MAP_FAILED) {
        munmap(m_addr, m_length);
    }
    if (m_fd != -1) {
        close(m_fd);
    }
}

// vue du fichier
std::string_view MMapLoader::getView() const { return std::string_view(static_cast<const char *>(m_addr), m_length); }
