#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <random>
#include <algorithm>
#include <chrono>
#include <thread>
#include "Jogador.h"
#include "Baralho.h"
#include "Automation.h"
#include "Contador.h"

using namespace std;
using namespace Baralhos;
using namespace Players;
using namespace Automations;
using namespace Contador;

void sleep(bool pode, int tempo) {
    if (pode) {std::this_thread::sleep_for(std::chrono::seconds(tempo));}
}
int main() {
    int NUM_MAX_PLAYERS = 3;
    int PRECO_BLIND = 10;
    int DIFICULDADE;
    int TEMPO_DE_DORMIR = 1;
    bool DORMIR = true;

    Players::Dealer dealer = Dealer(0, "James-san");
    vector<Jogador> jogadores;
    for (int i = 0; i <NUM_MAX_PLAYERS-1; i++) {
        jogadores.push_back(Jogador(i, true, "BOT " + std::to_string(i)));
    }
    
    Baralhos::BaralhoTotal baralho_jogo = BaralhoTotal();

    std::cout << dealer.getNome() <<": Bom dia! Tudo bem ? Bem vindo ao Casino DDC Texas Hold! Eu me chamo James-san, serei o Dealer de hoje. Está preparado para apostar tudo ? " << std::endl;
    sleep(DORMIR, TEMPO_DE_DORMIR);
    std::cout << dealer.getNome() << ": Digite 1 para começar o jogo ou 0 para sair." << std::endl;
    sleep(DORMIR, TEMPO_DE_DORMIR);
    int opcao;
    std::cin >> opcao;
    if (opcao == 1) {
        std::cout << dealer.getNome()   << ": Vamos começar o jogo!" << std::endl;
        sleep(DORMIR, TEMPO_DE_DORMIR);
        std::cout << dealer.getNome() << ": Para podermos dar início ao jogo, por favor nos diga qual seu nome!" << std::endl;
        sleep(DORMIR, TEMPO_DE_DORMIR);
        std::string nome;
        std::cin >> nome;

        // Adicionando o usuário da aplicação na Partida.
        int ID_USUARIO = NUM_MAX_PLAYERS-1;
        Players::Jogador usuario = Jogador(ID_USUARIO, false, nome);
        jogadores.push_back(usuario);
        reverse(jogadores.begin(), jogadores.end());
        ID_USUARIO = 0;
        // Definindo a dificuldade do jogo.
        std::cout << dealer.getNome() << ": Muito bem " << nome << ", agora nos diga de 0 à 1 a dificuldade do jogo que gostaria. Sendo 0 fácil e 1 difícil." << std::endl;        
        sleep(DORMIR, TEMPO_DE_DORMIR);
        std::cin >> DIFICULDADE;

        std::cout  << dealer.getNome() << ": Ótimo " << nome << ", todos os jogadores começaram com 100 fichas." << std::endl;
        sleep(DORMIR, TEMPO_DE_DORMIR);
        
        // Geradores aleatórios
        std::random_device rd;
        std::mt19937 gerador(rd());
        std::uniform_int_distribution<int> distribuicao(0, NUM_MAX_PLAYERS-1);        

        // RODADAS!!

        // INICIANDO A RODADA DE JOGO
        std::cout << "Agora vamos iniciar as rodada de jogo!" << std::endl;
        sleep(DORMIR, TEMPO_DE_DORMIR);
        bool JOGO_ATIVO = true;
        int RODADA = 0;
        int POTE = 0;
        int index_iniciador = distribuicao(gerador);

        do {
            POTE = 0;
            std::cout << "======== INÍCIO Rodada " << std::to_string(RODADA) << " ========" << std::endl;
            sleep(DORMIR, TEMPO_DE_DORMIR);
            if (RODADA == 0){
                std::cout << "| Nosso Dealar vai decidir a ordem dos jogadores:" << std::endl;
                sleep(DORMIR, TEMPO_DE_DORMIR);
            }
            // Define a ordem dos jogadores
            std::cout << "| A ordem do jogo será: " << std::endl;
            sleep(DORMIR, TEMPO_DE_DORMIR);
            for (int i = 0; i < NUM_MAX_PLAYERS; i++) {
                int index = (index_iniciador + i) % NUM_MAX_PLAYERS;
                if (index == ID_USUARIO) {
                    std::cout << "| - "  << std::to_string(i+1) <<"º - " << jogadores[index].getNome() << " (você)" << std::endl;
                } else {
                    std::cout << "| - " << std::to_string(i+1) <<"º - " << jogadores[index].getNome() << std::endl;
                }
            }
            
            // Momento do  SMALL BLIND 
            // Momento do  BIG BLIND 

            
            int rodada_de_aposta = 0;
            int ultima_aposta = 0;
            int acao;
            int jogadores_ativos = NUM_MAX_PLAYERS;
            int index_vencedor = -1;
            vector<int> empate = vector<int>();
            bool cartas_dadas = false;
            int nova_aposta = 0;

            baralho_jogo.iniciarBaralho();
            baralho_jogo.embaralhar();

            for (rodada_de_aposta = 0; rodada_de_aposta < 4; rodada_de_aposta++) {
                nova_aposta = 0;
                if (index_vencedor >=0) {
                    break;
                }
                std::cout << "| " << dealer.getNome() << ": Iniciando a " << std::to_string(rodada_de_aposta + 1) << "º rodada de apostas!" << std::endl;
                sleep(DORMIR, TEMPO_DE_DORMIR);
                for (int i = 0; i < NUM_MAX_PLAYERS; i++) {
                    int index = (index_iniciador + i) % NUM_MAX_PLAYERS;
                    if (jogadores[index].getAtividade() == false) {
                        continue;
                    }
                    if (rodada_de_aposta == 0) {
                        if (i==0) {
                            std::cout << "| " << dealer.getNome() << ": O jogador `" << jogadores[index].getNome() << "` faz o Small Blind" << std::endl; 
                            jogadores[index].apostarFichas(int(PRECO_BLIND/2));
                            POTE += int(PRECO_BLIND/2);
                        } else if (i == 1 ) {
                            std::cout << "| " << dealer.getNome() << ": O jogador `" << jogadores[index].getNome() << "` faz o Big Blind" << std::endl; 
                            jogadores[index].apostarFichas(PRECO_BLIND);
                            POTE += PRECO_BLIND;
                            ultima_aposta = PRECO_BLIND;
                        } 
                    }
                    if (rodada_de_aposta !=0 || i >= 2) {
                        if (!cartas_dadas) {
                            std::cout << "| " << dealer.getNome() << ": Distribuindo as cartas para os jogadores..." << std::endl; 
                            sleep(DORMIR, TEMPO_DE_DORMIR);
                            for (int i = 0; i < NUM_MAX_PLAYERS; i++) {
                                int sub_index = (index_iniciador + i) % NUM_MAX_PLAYERS;
                                std::vector<Carta> cartas = baralho_jogo.getCartas(2);
                                jogadores[sub_index].receberBaralho(cartas);
                            }
                            std::cout << "| " << dealer.getNome() << ": Senhor(a) " << jogadores[ID_USUARIO].getNome() <<" suas cartas são: " << std::endl; 
                            sleep(DORMIR, TEMPO_DE_DORMIR);
                            for (Carta carta : jogadores[ID_USUARIO].exibirCartas()) {
                                std::cout << "| - " << carta.getSimboloCarta() << std::endl;
                            }
                            cartas_dadas = true;
                        }
                        string opcoes = "[0 - CORRER, 1 - AUMENTAR ou 2 - COBRIR]";
                        if (rodada_de_aposta >= 0) {
                            opcoes = "[0 - CORRER, 1 - AUMENTAR, 2 - COBRIR, 3 - MESA]";
                        }
                        std::cout << "| " << dealer.getNome() << ": Jogador `" << jogadores[index].getNome() << "` O que você irá fazer ? " << opcoes << std::endl;
                        sleep(DORMIR, TEMPO_DE_DORMIR);
                        nova_aposta = 0;
                        if (index != ID_USUARIO ) {
                            acao = Automations::Decisor::decidirAcao(
                                &jogadores[index], 
                                POTE, 
                                dealer.exibirCartas(5),
                                ultima_aposta,
                                NUM_MAX_PLAYERS,
                                rodada_de_aposta,
                                DIFICULDADE
                            );
                            if (acao == 1) {
                                nova_aposta = Automations::Decisor::decidirAposta(
                                    &jogadores[index], 
                                    POTE, 
                                    dealer.exibirCartas(5),
                                    ultima_aposta,
                                    NUM_MAX_PLAYERS,
                                    rodada_de_aposta,
                                    DIFICULDADE
                                );
                            }
                        } else {
                            bool resposta_acao_invalida = true;
                            while (resposta_acao_invalida) {
                                std::cout << "| " << dealer.getNome() << ": Jogador `" << jogadores[index].getNome() << "` Digite a ação que deseja tomar! "<< std::endl;
                                sleep(DORMIR, TEMPO_DE_DORMIR);
                                std::cin >> acao;
                                
                                if (acao == 2 && jogadores[index].getFichas() + jogadores[index].getFichasApostadas()  < ultima_aposta) {
                                    std::cout << "| " << dealer.getNome() << ": Jogador `" << jogadores[index].getNome() << "` você não tem fichas suficientes para cobrir a aposta."<< std::endl;
                                    sleep(DORMIR, TEMPO_DE_DORMIR);
                                } else if (acao == 3 && rodada_de_aposta == 0) {
                                    std::cout << "| " << dealer.getNome() << ": Jogador `" << jogadores[index].getNome() << "` a ação de pedir MESA ainda não está habilitada para esta rodada de aposta."<< std::endl;
                                    sleep(DORMIR, TEMPO_DE_DORMIR);
                                } else if (acao == 1) {
                                    if (acao == 1 && jogadores[index].getFichas() + jogadores[index].getFichasApostadas() <= ultima_aposta) {
                                        std::cout << "| " << dealer.getNome() << ": Jogador `" << jogadores[index].getNome() << "` você não tem fichas suficientes para aumentar a aposta."<< std::endl;
                                        sleep(DORMIR, TEMPO_DE_DORMIR);
                                    } else {
                                        bool resposta_invalida = true;
                                        while(resposta_invalida){
                                            std::cout << "| " << dealer.getNome() << ": Jogador `" << jogadores[index].getNome() << "` você tem " << jogadores[index].getFichas() << " fichas. A aposta atual é de " << ultima_aposta << " fichas."<< std::endl;
                                            std::cout << "| " << dealer.getNome() << ": Para quanto deseja aumentar a aposta? "<< std::endl;
                                            std::cin >> nova_aposta;
                                            if (nova_aposta <= jogadores[index].getFichas() + jogadores[index].getFichasApostadas() && nova_aposta > ultima_aposta) {
                                                resposta_invalida = false;
                                                resposta_acao_invalida = false;
                                            } else {
                                                std::cout << "| " << dealer.getNome() << ": Jogador `" << jogadores[index].getNome() << "` faça uma aposta válida, maior do que a aposta atual e menor do que a quantidade de saldo que tem."<< std::endl;
                                            }
                                        }
                                    }
                                } else {
                                    resposta_acao_invalida = false;
                                }
                            }
                        }

                        if (acao == 0) {
                            std::cout << "| " << dealer.getNome() << ": Jogador `" << jogadores[index].getNome() << "` decidiu CORRER!"<< std::endl;
                            sleep(DORMIR, TEMPO_DE_DORMIR);
                            jogadores[index].setAtividade(false);
                            jogadores_ativos--;
                            if (jogadores_ativos == 1) {
                                for (int i = 0; i < NUM_MAX_PLAYERS; i++) {
                                    if (jogadores[i].getAtividade() == true) {
                                        std::cout << "| " << dealer.getNome() << ": Jogador `" << jogadores[i].getNome() << "` irá vencedor da rodada por desistência dos participantes!"<< std::endl;
                                        sleep(DORMIR, TEMPO_DE_DORMIR);
                                        index_vencedor = index;
                                        break;
                                    }
                                }
                            }
                        } else if (acao == 1) {
                            std::cout << "| " << dealer.getNome() << ": Jogador `" << jogadores[index].getNome() << "` decidiu AUMENTAR a aposta para " << nova_aposta << " fichas." << std::endl;
                            sleep(DORMIR, TEMPO_DE_DORMIR);
                            int valor_aumentado =  nova_aposta - jogadores[index].getFichasApostadas();
                            ultima_aposta = nova_aposta;
                            jogadores[index].apostarFichas(valor_aumentado);
                            POTE += valor_aumentado;
                        } else if (acao == 2) {
                            std::cout << "| " << dealer.getNome() << ": Jogador `" << jogadores[index].getNome() << "` decidiu COBRIR!" << std::endl;
                            sleep(DORMIR, TEMPO_DE_DORMIR);
                            int valor_aumentado =  ultima_aposta - jogadores[index].getFichasApostadas();
                            jogadores[index].apostarFichas(valor_aumentado);
                            POTE += valor_aumentado;
                        } else {
                            std::cout << "| " << dealer.getNome() << ": Jogador `" << jogadores[index].getNome() << "` decidiu pedir MESA." << std::endl;
                            sleep(DORMIR, TEMPO_DE_DORMIR);
                        }
                        std::cout << "| " << dealer.getNome() << ": O POTE de apostas da rodada está com " <<POTE << " fichas." << std::endl;
                        sleep(DORMIR, TEMPO_DE_DORMIR);
                        if (index_vencedor != -1) {
                            break;
                        } 
                    }
                }
                if (index_vencedor == -1) {
                    if (rodada_de_aposta == 0) {
                        std::cout << "| " << dealer.getNome() << ": Vou sortear as cartas e abrí-las na mesa." << std::endl;
                        sleep(DORMIR, TEMPO_DE_DORMIR);
                        dealer.receberBaralho(baralho_jogo.getCartas(5));
                    }
                    else if (rodada_de_aposta == 1 || rodada_de_aposta == 2) {
                        std::cout << "| " << dealer.getNome() << ": Vou sortear mais uma carta e abrí-la na mesa." << std::endl;
                        sleep(DORMIR, TEMPO_DE_DORMIR);
                    }
                    std::cout << "| " << dealer.getNome() << ": Vou virar as "<< min(3+rodada_de_aposta, 5)<<" cartas disponíveis na mesa! As cartas são: " << std::endl;
                    sleep(DORMIR, TEMPO_DE_DORMIR);
                    for (Carta carta : dealer.exibirCartas(3 + rodada_de_aposta)) {
                        std::cout << "| - " << carta.getSimboloCarta() << std::endl;
                    }
                }
            }
            if (index_vencedor != 1) {
                std::cout << "| " << dealer.getNome() << ": Chegamos ao fim das rodadas de apostas! Agora vamos verificar quem é o vencedor!" << std::endl;
                std::cout << "| " << dealer.getNome() << ": As cartas da mesa são: " << std::endl;
                sleep(DORMIR, TEMPO_DE_DORMIR);
                for (Carta carta : dealer.exibirCartas(5)) {
                    std::cout << "| - " << carta.getSimboloCarta() << std::endl;
                }
                std::cout << "| " << dealer.getNome() << ": Todos se preparem para virar as cartas. É o momento do SHOWDOWN!" << std::endl;
                sleep(DORMIR, TEMPO_DE_DORMIR);
                for (int i = 0; i < NUM_MAX_PLAYERS; i++) {
                    int sub_index = (index_iniciador + i) % NUM_MAX_PLAYERS;
                    if (jogadores[sub_index].getAtividade()) {
                        std::cout << "| " << dealer.getNome() << ": Jogador `" << jogadores[i].getNome() << "` suas cartas são: " << std::endl; 
                        for (Carta carta : jogadores[i].exibirCartas()) {
                            std::cout << "| - " << carta.getSimboloCarta() << std::endl;
                            sleep(DORMIR, TEMPO_DE_DORMIR);
                        }
                    }
                }
                std::cout << "| " << dealer.getNome() << ": Vamos contabilizar os pontos!" << std::endl;
                sleep(DORMIR, TEMPO_DE_DORMIR);
                std::cout << "| " << dealer.getNome() << ": ..." << std::endl;
                sleep(DORMIR, TEMPO_DE_DORMIR);
                vector<pair<int, int>> pontuacao = vector<pair<int, int>>();
                for (int i = 0; i < NUM_MAX_PLAYERS; i++) {
                    if (jogadores[i].getAtividade()) {
                        pontuacao.push_back(pair<int, int>(i, Contador::ContadorDeBaralho::pontuarJogada(jogadores[i].exibirCartas(), dealer.exibirCartas(5))));
                    }
                }
                std::sort(pontuacao.begin(), pontuacao.end(), [](pair<int, int> &left, pair<int, int> &right) {
                    return left.second > right.second;
                });
                std::cout << "| " << dealer.getNome() << ": A ordem de pontuação é: " << std::endl;
                sleep(DORMIR, TEMPO_DE_DORMIR);
                int contador = 1;
                for (pair<int, int> par : pontuacao) {
                    std::cout << "| - " << contador << "º Lugar - "<< jogadores[par.first].getNome() << " com " << par.second << " pontos. Fez um: " << Contador::ContadorDeBaralho::nomearJogada(par.second) << std::endl;
                    sleep(DORMIR, TEMPO_DE_DORMIR);
                    contador += 1;
                }
                std::cout << "| " << dealer.getNome() << ": Vamos verificar se houve empate..." << std::endl;
                sleep(DORMIR, TEMPO_DE_DORMIR+1);
                int maior_pontuacao = pontuacao[0].second;
                empate.push_back(pontuacao[0].first);
                for (unsigned int indice= 1; indice < pontuacao.size(); indice++) {
                    if (pontuacao[indice].second == maior_pontuacao) {
                        empate.push_back(pontuacao[indice].first);
                    }
                }
                if (empate.size() > 1) {
                    std::cout << "| " << dealer.getNome() << ": Houve empate entre os jogadores: " << std::endl;
                    sleep(DORMIR, TEMPO_DE_DORMIR);
                    for (unsigned int i = 0; i < empate.size(); i++) {
                        std::cout << "| - " << jogadores[empate[i]].getNome() << std::endl;
                    }
                } else {
                    std::cout << "| " << dealer.getNome() << ": Houve apenas um vencedor!" << std::endl;
                    sleep(DORMIR, TEMPO_DE_DORMIR);
                    index_vencedor = pontuacao[0].first;
                }
            }
            // Potuaçõ dos jogadores
            if (empate.size() == 1) {
                std::cout << "| " << dealer.getNome() << ": O vencedor da rodada foi o jogador `" << jogadores[index_vencedor].getNome() << "`" << std::endl;
                sleep(DORMIR, TEMPO_DE_DORMIR);
                jogadores[index_vencedor].adicionarFichas(POTE);
                std::cout << "| " << dealer.getNome() << ": O jogador `" << jogadores[index_vencedor].getNome() << "` recebeu " << POTE << " fichas. Parabens!"<< std::endl;
            } else {
                std::cout << "| " << dealer.getNome() << ": Como ocorreu um empate, temos os seguintes vencedores da rodada:" << std::endl;
                sleep(DORMIR, TEMPO_DE_DORMIR);
                for (long unsigned int i = 0; i < empate.size(); i++) {
                    std::cout << "| - " << jogadores[empate[i]].getNome() << std::endl;
                }
                std::cout << "| " << dealer.getNome() << ": O pote será dividido entre os jogadores!" << std::endl;
                sleep(DORMIR, TEMPO_DE_DORMIR);
                int fichas_por_jogador = int(floor(POTE/empate.size()));
                for (unsigned int i = 0; i < empate.size(); i++) {
                    jogadores[empate[i]].adicionarFichas(fichas_por_jogador);
                    std::cout << "| " << dealer.getNome() << ": O jogador `" << jogadores[empate[i]].getNome() << "` recebeu " << fichas_por_jogador << " fichas. Parabens!"<< std::endl;
                    sleep(DORMIR, TEMPO_DE_DORMIR);
                }
            }


            // Exibir rank de jogadores e fichas:
            std::cout << "| " << dealer.getNome() << ": O rank de jogadores e fichas atualmente é: " << std::endl;
            sleep(DORMIR, TEMPO_DE_DORMIR);
            vector<pair<int, int>> ranking = vector<pair<int, int>>();

            int tem_baixa_pontuacao = -1; 
            for (int i = 0; i < NUM_MAX_PLAYERS; i++) {
                ranking.push_back(pair<int, int>(i, jogadores[i].getFichas()));
                if (jogadores[i].getFichas() < PRECO_BLIND ) {
                    tem_baixa_pontuacao = i;
                }
            }
            std::sort(ranking.begin(), ranking.end(), [](pair<int,int> &left, pair<int,int> &right) {
                return left.second > right.second;
            });
            
            int contador = 1;
            for (pair<int, int> par : ranking) {
                std::cout << "| - " << contador << "º Lugar - " << jogadores[par.first].getNome() << " com " << par.second << " fichas."<< std::endl;
                sleep(DORMIR, TEMPO_DE_DORMIR);
                contador += 1;
            }

            if (tem_baixa_pontuacao != -1) {
                std::cout << "| " << dealer.getNome() << ": O jogador `" << jogadores[tem_baixa_pontuacao].getNome() << "` não tem mais fichas para jogar. Portanto o jogo acabou!"<< std::endl;
                sleep(DORMIR, TEMPO_DE_DORMIR);
                JOGO_ATIVO = false;
            } else {
                // inicializando configurações para o próximo jogo.

                // Definir proximo jogador que vai iniciar a partida.
                index_iniciador = (index_iniciador + 1) % NUM_MAX_PLAYERS;

                for(int i = 0; i < NUM_MAX_PLAYERS; i++) {
                    jogadores[i].setAtividade(true);
                    jogadores[i].setFichasApostadas(0);
                    jogadores[i].limparBaralho();
                }
                dealer.limparBaralho();
                baralho_jogo.iniciarBaralho();
                baralho_jogo.embaralhar();
                ultima_aposta = 0;
                acao = -1;
                jogadores_ativos = NUM_MAX_PLAYERS;
                index_vencedor = -1;
                empate.clear();
                empate = vector<int>(0);
                cartas_dadas = false;
                nova_aposta = 0;
            }
            

        } while (JOGO_ATIVO);
        std::cout << "O vencedor do Jogo foi: " << std::endl;
        sleep(DORMIR, TEMPO_DE_DORMIR);
        int vencedor = 0;
        for (int i = 0; i < NUM_MAX_PLAYERS; i++) {
            if (jogadores[i].getFichas() > jogadores[vencedor].getFichas()) {
                vencedor = i;
            }
        }
        std::cout << "| - " << jogadores[vencedor].getNome() << " com " << jogadores[vencedor].getFichas() << " fichas."<< std::endl;
        std::cout << "Obrigado por jogar conosco! Até a próxima!" << std::endl;
        sleep(DORMIR, TEMPO_DE_DORMIR);
    } else {
        std::cout << "Até a próxima! Perdedor!!!!" << std::endl;
    }

}
