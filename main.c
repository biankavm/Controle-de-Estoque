#include <stdio.h>
#include <string.h>
#include <stdlib.h>
# define nomeArquivo "ControleDeEstoque" // nome do arquivo do estoque.

typedef struct tipoItem{
    char nome[30];
    float preco;
    int qtd;
} tipoItem;

typedef struct tipoNo{
    tipoItem dado;
    struct tipoNo *prox;
} tipoNo;

typedef struct tipoLista{
    tipoNo *prim;
    int tamanho;
} tipoLista;

void criaLista(tipoLista *l){
    l->prim = NULL;
    l->tamanho = 0;
}

void insereNaLista(tipoLista *l, tipoItem dado){
    tipoNo *aux, *tmp;
    aux = (tipoNo*) malloc(sizeof(tipoNo));
    aux->dado = dado;
    aux->prox = NULL;
    if (!l->prim){ // se inicialmente a lista está vazia.
        l->prim = aux;
    }
    else{
        tmp = l->prim;
        while(tmp->prox){ // caminhar até o último da lista.
            tmp = tmp->prox;
        }
        tmp->prox = aux; // inserir item no final.
        l->tamanho++; // aumentar 1 no tamanho.
    }
}

void adicionarItem(tipoLista *l, tipoItem item){
    printf("Digite o nome do item: \n");
    scanf("%[^\n]%*c", item.nome);
    
    printf("Digite o valor estimado: \n");
    scanf("%f%*c", &item.preco);

    printf("Digite a quantidade: \n");
    scanf("%d%*c", &item.qtd);

    insereNaLista(l, item);
}

void mostraItem(tipoItem item){
    printf("\n=============================\n");
    printf("Item: %s\n", item.nome);
    
    printf("Valor: %.2f\n",item.preco);

    printf("Quantidade: %d\n", item.qtd); 

    printf("\n=============================\n"); 
}


void mostraLista(tipoLista *l){
    tipoNo *aux;
    aux = l->prim;
    while(aux){
        mostraItem(aux->dado);
        aux = aux->prox;
    }
}

void selecionaOperacao(){
    printf("Seja bem-vindo ao sistema de controle de estoque.\n");
    printf("1 - Mostrar itens \n");
    printf("2 - Adicionar itens \n");
    printf("3 - Remover item \n");
    printf("4 - Sair do controle de estoque \n");
    printf("Digite o processo que deseja realizar:\n"); 
}

void salvarItemsEmArquivo(tipoLista *l){
    FILE *fp;
    fp = fopen(nomeArquivo, "w");
    if (fp == NULL){
        printf("Erro ao criar o arquivo.\n");
        exit(1);
    }
    tipoNo *aux;
    aux = l->prim;
    while(aux){ // grava dados da lista encadeada em um arquivo de texto.
        fprintf(fp, "Item: %s\n", aux->dado.nome);
        fprintf(fp, "Preço: %.2f\n", aux->dado.preco);
        fprintf(fp, "Quantidade: %d\n", aux->dado.qtd);
        aux = aux->prox;
    }
    fclose(fp);
}

void carregaEstoqueAnterior(tipoLista *l){
    FILE *fp;
    tipoItem item;
    fp = fopen(nomeArquivo, "r");
    if (fp == NULL){
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }
    while(fscanf(fp, "%*[ :]%[^\n]%*c", item.nome) == 1){
        fscanf(fp, "%*[ :]%f%*c", &item.preco);
        fscanf(fp, "%*[ :]%d%*c)", &item.qtd);
        mostraItem(item);
        insereNaLista(l, item);

    }
    fclose(fp);
}

char removeItemDaLista(tipoLista *l, char *nome){
    tipoNo *aux, *tmp;
    if (l->prim){ // se há elementos para remover.
        aux = l->prim;
        if (!strcmp(aux->dado.nome, nome)){ // se o que será removido é o primeiro da lista.
            l->prim = aux->prox;
            free(aux);
            return 1;
        }
        else{
            tmp = aux->prox;
            while(tmp){
                if(!strcmp(tmp->dado.nome, nome)){
                    aux->prox = tmp->prox;
                    free(tmp);
                    return 1;
                }
            }
        }
    }
    return 0;

}

void lerDadosParaEstoque(tipoLista *l){
    int op;
    selecionaOperacao();
    scanf("%d%*c", &op);
    

    if (op == 1){        // Se a operação for "1", mostramos os elementos do estoque.
        carregaEstoqueAnterior(l);       
        mostraLista(l);
    }
    else if(op == 2){     // Se a operação for "2", adicionemos um novo elemento no estoque.
        tipoItem item;
        carregaEstoqueAnterior(l);    
        adicionarItem(l, item);
        printf("Estoque após a adição do item: \n");
        mostraLista(l);
        salvarItemsEmArquivo(l);
    }
    else if(op == 3){
        carregaEstoqueAnterior(l);  
        char nomeItem[30];
        printf("Digite o nome do item que deseja remover: \n");
        scanf("%[^\n]%*c", nomeItem);
        if(removeItemDaLista(l, nomeItem)){
            printf("Elemento removido com sucesso.\n");
            printf("Estoque após a remoção do item: \n");
            mostraLista(l);
        }
        else{
            printf("Erro ao remover item.\n");
        }
    }
}






int main(){
    tipoLista l;
    criaLista(&l);
    tipoItem item;  
    carregaEstoqueAnterior(&l);
    mostraLista(&l);

}