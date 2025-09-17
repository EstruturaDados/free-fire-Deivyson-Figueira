#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// --- Definição da struct Item ---
// A struct Item armazena as informações essenciais de cada objeto no inventário.
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// --- Variáveis Globais ---
// mochila: Vetor de structs que representa a mochila do jogador, com capacidade para 10 itens.
// numItens: Contador que rastreia o número de itens atualmente na mochila.
Item mochila[10];
int numItens = 0;

// --- Protótipos de Funções ---
// Declarações das funções para que o compilador saiba que elas existem.
void limparBuffer();
void inserirItem();
void removerItem();
void listarItens();
void buscarItem();
void stringParaMinusculo(char *str);

int main() {
    int opcao;

    // Laço principal que exibe o menu e controla o fluxo do programa.
    do {
        printf("\n--- Mochila de sobrevivência - Código da Ilha ---\n");
        printf("1. Inserir novo item\n");
        printf("2. Remover item\n");
        printf("3. Listar todos os itens\n");
        printf("4. Buscar item\n");
        printf("0. Sair\n");
        printf("-------------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBuffer(); // Limpa o buffer do teclado após a leitura de um inteiro.

        switch (opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                buscarItem();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida! Por favor, tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

// --- Implementação das Funções ---

// Função para limpar o buffer de entrada do teclado,
// resolvendo problemas com a mistura de scanf e fgets.
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função auxiliar que converte uma string inteira para minúsculo.
void stringParaMinusculo(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Função que permite ao jogador adicionar um novo item à mochila.
void inserirItem() {
    if (numItens >= 10) {
        printf("A mochila está cheia! Não é possível adicionar mais itens.\n");
        return;
    }

    printf("\n--- Cadastrar Novo Item ---\n");

    // Solicita e lê o nome do item.
    printf("Digite o nome do item: ");
    // fgets lê a linha inteira, incluindo a quebra de linha '\n'.
    fgets(mochila[numItens].nome, sizeof(mochila[numItens].nome), stdin);
    // strcspn encontra a posição da primeira quebra de linha e a substitui por '\0'.
    mochila[numItens].nome[strcspn(mochila[numItens].nome, "\n")] = '\0';

    // Solicita e lê o tipo do item.
    printf("Digite o tipo do item (ex: arma, munição, cura): ");
    fgets(mochila[numItens].tipo, sizeof(mochila[numItens].tipo), stdin);
    mochila[numItens].tipo[strcspn(mochila[numItens].tipo, "\n")] = '\0';

    // Solicita e lê a quantidade do item.
    printf("Digite a quantidade: ");
    scanf("%d", &mochila[numItens].quantidade);
    limparBuffer();

    numItens++; // Incrementa o contador de itens na mochila.
    printf("Item '%s' adicionado com sucesso!\n", mochila[numItens - 1].nome);
}

// Função que permite ao jogador remover um item da mochila pelo nome.
void removerItem() {
    if (numItens == 0) {
        printf("A mochila está vazia. Não há itens para remover.\n");
        return;
    }

    char nomeBusca[30];
    char nomeBuscaLower[30]; // Versão em minúsculo para a busca
    
    printf("\n--- Remover Item ---\n");
    printf("Digite o nome do item para remover: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    // Copia e converte a string de busca para minúsculo.
    strcpy(nomeBuscaLower, nomeBusca);
    stringParaMinusculo(nomeBuscaLower);

    int i, encontrado = 0;
    // Percorre o vetor para encontrar o item.
    for (i = 0; i < numItens; i++) {
        char itemNomeLower[30]; // Versão em minúsculo para o nome do item
        strcpy(itemNomeLower, mochila[i].nome);
        stringParaMinusculo(itemNomeLower);

        // Compara as strings em minúsculo para uma busca case-insensitive e exata.
        if (strcmp(itemNomeLower, nomeBuscaLower) == 0) {
            // Se o item for encontrado, move os itens seguintes uma posição para trás.
            int j;
            for (j = i; j < numItens - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            numItens--; // Decrementa o contador de itens.
            encontrado = 1;
            printf("Item '%s' removido com sucesso.\n", mochila[i].nome);
            break; // Sai do laço após a remoção.
        }
    }

    if (!encontrado) {
        printf("Item '%s' não encontrado na mochila.\n", nomeBusca);
    }
}

// Função que exibe uma lista de todos os itens atualmente na mochila.
void listarItens() {
    printf("\n--- Itens na Mochila (%d/10) ---\n", numItens);

    if (numItens == 0) {
        printf("A mochila está vazia.\n");
        return;
    }

    // Cabeçalho da tabela
    printf("+------------------------------+--------------------+----------+\n");
    printf("| %-28s | %-18s | %-8s |\n", "Nome", "Tipo", "Qtd");
    printf("+------------------------------+--------------------+----------+\n");

    // Conteúdo da tabela
    for (int i = 0; i < numItens; i++) {
        printf("| %-28s | %-18s | %-8d |\n", mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }

    // Rodapé da tabela
    printf("+------------------------------+--------------------+----------+\n");
}

// Função que busca um item específico e agora permite busca por substring e ignora maiúsculas/minúsculas.
void buscarItem() {
    if (numItens == 0) {
        printf("A mochila está vazia. Não há itens para buscar.\n");
        return;
    }

    char nomeBusca[30];
    char nomeBuscaLower[30]; // String temporária para a busca em minúsculo

    printf("\n--- Buscar Item ---\n");
    printf("Digite o nome do item para buscar: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    // Copia a string digitada e a converte para minúsculo para a busca.
    strcpy(nomeBuscaLower, nomeBusca);
    stringParaMinusculo(nomeBuscaLower);

    int encontrado = 0;
    // Percorre o vetor de forma sequencial para encontrar o item.
    for (int i = 0; i < numItens; i++) {
        char itemNomeLower[30]; // String temporária para o nome do item na mochila
        
        // Copia o nome do item da mochila e o converte para minúsculo para a comparação.
        strcpy(itemNomeLower, mochila[i].nome);
        stringParaMinusculo(itemNomeLower);
        
        // strstr() busca uma substring dentro de outra.
        // Se a string da busca for encontrada no nome do item (ambos em minúsculo),
        // a condição é verdadeira.
        if (strstr(itemNomeLower, nomeBuscaLower) != NULL) {
            printf("\n--- Item Encontrado ---\n");
            printf("  Nome: %s\n", mochila[i].nome);
            printf("  Tipo: %s\n", mochila[i].tipo);
            printf("  Quantidade: %d\n", mochila[i].quantidade);
            encontrado = 1;
            break; // Sai do laço após encontrar o item.
        }
    }

    if (!encontrado) {
        printf("Item '%s' não foi encontrado na mochila.\n", nomeBusca);
    }
}