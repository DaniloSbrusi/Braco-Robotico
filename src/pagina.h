#ifndef PAGINAS_H
#define PAGINAS_H

const char index_html[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Controle do Braço Robótico</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css" rel="stylesheet">
    <style>
        .slider-label {
            display: flex;
            justify-content: space-between;
            align-items: center;
        }
    </style>
</head>
<body class="bg-light">
    <div class="container py-4">
        <div class="text-center mb-4">
            <h1>Braço Robótico</h1>
        </div>

        <!-- Alternância Automático/Manual -->
        <div class="text-center mb-4">
            <div class="btn-group" role="group">
                <input type="radio" class="btn-check" name="btnradio" id="btnAutomatico" autocomplete="off" checked onchange="alternarModoOperacao()">
                <label class="btn btn-outline-primary" for="btnAutomatico">Automático (CLP)</label>
                <input type="radio" class="btn-check" name="btnradio" id="btnManual" autocomplete="off" onchange="alternarModoOperacao()">
                <label class="btn btn-outline-primary" for="btnManual">Manual</label>
            </div>
        </div>

        <!-- Controles dos motores -->
        <div id="motorControls">
            <div class="">
                <div class="slider-label">
                    <label for="motor1">Motor 1 (Cintura)</label>
                    <span id="valorMotor1">50%</span>
                </div>
                <input type="range" class="form-range" min="0" max="100" value="50" id="motor1" onchange="atualizarValorMotor('1')" disabled>
            </div>
            <div class="">
                <div class="slider-label">
                    <label for="motor2">Motor 2 (Ombro)</label>
                    <span id="valorMotor2">50%</span>
                </div>
                <input type="range" class="form-range" min="0" max="100" value="50" id="motor2" onchange="atualizarValorMotor('2')" disabled>
            </div>
            <div class="">
                <div class="slider-label">
                    <label for="motor3">Motor 3 (Cotovelo)</label>
                    <span id="valorMotor3">50%</span>
                </div>
                <input type="range" class="form-range" min="0" max="100" value="50" id="motor3" onchange="atualizarValorMotor('3')" disabled>
            </div>
            <div class="">
                <div class="slider-label">
                    <label for="motor4">Motor 4 (Rotação pulso)</label>
                    <span id="valorMotor4">50%</span>
                </div>
                <input type="range" class="form-range" min="0" max="100" value="50" id="motor4" onchange="atualizarValorMotor('4')" disabled>
            </div>
            <div class="">
                <div class="slider-label">
                    <label for="motor5">Motor 5 (Inclinação pulso)</label>
                    <span id="valorMotor5">50%</span>
                </div>
                <input type="range" class="form-range" min="0" max="100" value="50" id="motor5" onchange="atualizarValorMotor('5')" disabled>
            </div>
            <div class="">
                <div class="slider-label">
                    <label for="motor6">Motor 6 (Garra)</label>
                    <span id="valorMotor6">50%</span>
                </div>
                <input type="range" class="form-range" min="0" max="100" value="50" id="motor6" onchange="atualizarValorMotor('6')" disabled>
            </div>
            <div>

            </div>
        </div>

        <!-- Botão para salvar posição -->
        <div class="text-center my-4">
            <button id="btnSalvarPosicao" class="btn btn-primary" onclick="salvarPosicao()" hidden>Salvar nova posição</button>
        </div>

        <!-- Lista de posições salvas -->
        <div class="mt-4">
            <h3>Posições salvas</h3>
            <ul class="list-group" id="posicoesSalvas"></ul>
        </div>
    </div>

    <script>
        let posicaoAtual = {};
        let posicoes = []; // Array para armazenar as posições
        let indexPosicaoEditada = -1; // Variável para rastrear se estamos editando uma posição (valor -1 indica que não estamos editando)

        function atualizarBotoes() {

            const operacaoManual = document.getElementById('btnManual').checked;

            // Oculta/Exibe botão salvar posição
            document.getElementById('btnSalvarPosicao').hidden = !operacaoManual;

            // Habilita/Desabilita botões edição das posições
            const botoes = document.querySelectorAll('.btn-editar, .btn-excluir')
            botoes.forEach(btn => {
                btn.disabled = !operacaoManual;
            });

        }

        // Alternar entre modos Automático e Manual
        async function alternarModoOperacao() {
            const operacaoManual = document.getElementById('btnManual').checked;

            try {
                const response = await fetch('/atualizarModoOperacao', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json',
                    },
                    body: JSON.stringify({ modo: operacaoManual ? 1 : 0 }),
                });

                if (!response.ok) {
                    throw new Error('Erro ao atualizar o modo de operação no servidor');
                }

                const data = await response.json();
                console.log('Modo de operação atualizado no servidor:', data);

                // Habilitar ou desabilitar os controles com base no modo
                const sliders = document.querySelectorAll('#motorControls input[type="range"]');
                sliders.forEach(slider => {
                    slider.disabled = !operacaoManual;
                });

                // Atualizar botões da lista de posições
                atualizarBotoes();

            } catch (error) {
                console.error('Erro ao alternar modo de operação:', error);
            }
        }

        // Ao alterar posição do slider, função atualiza valores
        function atualizarValorMotor(motorId) {
            const valorSlider = document.getElementById(`motor${motorId}`).value;

            // Ajusta valor porcentagem lateral
            document.getElementById(`valorMotor${motorId}`).innerText = `${valorSlider}%`;

            // Atualiza posição e movimenta
            posicaoAtual[`motor${motorId}`] = valorSlider;
            moverParaPosicao();
        }

        function atualizarValores() {
            for (let i = 1; i <= 6; i++) {
                atualizarValorMotor(i.toString());
            }
        }

        async function atualizarPosicaoAtualDoServidor() {
            try {
                const response = await fetch('/obterPosicaoAtual');
                if (!response.ok) {
                    throw new Error('Erro ao obter a posição atual do braço');
                }

                // Recebe os dados como um array [motor1, motor2, ..., motor6]
                const posicaoAtualServidor = await response.json();

                // Atualiza os sliders e os valores de exibição
                posicaoAtualServidor.forEach((valor, index) => {
                    const motorId = index + 1;
                    document.getElementById(`motor${motorId}`).value = valor;
                    document.getElementById(`valorMotor${motorId}`).innerText = `${valor}%`;

                    // Atualiza o objeto posicaoAtual local para refletir a posição
                    posicaoAtual[`motor${motorId}`] = valor;
                });
            } catch (error) {
                console.error('Erro ao atualizar a posição atual do servidor:', error);
            }
        }
        
        function alterarPosicaoAtual(index) {
            // Atualiza para posição salva 
            posicaoAtual = posicoes[index];
            for (let i = 1; i <= 6; i++) {
                document.getElementById(`motor${i}`).value = posicaoAtual[`motor${i}`];
            }
            atualizarValores();
            moverParaPosicao();
        }

        // Atualizar lista de posições salvas
        function atualizarListaPosicoes() {
            const list = document.getElementById('posicoesSalvas');
            list.innerHTML = ''; 
            posicoes.forEach((pos, index) => {
                const listItem = document.createElement('li');
                listItem.className = 'list-group-item d-flex justify-content-between align-items-center';
                listItem.innerHTML = `
                    <div class="d-flex align-items-center m-0 p-1 pos-item">
                        <span class="p-1">${pos.motor1}%</span>
                        <span class="p-1">${pos.motor2}%</span>
                        <span class="p-1">${pos.motor3}%</span>
                        <span class="p-1">${pos.motor4}%</span>
                        <span class="p-1">${pos.motor5}%</span>
                        <span class="p-1">${pos.motor6}%</span>
                    </div>
                    <div class="text-center">
                        <button class="btn btn-sm btn-warning m-1 btn-editar" onclick="editarPosicao(${index})" disabled>Editar</button>
                        <button class="btn btn-sm btn-danger m-1 btn-excluir" onclick="deletarPosicao(${index})" disabled>Excluir</button>
                    </div>
                `;
                const posicaoDiv = listItem.querySelector('.pos-item');
                posicaoDiv.addEventListener('click', function() {
                    const operacaoManual = document.getElementById('btnManual').checked;
                    if(operacaoManual){
                        alterarPosicaoAtual(index); 
                    } else {
                        console.error('Erro ao atualizar a posição: Altere o modo de operação');
                    }
                });
                list.appendChild(listItem);
            });
            atualizarBotoes();
        }

        async function salvarPosicao() {

            const btnSalvar = document.getElementById('btnSalvarPosicao');
            btnSalvar.disabled = true; // Bloqueia o botão

            // Salvar edição
            if (indexPosicaoEditada >= 0) {
                // Sobrescrever a posição editada
                posicoes[indexPosicaoEditada] = posicaoAtual;
                try {
                    const response = await fetch('/editarPosicao', {
                        method: 'PUT',
                        headers: {
                            'Content-Type': 'application/json'
                        },
                        body: 
                            JSON.stringify({ 
                                index: indexPosicaoEditada, 
                                posicao: posicaoAtual 
                            })
                    });

                    if (!response.ok) {
                        throw new Error('Erro ao editar posição no servidor');
                    }

                    const data = await response.json();
                    console.log('Resposta do servidor:', data);
                } catch (error) {
                    console.error('Erro ao editar posição:', error);
                }
                indexPosicaoEditada = -1; // Resetar a edição
                btnSalvar.innerText = 'Salvar nova posição'; // Resetar texto do botão
            
            // Salvar nova posição
            } else {
                posicoes.push(posicaoAtual);
                // Salvar no servidor
                try {
                    const response = await fetch('/salvarPosicao', {
                        method: 'POST',
                        headers: {'Content-Type': 'application/json'},
                        body: JSON.stringify(posicaoAtual)
                    });
                    if (!response.ok) {
                        throw new Error('Erro ao salvar posição no servidor');
                    }
                    const data = await response.json();
                    console.log('Resposta do servidor:', data);
                } catch (error) {
                    console.error('Erro ao salvar posição:', error);
                } 
            }
            btnSalvar.disabled = false; // Libera o botão
        }

        function editarPosicao(index) {
            alterarPosicaoAtual(index);
            document.getElementById('btnSalvarPosicao').innerText = 'Salvar alterações'; // Alterar o texto do botão
            indexPosicaoEditada = index;
        }

        async function deletarPosicao(index) {
            const data = JSON.stringify({ index: index });

            try {
                const response = await fetch('/deletarPosicao', {
                    method: 'DELETE',
                    headers: {
                        'Content-Type': 'application/json',
                    },
                    body: data,
                });

                const responseData = await response.json();
                
                if (responseData.status === 'success') {
                    console.log('Posição deletada com sucesso');
                } else {
                    console.error('Erro ao deletar posição:', responseData.message);
                }
            } catch (error) {
                console.error('Erro de rede ou servidor:', error);
            }
        }


        async function obterPosicoes() {
            try {
                const response = await fetch('/obterPosicoes');
                if (!response.ok) {
                    throw new Error('Erro ao obter dados');
                }
                posicoes = await response.json();
                atualizarListaPosicoes();
            } catch (error) {
                console.error('Erro ao atualizar as posições:', error);
            }
        }

        // Função para mover o braço robótico para a posição salva
        async function moverParaPosicao() {
            try {
                const response = await fetch('/moverBraco', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json', // Certifique-se de que este cabeçalho está presente
                    },
                    body: JSON.stringify(posicaoAtual), // Serializa o objeto para JSON
                });

                if (!response.ok) {
                    throw new Error('Erro na resposta do servidor');
                }

                const data = await response.json(); // Espera pela resolução da Promise e interpreta a resposta como JSON
                console.log('Braço movido para a posição:', data);
            } catch (error) {
                console.error('Erro ao mover o braço:', error);
            }
        }

        function atualizarModoNaInicializacao() {
            fetch('/getModoOperacao')
                .then(response => {
                    if (!response.ok) {
                        throw new Error(`Erro ao obter o modo de operação: ${response.statusText}`);
                    }
                    return response.json();
                })
                .then(data => {
                    // Atualizar os botões de acordo com o valor retornado pelo servidor
                    if (data.modo === 0) {
                        document.getElementById('btnAutomatico').checked = true;
                    } else if (data.modo === 1) {
                        document.getElementById('btnManual').checked = true;
                    }
                })
                .catch(error => {
                    console.error('Erro:', error);
                });
        }

        window.onload = function() {
            
            obterPosicoes(); // Atualiza a lista de posições salvas
            atualizarPosicaoAtualDoServidor();
            atualizarModoNaInicializacao();

            setInterval(() => {
                const operacaoAutomatica = document.getElementById('btnAutomatico').checked;
                if (operacaoAutomatica) {
                    atualizarPosicaoAtualDoServidor();
                } else {
                    obterPosicoes();
                }
            }, 1000); // Atualiza a posição atual a cada 1 segundo
            
        };


    </script>
</body>
</html>

)rawliteral";

#endif
