#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

//Irineu A. Silva

/*  Lista de listas
    A ideia desse programa � ser um gerenciador de filas, essas filas s�o implementadas por listas,
    Ent�o � literalemnte uma lista de listas, uma vez que as filas s�o armazenada em uma lista principal
*/

typedef struct lista
{
    int qtd;
    char nome[30];
    char tipo[5]; //fila ou pilha
    struct registro *inicio;
}lista;

typedef struct pessoa
{
    char nome[30];
    int idade;
}pessoa;

typedef struct registro
{
    pessoa *p;
    lista *endFila; //endere�o da lista
    struct registro *prox;
}registro;


lista* aloca_lista();
void inserir_fila(lista *listas);
void lista_filas(lista *listas);
void incluir_na_fila (lista *l, int x);
registro* aloca_registro();
void abrir_fila (lista *l);
lista* buscar_por_listas (lista *l, char *x, int y, int op);

int main()
{
    char tempText[30];
    int op;
    //compatibilidade com portugues
    setlocale(LC_ALL, "portuguese");
    printf("\nGerenciador de Pilhas/Filas\n");
    //criando lista principal
    lista *listas;
    listas = aloca_lista();

    do
    {
        printf("\n1.Criar");
        printf("\n2.Abrir");
        printf("\n3.Listar");
        printf("\n4.Sair\n");
        scanf("%d", &op);

        switch(op)
        {
        case 1:
            printf("\n[+] Criar Fila/Pilha");
            inserir_fila(listas);
            break;
        case 2:
            abrir_fila(listas);
            break;
        case 3:
            printf("\nFilas na mem�ria:\n");
            lista_filas(listas);
            break;
        case 4:
            printf("\nSaindo do programa.");
            break;
        default:
            printf("\nOpc�o inv�lida!\n");
            break;
        }

    }while(op!=4);

    return 0;
}
//
void lista_filas(lista *listas)
{
    /*
       fun��o exclusiva da lista principal
       listas as listas (filas) presentes na lista principal
    */
    registro *aux;
    int i=1, tam[2] = {0, 0};

    if (listas->qtd==0)
    {
        printf("\nA lista est� vazia!\n");
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
                printf("\n[%d] %s (%s) | %d Registros | %d Kb", i, aux->endFila->nome, aux->endFila->tipo, aux->endFila->qtd, (sizeof(registro)+sizeof(pessoa))*(aux->endFila->qtd) + sizeof(lista));

                //Somando total
                tam[0] += aux->endFila->qtd;
                tam[1] += (sizeof(registro)+sizeof(pessoa))*(aux->endFila->qtd) + sizeof(lista);
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
       fun��o exclusiva da lista principal
       alocar novas listas (filas) para adicionar na lista principal
    */

    registro *temp, *aux; int tipo;
    temp = aloca_registro();
    temp->endFila = aloca_lista(); //diferen�a mais fundamental da fun�o principal

    //recebendo
    printf("\n    Nome: "); fflush(stdin); fgets(temp->endFila->nome, 30, stdin); temp->endFila->nome[strlen(temp->endFila->nome)-1]='\0';
    do{ printf("    Tipo: (1) fila, (2) pilha: "); scanf("%d", &tipo); } while (tipo!= 1 && tipo !=2);

    //tipificador
    if(tipo==1)
    {
        strcpy(temp->endFila->tipo, "fila");
    } else
    {
        if(tipo==2)
        {
            strcpy(temp->endFila->tipo, "pilha");
        } else
        {
            printf("\nErro inesperado\n");
        }
    }

    //estrutura padr�o
    if(listas->inicio==NULL)
    {
        listas->inicio=temp;
    }
    else
    {
        aux = listas->inicio;
        while(aux->prox!=NULL)
        {
           aux=aux->prox; //estrutura padr�o para avan�ar pela lista
        }
        aux->prox = temp;

    }
    //
    listas->qtd++;
}
void push(lista *l);
void mostrar_tudo(lista *l);
void pop(lista *l, int i);
void stackpop(lista *l, int tipo);
void renomear(lista *l);

void menu(lista *l)
{
    /*
        fun��o para realizar opera��es dentro das filas
    */
    printf("\n[>] %s (%s) | %d Registros | %d Kb\n", l->nome, l->tipo, l->qtd, (sizeof(registro)+sizeof(pessoa))*(l->qtd) + sizeof(lista));
    int op, tipo;

    if(!strcmp(l->tipo, "fila"))
    {

        tipo=1;
    } else
    {

        if(!strcmp(l->tipo, "pilha"))
        {
            tipo=2;
        } else
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
        scanf("%d",&op);

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
            printf("\nRegistros na mem�ria:\n");
            mostrar_tudo(l);
            break;
        case 5:
            printf("\nRenomear:");
            renomear(l);
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            printf("\nSaindo da %s.\n", l->tipo);
            break;
        default:
            printf("\nOpcao invalida!\n");
            break;

        }

    }while(op!=8);
}
lista* aloca_lista()
{
    /*
        alocador universal de listas, mais especificamente a cabe�a da lista
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
void abrir_fila (lista *l)
{
    /*
        fun��o responsavel por abrir filas
        na verdade ela apenar gerencia o uso da fun��o buscar_por_filas e menu,
        esse ultimo respons�vel pelas oprea��es realizadas dentro de uma fila
    */
    lista *temp; int op, y; char nome[30];

    printf("\nBuscar por:");
    printf("\n1.Nome\n2.N�mero\n"); scanf("%d", &op);
    if(op==1)
    {
        printf("\nNome: ");
        fflush(stdin); fgets(nome, 30, stdin); nome[strlen(nome)-1] = '\0' /*removendo a quebra de linha*/;
        temp = buscar_por_listas(l, nome, 0, 1);
    } else
    {
        if(op==2)
        {
            printf("\nN�mero: "); scanf("%d", &y);
            if(y>0)
            {
                temp = buscar_por_listas(l, '\0', y, 2);
            } else
            {
                printf("\nInv�lido! Insira um n�mero maior que zero.\n");
                temp=NULL;
            }
        } else
        {
            printf("\nOp��o inv�lida!\n");
            temp=NULL;
        }
    }
    if(temp==NULL)
    {
        //printf("\nN�o foi possivel abrir est� lista\n");
    } else
    {
        menu(temp);
    }

}
//
int ver_igualdade(char *x, char *y);
//
lista* buscar_por_listas (lista *l, char *x, int y, int op)
{
    /*
        fun��o para realizar buscar dentro da lista principal, a lista de filas
    */
    registro *aux; int i=1;

    if (l->qtd==0)
    {
        printf("\nA lista est� vazia!\n");
        return NULL;
    }
    else
    {
        aux = l->inicio;
        if(op==1)
        {
            while(aux!=NULL)
            {
                if (ver_igualdade(x, aux->endFila->nome))
                {
                    printf("\nEncontrado, abrindo.\n");
                    return aux->endFila; //retorna a cabe�a da fila caso encontre x
                }
                aux=aux->prox;
            }
        } else
        {
            if(op==2)
            {
                //buscar pelo n�mero informado
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

}
//
void incluir_na_fila (lista *l, int x)
{
    /*
        fun��o exclusiva para inserir conte�do numa fila especifica,
        ela deve receber o endere�o da lista dessa respectiva fila,

        [!] n�o pode receber o endereno da lista principal
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
int ver_igualdade(char *x, char *y)
{
    /*
        fun��o para verificar a igualdade entre duas array
        para o fim de pesquisa, x � a variavel mestre
    */
    int i;
    for(i=0; i<strlen(x); i++)
    {
        if(x[i]==y[i])
        {

        } else
        {
            return 0;
        }
    }
    return 1;
}
/*
    Abaixo as fun��es padr�o push, pop, stackpop e mostras tudo
    Basicamente as fun��es exclusiva para as filas
*/
pessoa* criar_pessoa();
void push(lista *l)
{
    /*
        fun��o push padr�o
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
        fun��o para alocar e preencher formul�rio pessoa
    */
    pessoa *temp; int tipo;

    temp = (pessoa*)malloc(sizeof(pessoa));

    printf("    Nome: "); fflush(stdin); fgets(temp->nome, 30, stdin); temp->nome[strlen(temp->nome)-1] = '\0';
    printf("    Idade: "); scanf("%d", &temp->idade);

    return temp;
}
void pop(lista *l, int tipo)
{
    registro *aux, *temp;
    if(l->qtd==0)
    {
        printf("\nEst� %s est� vazia!\n", l->tipo);
    } else
    {
        if(tipo==2)
        {
            // tipo pilha, ultimo a entrar � primeiro a sair
            aux = l->inicio;
            while(aux->prox!=NULL)
            {
                temp=aux; //anterior
                aux=aux->prox;
            }
            printf("\n%s, %d anos.\n", aux->p->nome, aux->p->idade);
            free(aux->p); //liberar pessoa desse registro
            free(aux); //liberar esse registro
            temp->prox=NULL; //retira endere�o desse registro do registro anterior
            //
        } else
        {
            if(tipo==1)
            {
                // tipo pilha, primeiro a entrar � o primeiro a sair
                aux = l->inicio;
                printf("%s, %d anos.\n", aux->p->nome, aux->p->idade);
                l->inicio = aux->prox; //inicio recebe o proximo
                free(aux->p); //libera pessoa
                free(aux);

                //
            } else
            {
                printf("\nErro inesperado\n"); //Um erro inesperado � um erro que n�o estava no plano geral, que deveria ser impossivel de acontecer
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
        printf("\nEst� %s est� vazia!\n", l->tipo);
    } else
    {
        if(tipo==2)
        {
            // tipo pilha, ultimo a entrar � primeiro a sair
            aux = l->inicio;
            while(aux->prox!=NULL)
            {
                temp=aux; //anterior
                aux=aux->prox;
            }
            printf("\n%s, %d anos.\n", aux->p->nome, aux->p->idade);
            //
        } else
        {
            if(tipo==1)
            {
                // tipo pilha, primeiro a entrar � o primeiro a sair
                aux = l->inicio;
                printf("%s, %d anos.\n", aux->p->nome, aux->p->idade);

                //
            } else
            {
                printf("\nErro inesperado\n"); //Um erro inesperado � um erro que n�o estava no plano geral, que deveria ser impossivel de acontecer
            }
        }
    }
}
void mostrar_tudo(lista *l)
{
    /*
        fun��o exclusiva para as filas
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
    printf("\nNome Antigo: %s", l->nome);
    printf("Novo nome: "); fflush(stdin); fgets(l->nome, 30, stdin); l->nome[strlen(l->nome)-1] = '\0';
    printf("\nRenomeado para %s", l->nome);
}
