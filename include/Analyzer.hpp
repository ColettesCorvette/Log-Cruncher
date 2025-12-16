#pragma once
#include <cstdint> // uint64_t
#include <string_view>

class Analyzer {
  public:
    explicit Analyzer();
    ~Analyzer();

    void analyze(std::string_view file);
    size_t getLineCount() const; // getter

  private:
    const char *cursor;
    const char *end;
    size_t m_lineCount; // compteur
};
