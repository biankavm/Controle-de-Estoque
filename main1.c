/* Desafio proposto pelo Neeps Academy, propondo a elaboração de um programa que faça o controle de um estoque de produtos.*/

/*
Desenvolvi esse projeto com o objetivo de me desafiar no âmbito da programação e pôr em prática os conhecimentos adquiridos
na disciplina de Algoritmos e Estruturas de Dados I.

Fiz o uso de listas encadeadas dinâmicas e manipulação de arquivos para desenvolver esse sistema. Espero conseguir contribuir
em novas ideias e possíveis aprimoramentos para esse projeto :D

Funcionamento:
De um modo geral, ao iniciar o programa, pede-se ao usuário que informe a operação que será realizada, dentre elas:
1 - Mostrar estoque
2 - Adicionar item ao estoque
3 - Remover item do estoque
4 - Sair do sistema de controle

Ao executar a operação desejada, um arquivo em formato txt é atualizado / criado conforme as informações fornecidas pelo usuário.
O nome do arquivo é "ControleDeEstoque", e é nele que serão salvas as informações conforme o decorrer do programa.

By: Bianka Vasconcelos.
*/



#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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
    if (!l->prim){
        l->prim = aux;
    }
    else{
        tmp = l->prim;
        while(tmp->prox){ 
            tmp = tmp->prox;
        }
        tmp->prox = aux; 
        l->tamanho++; 
    }
}


void mostraItem(tipoItem item){
    
    printf("Item: %s\n", item.nome);
    
    printf("Valor: %.2f\n",item.preco);

    printf("Quantidade: %d\n", item.qtd); 

    
}

void mostraLista(tipoLista *l){
    tipoNo *aux;
    aux = l->prim;
    if (!aux){
        printf("Estoque vazio.\n");
    }
    else{
        printf("Estoque Atual");
        printf("\n=============================\n");
        while(aux){
            mostraItem(aux->dado);
            printf("\n");
            aux = aux->prox;
    }
    printf("============================="); 
    }
    
}

void adicionarItemNaLista(tipoLista *l){
    tipoItem item;

    printf("Digite o nome do item que quer adicionar: \n");
    scanf("%[^\n]%*c", item.nome);
    
    printf("Digite o valor estimado: \n");
    scanf("%f%*c", &item.preco);

    printf("Digite a quantidade: \n");
    scanf("%d%*c", &item.qtd);

    insereNaLista(l, item);
    mostraLista(l);
}

void removeItemDaLista(tipoLista *l){
    tipoNo *aux, *tmp;
    char nome[50];
    printf("Digite o nome do produto que deseja remover do estoque: ");
    scanf("%[^\n]%*c", nome);

    aux = l->prim;
    if (!strcmp(aux->dado.nome, nome)){ 
        l->prim = aux->prox;
        free(aux);
    }
    else{
        tmp = aux->prox;
        while(tmp){
            if(!strcmp(tmp->dado.nome, nome)){
                aux->prox = tmp->prox;
                free(tmp);
            }
            aux = tmp;
            tmp = tmp->prox;
        }
    }
    
    if (!l->prim){
        printf("Vazia.\n");
    }
    else{
        mostraLista(l);
    }
    
}

int mensagemEstoque(){
    int operacao;
    printf("\nSeja bem-vindo ao sistema de controle de estoque.\n");
    printf("1 - Mostrar itens \n");
    printf("2 - Adicionar itens \n");
    printf("3 - Remover item \n");
    printf("4 - Sair do controle de estoque \n");
    printf("\nDigite o processo que deseja realizar:\n"); 
    scanf("%d%*c", &operacao);
    return operacao;
}

void gravaListaEmArquivo(char *nomeArquivo, tipoLista *l){
    tipoNo *aux;
    FILE *fp;
    fp = fopen(nomeArquivo, "w");
    if (fp == NULL){
        printf("Erro ao criar o arquivo.\n");
    }
    if (l->prim){
        aux = l->prim;
        while(aux){
            fprintf(fp, "%s\n", aux->dado.nome);
            fprintf(fp, "%.2f\n", aux->dado.preco);
            fprintf(fp, "%d\n", aux->dado.qtd);
            aux = aux->prox;
    }
    }
    fclose(fp);
}


void carregaArquivoParaLista(char *nomeArquivo, tipoLista *l){
    FILE *fp;
    tipoItem item;
    fp = fopen(nomeArquivo, "r");
    if (fp == NULL){
        printf("Estoque vazio.\n");
        exit(1);
    }
    while(fscanf(fp, "%[^\n]%*c", item.nome) == 1){
        fscanf(fp, "%f%*c", &item.preco);
        fscanf(fp, "%d%*c", &item.qtd);

        insereNaLista(l, item);
    }
    fclose(fp);

}


void carregaArquivoParaAdicionarNaLista(char *nomeArquivo, tipoLista *l){
    FILE *fp;
    tipoItem item;
    fp = fopen(nomeArquivo, "r");
    if (fp == NULL){ // ao adicionar em lista vazia, o arquivo está vazio.
        adicionarItemNaLista(l);
    }
    else{               // carregar produtos do estoque anterior.
        while(fscanf(fp, "%[^\n]%*c", item.nome) == 1){
        fscanf(fp, "%f%*c", &item.preco);
        fscanf(fp, "%d%*c", &item.qtd);

        insereNaLista(l, item);
    }
    adicionarItemNaLista(l);
    }
    
    fclose(fp);

}

void digito1(tipoLista *l){
    carregaArquivoParaLista("ControleDeEstoque", l);
    mostraLista(l);

    
}

void digito2(tipoLista *l){
    carregaArquivoParaAdicionarNaLista("ControleDeEstoque", l);
    gravaListaEmArquivo("ControleDeEstoque", l);

}

void digito3(tipoLista *l){
    carregaArquivoParaLista("ControleDeEstoque", l);
    if (!l->prim){
        printf("Estoque vazio.\n");
        exit(1);
    }
    else{
        removeItemDaLista(l);
        gravaListaEmArquivo("ControleDeEstoque", l);
    }
    
}

void digito4(){
    printf("Voce saiu do controle de estoque.\n");
    exit(1);
}


int main(){
    tipoLista l;
    criaLista(&l);
    int op = mensagemEstoque();
    if (op == 1){
        digito1(&l);
    }
    else if (op == 2){
            digito2(&l);
        }
        
    else if (op == 3){
        digito3(&l);
    }
    else if (op == 4){
        digito4();
    }
}



