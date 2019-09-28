#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

//Irineu A. Silva

/*  Gerenciador de Filas/Pilhas com listas
    A ideia desse programa é ser um gerenciador de filas e pilhas, implementadas por listas (o que o torna virtualmente).
    Literalmente uma lista de listas, com dois menus de operação, um para a lista principal e outro para as secundárias que podem se comportar como filas ou pilhas,
    O comportamente das listas secundárias pode ser alterado alterando o tipo, e nelas podem ser adicionado e removido cadastro de pessoas, atravez de push e pop
*/


//structs
typedef struct lista //cabeça da lista
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
    lista *endFila; //endereço da lista, usado apenas na lista principal para apontar para a cabeça da respectiva lista secundária, que pode se ruma fila ou pilha
    struct registro *prox;
} registro;

//minhas funções pessoais de entrada, imuneis aos erros comuns do fgets e scanf, podem ser encontradas em https://github.com/Irineu333/Funcoes-de-Entrada-Perfeitas
char* e_texto(int tam)
{
    char *temp=NULL;
    temp=(char*)malloc(sizeof(char)*tam);
    fgets(temp, tam, stdin);
    fflush(stdin); //uso não recomendado segundo alguns foruns, mas evita erros ralacionados ao buffer
    temp[strlen(temp)-1] = '\0'; //remove o \n que o fgets adiciona no final da string

    /*
     *  foi identificado um erro relacionado ao buffer quando o usuario digita mais que o temanho especificado, o uso do fflush(stdin) foi para corrigir isso
     *  o fgets não deixa lixo no buffer exceto quando o texto inserido excede o tamanho especificado, mas o scanf por outro lado sempre deixa, o uso do fflush(stdin) também problemas com isso
     */

    return temp;
}

int e_inteiro()
{
    int temp=0;
    temp = atoi(e_texto(11)); //função atoi converte char para int, e atof de char para float

    return temp;
}

//declarações das funções padrão de listas
lista* aloca_lista();
registro* aloca_registro();

//declarações para uso no main
void inserir_fila_pilha(lista *listas);
void listar(lista *listas);
void abrir_fila_pilha(lista *l);

int main()
{
    char tempText[30];
    int op=0;
    //compatibilidade com portugues
    setlocale(LC_ALL, "portuguese");
    //criando lista principal
    lista *listas=NULL;
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
            inserir_fila_pilha(listas);
            break;
        case 2:
            printf("\nAbrir:\n");
            abrir_fila_pilha(listas);
            break;
        case 3:
            printf("\nReg. na memória:\n");
            listar(listas);
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

//abaixo funções da lista principal
void listar(lista *listas)
{
    /*
       função exclusiva da lista principal
       listas as listas (filas) presentes na lista principal
    */
    registro *aux=NULL;
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

void inserir_fila_pilha(lista *listas)
{
    /*
       função exclusiva da lista principal
       alocar novas listas (fila ou pilha) para adicionar na lista principal
    */

    registro *temp=NULL, *aux=NULL;
    int tipo=0;
    temp = aloca_registro();
    temp->endFila = aloca_lista(); //diferença mais fundamental da funão principal, permanece sempre null nas sevundarias
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

//declarações funções padrão de pilhas/filas, para usar no menu
void push(lista *l);
void mostrar_tudo(lista *l);
void pop(lista *l, int i);
void stackpop(lista *l, int tipo);

//declarações para usar no menu
void renomear(lista *l);
void mudar_tipo(lista *l);
void info(lista *l);
int excluir_op(lista *l);
int tipificador(lista *l);

int menu(lista *l)
{
    /*
        função para realizar operações dentro das filas/pilhas
    */
    info(l);
    int op=0, tipo=0;

    tipo = tipificador(l);

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
                return 1; //retornar 1 para limpar a lista principal, do qual essa função não tem acesso
            }
            break;
        case 7:
            printf("\nMudar tipo:");
            mudar_tipo(l);
            tipo = tipificador(l);
            info(l);
            break;
        case 8:
            printf("\nSaindo da %s.\n", l->tipo);
            break;
        default:
            printf("\nOpcão inválida!\n");
            break;

        }

    }
    while(op!=8);

    return 0;
}

int tipificador(lista *l)
{
    if(!strcmp(l->tipo, "fila"))
    {
        return 1;
    }
    else
    {

        if(!strcmp(l->tipo, "pilha"))
        {
            return 2;
        }
        else
        {
            printf("\nErro inesperado\n");
        }
    }
}

lista* aloca_lista()
{
    /*
        alocador universal de listas, mais especificamente a cabeça da lista
    */
    lista *temp=NULL;
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
    registro *temp=NULL;
    temp =(registro*)malloc(sizeof(registro));
    temp->p = NULL;
    temp->endFila=NULL; //estrutura preenchida apela na lista principal
    temp->prox = NULL;
    return temp;
}

//declrações para usar na função abrir pilha/fila
lista* buscar_fun (lista *l, char *x, int y, int op);

void abrir_fila_pilha (lista *listas)
{
    /*
        função responsavel por abrir filas e pilhas
        na verdade ela apenar gerencia o uso da função buscar_por_filas e menu,
        esse ultimo responsável pelas opreações realizadas dentro de uma fila,
    */
    lista *temp=NULL;
    int op=0, y=0;
    char nome[30];
    registro *aux=NULL, *ant_temp=NULL;
    if (listas->qtd==0)
    {
        printf("\nA lista está vazia!\n");
        return;
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
            //return 1=alguma lista/pilha foi excluída
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

//declarações para usar na função buscar
int comparar(char *x, char *y);

lista* buscar_fun (lista *l, char *x, int y, int op)
{
    /*
        função para realizar buscar dentro da lista principal, a lista de filas/pilhas
    */
    registro *aux=NULL;
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

//declarações para usar na função push
pessoa* criar_pessoa();

void push(lista *l)
{
    /*
        função push padrão
        exclusiva para as filas
    */
    pessoa *x = criar_pessoa(); //criar pessoa

    registro *temp=NULL, *aux=NULL;
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
        exclusiva para as filas/pilhas
        função para alocar e preencher formulário pessoa
    */
    pessoa *temp=NULL;
    int tipo=0;

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
    registro *aux=NULL, *temp=NULL;
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
                printf("\n%s, %d anos.\n", aux->p->nome, aux->p->idade);
                l->inicio = aux->prox; //inicio recebe o proximo
                free(aux->p); //libera pessoa
                free(aux);
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
    registro *aux=NULL, *temp=NULL;
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
                aux=aux->prox;
            }
            printf("\n%s, %d anos.\n", aux->p->nome, aux->p->idade);
        }
        else
        {
            if(tipo==1)
            {
                // tipo pilha, primeiro a entrar é o primeiro a sair
                aux = l->inicio;
                printf("\n%s, %d anos.\n", aux->p->nome, aux->p->idade);
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
        função exclusiva para as filas/pilhas
        visa mostrar todos os resgistro dentro daquela fila/pilhas
        para a lista principal a função tulizada é a listar
    */
    registro *aux=NULL;
    int i;

    if (l->qtd==0)
    {
        printf("\nNenhum registro encontrado.\n");
    }
    else
    {

        aux=l->inicio;
        i=1;
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

    printf("\nRenomeado para %s.\n", l->nome);
}

void mudar_tipo(lista *l)
{
    int op=-1;
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

//declarações para usar na função excluir com opções
void fun_free(registro *aux);

int excluir_op(lista *l)
{
    /*
        função para excluir pilhas/listas
    */
    int op=0, i=-1, qtd = l->qtd;
    registro *aux=NULL, *exc=NULL;

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
