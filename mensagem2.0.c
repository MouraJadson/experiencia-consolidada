#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAM_MENSAGEM 1024
#define TAM_CHAVE    10

void limpabuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
int ler_opcao(void)
{
    int c;
    int opcao = -1;

    do
    {
        c = getchar();

        if (c == '\n')
            continue;
        if (c >= '0' && c <= '6')
        {
            opcao = c - '0';
            limpabuffer();
        }
        else
        {
            limpabuffer();
            printf("Entrada invalida! Insira apenas um numero entre 0 e 6.\n");
            printf("Opcao: ");
        }

    } while (opcao == -1);
    return opcao;
}
int ler_numero_secreto(void)
{
    char digitos[TAM_CHAVE + 1];
    int  i = 0;
    int  c;

    while ((c = _getch()) != '\r')
    {
        if (c == '\r')
            break;

        if (c == 8 && i > 0)
        {
            i--;
            printf("\b \b");
            continue;
        }

        if (i < TAM_CHAVE && (c >= '0' && c <= '9'))
        {
            digitos[i] = (char)c;
            i++;
            printf("*");
        }
    }

    digitos[i] = '\0';
    printf("\n");
    return atoi(digitos);
}

void inserir_mensagem(char* mensagem, int* encriptada)
{
    int tamanho;

    printf("Insira a mensagem: ");
    fgets(mensagem, TAM_MENSAGEM, stdin);

    tamanho = strlen(mensagem);
    if (tamanho > 0 && mensagem[tamanho - 1] == '\n')
        mensagem[tamanho - 1] = '\0';

    *encriptada = 0;

    printf("[OK] Mensagem guardada.\n");
}

void inserir_chave_enc(int* chave_enc)
{
    printf("Introduza o valor da chave de encriptacao: ");
    *chave_enc = ler_numero_secreto();
    printf("[OK] Chave de encriptacao guardada.\n");
}

void inserir_chave_desc(int* chave_desc)
{
    printf("Introduza o valor da chave de desencriptacao: ");
    *chave_desc = ler_numero_secreto();
    printf("[OK] Chave de desencriptacao guardada.\n");
}

void encriptar(char* mensagem, int* chave_enc, int* encriptada)
{
    int i;
    int tamanho;
    int deslocamento;
    int novovalor;

    if (*encriptada)
    {
        printf("[AVISO] A mensagem ja esta encriptada!\n");
        return;
    }

    if (strlen(mensagem) == 0)
    {
        printf("[AVISO] Nao ha mensagem para encriptar.\n");
        return;
    }

    deslocamento = *chave_enc % 95;
    if (deslocamento < 0)
        deslocamento = deslocamento + 95;

    tamanho = strlen(mensagem);

    for (i = 0; i < tamanho; i++)
    {
        if (mensagem[i] >= 32 && mensagem[i] <= 126)
        {
            novovalor = mensagem[i] + deslocamento;

            if (novovalor > 126)
                novovalor = novovalor - 95;

            mensagem[i] = (char)novovalor;
        }
    }
    *encriptada = 1;
    printf("[OK] Mensagem encriptada: \"%s\"\n", mensagem);
}

void desencriptar(char* mensagem, int chave_enc, int* chave_desc, int* encriptada)
{
    int i;
    int tam;
    int deslocamento;
    int novovalor;

    if (!(*encriptada))
    {
        printf("[AVISO] A mensagem nao esta encriptada!\n");
        return;
    }

    if (*chave_desc != chave_enc)
    {
        printf("[ERRO] Chave incorreta! Nao e possivel desencriptar.\n");
        return;
    }

    deslocamento = *chave_desc % 95;
    if (deslocamento < 0)
        deslocamento = deslocamento + 95;

    tam = strlen(mensagem);

    for (i = 0; i < tam; i++)
    {
        if (mensagem[i] >= 32 && mensagem[i] <= 126)
        {
            novovalor = mensagem[i] - deslocamento;

            if (novovalor < 32)
                novovalor = novovalor + 95;

            mensagem[i] = (char)novovalor;
        }
    }
    *encriptada = 0;
    printf("[OK] Mensagem desencriptada: \"%s\"\n", mensagem);
}

void mostrar_estado(const char* mensagem, const int* chave_enc, const int* chave_desc, const int* encriptada)
{
    printf("\n+---------------------------------+\n");

    if (strlen(mensagem) == 0)
        printf("|Mensagem: (vazia)\n");
    else
        printf("|Mensagem: \"%s\"\n", mensagem);
    if (*encriptada)
        printf("|Estado: ENCRIPTADA\n");
    else
        printf("|Estado: Original\n");

    printf("|Chave enc.  : %s\n", *chave_enc != 0 ? "Chave Definida" : "[nao definida]");
    printf("|Chave desc. : %s\n", *chave_desc != 0 ? "Chave Definida" : "[nao definida]");
    printf("+---------------------------------+\n");
}

int main(void)
{
    char mensagem[TAM_MENSAGEM] = "";
    int  chave_enc = 0;
    int  chave_desc = 0;
    int  encriptada = 0;
    int  opcao;

    printf("Bem-vindo ao CriptoMensagem!\n");

    do {
        printf("\n+----------------------------------+\n");
        printf("|     CRIPTO MENSAGEM - Menu       |\n");
        printf("+----------------------------------+\n");
        printf("|1. Inserir nova mensagem          |\n");
        printf("|2. Inserir chave de encriptacao   |\n");
        printf("|3. Encriptar mensagem             |\n");
        printf("|4. Inserir chave de desencriptacao|\n");
        printf("|5. Desencriptar mensagem          |\n");
        printf("|6. Mostrar estado atual           |\n");
        printf("|0. Sair                           |\n");
        printf("+----------------------------------+\n");
        printf("Opcao: ");
        opcao = ler_opcao();

        switch (opcao)
        {
        case 1: inserir_mensagem(mensagem, &encriptada);                        break;
        case 2: inserir_chave_enc(&chave_enc);                                  break;
        case 3: encriptar(mensagem, &chave_enc, &encriptada);                   break;
        case 4: inserir_chave_desc(&chave_desc);                                break;
        case 5: desencriptar(mensagem,&chave_enc, &chave_desc, &encriptada);    break;
        case 6: mostrar_estado(mensagem, &chave_enc, &chave_desc, &encriptada); break;
        case 0: printf("Fechar programa!\n");                                   break;
        default: printf("Opcao invalida! Escolha entre 0 e 6.\n");              break;
        }
    } while (opcao != 0);
    printf("\n  Ate breve!\n\n");
    return 0;
}