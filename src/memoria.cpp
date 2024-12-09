#include "memoria.h"

void Memoria::add_posicao(const std::array<int, 6> &posicao) {
    posicoes.push_back(posicao);
}

std::array<int, 6> Memoria::get_posicao(int indice) {
    if (indice >= 0 && indice < tamanho()) {
        return posicoes[indice];
    } else {
        throw std::out_of_range("ERRO! Posicao inexistente.");
    }
}

void Memoria::set_posicao(int index, const std::array<int, 6>& novaPosicao) {
    if (index >= 0 && index < tamanho()) {
        posicoes[index] = novaPosicao;
    }
}

int Memoria::tamanho() {
    return posicoes.size();
}

void Memoria::deletar_posicao(int indice) {
    if (indice >= 0 && indice < tamanho()) {
        posicoes.erase(posicoes.begin() + indice);
    } else {
        throw std::out_of_range("ERRO! Índice fora do alcance.");
    }
}
