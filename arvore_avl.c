#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define true 1
#define false 0

typedef int bool;
typedef int TIPOCHAVE;

typedef struct aux {
TIPOCHAVE chave;
struct aux *esq;
struct aux *dir;
int bal;
} NO, *PONT;

/* cria um novo (aloca memoria e preenche valores) no com chave=ch e retorna seu endereco */
PONT criarNovoNo(TIPOCHAVE ch){
    PONT novo = (PONT)malloc(sizeof(NO));

    if (novo) {
        novo->chave = ch;
        novo->bal = 0;
        novo->esq = NULL;
        novo->dir = NULL;
    }
    else {
        printf("Erro de alocacao de memoria\n");
        exit(0);
    }

    return novo;
}

// Retorna o maior valor entre dois inteiros
int max(int a, int b){
    return (a > b) ? a : b;
}

// Retorna a altura de uma (sub-)arvore
int altura(PONT p){
    if (p == NULL) 
        return -1;

    return 1 + max(altura(p->esq), altura(p->dir));
}

/* Exibe arvore Em Ordem */
void exibirArvoreEmOrdem(PONT raiz){
    if (raiz) {
        exibirArvoreEmOrdem(raiz->esq);
        printf("%d ", raiz->chave);
        exibirArvoreEmOrdem(raiz->dir);
    }
}
/* Exibe arvore Pre Ordem */
void exibirArvorePreOrdem(PONT raiz){
    if (raiz) {
        printf("%d ", raiz->chave);
        exibirArvorePreOrdem(raiz->esq);
        exibirArvorePreOrdem(raiz->dir);
    }
}
/* Exibe arvore Pos Ordem */
void exibirArvorePosOrdem(PONT raiz){
    if (raiz) {
        exibirArvorePosOrdem(raiz->esq);
        exibirArvorePosOrdem(raiz->dir);
        printf("%d ", raiz->chave);
    }
}

/* Exibe arvore Em Ordem (com parenteses para os filhos) */
void exibirArvore(PONT raiz){
    if (raiz) {
        printf("(");
        exibirArvore(raiz->esq);
        printf("%d", raiz->chave);
        exibirArvore(raiz->dir);
        printf(")");
    }
}

/* Exibe arvore Pre-Ordem indicando pai de cada no */
void exibirArvore2(PONT raiz, TIPOCHAVE chavePai){
    if (raiz) {
        printf("(pai: %d ", chavePai);
        printf("filho: %d) ", raiz->chave);
        exibirArvore2(raiz->esq, raiz->chave);
        exibirArvore2(raiz->dir, raiz->chave);
    }
}

// Verifica se árvore é AVL
bool ehAVL(PONT p){
    if (p == NULL) 
        return true;
    
    if (p->bal > 1 || p->bal < -1)
        return false;

    return ehAVL(p->esq) && ehAVL(p->dir);
}

// Atualiza o balancemento total
int atualizarBalanceamentoTotal(PONT raiz){
    if (raiz == NULL)
        return 0;
    
    return altura(raiz->dir) - altura(raiz->esq); 
}   

/* Rotações à direita (LL e LR)
Retornará o endereço do nó que será a nova raiz da subárvore originalmente iniciada por p */
PONT rotacaoL(PONT p){
    PONT u = p->esq;

    if (u->bal == -1) {
        p->esq = u->dir;
        u->dir = p;
        p->bal = 0;
        u->bal = 0;
        
        return u;
    }

    if (u->bal == 1) {
        PONT v = u->dir;

        u->dir = v->esq;
        v->esq = u;
        p->esq = v->dir;
        v->dir = p;

        if (v->bal == -1)
            p->bal = -1;
        else
            p->bal = 0;

        if (v->bal == 1)
            u->bal = -1;
        else
            u->bal = 0;

        v->bal = 0;
        
        return v;
    }

    if (u->bal == 0) {
        p->esq = u->dir;
        u->dir = p;
        p->bal = atualizarBalanceamentoTotal(p);
        u->bal = atualizarBalanceamentoTotal(u);
        
        return u;
    }

    return p;
}

/* Rotações à esquerda (RR e RL)
Retornará o endereço do nó que será a nova raiz da subárvore originalmente iniciada por p */
PONT rotacaoR(PONT p){
    PONT u = p->dir;
    if (u->bal == 1) {
        p->dir = u->esq;
        u->esq = p;
        p->bal = 0;
        u->bal = 0;
        
        return u;
    }
    
    if (u->bal == -1) {
        PONT v = u->esq;
        u->esq = v->dir;
        v->dir = u;
        p->dir = v->esq;
        v->esq = p;
        if (v->bal == -1)
            p->bal = -1;
        else
            p->bal = 0;

        if (v->bal == 1)
            u->bal = -1;
        else
            u->bal = 0;

        v->bal = 0;

        return v;
    }

    if (u->bal == 0) {
        p->dir = u->esq;
        u->esq = p;
        p->bal = atualizarBalanceamentoTotal(p);
        u->bal = atualizarBalanceamentoTotal(u);
        
        return u;
    }

    return p;
}

/* Inserção AVL: p é inicializado com o endereco do nó raiz e *alterou com false */
void inserirAVL(PONT* pp, TIPOCHAVE ch, bool* alterou){
    if (*pp == NULL) {
        *pp = criarNovoNo(ch);
        *alterou = true;
    }
    else {
        if (ch < (*pp)->chave) 
            inserirAVL(&(*pp)->esq, ch, alterou);
        else if (ch > (*pp)->chave) 
            inserirAVL(&(*pp)->dir, ch, alterou);
        else {
            *alterou = false;
            return;
        }
    }    
        
    (*pp)->bal = atualizarBalanceamentoTotal(*pp);
    
    if ((*pp)->bal < -1)
        *pp = rotacaoL(*pp);
    
    if ((*pp)->bal > 1)
        *pp = rotacaoR(*pp);

    *alterou = false;
}

/* retorna o endereco do NO que contem chave=ch ou NULL caso a chave nao seja encontrada. Utiliza busca binaria recursiva */
PONT buscaBinaria(TIPOCHAVE ch, PONT raiz){
    if (raiz == NULL || raiz->chave == ch)
        return raiz;
    
    if (ch < raiz->chave)
        return buscaBinaria(ch, raiz->esq);
    else
        return buscaBinaria(ch, raiz->dir);
}

// Busca binária não recursiva devolvendo o nó pai
PONT buscaNo(PONT raiz, TIPOCHAVE ch, PONT *pai){
    PONT atual = raiz;

    while (atual != NULL) {
        if (atual->chave == ch) {
            return atual;
        }

        *pai = atual;

        if (ch < atual->chave) {
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }

    return NULL;
}

/* Auxilir da funcao excluirChave, procura a maior chave menor que a chave que serah excluida */
PONT maiorAEsquerda(PONT p, PONT *ant){
    while (p->dir) {
        (*ant) = p;
        p = p->dir;
    }
    
    return p;
}

// exclui a chave com valor igual a ch
bool excluirAVL(PONT* raiz, TIPOCHAVE ch, bool* alterou){
    if (*raiz == NULL) 
        return false;

    if (ch < (*raiz)->chave) 
        *alterou = excluirAVL(&(*raiz)->esq, ch, alterou);
    else if (ch > (*raiz)->chave)
        *alterou = excluirAVL(&(*raiz)->dir, ch, alterou);
    else {
        if ((*raiz)->chave == ch) {
            if ((*raiz)->esq == NULL && (*raiz)->dir == NULL){
                free(*raiz);
                *raiz = NULL;
            }
            else if ((*raiz)->esq != NULL && (*raiz)->dir != NULL) {
                PONT ant = *raiz;
                PONT temp;
                temp = maiorAEsquerda((*raiz)->esq, &ant);
                (*raiz)->chave = temp->chave;
                *alterou = excluirAVL(&(*raiz)->esq, temp->chave, alterou); 
            }
            else {
                if ((*raiz)->esq) {
                    (*raiz)->chave = (*raiz)->esq->chave;
                    free((*raiz)->esq);
                    (*raiz)->esq = NULL;
                }
                else {
                    (*raiz)->chave = (*raiz)->dir->chave;
                    free((*raiz)->dir);
                    (*raiz)->dir = NULL;
                }
            }
        }
    }

    if (*raiz) {
        (*raiz)->bal = atualizarBalanceamentoTotal(*raiz);
    
        if ((*raiz)->bal < -1)
            *raiz = rotacaoL(*raiz);
    
        if ((*raiz)->bal > 1)
            *raiz = rotacaoR(*raiz);
    }
    
    return true;
}

/* funcao auxiliar na destruicao (liberacao da memoria) de uma arvore */
void destruirAux(PONT subRaiz){
    if (subRaiz) {
        destruirAux(subRaiz->esq);
        destruirAux(subRaiz->dir);
        free(subRaiz);
    }
}
/* libera toda memoria de uma arvore e coloca NULL no valor da raiz */
void destruirArvore(PONT * raiz){
    destruirAux(*raiz);
    *raiz = NULL;
}

//inicializa arvore
void inicializar(PONT *raiz){
    *raiz = NULL;
}

int main() {    
    PONT raiz;
    bool alterou = false;
    int op, valor;

    inicializar(&raiz);

    do {
        printf("\n[1]Inserir\n[2]Exibir\n[3]Busca\n[4]Remover\n[5]Finalizar\nOpcao: ");
        scanf("%d", &op);

        switch (op) {
        case 1:
            system("cls");
            printf("Valor a ser inserido: ");
            scanf("%d", &valor);
            inserirAVL(&raiz, valor, &alterou);
            printf("Valor %d inserido\n", valor);
            break;

        case 2:
            system("cls");
            printf("\nArvore AVL em ordem: ");
            exibirArvoreEmOrdem(raiz);
            printf("\n");

            printf("\nArvore AVL pre-ordem: ");
            exibirArvorePreOrdem(raiz);
            printf("\n");

            printf("\nArvore AVL pos-ordem: ");
            exibirArvorePosOrdem(raiz);
            printf("\n");

            printf("\nArvore AVL: ");
            exibirArvore(raiz);
            printf("\n");

            printf("\nArvore AVL: ");
            exibirArvore2(raiz, 0);
            printf("\n");
            break;

        case 3:
            system("cls");
            printf("Valor a ser buscado: ");
            scanf("%d", &valor);

            PONT busca;
            
            busca = buscaBinaria(valor, raiz);
            if (busca)
                printf("\nBusca Binaria: %d\n", busca->chave);
            else
                printf("\nValor nao encontrado\n");

            PONT pai = NULL;

            busca = buscaNo(raiz, valor, &pai);
            if (pai && busca) 
                printf("\nBusca No: pai: %d  - filho: %d\n",pai->chave, busca->chave);
            else if (busca)
                printf("Busca No: (sem pai) no: %d\n", busca->chave);
            else
                printf("Valor nao encontrado\n");
            break;

        case 4:
            system("cls");
            alterou = false;

            printf("Valor a ser removido: ");
            scanf("%d", &valor);

            if (excluirAVL(&raiz, valor, &alterou) == true) 
                printf("\nElemento %d excluido\n", valor);
            else
                printf("\nElemento %d nao encontrado\n", valor);
            break;

        case 5:
            system("cls");
            destruirArvore(&raiz);
            printf("\nArvore destruida\n");
            printf("Programa finalizado\n");
            break;
            
        default:
            printf("\nOpcao invalida. Tente novamente\n");
            break;
        }

    } while (op != 5);
    
    return 0;
}