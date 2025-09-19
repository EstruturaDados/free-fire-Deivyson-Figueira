#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_COMPONENTES 10

// --- Estruturas e Variáveis Globais ---

// Struct para armazenar os dados de cada componente
typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
} Componente;

// Vetor para a coleção de componentes
Componente torre[MAX_COMPONENTES];
int numComponentes = 0;

// Variáveis de estado
int ordenadoPorNome = 0;
long long comparacoes = 0;

// --- Protótipos de Funções ---
void limparBuffer();
void stringParaMinusculo(char *str);
void mostrarComponentes();
void cadastrarComponente();
void descartarComponente();

// Funções de ordenação
long long bubbleSortNome();
long long insertionSortTipo();
long long selectionSortPrioridade();

// Função de busca
void buscaBinariaPorNome();

// Funções de menu
void menuOrdenacao();
void medirTempoEExecutar(long long (*algoritmoDeOrdenacao)());

// --- Implementação Principal ---

int main() {
    int opcao;

    do {
        printf("\n========================================\n");
        printf("PLANO DE FUGA - CÓDIGO DA ILHA (NÍVEL MESTRE)\n");
        printf("========================================\n");
        printf("Itens na Mochila: %d/%d\n", numComponentes, MAX_COMPONENTES);
        printf("Status da Ordenação por Nome: %s\n", ordenadoPorNome ? "ORDENADO" : "NÃO ORDENADO");
        printf("\n");
        printf("1. Adicionar Componente\n");
        printf("2. Descartar Componente\n");
        printf("3. Listar Componentes (Inventário)\n");
        printf("4. Organizar Mochila (Ordenar Componentes)\n");
        printf("5. Busca Binária por Componente-Chave (por nome)\n");
        printf("0. ATIVAR TORRE DE FUGA (Sair)\n");
        printf("\n");
        printf("Escolha uma opção: ");

        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                cadastrarComponente();
                break;
            case 2:
                descartarComponente();
                break;
            case 3:
                mostrarComponentes();
                break;
            case 4:
                menuOrdenacao();
                break;
            case 5:
                buscaBinariaPorNome();
                break;
            case 0:
                printf("\nDesativando o sistema. Até mais!\n");
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

// --- Funções de Entrada e Saída ---

// Limpa o buffer de entrada para evitar erros com fgets.
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Converte uma string para letras minúsculas.
void stringParaMinusculo(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Recebe os dados de um novo componente do usuário.
void cadastrarComponente() {
    if (numComponentes >= MAX_COMPONENTES) {
        printf("\nA mochila está cheia! Não é possível adicionar mais componentes.\n");
        return;
    }

    printf("\n--- Adicionar Componente ---\n");
    printf("Nome: ");
    fgets(torre[numComponentes].nome, sizeof(torre[numComponentes].nome), stdin);
    torre[numComponentes].nome[strcspn(torre[numComponentes].nome, "\n")] = 0;

    printf("Tipo (ex: controle, suporte, propulsao): ");
    fgets(torre[numComponentes].tipo, sizeof(torre[numComponentes].tipo), stdin);
    torre[numComponentes].tipo[strcspn(torre[numComponentes].tipo, "\n")] = 0;

    do {
        printf("Prioridade (1 a 10): ");
        scanf("%d", &torre[numComponentes].prioridade);
        limparBuffer();
    } while (torre[numComponentes].prioridade < 1 || torre[numComponentes].prioridade > 10);

    numComponentes++;
    ordenadoPorNome = 0;
    printf("\nComponente adicionado com sucesso!\n");
}

// Descarta um componente do inventário.
void descartarComponente() {
    if (numComponentes == 0) {
        printf("\nA mochila está vazia. Não há componentes para descartar.\n");
        return;
    }

    char nomeBusca[30];
    char nomeBuscaLower[30];
    
    printf("\n--- Descartar Componente ---\n");
    printf("Digite o nome do componente para descartar: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0;
    
    strcpy(nomeBuscaLower, nomeBusca);
    stringParaMinusculo(nomeBuscaLower);

    int i, encontrado = 0;
    for (i = 0; i < numComponentes; i++) {
        char componenteNomeLower[30];
        strcpy(componenteNomeLower, torre[i].nome);
        stringParaMinusculo(componenteNomeLower);

        if (strcmp(componenteNomeLower, nomeBuscaLower) == 0) {
            for (int j = i; j < numComponentes - 1; j++) {
                torre[j] = torre[j + 1];
            }
            numComponentes--;
            encontrado = 1;
            ordenadoPorNome = 0; // A ordem foi alterada.
            printf("\nComponente '%s' descartado com sucesso.\n", nomeBusca);
            break;
        }
    }

    if (!encontrado) {
        printf("\nComponente '%s' não encontrado na mochila.\n", nomeBusca);
    }
}

// Exibe a lista de componentes formatada.
void mostrarComponentes() {
    if (numComponentes == 0) {
        printf("\nNenhum componente cadastrado.\n");
        return;
    }

    printf("\n--- Componentes em Inventário ---\n");
    printf("+------------------------------+--------------------+------------+\n");
    printf("| %-28s | %-18s | %-10s |\n", "Nome", "Tipo", "Prioridade");
    printf("+------------------------------+--------------------+------------+\n");

    for (int i = 0; i < numComponentes; i++) {
        printf("| %-28s | %-18s | %-10d |\n",
               torre[i].nome, torre[i].tipo, torre[i].prioridade);
    }

    printf("+------------------------------+--------------------+------------+\n");
}

// --- Algoritmos de Ordenação ---

// Bubble Sort para ordenar por nome
long long bubbleSortNome() {
    long long comparacoesCount = 0;
    int i, j;
    Componente temp;
    for (i = 0; i < numComponentes - 1; i++) {
        for (j = 0; j < numComponentes - i - 1; j++) {
            comparacoesCount++;
            if (strcmp(torre[j].nome, torre[j + 1].nome) > 0) {
                temp = torre[j];
                torre[j] = torre[j + 1];
                torre[j + 1] = temp;
            }
        }
    }
    ordenadoPorNome = 1;
    return comparacoesCount;
}

// Insertion Sort para ordenar por tipo
long long insertionSortTipo() {
    long long comparacoesCount = 0;
    int i, j;
    Componente chave;
    for (i = 1; i < numComponentes; i++) {
        chave = torre[i];
        j = i - 1;
        while (j >= 0) {
            comparacoesCount++;
            if (strcmp(torre[j].tipo, chave.tipo) > 0) {
                torre[j + 1] = torre[j];
                j = j - 1;
            } else {
                break;
            }
        }
        torre[j + 1] = chave;
    }
    ordenadoPorNome = 0;
    return comparacoesCount;
}

// Selection Sort para ordenar por prioridade
long long selectionSortPrioridade() {
    long long comparacoesCount = 0;
    int i, j, min_idx;
    Componente temp;
    for (i = 0; i < numComponentes - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < numComponentes; j++) {
            comparacoesCount++;
            if (torre[j].prioridade < torre[min_idx].prioridade) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            temp = torre[i];
            torre[i] = torre[min_idx];
            torre[min_idx] = temp;
        }
    }
    ordenadoPorNome = 0;
    return comparacoesCount;
}

// --- Funções de Lógica do Jogo ---

// Menu para escolher o critério de ordenação
void menuOrdenacao() {
    if (numComponentes <= 1) {
        printf("\nNão há componentes suficientes para organizar.\n");
        return;
    }

    int opcao;
    printf("\n--- Escolha o Critério de Organização ---\n");
    printf("1. Por Nome (Bubble Sort)\n");
    printf("2. Por Tipo (Insertion Sort)\n");
    printf("3. Por Prioridade (Selection Sort)\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);
    limparBuffer();

    long long (*algoritmo)(void);

    switch (opcao) {
        case 1:
            algoritmo = bubbleSortNome;
            break;
        case 2:
            algoritmo = insertionSortTipo;
            break;
        case 3:
            algoritmo = selectionSortPrioridade;
            break;
        default:
            printf("\nOpção inválida. Nenhum algoritmo foi executado.\n");
            return;
    }
    
    medirTempoEExecutar(algoritmo);
    mostrarComponentes();
}

// Wrapper para medir tempo de execução de um algoritmo de ordenação
void medirTempoEExecutar(long long (*algoritmoDeOrdenacao)()) {
    clock_t inicio, fim;
    double tempoExecucao;

    inicio = clock();
    long long comparacoes = algoritmoDeOrdenacao();
    fim = clock();
    tempoExecucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("\nOrganização concluída com sucesso!\n");
    printf("Comparações realizadas: %lld\n", comparacoes);
    printf("Tempo de execução: %.6f segundos\n", tempoExecucao);
}

// Realiza a busca binária pelo nome.
void buscaBinariaPorNome() {
    if (numComponentes == 0) {
        printf("\nNão há componentes para buscar.\n");
        return;
    }
    if (!ordenadoPorNome) {
        printf("\nErro: A lista deve estar ordenada por nome para usar a busca binária.\n");
        printf("Por favor, organize-a por nome na opção 4.\n");
        return;
    }

    char componenteChave[] = "chip central"; // Componente chave para a busca
    int encontrado = 0;
    long long comparacoesCount = 0;

    int inicio = 0;
    int fim = numComponentes - 1;
    int meio;
    
    char nomeChaveLower[30];
    strcpy(nomeChaveLower, componenteChave);
    stringParaMinusculo(nomeChaveLower);

    printf("\n--- Buscando o Componente-Chave '%s' ---\n", componenteChave);

    while (inicio <= fim) {
        comparacoesCount++;
        meio = inicio + (fim - inicio) / 2;
        
        char componenteNomeLower[30];
        strcpy(componenteNomeLower, torre[meio].nome);
        stringParaMinusculo(componenteNomeLower);
        
        int cmp = strcmp(componenteNomeLower, nomeChaveLower);

        if (cmp == 0) {
            printf("\n=========================================\n");
            printf("  COMPONENTE CHAVE ENCONTRADO!         \n");
            printf("  TORRE DE FUGA PODE SER ATIVADA!      \n");
            printf("=========================================\n");
            printf("Detalhes do Componente:\n");
            printf("  Nome: %s\n", torre[meio].nome);
            printf("  Tipo: %s\n", torre[meio].tipo);
            printf("  Prioridade: %d\n", torre[meio].prioridade);
            encontrado = 1;
            break;
        } else if (cmp < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    if (!encontrado) {
        printf("\nO componente-chave '%s' não foi encontrado.\n", componenteChave);
    }
    printf("Total de comparações: %lld\n", comparacoesCount);
}