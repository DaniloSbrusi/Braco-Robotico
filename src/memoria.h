#ifndef MEMORIA_H
#define MEMORIA_H

#include <vector>
#include <array>
#include <stdexcept>

class Memoria {
private:
    std::vector<std::array<int, 6>> posicoes; 
public:
    void add_posicao(const std::array<int, 6> &posicao);
    std::array<int, 6> get_posicao(int indice);
    void set_posicao(int index, const std::array<int, 6>& novaPosicao);
    int tamanho();
    void deletar_posicao(int indice);
};

#endif
