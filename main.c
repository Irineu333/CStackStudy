#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

//Irineu A. Silva

/*  Gerenciador de Filas/Pilhas com listas

    A ideia desse programa é ser um gerenciador de filas e pilhas, implementadas por listas.
    Literalmente uma lista de listas, com dois menus de operação, um para a lista principal e outro para as secundárias que podem se comportar como filas ou pilhas,
    O comportamente das listas secundárias pode ser alterado alterando o tipo, e nelas podem ser adicionado e removido cadastro de pessoas, atravez de push e pop

    Obs: o programa inicialemnte tinha a ideia de ser uma listas de filas, sendo adicionado o suporte a pilha posteriormente
*/


//structs
typedef struct lista
{
    int qtd;
    char nome[30];
    char tipo[5]; //fila ou pilha
    struct registro *inicio;
} lista;

typedef struct pessoa
{
    char nome[30];
    int idade;
} pessoa;

typedef struct registro
{
    pessoa *p;
    lista *endFila; //endereço da lista
    struct registro *prox;
} registro;

///foi incorporado minhas funções pessoais de entrada, que podem ser encontradas em https://github.com/Irineu333/Funcoes-de-Entrada-Perfeitas

//minhas funções pessoais de entrada
char* e_texto(int tam)
{
    char *temp;
    temp=(char*)malloc(sizeof(char)*tam);
    fgets(temp, tam, stdin);
    fflush(stdin); //uso não recomendado segundo alguns foruns, mas evita erros ralacionados ao buffer
    temp[strlen(temp)-1] = '\0';

    //foi identificado um erro relacionado com o buffer quando o usuario digita mais que o temanho especificado, o fflush(stdin) corrige isso

    return temp;
}

int e_inteiro()
{
    int temp;
    temp = atoi(e_texto(11));

    return temp;
}

//declarações
lista* aloca_lista();
void inserir_fila(lista *listas);
void lista_filas(lista *listas);
void incluir_na_fila (lista *l, int x);
registro* aloca_registro();
void abrir_fila (lista *l);
lista* buscar_fun (lista *l, char *x, int y, int op);

int main()
{
    char tempText[30];
    int op;
    //compatibilidade com portugues
    setlocale(LC_ALL, "portuguese");
    printf("\nGer. Pilhas/Filas\n");
    //criando lista principal
    lista *listas;
    listas = aloca_lista();

    do
    {
        printf("\n1.Criar");
        printf("\n2.Abrir");
        printf("\n3.Listar");
        printf("\n4.Sair\n");
        op=e_inteiro();

        switch(op)
        {
        case 1:
            printf("\n[+] Criar Fila/Pilha");
            inserir_fila(listas);
            break;
        case 2:
            printf("\nAbrir:\n");
            abrir_fila(listas);
            break;
        case 3:
            printf("\nReg. na memória:\n");
            lista_filas(listas);
            break;
        case 4:
            printf("\nSaindo do programa.");
            break;
        default:
            printf("\nOpcão inválida!\n");
            break;
        }

    }
    while(op!=4);

    return 0;
}
//

void lista_filas(lista *listas)
{
    /*
       função exclusiva da lista principal
       listas as listas (filas) presentes na lista principal
    */
    registro *aux;
    int i=1, tam[2] = {0, 0};

    if (listas->qtd==0)
    {
        printf("\nA lista está vazia!\n");
    }
    else
    {
        //

        //
        aux=listas->inicio;
        while(aux!=NULL)
        {
            if(aux!=NULL)
            {
                printf("\n[%d] %s (%s)| %d Registros | %d Kb", i, aux->endFila->nome, aux->endFila->tipo, aux->endFila->qtd, (sizeof(registro)+sizeof(pessoa))*(aux->endFila->qtd) + sizeof(lista));

                //Somando total
                tam[0] += aux->endFila->qtd; //quantidade total de registros
                tam[1] += (sizeof(registro)+sizeof(pessoa))*(aux->endFila->qtd) + sizeof(lista); //quantidade total de kb
            }
            aux=aux->prox;
            i++;
        }
        printf("\n");
    }
    printf("\nTotal: %d Filas/Pilhas | %d Registros | %d Kb\n",listas->qtd, tam[0], tam[1] + sizeof(lista)); //exibindo total
}
void inserir_fila(lista *listas)
{
    /*
       função exclusiva da lista principal
       alocar novas listas (filas) para adicionar na lista principal
    */

    registro *temp, *aux;
    int tipo;
    temp = aloca_registro();
    temp->endFila = aloca_lista(); //diferença mais fundamental da funão principal
    printf("\n");
    //recebendo
    do
    {
        printf("    Nome: ");
        strcpy(temp->endFila->nome, e_texto(30));
    }
    while(strlen(temp->endFila->nome)==0);
    do
    {
        printf("    Tipo: (1) fila, (2) pilha: ");
        tipo=e_inteiro();
    }
    while (tipo!=1 && tipo!=2);


    //tipificador
    if(tipo==1)
    {
        strcpy(temp->endFila->tipo, "fila");
    }
    else
    {
        if(tipo==2)
        {
            strcpy(temp->endFila->tipo, "pilha");
        }
        else
        {
            printf("\nErro inesperado\n");
        }
    }
    printf("    Nova %s criada!\n", temp->endFila->tipo);

    //estrutura padrão
    if(listas->inicio==NULL)
    {
        listas->inicio=temp;
    }
    else
    {
        aux = listas->inicio;
        while(aux->prox!=NULL)
        {
            aux=aux->prox; //estrutura padrão para avançar pela lista
        }
        aux->prox = temp;

    }
    //
    listas->qtd++;
}

//declarações
void push(lista *l);
void mostrar_tudo(lista *l);
void pop(lista *l, int i);
void stackpop(lista *l, int tipo);
void renomear(lista *l);
void mudar_tipo(lista *l);
void info(lista *l);
int excluir_op(lista *l);

int menu(lista *l)
{
    /*
        função para realizar operações dentro das filas
    */
    info(l);
    int op, tipo;

    if(!strcmp(l->tipo, "fila"))
    {

        tipo=1;
    }
    else
    {

        if(!strcmp(l->tipo, "pilha"))
        {
            tipo=2;
        }
        else
        {
            printf("\nErro inesperado\n");
        }
    }

    do
    {
        printf("\n1.Push");
        printf("\n2.Pop");
        printf("\n3.StackPop");
        printf("\n4.Mostrar tudo");
        printf("\n5.Renomear fila");
        printf("\n6.Excluir fila");
        printf("\n7.Mudar tipo");
        printf("\n8.Sair\n");
        op=e_inteiro();

        switch(op)
        {
        case 1:
            printf("\n[+] Criar Pessoa:\n");
            push(l);
            break;
        case 2:
            printf("\nPop %s:\n", l->tipo);
            pop(l, tipo); //1 para fila e 2 para pilha
            break;
        case 3:
            printf("\nStackPop %s:\n", l->tipo);
            stackpop(l, tipo);
            break;
        case 4:
            printf("\nRegistros na memória:\n");
            mostrar_tudo(l);
            break;
        case 5:
            printf("\nRenomear %s:", l->tipo);
            renomear(l);
            info(l);
            break;
        case 6:
            printf("\nExcluir %s:", l->tipo);
            if(excluir_op(l))
            {
                return 1;
            }
            break;
        case 7:
            printf("\nMudar tipo:");
            mudar_tipo(l);
            info(l);
            break;
        case 8:
            printf("\nSaindo da %s.\n", l->tipo);
            break;
        default:
            printf("\nOpcao invalida!\n");
            break;

        }

    }
    while(op!=8);

    return 0;
}
lista* aloca_lista()
{
    /*
        alocador universal de listas, mais especificamente a cabeça da lista
    */
    lista *temp;
    temp =(lista*)malloc(sizeof(lista));
    temp->qtd=0;
    temp->tipo[0]='\0';
    temp->nome[0]='\0';
    temp->inicio=NULL;

    return temp;
}

registro* aloca_registro()
{
    /*
        alocador universal de registros
    */
    registro *temp;
    temp =(registro*)malloc(sizeof(registro));
    temp->p = NULL;
    temp->endFila=NULL; //estrutura preenchida apela na lista principal
    temp->prox = NULL;
    return temp;
}
//
void abrir_fila (lista *listas)
{
    /*
        função responsavel por abrir filas
        na verdade ela apenar gerencia o uso da função buscar_por_filas e menu,
        esse ultimo responsável pelas opreações realizadas dentro de uma fila
    */
    lista *temp;
    int op, y;
    char nome[30];
    registro *aux, *ant_temp;
    if (listas->qtd==0)
    {
        printf("\nA lista está vazia!\n");
        return NULL;
    }
    else
    {
        printf("\nBuscar: (1) Nome, (2) Número: ");
        op=e_inteiro();
        if(op==1)
        {
            printf("Nome: ");
            do
            {
                strcpy(nome, e_texto(30));
            }
            while(strlen(nome)==0);
            temp = buscar_fun(listas, nome, 0, 1);
        }
        else
        {
            if(op==2)
            {
                printf("Número: ");
                y=e_inteiro();
                if(y>0)
                {
                    temp = buscar_fun(listas, '\0', y, 2);
                }
                else
                {
                    printf("\nInválido! Insira um número maior que zero.\n");
                    temp=NULL;
                }
            }
            else
            {
                printf("\nOpção inválida!\n");
                temp=NULL;
            }
        }
        if(temp==NULL)
        {
            //printf("\nNão foi possivel abrir está lista\n");
        }
        else
        {
            //return 1=alguma lista/pilha foi escluída
            if(menu(temp))
            {
                //limpando lista principal
                aux = listas->inicio;
                if(aux->prox==NULL) //verifica se é o primeiro e ultimo
                {
                    if(aux->endFila==temp)
                    {
                        free(aux);
                        listas->inicio=NULL;
                    }
                    else
                    {
                        printf("\nErro inesperado\n");
                    }
                }
                else
                {
                    if(aux->endFila!=temp)
                    {
                        while(aux->endFila!=temp) //procura
                        {
                            ant_temp=aux; //anterior
                            aux=aux->prox;
                        }
                        //verificar se é o ultimo
                        ant_temp->prox=aux->prox;
                        free(aux);
                    }
                    else
                    {
                        listas->inicio=aux->prox;
                        free(aux);
                    }
                }
                listas->qtd--;
                printf("\nLimpo da lista principal!\n");
            }
        }
    }

}
//
int comparar(char *x, char *y);
//
lista* buscar_fun (lista *l, char *x, int y, int op)
{
    /*
        função para realizar buscar dentro da lista principal, a lista de filas/pilhas
    */
    registro *aux;
    int i=1;

    aux = l->inicio;
    if(op==1)
    {
        while(aux!=NULL)
        {
            if (comparar(x, aux->endFila->nome))
            {
                printf("\nEncontrado, abrindo.\n");
                return aux->endFila; //retorna a cabeça da fila caso encontre x
            }
            aux=aux->prox;
        }
    }
    else
    {
        if(op==2)
        {
            //buscar pelo número informado
            while(aux!=NULL)
            {
                if(i==y)
                {
                    printf("\nEncontrado, abrindo.\n");
                    return aux->endFila;
                }
                aux=aux->prox;
                i++;
            }
        }
    }
    printf("\nNada encontrado!\n");
    return NULL;

}
//
void incluir_na_fila (lista *l, int x)
{
    /*
        função exclusiva para inserir conteúdo numa fila especifica,
        ela deve receber o endereço da lista dessa respectiva fila,

        [!] não pode receber o endereno da lista principal
    */
    registro *temp, *aux;
    temp = aloca_registro();

    if (l->inicio==NULL)
    {
        l->inicio=temp;
    }
    else
    {
        aux = l->inicio;
        while(aux->prox!=NULL)
        {
            aux=aux->prox;
        }
        aux->prox = temp;

    }
    l->qtd++;
}
int comparar(char *x, char *y)
{
    /*
        função para verificar a igualdade entre duas array
        para o fim de pesquisa, x é a variavel mestre
    */
    int i;
    for(i=0; i<strlen(x); i++)
    {
        if(x[i]==y[i])
        {

        }
        else
        {
            return 0;
        }
    }
    return 1;
}
/*
    Abaixo as funções padrão push, pop, stackpop e mostras tudo
    Basicamente as funções exclusiva para as filas
*/

pessoa* criar_pessoa();
void push(lista *l)
{
    /*
        função push padrão
        exclusiva para as filas
    */
    pessoa *x = criar_pessoa(); //criar pessoa

    registro *temp, *aux;
    temp = (registro*)malloc(sizeof(registro));
    temp->endFila = NULL;
    temp->prox = NULL;
    temp->p = x;

    if (l->inicio==NULL)
    {
        l->inicio=temp;
    }
    else
    {
        aux = l->inicio;
        while(aux->prox!=NULL)
        {
            aux=aux->prox;
        }
        aux->prox = temp;

    }
    l->qtd++;
}

pessoa* criar_pessoa()
{
    /*
        exclusiva para as filas
        função para alocar e preencher formulário pessoa
    */
    pessoa *temp;
    int tipo;

    temp = (pessoa*)malloc(sizeof(pessoa));

    do
    {
        printf("    Nome: ");
        strcpy(temp->nome, e_texto(30));
    }
    while(!strlen(temp->nome));
    do
    {
        printf("    Idade: ");
        temp->idade=e_inteiro();
    }
    while(temp->idade<1);
    printf("    Nova pessoa criada!\n");

    return temp;
}
void pop(lista *l, int tipo)
{
    registro *aux, *temp;
    if(l->qtd==0)
    {
        printf("\nEstá %s está vazia!\n", l->tipo);
    }
    else
    {
        if(tipo==2)
        {
            // tipo pilha, ultimo a entrar é primeiro a sair
            aux = l->inicio;
            if(aux->prox==NULL)
            {
                printf("\n%s, %d anos.\n", aux->p->nome, aux->p->idade);
                l->inicio = NULL;
                free(aux->p); //liberar pessoa desse registro
                free(aux);
            }
            else
            {
                while(aux->prox!=NULL)
                {
                    temp=aux; //anterior
                    aux=aux->prox;
                }
                printf("\n%s, %d anos.\n", aux->p->nome, aux->p->idade);
                free(aux->p); //liberar pessoa desse registro
                free(aux); //liberar esse registro
                temp->prox=NULL; //retira endereço desse registro do registro anterior
            }

        }
        else
        {
            if(tipo==1)
            {

                // tipo pilha, primeiro a entrar é o primeiro a sair
                aux = l->inicio;
                printf("%s, %d anos.\n", aux->p->nome, aux->p->idade);
                l->inicio = aux->prox; //inicio recebe o proximo
                free(aux->p); //libera pessoa
                free(aux);

                //
            }
            else
            {
                printf("\nErro inesperado\n"); //Um erro inesperado é um erro que não estava no plano geral, que deveria ser impossivel de acontecer
            }
        }
        l->qtd--;
    }
}
void stackpop(lista *l, int tipo)
{
    registro *aux, *temp;
    if(l->qtd==0)
    {
        printf("\nEstá %s está vazia!\n", l->tipo);
    }
    else
    {
        if(tipo==2)
        {
            // tipo pilha, ultimo a entrar é primeiro a sair
            aux = l->inicio;
            while(aux->prox!=NULL)
            {
                temp=aux; //anterior
                aux=aux->prox;
            }
            printf("\n%s, %d anos.\n", aux->p->nome, aux->p->idade);
            //
        }
        else
        {
            if(tipo==1)
            {
                // tipo pilha, primeiro a entrar é o primeiro a sair
                aux = l->inicio;
                printf("%s, %d anos.\n", aux->p->nome, aux->p->idade);

                //
            }
            else
            {
                printf("\nErro inesperado\n"); //Um erro inesperado é um erro que não estava no plano geral, que deveria ser impossivel de acontecer
            }
        }
    }
}
void mostrar_tudo(lista *l)
{
    /*
        função exclusiva para as filas
        visa mostrar todos os resgistro dentro daquela fila
    */
    registro *aux;
    int i=1;

    if (l->qtd==0)
    {
        printf("\nNenhum registro encontrado.\n");
    }
    else
    {

        aux=l->inicio;
        while(aux!=NULL)
        {
            printf("\n[%d] %s, %d anos.", i, aux->p->nome, aux->p->idade);
            aux=aux->prox;
            i++;
        }
        printf("\n");
    }
    printf("\nTotal: %d Registros | %d Kb\n",l->qtd, sizeof(registro)*l->qtd + sizeof(lista));
}
void renomear(lista *l)
{
    printf("\nNome antigo: %s.\n", l->nome);
    do
    {
        printf("Novo nome: ");
        strcpy(l->nome, e_texto(30));
    }
    while (strlen(l->nome)==0);

    printf("\nRenomeado para %s\n.", l->nome);
}
void mudar_tipo(lista *l)
{
    int op;
    printf("\nTipo antigo: %s.", l->tipo);
    printf("\nAlterar para ");
    if(!strcmp(l->tipo, "fila"))
    {
        printf("pilha? ");
        do
        {
            printf("(1) sim, (0) não: ");
            op=e_inteiro();
            if(op==1)
            {
                strcpy(l->tipo, "pilha");
            }
            else
            {
                if(op==0)
                {
                    printf("\nOperação cancelada!\n");
                    return;
                }
            }
        }
        while (op!=0&&op!=1);
    }
    else
    {
        if(!strcmp(l->tipo, "pilha"))
        {
            printf("fila? ");
            do
            {
                printf("(1) sim, (0) não: ");
                op=e_inteiro();
                if(op==1)
                {
                    strcpy(l->tipo, "fila");
                }
                else
                {
                    if(op==0)
                    {
                        printf("\nOperação cancelada!\n");
                        return;
                    }
                }
            }
            while (op!=0&&op!=1);
        }
        else
        {
            printf("[ERRO]\n\nErro inesperado\n");
            return;
        }
    }
    printf("\nAlterado para %s.\n", l->tipo);
}
void info(lista *l)
{
    printf("\n[>] %s (%s) | %d Registros | %d Kb\n", l->nome, l->tipo, l->qtd, (sizeof(registro)+sizeof(pessoa))*(l->qtd) + sizeof(lista));
}
//
void fun_free(registro *aux);
//
int excluir_op(lista *l)
{
    /*
        função para excluir pilhas/listas
    */
    int op, i, qtd = l->qtd;
    registro *aux, *exc;

    printf("\n");
    do
    {
        printf("Certeza? (1) sim, (0) não: ");
        op=e_inteiro();
    }
    while(op!=1 && op!=0);
    if(op==1)
    {
        // excluir fila/pilha de lista
        //vai ter que usar recursividade, porque preciso limpar do fim pro inicio
        if(l->inicio!=NULL)//recebe primeiro registro
        {
            fun_free(l->inicio);
        }
        free(l);
        printf("\nExcluído!\n");
        return 1;
    }
    else
    {
        if(op==0)
        {
            return 0;
        }
        else
        {
            printf("\nErro inesperado\n");
        }
    }

}

void fun_free(registro *aux)
{
    /*
        função desalocar recursiva, limpa do fim até o endereço passado por parametro

    */
    free(aux->p);
    free(aux);
}
