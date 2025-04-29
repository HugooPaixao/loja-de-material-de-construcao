#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int itensDisponiveis = 7;
char *arquivo_de_vendas = "vendas.txt";
float *precos;

// dados 
struct Item {
    char nome[20];
    float preco;
    int quantidade;
};

// lista dos itens disponiveis
char* lista_de_itens[7] = {"Areia", "Tijolo", "Telha", "Cimento", "Argamassa", "Pisos", "Rejunte"};

void removerCaracteresExtras() {
    int limpar;
    while ((limpar = getchar()) != '\n' && limpar != EOF);
}

// aloca memoria para o preco dos itens
void inicializarPrecos() {
    precos = malloc(itensDisponiveis* sizeof(float));
    precos[0] = 400.0;  // Areia
    precos[1] = 1.0;    // Tijolo
    precos[2] = 1.5;    // Telha
    precos[3] = 40.0;   // Cimento
    precos[4] = 19.0;   // Argamassa
    precos[5] = 43.0;   // Pisos
    precos[6] = 7.0;    // Rejunte
}
//libera a memoria alocada para precos
void liberarItens() {
    free(precos);
}

void registrarVenda(struct Item *item) {
    FILE *arquivo = fopen(arquivo_de_vendas, "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de vendas.\n");
        return;
    }
    
    fprintf(arquivo, "Item: %s, Preco: %.2f, Quantidade: %d\n", 
            item->nome, item->preco, item->quantidade);
    fclose(arquivo);
    printf("Venda registrada.\n");
}

void notasVendas() {
    FILE *arquivo = fopen(arquivo_de_vendas, "r");
    if (arquivo == NULL) {
        printf("Nenhuma venda registrada.\n");
        return;
    }
    
    float total = 0;
    struct Item item;
    int contador = 0;

    printf("\nNotas de Venda:\n");
    while(fscanf(arquivo, "Item: %19[^,], Preco: %f, Quantidade: %d\n", 
                item.nome, &item.preco, &item.quantidade) == 3) {
        total += item.preco * item.quantidade;
        contador++;
        printf("%d. %s - Qtd: %d - Preco: R$%.2f\n", 
               contador, item.nome, item.quantidade, item.preco);
    }
    
    if (contador == 0) {
        printf("Nenhuma venda registrada.\n");
    } else {
        printf("\nTotal de vendas: R$%.2f\n", total);
    }
    
    fclose(arquivo);
}

void excluirVenda() {
    FILE *arquivoAtual = fopen(arquivo_de_vendas, "r");
    FILE *novoArquivo = fopen("arquivoNovo.txt", "w");
    
    if (arquivoAtual == NULL || novoArquivo == NULL) {
        printf("Erro ao manipular arquivos.\n");
        return;
    }
    
    struct Item item;
    int linhaAlvo, contador = 0, linhaParaExcluir;

    // conta e mostra as vendas
    while(fscanf(arquivoAtual, "Item: %19[^,], Preco: %f, Quantidade: %d\n", 
                item.nome, &item.preco, &item.quantidade) == 3) {
        contador++;
    }

    if (contador == 0) {
        printf("Nenhuma venda para excluir.\n");
        fclose(arquivoAtual);
        fclose(novoArquivo);
        return;
    }

    printf("\nNotas de Venda:\n");
    rewind(arquivoAtual);
    contador = 0;
    while(fscanf(arquivoAtual, "Item: %19[^,], Preco: %f, Quantidade: %d\n", 
                item.nome, &item.preco, &item.quantidade) == 3) {
        contador++;
        printf("%d. %s - Qtd: %d - Preco: R$%.2f\n", 
               contador, item.nome, item.quantidade, item.preco);
    }

    printf("\nDigite o numero da venda para excluir (0 para cancelar): ");
    scanf("%d", &linhaParaExcluir);
    removerCaracteresExtras();

    if (linhaParaExcluir == 0 || linhaParaExcluir > contador) {
        printf("Operacao cancelada.\n");
        fclose(arquivoAtual);
        fclose(novoArquivo);
        remove("arquivoNovo.txt");
        return;
    }

    // reescreve arquivo excluindo linha escolhida
    rewind(arquivoAtual);
    contador = 0;
    while(fscanf(arquivoAtual, "Item: %19[^,], Preco: %f, Quantidade: %d\n", 
                item.nome, &item.preco, &item.quantidade) == 3) {
        contador++;
        if(contador != linhaParaExcluir) {
            fprintf(novoArquivo, "Item: %s, Preco: %.2f, Quantidade: %d\n", 
                    item.nome, item.preco, item.quantidade);
        }
    }

    fclose(arquivoAtual);
    fclose(novoArquivo);

    remove(arquivo_de_vendas);
    rename("arquivoNovo.txt", arquivo_de_vendas);
    printf("Venda removida!\n");
}

void editarVenda() {
    FILE *arquivo = fopen(arquivo_de_vendas, "r");
    FILE *arquivoNovo = fopen("arquivoNovo.txt", "w");
    
    if (arquivo == NULL || arquivoNovo == NULL) {
        printf("Erro ao manipular arquivos.\n");
        return;
    }
    
    struct Item item;
    int contador = 0, linhaParaEditar, opcao, novaQtd;

    // conta e mostra as vendas
    while(fscanf(arquivo, "Item: %19[^,], Preco: %f, Quantidade: %d\n", 
                item.nome, &item.preco, &item.quantidade) == 3) {
        contador++;
    }

    if (contador == 0) {
        printf("Nenhuma venda para editar.\n");
        fclose(arquivo);
        fclose(arquivoNovo);
        remove("arquivoNovo.txt");
        return;
    }

    printf("\nNotas de Venda:\n");
    rewind(arquivo);
    contador = 0;
    while(fscanf(arquivo, "Item: %19[^,], Preco: %f, Quantidade: %d\n", 
                item.nome, &item.preco, &item.quantidade) == 3) {
        contador++;
        printf("%d. %s - Qtd: %d - Preco: R$%.2f\n", 
               contador, item.nome, item.quantidade, item.preco);
    }

    printf("\nDigite o numero da venda para editar (0 para cancelar): ");
    scanf("%d", &linhaParaEditar);
    removerCaracteresExtras();

    if (linhaParaEditar == 0 || linhaParaEditar > contador) {
        printf("Operacao cancelada.\n");
        fclose(arquivo);
        fclose(arquivoNovo);
        remove("arquivoNovo.txt");
        return;
    }

    // reescreve arquivo editando linha escolhida
    rewind(arquivo);
    contador = 0;
    while(fscanf(arquivo, "Item: %19[^,], Preco: %f, Quantidade: %d\n", 
                item.nome, &item.preco, &item.quantidade) == 3) {
        contador++;
        
        if(contador == linhaParaEditar) {
            printf("\nSelecione novo item:\n");
            for(int i = 0; i < itensDisponiveis; i++) 
                printf("%d. %s\n", i+1, lista_de_itens[i]);
            
            printf("Opcao: ");
            scanf("%d", &opcao);
            removerCaracteresExtras();
            
            if (opcao < 1 || opcao > itensDisponiveis) {
                printf("Opcao invalida, portanto o item será mantido\n");
            } else {
                strcpy(item.nome, lista_de_itens[opcao-1]);
                item.preco = precos[opcao-1];
                
                printf("Nova quantidade: ");
                scanf("%d", &novaQtd);
                removerCaracteresExtras();
                item.quantidade = novaQtd;
            }
        }
        
        fprintf(arquivoNovo, "Item: %s, Preco: %.2f, Quantidade: %d\n", 
                item.nome, item.preco, item.quantidade);
    }

    fclose(arquivo);
    fclose(arquivoNovo);

    remove(arquivo_de_vendas);
    rename("arquivoNovo.txt", arquivo_de_vendas);
    printf("Venda atualizada!\n");
}

void processarOpcao(int opcao) {
    struct Item *item = malloc(sizeof(struct Item));
    if (item == NULL) {
        printf("Erro ao alocar memória para o item.\n");
        return;
    }

    switch (opcao) {
        case 0:
            printf("Volte Sempre!\n");
            break;

        case 1:
            printf("Quantas carradas de areia voce quer?\n");
            scanf("%i", &item->quantidade);
            item->preco = precos[0];
            strcpy(item->nome, lista_de_itens[0]);
            registrarVenda(item);
            break;

        case 2:
            printf("Quantos Tijolos voce quer?\n");
            scanf("%i", &item->quantidade);
            item->preco = precos[1];
            strcpy(item->nome, lista_de_itens[1]);
            registrarVenda(item);
            break;

        case 3:
            printf("Quantas Telhas voce quer?\n");
            scanf("%i", &item->quantidade);
            item->preco = precos[2];
            strcpy(item->nome, lista_de_itens[2]);
            registrarVenda(item);
            break;

        case 4:
            printf("Quantos sacos de cimento voce quer?\n");
            scanf("%i", &item->quantidade);
            item->preco = precos[3];
            strcpy(item->nome, lista_de_itens[3]);
            registrarVenda(item);
            break;

        case 5:
            printf("Quantos sacos de argamassa voce quer?\n");
            scanf("%i", &item->quantidade);
            item->preco = precos[4];
            strcpy(item->nome, lista_de_itens[4]);
            registrarVenda(item);
            break;

        case 6:
            printf("Quantas caixas de piso voce quer?\n");
            scanf("%i", &item->quantidade);
            item->preco = precos[5];
            strcpy(item->nome, lista_de_itens[5]);
            registrarVenda(item);
            break;

        case 7:
            printf("Quantos pacotes de rejunte voce quer?\n");
            scanf("%i", &item->quantidade);
            item->preco = precos[6];
            strcpy(item->nome, lista_de_itens[6]);
            registrarVenda(item);
            break;

        case 8:
            notasVendas();
            break;

        case 9:
            editarVenda();
            break;

        case 10:
            excluirVenda();
            break;

        default:
            printf("Opcao invalida.\n");
    }

    free(item);
}

void imprimirOpcoes() {
    printf("Escolha uma opcao:\n");
    printf("+-----------------------------------+\n");
    printf("| 0- Sair                           |\n");
    printf("| 1- Carrada de Areia (R$ 400,00)   |\n");
    printf("| 2- Tijolo (R$ 1,00)               |\n");
    printf("| 3- Telha (R$ 1,50)                |\n");
    printf("| 4- Saco de Cimento (R$ 40,00)     |\n");
    printf("| 5- Saco de Argamassa (R$ 19,00)   |\n");
    printf("| 6- Caixa de Pisos (R$ 43,00)      |\n");
    printf("| 7- Pacote de Rejunte (R$ 7,00)    |\n");
    printf("| 8- Vendas acumuladas              |\n");
    printf("| 9- Editar item da venda           |\n");
    printf("| 10- Excluir venda                 |\n");
    printf("+-----------------------------------+\n");
}

int main() {
    inicializarPrecos();

    int opcao;
    do {
        imprimirOpcoes();
        printf("Digite a sua opcao:\n");
        scanf("%i", &opcao);
        removerCaracteresExtras();
        processarOpcao(opcao);
    } while(opcao != 0);

    liberarItens();
    return 0;
}
