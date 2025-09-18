#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// --- Definição das Structs ---

// A struct Item armazena as informações essenciais de cada objeto.
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// A struct No representa um nó na lista encadeada.
// Contém os dados do Item e um ponteiro para o próximo nó.
typedef struct No {
    Item dados;
    struct No* proximo;
} No;

// --- Variáveis Globais ---
// Mochila é representada pelo ponteiro 'mochila' que aponta para o primeiro nó da lista.
// Se a mochila estiver vazia, este ponteiro será NULL.
No* mochila = NULL;
// Contador de itens para exibir no menu.
int numItens = 0;

// --- Protótipos de Funções ---
void limparBuffer();
void stringParaMinusculo(char *str);
void inserirItem();
void removerItem();
void listarItens();
void buscarItem();
void liberarMochila();

int main() {
    int opcao;

    // Laço principal que exibe o menu e controla o fluxo do programa.
    do {
        // --- Exibe o contador de itens no topo do menu ---
        printf("------------------------")
        printf("\nItens na mochila: %d\n", numItens);
        printf("------------------------")

        printf("----------------------------------------------------")
        printf("\n--- MOCHILA DE SOBREVIVÊNCIA - CÓDIGO DA ILHA ---\n");
        printf("----------------------------------------------------")
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
                liberarMochila(); // Libera a memória alocada antes de sair.
                break;
            default:
                printf("Opção inválida! Por favor, tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

// --- Implementação das Funções ---

// Função para limpar o buffer de entrada do teclado.
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
// A inserção é feita no início da lista.
void inserirItem() {
    // Aloca dinamicamente um novo nó.
    No* novoNo = (No*)malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("Erro de alocação de memória. Não foi possível adicionar o item.\n");
        return;
    }

    printf("\n--- Cadastrar Novo Item ---\n");
    printf("Digite o nome do item: ");
    fgets(novoNo->dados.nome, sizeof(novoNo->dados.nome), stdin);
    novoNo->dados.nome[strcspn(novoNo->dados.nome, "\n")] = '\0';

    printf("Digite o tipo do item (ex: arma, munição, cura): ");
    fgets(novoNo->dados.tipo, sizeof(novoNo->dados.tipo), stdin);
    novoNo->dados.tipo[strcspn(novoNo->dados.tipo, "\n")] = '\0';

    printf("Digite a quantidade: ");
    scanf("%d", &novoNo->dados.quantidade);
    limparBuffer();

    // O novo nó aponta para o que era o primeiro da lista.
    novoNo->proximo = mochila;
    // O ponteiro da mochila agora aponta para o novo nó.
    mochila = novoNo;
    
    // Incrementa o contador de itens.
    numItens++;

    printf("Item '%s' adicionado com sucesso!\n", novoNo->dados.nome);
}

// Função que permite ao jogador remover um item da mochila pelo nome.
void removerItem() {
    if (mochila == NULL) {
        printf("A mochila está vazia. Não há itens para remover.\n");
        return;
    }

    char nomeBusca[30];
    char nomeBuscaLower[30];
    
    printf("\n--- Remover Item ---\n");
    printf("Digite o nome do item para remover: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
    
    strcpy(nomeBuscaLower, nomeBusca);
    stringParaMinusculo(nomeBuscaLower);

    No* atual = mochila;
    No* anterior = NULL;
    int encontrado = 0;

    // Percorre a lista para encontrar o item.
    while (atual != NULL) {
        char itemNomeLower[30];
        strcpy(itemNomeLower, atual->dados.nome);
        stringParaMinusculo(itemNomeLower);

        // Compara as strings em minúsculo.
        if (strcmp(itemNomeLower, nomeBuscaLower) == 0) {
            if (anterior == NULL) {
                // Se o item a ser removido é o primeiro (cabeça).
                mochila = atual->proximo;
            } else {
                // Se o item está no meio ou no fim da lista.
                anterior->proximo = atual->proximo;
            }
            free(atual); // Libera a memória do nó removido.
            encontrado = 1;
            // Decrementa o contador de itens.
            numItens--;
            printf("Item '%s' removido com sucesso.\n", nomeBusca);
            break; // Sai do laço após a remoção.
        }
        anterior = atual;
        atual = atual->proximo;
    }

    if (!encontrado) {
        printf("Item '%s' não encontrado na mochila.\n", nomeBusca);
    }
}

// Função que exibe uma lista de todos os itens na mochila.
void listarItens() {
    printf("\n--- Itens na Mochila ---\n");

    if (mochila == NULL) {
        printf("A mochila está vazia.\n");
        return;
    }

    No* atual = mochila;
    // Cabeçalho da tabela
    printf("+------------------------------+--------------------+----------+\n");
    printf("| %-28s | %-18s | %-8s |\n", "Nome", "Tipo", "Qtd");
    printf("+------------------------------+--------------------+----------+\n");

    // Percorre a lista do início ao fim.
    while (atual != NULL) {
        printf("| %-28s | %-18s | %-8d |\n", atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
        atual = atual->proximo;
    }

    printf("+------------------------------+--------------------+----------+\n");
}

// Função que busca um item específico na lista encadeada.
void buscarItem() {
    if (mochila == NULL) {
        printf("A mochila está vazia. Não há itens para buscar.\n");
        return;
    }

    char nomeBusca[30];
    char nomeBuscaLower[30];

    printf("\n--- Buscar Item ---\n");
    printf("Digite o nome do item para buscar: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';
    
    strcpy(nomeBuscaLower, nomeBusca);
    stringParaMinusculo(nomeBuscaLower);

    No* atual = mochila;
    int encontrado = 0;

    // Percorre a lista sequencialmente.
    while (atual != NULL) {
        char itemNomeLower[30];
        strcpy(itemNomeLower, atual->dados.nome);
        stringParaMinusculo(itemNomeLower);
        
        // strstr() busca uma substring dentro de outra.
        if (strstr(itemNomeLower, nomeBuscaLower) != NULL) {
            printf("\n--- Item Encontrado ---\n");
            printf("  Nome: %s\n", atual->dados.nome);
            printf("  Tipo: %s\n", atual->dados.tipo);
            printf("  Quantidade: %d\n", atual->dados.quantidade);
            encontrado = 1;
            break; // Sai do laço após encontrar o item.
        }
        atual = atual->proximo;
    }

    if (!encontrado) {
        printf("Item '%s' não foi encontrado na mochila.\n", nomeBusca);
    }
}

// Função para liberar a memória alocada por todos os nós da lista antes do programa terminar.
void liberarMochila() {
    No* atual = mochila;
    No* proximo;
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    mochila = NULL; // Opcional, garante que a lista esteja vazia.
}