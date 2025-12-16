# Log-Cruncher







# 1. Génération des fichiers de build Makefiles
# -DCMAKE_BUILD_TYPE=Release : Active automatiquement -O3 et désactive le debug
# -S .                       : sources  
# -B build                   : mettre dans le dossier 'build' 
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build

# 2. Compilation
# --build build : Lance la compilation dans le dossier build
# --parallel    : Utilise tous les cœurs du CPU 
cmake --build build --parallel

# 3. Lancer l'exécutable
./build/lcruncher <fichier>

# 4. formatte les fichiers avec .clang
find src include -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i
