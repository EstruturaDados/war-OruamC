// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
// Tamanhos conforme requisitos adicionais
#define MAX_NOME 30
#define MAX_COR 10

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct Territorio {
    char nome[MAX_NOME];  // até 29 chars + terminador
    char cor[MAX_COR];    // até 9 chars + terminador
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória
Territorio* alocarMapa(int quantidade);
void liberarMemoria(Territorio* mapa);

// Funções de interface com o usuário
void cadastrarTerritorios(Territorio* mapa, int quantidade);
void exibirMapa(const Territorio* mapa, int quantidade);
int exibirMenuPrincipal();

// Funções de lógica principal do jogo
void atacar(Territorio* atacante, Territorio* defensor);
void faseDeAtaque(Territorio* mapa, int quantidade);

// Função utilitária
void limparBufferEntrada()
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
}

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup)
    srand((unsigned)time(NULL));

    printf("====================================================\n");
    printf("\tWAR ESTRUTURADO - CADASTRO DE TERRITORIOS\n");
    printf("====================================================\n");

    int quantidade = 0;
    printf("Informe a quantidade de territorios do mapa: ");
    if (scanf("%d", &quantidade) != 1 || quantidade <= 0) {
        printf("Quantidade invalida. Encerrando.\n");
        return 1;
    }
    limparBufferEntrada();

    Territorio* mapa = alocarMapa(quantidade);
    if (!mapa) {
        printf("Falha ao alocar memoria para o mapa.\n");
        return 1;
    }

    cadastrarTerritorios(mapa, quantidade);
    printf("\nCadastro inicial concluido com sucesso!\n\n");
    exibirMapa(mapa, quantidade);

    // 2. Laço Principal do Jogo (Game Loop)
    int opcao;
    do {
        opcao = exibirMenuPrincipal();
        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, quantidade);
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    // 3. Limpeza
    liberarMemoria(mapa);
    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
Territorio* alocarMapa(int quantidade) {
    if (quantidade <= 0) return NULL;
    Territorio* mapa = (Territorio*)calloc((size_t)quantidade, sizeof(Territorio));
    return mapa;
}

// cadastrarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
void cadastrarTerritorios(Territorio* mapa, int quantidade) {
    if (!mapa || quantidade <= 0) return;

    for (int i = 0; i < quantidade; i++) {
        printf("--- Cadastrando territorio %d ---\n", i + 1);

        printf("Nome do territorio (max %d chars): ", MAX_NOME - 1);
        if (!fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin)) {
            mapa[i].nome[0] = '\0';
        }
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor do Exercito (ex: Azul, Verde) (max %d chars): ", MAX_COR - 1);
        if (!fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin)) {
            mapa[i].cor[0] = '\0';
        }
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Numero de Tropas (>= 1): ");
        if (scanf("%d", &mapa[i].tropas) != 1 || mapa[i].tropas < 1) {
            mapa[i].tropas = 1;
        }
        limparBufferEntrada();
        printf("\n");
    }
}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
void liberarMemoria(Territorio* mapa) {
    if (mapa) free(mapa);
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.
int exibirMenuPrincipal() {
    printf("====================================================\n");
    printf("1 - Atacar\n");
    printf("0 - Sair\n");
    printf("Escolha uma opcao: ");
    int op = -1;
    if (scanf("%d", &op) != 1) op = -1;
    limparBufferEntrada();
    return op;
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(const Territorio* mapa, int quantidade) {
    if (!mapa || quantidade <= 0) return;
    printf("====================================================\n");
    printf("\tMAPA DO MUNDO - ESTADO ATUAL\n");
    printf("====================================================\n");
    for (int i = 0; i < quantidade; i++) {
        printf("TERRITORIO %d:\n", i + 1);
        printf("\t- Nome: %s\n", mapa[i].nome);
        printf("\t- Dominado por: Exercito %s\n", mapa[i].cor[0] ? mapa[i].cor : "(sem dono)");
        printf("\t- Tropas: %d\n\n", mapa[i].tropas);
    }
}

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função atacar() para executar a lógica da batalha.
void faseDeAtaque(Territorio* mapa, int quantidade) {
    if (!mapa || quantidade <= 1) {
        printf("Mapa insuficiente para ataques.\n");
        return;
    }

    exibirMapa(mapa, quantidade);
    printf("Selecione os territorios pelo numero listado.\n");

    int idxAtacante = -1, idxDefensor = -1;
    printf("Numero do territorio ATACANTE: ");
    if (scanf("%d", &idxAtacante) != 1) { limparBufferEntrada(); return; }
    printf("Numero do territorio DEFENSOR: ");
    if (scanf("%d", &idxDefensor) != 1) { limparBufferEntrada(); return; }
    limparBufferEntrada();

    // Ajuste para 0-based
    idxAtacante -= 1;
    idxDefensor -= 1;

    // Validacoes
    if (idxAtacante < 0 || idxAtacante >= quantidade || idxDefensor < 0 || idxDefensor >= quantidade) {
        printf("Indice invalido.\n");
        return;
    }
    if (idxAtacante == idxDefensor) {
        printf("Um territorio nao pode atacar a si mesmo.\n");
        return;
    }
    if (mapa[idxAtacante].tropas < 2) {
        printf("O atacante precisa ter pelo menos 2 tropas para atacar.\n");
        return;
    }
    if (mapa[idxAtacante].cor[0] && mapa[idxDefensor].cor[0] && strcmp(mapa[idxAtacante].cor, mapa[idxDefensor].cor) == 0) {
        printf("Voce nao pode atacar um territorio da mesma cor.\n");
        return;
    }

    atacar(&mapa[idxAtacante], &mapa[idxDefensor]);
    printf("\nResultado do ataque:\n");
    exibirMapa(mapa, quantidade);
}

// atacar():
// Executa a lógica de uma batalha entre dois territórios.
// Rola os dados, compara os resultados e atualiza tropas e dono do território defensor em caso de vitória.
void atacar(Territorio* atacante, Territorio* defensor) {
    if (!atacante || !defensor) return;

    if (atacante->cor[0] && defensor->cor[0] && strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Ataque cancelado: territorios da mesma cor.\n");
        return;
    }
    if (atacante->tropas < 2) {
        printf("Ataque cancelado: tropas insuficientes no atacante.\n");
        return;
    }

    int dadoA = (rand() % 6) + 1;
    int dadoD = (rand() % 6) + 1;
    printf("Dados: atacante=%d, defensor=%d\n", dadoA, dadoD);

    if (dadoA > dadoD) {
        printf("Atacante venceu! Territorio conquistado.\n");
        // Mover metade das tropas do atacante para o novo territorio
        int mover = atacante->tropas / 2;
        if (mover < 1) mover = 1; // garantia de pelo menos 1 indo se possivel
        if (mover >= atacante->tropas) mover = atacante->tropas - 1; // manter pelo menos 1 no atacante

        // Atualiza dono e tropas do defensor
        strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
        defensor->cor[sizeof(defensor->cor) - 1] = '\0';
        defensor->tropas = mover;

        // Retira tropas do atacante
        atacante->tropas -= mover;
    } else {
        printf("Defensor resistiu ao ataque. Atacante perde 1 tropa.\n");
        if (atacante->tropas > 0) atacante->tropas -= 1;
    }
}

// (Funções de missão/checagem de vitória podem ser adicionadas em etapas futuras.)
// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
