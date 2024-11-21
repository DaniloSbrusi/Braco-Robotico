#include "Memoria.h"

void Memoria::add_posicao(const std::array<int, 6> &posicao) {
    posicoes.push_back(posicao);
}

std::array<int, 6> Memoria::get_posicao(int indice) {
    if (indice >= 0 && indice < posicoes.size()) {
        return posicoes[indice];
    } else {
        throw std::out_of_range("ERRO! Posicao inexistente.");
    }
}

int Memoria::tamanho() {
    return posicoes.size();
}
