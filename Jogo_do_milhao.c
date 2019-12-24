#include<stdio.h>
#include<stdlib.h>  // função rand
#include<conio.h>   // alguns usos durante o codigo
#include<string.h>
#include<locale.h>  // acentuação
#include<ctype.h>   // função toupper/tolower
#include<time.h>    // necessário p/ função time()

// defines a serem usadas durante o codigo
#define MAX_STRING 256
#define ENTER 13
#define ESC 27
#define ACABOU -5

// STRUCTS
typedef struct tela_st
{
    int acerto;           // pontuacao definida em tres vetores diferentes   |
    int parou[16];        // pontuacao definida em tres vetores diferentes   |- PONTUACAO
    int errou[16];        // pontuacao definida em tres vetores diferentes   |
    int universitario;    // opcao de ajuda do jogo
    int cartas;           // opcao de ajuda do jogo
    int placas;           // opcao de ajuda do jogo
    int pulos;            // opcao de ajuda do jogo
    int pontuacao;
    char nome[MAX_STRING];
} DADOS_JOGO;

typedef struct pergunta_01_st               // STRUCT DAS PERGUNTAS/RESPOSTAS (ALTERNATIVAS CERTAS E ERRADAS) DA DIFICULDADE UM
{
    char questao_nivel_01[8][MAX_STRING];
    char alternativas_nivel_01[24][MAX_STRING];
    char alt_certa_nivel_01[8][MAX_STRING];
} NIVEL_01;

typedef struct pergunta_02_st               // STRUCT DAS PERGUNTAS/RESPOSTAS (ALTERNATIVAS CERTAS E ERRADAS) DA DIFICULDADE DOIS
{
    char questao_nivel_02[8][MAX_STRING];
    char alternativas_nivel_02[24][MAX_STRING];
    char alt_certa_nivel_02[8][MAX_STRING];
} NIVEL_02;

typedef struct pergunta_03_st               // STRUCT DAS PERGUNTAS/RESPOSTAS (ALTERNATIVAS CERTAS E ERRADAS) DA DIFICULDADE TRÊS
{
    char questao_nivel_03[8][MAX_STRING];
    char alternativas_nivel_03[24][MAX_STRING];
    char alt_certa_nivel_03[8][MAX_STRING];
} NIVEL_03;

typedef struct pergunta_04_st               // STRUCT DAS PERGUNTAS/RESPOSTAS (ALTERNATIVAS CERTAS E ERRADAS) DA DIFICULDADE FINAL
{
    char questao_nivel_04[8][MAX_STRING];
    char alternativas_nivel_04[24][MAX_STRING];
    char alt_certa_nivel_04[8][MAX_STRING];
} NIVEL_04;

typedef struct perguntas_st                 // STRUCT QUE POSSUI AS OUTRAS STRUCTS DE PERGUNTAS/RESPOSTAS
{
    struct pergunta_01_st nivel_01;
    struct pergunta_02_st nivel_02;
    struct pergunta_03_st nivel_03;
    struct pergunta_04_st nivel_04;
} PERGUNTAS;

// FUNÇÕES -- PROTOTIPOS
void Menu();
void abre_arquivo(char *nome_arq, PERGUNTAS *lista_de_perguntas);
void imprime_erro(char *nome_arq);
void regras();
void desenha_indice(DADOS_JOGO *dados);
void preenche_struct(FILE *arquivo, PERGUNTAS *perguntas);
void preenche_perguntas_nivel_01(FILE *arquivo, PERGUNTAS *perguntas);
void preenche_perguntas_nivel_02(FILE *arquivo, PERGUNTAS *perguntas);
void preenche_perguntas_nivel_03(FILE *arquivo, PERGUNTAS *perguntas);
void preenche_perguntas_nivel_04(FILE *arquivo, PERGUNTAS *perguntas);
void chama_mostra_perguntas_01(PERGUNTAS perguntas, DADOS_JOGO *dados);
void chama_mostra_perguntas_02(PERGUNTAS perguntas, DADOS_JOGO *dados);
void chama_mostra_perguntas_03(PERGUNTAS perguntas, DADOS_JOGO *dados);
void chama_mostra_perguntas_04(PERGUNTAS perguntas, DADOS_JOGO *dados);
char mostra_perguntas_01(PERGUNTAS perguntas, DADOS_JOGO *dados, int i, int k);
char mostra_perguntas_02(PERGUNTAS perguntas, DADOS_JOGO *dados, int i, int k);
char mostra_perguntas_03(PERGUNTAS perguntas, DADOS_JOGO *dados, int i, int k);
char mostra_perguntas_04(PERGUNTAS perguntas, DADOS_JOGO *dados, int i, int k);
void perdeu(DADOS_JOGO *dados);
void ganhou();
void mostra_ganho(DADOS_JOGO *dados);
void mostra_erro (DADOS_JOGO *dados);
void mostra_pause(DADOS_JOGO *dados);
void ajuda_universitario(int resp_certa);
int ajuda_cartas(char perguntas[][MAX_STRING], int resp_certa);
void ajuda_placas();
void pulos_pergunta();

//VARIAVEIS GLOBAIS
int j_01=0, k_01=0, l_01=0;
int j_02=0, k_02=0, l_02=0;
int j_03=0, k_03=0, l_03=0;
int j_04=0, k_04=0, l_04=0;
int cont_perg=0;

//MAIN
int main()
{
    setlocale(LC_ALL, "");              //“Portuguese_Brasil.1252”  -- "Portuguese"
    //system("mode con:cols=110 lines=30");
    srand(time(NULL));
    DADOS_JOGO dados;
    PERGUNTAS perguntas;

    char nome_arq[100];               // DECLARACAO DE VARIAVEIS
    char op;

    Menu();                          // CHAMA FUNCAO MENU
    do                               // LACO PRINCIPAL
    {
        op = getch();               // RECEBE A OPCAO DO USUARIO; PARA CADA FUNCIONALIDADE -> UMA TECLA
        op = tolower(op);

        switch(op)
        {
        case 's':
            abre_arquivo(nome_arq,&perguntas);
            chama_mostra_perguntas_01(perguntas, &dados);
            break;

        case 'r':
            regras();
            main();
            break;

        case 'e':
            return 0;
        }
    }
    while(op!='s' || op!='r' || op!='h' || op!='c');  // REPETIR ENQUANTO 'OP' FOR DIFERENTE DAS OPCOES DISPONIVEIS

    return 0;
}

//FUNÇÕES -- CODIGOS
void Menu()
{
    char op;                           // PRINTA NA TELA AS OPCOES DISPONIVEIS PARA O USUARIO

    clrscr();
    textcolor(11);
    printf("------------------------------------------------------------------------------------------------------------------------\n");
    gotoxy(35,5);
    puts("-SHOW DO MILHÃO : O GAME-");
    gotoxy(35,10);
    puts("<(S) Para comecar a jogar!>");
    gotoxy(35,12);
    puts("<(R) Regras>");
    gotoxy(35,14);
    puts("<(E) Exit>");
    gotoxy(1,16);
    puts("\n------------------------------------------------------------------------------------------------------------------------\n");
}

void abre_arquivo(char *nome_arq, PERGUNTAS *lista_de_perguntas)
{
    clrscr();
    FILE *arquivo;                       // DECLARACAO DE VARIAVEIS
    char c;
    do
    {
        clrscr();
        gotoxy(25,5);
        puts("Arquivo: ");
        fflush(stdin);
        gotoxy(34,5);
        gets(nome_arq);
        arquivo=fopen(nome_arq, "r");
        if(arquivo==NULL)
            imprime_erro(nome_arq);
        rewind(arquivo);
    }
    while(arquivo==NULL);
    clrscr();
    preenche_struct(arquivo, lista_de_perguntas);
}
void imprime_erro(char *nome_arq)
{
    clrscr();
    gotoxy(25,5);
    puts("Arquivo: ");
    gotoxy(25,7);
    printf("Erro ao encontrar o arquivo '%s'.\n\t\t\tDigite Novamente :(", nome_arq);
    gotoxy(25,9);
    printf("Aguardando nome de arquivo...");
    while (!kbhit()) // AGUARDA O USUARIO APERTAR EM ALGUMA TECLA
    {
    }
}
void preenche_struct(FILE *arquivo, PERGUNTAS *lista_de_perguntas)
{
    clrscr();
    textcolor (11);
    char texto[300];
    while(!feof(arquivo))
    {
        if(fgets(texto,300,arquivo)!=NULL)
        {
            if(texto[0]=='1')
                preenche_perguntas_nivel_01(arquivo,lista_de_perguntas);
            else if(texto[0]=='2')
                preenche_perguntas_nivel_02(arquivo,lista_de_perguntas);
            else if(texto[0]=='3')
                preenche_perguntas_nivel_03(arquivo,lista_de_perguntas);
            else if(texto[0]=='4')
                preenche_perguntas_nivel_04(arquivo,lista_de_perguntas);
        }
    }
}
void preenche_perguntas_nivel_01(FILE *arquivo, PERGUNTAS *perguntas)
{
    char texto[300];
    char op;
    int i, n, m=0;
    for(i=0; i<5; i++)
    {
        if(fgets(texto, 300, arquivo)!=NULL)
        {
            if(i==0)
            {
                strcpy(perguntas->nivel_01.questao_nivel_01[j_01],texto);
                j_01++;
            }
            else if(texto[0]=='*')
            {
                for(n=1; texto[n]!='\0'; n++)
                {
                    perguntas->nivel_01.alt_certa_nivel_01[k_01][m]=texto[n];
                    m++;
                }
                perguntas->nivel_01.alt_certa_nivel_01[k_01][m]='\0';
                k_01++;
            }
            else
            {
                strcpy(perguntas->nivel_01.alternativas_nivel_01[l_01],texto);
                l_01++;
            }
        }
    }
}
void preenche_perguntas_nivel_02(FILE *arquivo, PERGUNTAS *perguntas)
{
    char texto[300];
    int i, n, m=0;
    char op;
    for(i=0; i<5; i++)
    {
        if(fgets(texto, 300, arquivo)!=NULL)
        {
            if(i==0)
            {
                strcpy(perguntas->nivel_02.questao_nivel_02[j_02],texto);
                j_02++;
            }
            else if(texto[0]=='*')
            {
                for(n=1; texto[n]!='\0'; n++)
                {
                    perguntas->nivel_02.alt_certa_nivel_02[k_02][m]=texto[n];
                    m++;
                }
                perguntas->nivel_02.alt_certa_nivel_02[k_02][m]='\0';
                k_02++;
            }
            else
            {
                strcpy(perguntas->nivel_02.alternativas_nivel_02[l_02],texto);
                l_02++;
            }
        }
    }
}
void preenche_perguntas_nivel_03(FILE *arquivo, PERGUNTAS *perguntas)
{
    char texto[600];
    int i, n, m=0;
    char op;
    for(i=0; i<5; i++)
    {
        if(fgets(texto, 600, arquivo)!=NULL)
        {
            if(i==0)
            {
                strcpy(perguntas->nivel_03.questao_nivel_03[j_03],texto);
                j_03++;
            }
            else if(texto[0]=='*')
            {
                for(n=1; texto[n]!='\0'; n++)
                {
                    perguntas->nivel_03.alt_certa_nivel_03[k_03][m]=texto[n];
                    m++;
                }
                perguntas->nivel_03.alt_certa_nivel_03[k_03][m]='\0';
                k_03++;
            }
            else
            {
                strcpy(perguntas->nivel_03.alternativas_nivel_03[l_03],texto);
                l_03++;
            }
        }
    }
}
void preenche_perguntas_nivel_04(FILE *arquivo, PERGUNTAS *perguntas)
{
    char texto[300];
    int i, n, m=0;
    char op;
    for(i=0; i<5; i++)
    {
        if(fgets(texto, 300, arquivo)!=NULL)
        {
            if(i==0)
            {
                strcpy(perguntas->nivel_04.questao_nivel_04[j_04],texto);
                j_04++;
            }
            else if(texto[0]=='*')
            {
                for(n=1; texto[n]!='\0'; n++)
                {
                    perguntas->nivel_04.alt_certa_nivel_04[k_04][m]=texto[n];
                    m++;
                }
                perguntas->nivel_04.alt_certa_nivel_04[k_04][m]='\0';
                k_04++;
            }
            else
            {
                strcpy(perguntas->nivel_04.alternativas_nivel_04[l_04],texto);
                l_04++;
            }
        }
    }
}
void chama_mostra_perguntas_01(PERGUNTAS perguntas, DADOS_JOGO *dados)
{
    int i=0, k=0; // K E' O INDEX DAS ALTERNATIVAS
    char j;
    dados->acerto = 0; // DEFINE OS VALORES DAS AJUDAS E DOS ACERTOS
    dados->universitario = 1;
    dados->cartas = 1;
    dados->placas = 1;
    dados->pulos = 3;
    int errou = 0;
    while(dados->acerto<5 && errou!=1) // se acertos < 5 ainda esta no nivel 1, se é igual a 1 -> errou a questão
    {
        j = mostra_perguntas_01(perguntas, dados, i, k); // manda ponteiro dados que é o valor
        if(j=='C')  // Resposta Certa
        {
            dados->acerto++;
            mostra_ganho(dados);
        }
        else if(j=='P')
        {
            //função pula
        }
        else  // Resposta Errada
        {
            errou=1;
            mostra_erro(dados);
        }
        i++;  // i é o index das perguntas
        k+=3; // k é o index das alternativas
    }
    if(errou==1)
        perdeu(dados);
    else
        chama_mostra_perguntas_02(perguntas, dados);

}
void chama_mostra_perguntas_02(PERGUNTAS perguntas, DADOS_JOGO *dados)
{
    int i=0, k=0; // k é o index das alternativas
    char j;
    dados->acerto = 5;
    int errou = 0;
    while(dados->acerto<10 && errou!=1) // se acertos < 5 ainda esta no nivel 1, se é igual a 1 -> errou a questão
    {
        j = mostra_perguntas_02(perguntas, dados, i, k); // manda ponteiro dados que é o valor
        if(j=='C')  // Resposta Certa
        {
            dados->acerto++;
            mostra_ganho(dados);
        }
        else if(j=='P')
        {
            //função pula
        }
        else  // Resposta Errada
        {
            errou=1;
            mostra_erro(dados);
        }
        i++;  // i é o index das perguntas
        k+=3; // k é o index das alternativas
    }
    if(errou==1)
        perdeu(dados);
    else
        chama_mostra_perguntas_03(perguntas, dados);
}
void chama_mostra_perguntas_03(PERGUNTAS perguntas, DADOS_JOGO *dados)
{
    int i=0, k=0; // k é o index das alternativas
    char j;
    dados->acerto = 10;
    int errou=0;
    while(dados->acerto<15 && errou!=1) // se acertos < 5 ainda esta no nivel 1, se é igual a 1 -> errou a questão
    {
        j = mostra_perguntas_03(perguntas, dados, i, k); // manda ponteiro dados que é o valor
        if(j=='C')  // Resposta Certa
        {
            dados->acerto++;
            mostra_ganho(dados);
        }
        else if(j=='P')
        {
            //função pula
        }
        else  // Resposta Errada
        {
            errou=1;
            mostra_erro(dados);
        }
        i++;  // i é o index das perguntas
        k+=3; // k é o index das alternativas
    }
    if(errou==1)
        perdeu(dados);
    else
        chama_mostra_perguntas_04(perguntas, dados);
}
void chama_mostra_perguntas_04(PERGUNTAS perguntas, DADOS_JOGO *dados)
{
    int i=0, k=0; // k é o index das alternativas
    char j;
    dados->acerto = 15;
    int errou=0;
    j = mostra_perguntas_04(perguntas, dados, i, k);
    if(j=='C')
        dados->acerto++;
    else
    {
        errou=1;
        mostra_erro(dados);
    }
    if(errou==1)
        perdeu(dados);
    else
        ganhou();
}
char mostra_perguntas_01(PERGUNTAS perguntas, DADOS_JOGO *dados, int i, int k) // RECEBE A COPIA DOS DADOS
{
    clrscr();
    char alts[4][MAX_STRING];
    int resp_certa;         // VARIAVEL PARA DEFINIR A ALTERNATIVA CERTA
    int funcao_carta;
    int alt_a, alt_b, alt_c; // VARIAVEIS PARA RANDOMIZAR AS ALTERNATIVAS ERRADAS (3)
    int l,j,m;
    j=k+3;
    char op;
    do
    {
        resp_certa = rand() % 4; // RANDOMIZACAO DA ALTERNATIVA CERTA E ABAIXO A RANDOMIZACAO DAS ALTERNATIVAS ERRADAS
        alt_a = rand() % 4;
        alt_b = rand() % 4;
        alt_c = rand() % 4;
    }                           // O LACO REPETE ATE TODAS AS ALTERNATIVAS FOREM DIFERENTES
    while(alt_a==alt_b || alt_a==alt_c || alt_a==resp_certa || alt_b==alt_c || alt_b==resp_certa || alt_c==resp_certa);

    strcpy(alts[alt_a],perguntas.nivel_01.alternativas_nivel_01[k]);        // | COPIA AS ALTERNATIVAS PARA A STRUCT DE PERGUNTAS
    strcpy(alts[alt_b],perguntas.nivel_01.alternativas_nivel_01[k+1]);      // | COPIA AS ALTERNATIVAS PARA A STRUCT DE PERGUNTAS
    strcpy(alts[alt_c],perguntas.nivel_01.alternativas_nivel_01[k+2]);      // | COPIA AS ALTERNATIVAS PARA A STRUCT DE PERGUNTAS
    strcpy(alts[resp_certa],perguntas.nivel_01.alt_certa_nivel_01[i]);      // | COPIA AS ALTERNATIVAS PARA A STRUCT DE PERGUNTAS

    do
    {
        clrscr();
        printf("Dificuldade 1 ------------------------------------------------------------------------------------------------------------------------\n");
        puts("");
        printf("%s\n", perguntas.nivel_01.questao_nivel_01[i]);
        for(m=0; m<4; m++)
            printf("<%d> %s\n", m+1, alts[m]);
        cont_perg++;
        printf("\n\nPergunta %d  --------------------------------------------------------------------------------------------------------------------------\n", cont_perg);
        desenha_indice(dados);
        printf("\nAjudas  ------------------------------------------------------------------------------------------------------------------------------\n");
        if(dados->universitario==1)
            printf("[Universitarios (u)]");
        if(dados->placas==1)
            printf(" [Placas (a)]");
        if(dados->cartas==1)
            printf(" [Cartas (c)]");
        if(dados->pulos>0)
            printf(" [Pulo (p) : %d]\n", dados->pulos);
        char x[2];
        itoa(resp_certa+1, x, 10);
        /*printf("\ndigite a reposta %c\n", x[0]);*/
        fflush(stdin);

        char a; // VARIAVEL CRIADA APENAS PARA AJUDAR [(a)ssist)] NA INTERATIVIDADE COM O USUARIO
        op = getch();
        op = toupper(op);
        if(op == x[0]) /* index resposta certa  MUDAR */
            return 'C';                                                // C - CERTO
        else if(op=='1' || op=='2' || op=='3')
            return 'E';                                               // E - ERRADO
        else if(op == ESC)
        {
            //função salva jogo
            Menu();
        }
        else if(op == 'R') // R -> PARAR
        {
            mostra_pause(dados);
        }
        else if(op == 'U') // U -> UNIVERSITARIOS
        {
            if(dados->universitario==1)
            {
                dados->universitario--;
                ajuda_universitario(resp_certa);
                op='Z';
            }
            else
            {
                puts("Tecla Invalida\n");
                op = 'Z';                               // Z representa o caracter de erro
                cont_perg--;
                a = getch();                            // PEGA A TECLA DIGITADA PELO USUARIO
            }
        }
        else if(op == 'C') // C -> CARTAS
        {
            if(dados->cartas==1)
            {
                dados->cartas--;
                funcao_carta = ajuda_cartas(alts, resp_certa);
                if(funcao_carta==1)
                    return 'C';
                else
                    return 'E';
            }
            else
            {
                puts("Tecla Invalida\n");
                op = 'Z';                               // Z representa o caracter de erro
                cont_perg--;
                a = getch();                            // PEGA A TECLA DIGITADA PELO USUARIO
            }
        }
        else if(op == 'A') // A -> PLACAS
        {
            if(dados->placas==1)
            {
                ajuda_placas(resp_certa);
                op='Z';
                dados->placas--;
            }
            else
            {
                puts("Tecla Invalida\n");
                op = 'Z';                               // Z representa o caracter de erro
                cont_perg--;
                a = getch();                            // PEGA A TECLA DIGITADA PELO USUARIO
            }
        }
        else if(op == 'P') // P -> PULOS
        {
            if(dados->pulos > 0)
            {
                dados->pulos--;
                return 'P';
            }
            else
            {
                puts("Tecla Invalida\n");
                op = 'Z';                               // Z representa o caracter de erro
                a = getch();
            }
        }
    }
    while (op == 'Z');
}
char mostra_perguntas_02(PERGUNTAS perguntas, DADOS_JOGO *dados, int i, int k)
{
    clrscr();
    char alts[4][MAX_STRING];
    int resp_certa;         // VARIAVEL PARA DEFINIR A ALTERNATIVA CERTA
    int funcao_carta;
    int alt_a, alt_b, alt_c; // VARIAVEIS PARA RANDOMIZAR AS ALTERNATIVAS ERRADAS (3)
    int l,j,m;
    j=k+3;
    char op;

    do
    {
        resp_certa = rand() % 4;    // RANDOMIZACAO DA ALTERNATIVA CERTA E ABAIXO A RANDOMIZACAO DAS ALTERNATIVAS ERRADAS.
        alt_a = rand() % 4;
        alt_b = rand() % 4;
        alt_c = rand() % 4;
    }                               // O LACO REPETE ATE TODAS AS ALTERNATIVAS FOREM DIFERENTES
    while(alt_a==alt_b || alt_a==alt_c || alt_a==resp_certa || alt_b==alt_c || alt_b==resp_certa || alt_c==resp_certa);
    strcpy(alts[alt_a],perguntas.nivel_02.alternativas_nivel_02[k]);
    strcpy(alts[alt_b],perguntas.nivel_02.alternativas_nivel_02[k+1]);
    strcpy(alts[alt_c],perguntas.nivel_02.alternativas_nivel_02[k+2]);
    strcpy(alts[resp_certa],perguntas.nivel_02.alt_certa_nivel_02[i]);

    do
    {
        clrscr();
        printf("Dificuldade 2 ------------------------------------------------------------------------------------------------------------------------\n");
        puts("");
        printf("%s\n", perguntas.nivel_02.questao_nivel_02[i]);
        for(m=0; m<4; m++)
            printf("<%d> %s\n", m+1, alts[m]);
        cont_perg++;
        printf("\n\nPergunta %d  --------------------------------------------------------------------------------------------------------------------------\n", cont_perg);
        desenha_indice(dados);
        printf("\nAjudas  ------------------------------------------------------------------------------------------------------------------------------\n");
        if(dados->universitario==1)
            printf("\n[Universitarios (u)]");
        if(dados->placas==1)
            printf("  [Placas (a)]");
        if(dados->cartas==1)
            printf("  [Cartas (c)]");
        if(dados->pulos>0)
            printf("  [Pulo (p) : %d]\n", dados->pulos);

        char x[2];
        char a; // VARIAVEL CRIADA APENAS PARA AJUDAR [(a)ssist)] NA INTERATIVIDADE COM O USUARIO
        itoa(resp_certa+1, x, 10);
        /*printf("\ndigite a reposta %c\n", x[0]);*/
        fflush(stdin);
        op = getch();
        op = toupper(op);
        if(op == x[0]) /* index resposta certa  MUDAR */
            return 'C';                                                // C - CERTO
        else if(op=='1' || op=='2' || op=='3')
            return 'E';                                               // E - ERRADO
        else if(op == ESC)
        {
            //função salva jogo
            Menu();
        }
        else if(op == 'U')  // U -> UNIVERSITARIOS
        {
            if(dados->universitario==1)
            {
                dados->universitario--;
                ajuda_universitario(resp_certa);
                op='Z';
            }
            else
            {
                puts("Tecla Invalida\n");
                op = 'Z';                               // Z representa o caracter de erro
                cont_perg--;
                a = getch();                            // PEGA A TECLA DIGITADA PELO USUARIO
            }
        }
        else if(op == 'C') // C -> Cartas
        {
            if(dados->cartas==1)
            {
                dados->cartas--;
                funcao_carta = ajuda_cartas(alts, resp_certa);
                if(funcao_carta==1)
                    return 'C';
                else
                    return 'E';
            }
            else
            {
                puts("Tecla Invalida\n");
                op = 'Z';                               // Z representa o caracter de erro
                cont_perg--;
                a = getch();                             // PEGA A TECLA DIGITADA PELO USUARIO
            }
        }
        else if(op == 'A') // A -> Placas
        {
            if(dados->placas==1)
            {
                ajuda_placas(resp_certa);
                op='Z';
                dados->placas--;
            }
            else
            {
                puts("Tecla Invalida\n");
                op = 'Z';                               // Z representa o caracter de erro
                cont_perg--;
                a = getch();                            // PEGA A TECLA DIGITADA PELO USUARIO
            }
        }
        else if(op == 'P') // p -> PULOS
        {
            if(dados->pulos > 0)
            {
                dados->pulos--;
                return 'P';
            }
            else
            {
                puts("Tecla Invalida\n");
                op = 'Z';                               // Z representa o caracter de erro
            }
        }
    }
    while (op == 'Z' );
}
char mostra_perguntas_03(PERGUNTAS perguntas, DADOS_JOGO *dados, int i, int k)
{
    clrscr();
    char alts[4][MAX_STRING];
    int resp_certa;         // VARIAVEL PARA DEFINIR A ALTERNATIVA CERTA
    int funcao_carta;
    int alt_a, alt_b, alt_c; // VARIAVEIS PARA RANDOMIZAR AS ALTERNATIVAS ERRADAS (3)
    int l,j,m;
    j=k+3;
    char op;

    do
    {
        resp_certa = rand() % 4;    // RANDOMIZACAO DA ALTERNATIVA CERTA E ABAIXO A RANDOMIZACAO DAS ALTERNATIVAS ERRADAS.
        alt_a = rand() % 4;
        alt_b = rand() % 4;
        alt_c = rand() % 4;
    }                               // O LACO REPETE ATE TODAS AS ALTERNATIVAS FOREM DIFERENTES
    while(alt_a==alt_b || alt_a==alt_c || alt_a==resp_certa || alt_b==alt_c || alt_b==resp_certa || alt_c==resp_certa);
    strcpy(alts[alt_a],perguntas.nivel_03.alternativas_nivel_03[k]);
    strcpy(alts[alt_b],perguntas.nivel_03.alternativas_nivel_03[k+1]);
    strcpy(alts[alt_c],perguntas.nivel_03.alternativas_nivel_03[k+2]);
    strcpy(alts[resp_certa],perguntas.nivel_03.alt_certa_nivel_03[i]);
    do
    {
        clrscr();
        printf("Dificuldade 3 ---------------------------------------------------------------------------------------------------------------------------\n");
        puts("");
        printf("%s\n", perguntas.nivel_03.questao_nivel_03[i]);
        for(m=0; m<4; m++)
            printf("<%d> %s\n", m+1, alts[m]);
        cont_perg++;
        printf("\n\nPergunta %d  ----------------------------------------------------------------------------------------------------------------------------\n", cont_perg);
        desenha_indice(dados);
        printf("\nAjudas  ---------------------------------------------------------------------------------------------------------------------------------\n");
        if(dados->universitario==1)
            printf("\n[Universitarios (u)]");
        if(dados->placas==1)
            printf("  [Placas (a)]");
        if(dados->cartas==1)
            printf("  [Cartas (c)]");
        if(dados->pulos>0)
            printf("  [Pulo (p) : %d]\n", dados->pulos);

        char a; // VARIAVEL CRIADA APENAS PARA AJUDAR [(a)ssist)] NA INTERATIVIDADE COM O USUARIO
        char x[2];
        itoa(resp_certa+1, x, 10);
        /*printf("\ndigite a reposta %c\n", x[0]);*/
        fflush(stdin);
        op = getch();
        op = toupper(op);
        if(op == x[0])
            return 'C';                                                // C - CERTO
        else if(op=='1' || op=='2' || op=='3')
            return 'E';                                               // E - ERRADO
        else if(op == ESC)
        {
            //função salva jogo
            Menu();
        }
        else if(op == 'U') // U -> UNIVERSITARIOS
        {
            if(dados->universitario==1)// O LACO REPETE ATE TODAS AS ALTERNATIVAS FOREM DIFERENTES
            {
                dados->universitario--;
                ajuda_universitario(resp_certa);
                op='Z';
            }
            else
            {
                puts("Tecla Invalida\n");
                op = 'Z';                               // Z representa o caracter de erro
                cont_perg--;
                a = getch();                            // PEGA A TECLA DIGITADA PELO USUARIO
            }
        }
        else if(op == 'C') // C -> Cartas
        {
            if(dados->cartas==1)
            {
                dados->cartas--;
                funcao_carta = ajuda_cartas(alts, resp_certa);
                if(funcao_carta==1)
                    return 'C';
                else
                    return 'E';
            }
            else
            {
                puts("Tecla Invalida\n");
                op = 'Z';                               // Z representa o caracter de erro
                cont_perg--;
                a = getch();                            // PEGA A TECLA DIGITADA PELO USUARIO
            }
        }
        else if(op == 'A') // A -> Placas
        {
            if(dados->placas==1)
            {
                ajuda_placas(resp_certa);
                op='Z';
                dados->placas--;
            }
            else
            {
                puts("Tecla Invalida\n");
                op = 'Z';                               // Z representa o caracter de erro
                cont_perg--;
                a = getch();                            // PEGA A TECLA DIGITADA PELO USUARIO
            }
        }
        else if(op == 'P') // P -> PULOS
        {
            if(dados->pulos > 0)
            {
                dados->pulos--;
                return 'P';
            }
            else
            {
                puts("Tecla Invalida\n");
                op = 'Z';                               // Z representa o caracter de erro
            }
        }
    }
    while(op == 'Z');
}
char mostra_perguntas_04(PERGUNTAS perguntas, DADOS_JOGO *dados, int i, int k)
{
    clrscr();
    char alts[4][MAX_STRING];
    int resp_certa;          // VARIAVEL PARA DEFINIR A ALTERNATIVA CERTA
    int alt_a, alt_b, alt_c; // VARIAVEIS PARA RANDOMIZAR AS ALTERNATIVAS ERRADAS (3)
    int l,j,m;
    j=k+3;
    char op;

    do
    {
        resp_certa = rand() % 4;    // RANDOMIZACAO DA ALTERNATIVA CERTA E ABAIXO A RANDOMIZACAO DAS ALTERNATIVAS ERRADAS.
        alt_a = rand() % 4;
        alt_b = rand() % 4;
        alt_c = rand() % 4;
    }                               // O LACO REPETE ATE TODAS AS ALTERNATIVAS FOREM DIFERENTES
    while(alt_a==alt_b || alt_a==alt_c || alt_a==resp_certa || alt_b==alt_c || alt_b==resp_certa || alt_c==resp_certa);
    strcpy(alts[alt_a],perguntas.nivel_04.alternativas_nivel_04[k]);
    strcpy(alts[alt_b],perguntas.nivel_04.alternativas_nivel_04[k+1]);
    strcpy(alts[alt_c],perguntas.nivel_04.alternativas_nivel_04[k+2]);
    strcpy(alts[resp_certa],perguntas.nivel_04.alt_certa_nivel_04[i]);

    printf("Dificuldade 4 ------------------------------------------------------------------------------------------------------------------------\n");
    puts("");
    printf("%s\n", perguntas.nivel_04.questao_nivel_04[i]);
    for(m=0; m<4; m++)
        printf("<%d> %s\n", m+1, alts[m]);
    cont_perg++;
    printf("\n\nPergunta do Milhão!!!!!  -------------------------------------------------------------------------------------------------------------\n");
    desenha_indice(dados);
    do
    {
        char x[2];
        itoa(resp_certa+1, x, 10);
        printf("digite a reposta %c\n", x[0]);
        fflush(stdin);
        op = getch();
        op = toupper(op);
        if(op == x[0]) /* index resposta certa  MUDAR */
            return 'C';                                                // C - CERTO
        else if(op=='1' || op=='2' || op=='3')
            return 'E';                                               // E - ERRADO
        else if(op == ESC)
        {
            //função salva jogo
            Menu();
        }
    }
    while(op!='1' || op!='2' || op!='3' || op!='4' || op!=ESC);
}
void regras()
{
    FILE *regras;
    regras = fopen("regras.txt", "r");
    clrscr();

    if(regras==NULL)
        printf("Erro ao abrir o arquivo '%s'", regras);

    //system("mode con:cols=110 lines=100");
    char texto[500];
    while (!feof(regras))
    {
        if(fgets(texto, 500, regras)!= NULL)
            printf("%s\n", texto);
    }
    puts("\n\n");
    system("pause");
}
void mostra_ganho(DADOS_JOGO *dados)
{
    clrscr();
    char op;
    int acerto[] = {1000, 2000, 3000, 4000, 5000, 10000, 20000, 30000, 40000,           // DEFINICAO DE VALORES DA PONTUACAO GERAL DO JOGO
                    50000, 100000, 200000, 300000, 400000, 500000, 1000000
                   };
    dados->pontuacao=acerto[dados->acerto];
    printf("\n");
    printf("\n");
    printf("\t\t_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n");
    printf("\t\t                                                                             \n");
    printf("\t\t|                                                                               |\n");
    printf("\t\t                                                                               \n");
    printf("\t\t|                               CERTA RESPOSTA                                  |\n");
    printf("\t\t                          Você GANHOU %d mil Reais !!!                         \n", acerto[dados->acerto-1]);
    printf("\t\t|                             <PRESSIONE ENTER>                                 |\n");
    printf("\t\t                                                                               \n");
    printf("\t\t|                                                                               |\n");
    printf("\t\t_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n");
    op=getch();
    if(op == ENTER)
        return;
}
void ganhou()
{
    system("mode con:cols=110 lines=30");

    clrscr();
    char op;
    printf("\n");
    printf("\t\t\t\t       ____________________       \n");
    printf("\t\t\t\t     /|                    |\\    \n");
    printf("\t\t\t\t    / |                    | \\   \n");
    printf("\t\t\t\t  ||  |   SHOW DO MILHÃO   |  ||  \n");
    printf("\t\t\t\t    \\ |                    | /   \n");
    printf("\t\t\t\t     \\|                    |/    \n");
    printf("\t\t\t\t      |                    |      \n");
    printf("\t\t\t\t       \\                  /       \n");
    printf("\t\t\t\t        \\                /        \n");
    printf("\t\t\t\t         \\              /         \n");
    printf("\t\t\t\t          |            |          \n");
    printf("\t\t\t\t           |          |           \n");
    printf("\t\t\t\t            |        |            \n");
    printf("\t\t\t\t             |      |             \n");
    printf("\t\t\t\t             |      |             \n");
    printf("\t\t\t\t      _______|      |_______      \n");
    printf("\t\t\t\t     |                      |     \n");
    printf("\t\t\t\t     |   VOCÊ É O CAMPEÃO   |     \n");
    printf("\t\t\t\t     |      R$ 1000000      |     \n");
    printf("\t\t\t\t     |  EM BARRAS DE OURO   |     \n");
    printf("\t\t\t\t     |(que valem mais do que|     \n");
    printf("\t\t\t\t     |     em dinheiro)     |     \n");
    printf("\t\t\t\t     |______________________|     \n");
    puts("");
    printf("\t\t\t\t   Pressione tecla E para sair\n");

    op = getch();
    toupper(op);
    if(op=='E')
        main();
}

void perdeu(DADOS_JOGO *dados)
{
    int errou[] = {0, 500, 1000, 1500, 2000, 2500, 5000, 10000, 15000,
                   20000, 25000, 50000, 100000, 150000, 200000, 0
                  };  // DEFINICAO DE VALORES DA PONTUACAO GERAL DO JOGO

    clrscr();
    printf("\n\n");
    char op;
    printf("                      ||      \n");
    printf("                     ||       \n");
    printf("                    ||       \n");
    printf("                   ||       \n");
    printf("                  ||      VOCE PERDEU  \n");
    printf("      _          ||     \n");
    printf("     |_|        ||        GANHOU %d REAIS  \n", errou[dados->acerto] );
    printf("      _        ||       \n");
    printf("     |_|        ||        APERTE <S> PARA SAIR  \n");
    printf("                 ||       \n");
    printf("                  ||      APERTE <ENTER> PARA MENU\n");
    printf("                   ||        \n");
    printf("                    ||       \n");
    printf("                     ||       \n");
    printf("                      ||       \n");
    dados->pontuacao=errou[dados->acerto];
    do
    {
        fflush(stdin);
        op = getch();
        op = toupper(op);
        if(op==ENTER)
        {
            main();
            cont_perg = 0;                                  // ZERA O INDICE DAS PERGUNTAS
        }
        if (op == 'S')
            exit(0);
    }while(op!=ENTER);
}
void mostra_pause (DADOS_JOGO *dados)
{
    int parou[] = {0, 1, 2, 3, 4, 5, 10, 20, 30, 40,         // DEFINICAO DE VALORES DA PONTUACAO GERAL DO JOGO
                   50, 100, 200, 300, 400, 500
                  };
    char op;
    clrscr();
    printf("\n");
    printf("\n");
    printf("\t\t_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n");
    printf("\t\t|                                                                               |\n");
    printf("\t\t|                                                                               |\n");
    printf("\t\t|                                                                               |\n");
    printf("\t\t|                               VOCÊ PAROU!                                     |\n");
    printf("\t\t|                          Você GANHOU %d mil Reais !!!                          |\n", parou[dados->acerto]);
    printf("\t\t|                             <PRESSIONE ENTER>                                 |\n");
    printf("\t\t|                                                                               |\n");
    printf("\t\t|                                                                               |\n");
    printf("\t\t|_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _|\n");
    op=getch();

    if(op == ENTER)
        exit(0);
}
void mostra_erro (DADOS_JOGO *dados)
{
    clrscr();
    int errou[] = {0, 500, 1000, 1500, 2000, 2500, 5000, 10000, 15000,
                   20000, 25000, 50000, 100000, 150000, 200000, 0
                  };         // DEFINICAO DE VALORES DA PONTUACAO GERAL DO JOGO|

    char op;
    printf("\n");
    printf("\n");
    printf("\t\t_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n");
    printf("\t\t                                                                               \n");
    printf("\t\t|                                                                               |\n");
    printf("\t\t                                                                               \n");
    printf("\t\t|                               VOCÊ ERROU!                                     |\n");
    printf("\t\t                         Entretanto GANHOU %d Reais !!!                         \n", errou[dados->acerto]);
    printf("\t\t|                             <PRESSIONE ENTER>                                 |\n");
    printf("\t\t                                                                               \n");
    printf("\t\t|                                                                               |\n");
    printf("\t\t_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n");
    op=getch();
    if(op == ENTER)
        return;
}
void ajuda_universitario(int resp_certa)
{
    clrscr();
    int i=0;
    int correta = resp_certa;
    int incorreta_selecionada;
    printf("\n\n\n\n\n\n");
    do
    {

        if(rand()% 100 + 1 <80) // CALCULO DA RANDOMIZACAO DA AJUDA UNIVERSITARIA DE 80%
        {
            printf("\t\t\t\t\tUniversitario %c : %d\n", 65+i, resp_certa+1);
        }
        else
        {
            incorreta_selecionada = rand() % 4 + 1;
            if (incorreta_selecionada == correta)
            {
                incorreta_selecionada++;
            }
            printf("\t\t\t\t\tUniversitario %c : %d\n", 65+i, incorreta_selecionada);
        }
        i++;
    }
    while(i<3);
    printf("\t\t\t\t\t<Tecle Enter>\n");
    cont_perg--;
    char op;
    op = getch();
}
int ajuda_cartas(char perguntas[][MAX_STRING], int resp_certa)
{
    clrscr();
    int i, k, carta;
    int resposta;
    int tira_as;
    int tira_2, tira_2_2;
    int tira_3, tira_3_2, tira_3_3;
    char op;
    /*printf("%d resp_certa\n", resp_certa);*/
    printf("Ajuda cartas\n");
    puts("\t _______   _______   _______   _______");
    puts("\t|       | |       | |       | |       |");
    puts("\t|       | |       | |       | |       |");
    puts("\t|   1   | |   2   | |   3   | |   4   |");
    puts("\t|       | |       | |       | |       |");
    puts("\t|       | |       | |       | |       |");
    puts("\t|_______| |_______| |_______| |_______|");

    do
    {
        printf("Digite qual a sua carta\n");
        scanf("%d", &carta);
    }
    while(carta < 1 || carta > 4);
    k = rand() % 3;
    if(k==0)  // Tira nenhuma alternativa
    {
        printf("Carta Sorteada: Rei\n");//desenha carta rei
        printf("Não Tira nenhuma Alternativa\n");
        for(i=0; i<4; i++)
            printf("<%d> %s\n", i+1, perguntas[i]);
    }
    if(k==1) // Tira Uma alternativa
    {
        printf("Carta Sorteada: Ás\n");
        printf("Tira Uma Alternativa\n");
        do
        {
            tira_as = rand() % 3;
        }
        while(tira_as==resp_certa);
        for(i=0; i<4; i++)
        {
            if(tira_as!=i)
                printf("<%d> %s\n", i+1, perguntas[i]);
            else
                printf("\n\n");
        }
    }
    if(k==2) // Tira duas alternativas
    {
        printf("Carta Sorteada: 2\n");
        printf("Tira Duas Alternativa\n");
        do
        {
            tira_2 = rand() % 3;
            tira_2_2 = rand() % 3;
        }
        while(tira_2==resp_certa || tira_2_2==resp_certa || tira_2==tira_2_2);
        for(i=0; i<4; i++)
        {
            if(tira_2!=i && tira_2_2!=i)
                printf("<%d> %s\n", i+1, perguntas[i]);
            else
                printf("\n\n");
        }
    }
    if(k==3) // Tira tres alternativas
    {
        printf("Carta Sorteada: 3\n");
        printf("Tira Três Alternativa\n");
        printf("<%d> %s\n",resp_certa, perguntas[resp_certa]);
    }
    printf("Digite a resposta: \n");
    scanf("%d", &resposta);
    if(resposta==(resp_certa+1))
        return 1;
    else
        return 0;
}
void ajuda_placas(int resp_certa)
{
    clrscr();
    int j, i=0;
    int alternativa_mostrada;
    int incorreta;
    printf("\n\n\n\n");
    printf("\t\t\t\t\tPlacas \n");
    for(j=0; j<2; j++)
    {
        i=0;
        printf("\t\t\t\t\t");
        while(i<10)
        {

            if(rand() % 100 + 1 < 65) // CALCULO DA RANDOMIZACAO DA AJUDA PLACAS DE 65%
                printf("[%d]", resp_certa+1);
            else
            {
                incorreta = rand() % 3;
                if (incorreta == resp_certa)
                {
                    incorreta++;
                }
                printf("[%d]", incorreta+1);
            }
            i++;
        }
        puts("");
    }
    cont_perg--;
    printf("\t\t\t\t\t<Tecle Enter>\n");
    char a;  // VARIAVEL CRIADA APENAS PARA AJUDAR [(a)ssist)] NA INTERATIVIDADE COM O USUARIO
    a = getch();
}

void desenha_indice(DADOS_JOGO *dados)
{
    int acerto[] = {1, 2, 3, 4, 5, 10, 20, 30, 40,           // DEFINICAO DE VALORES DA PONTUACAO GERAL DO JOGO
                    50, 100, 200, 300, 400, 500, 1
                   };
    int parou[] = {0, 1, 2, 3, 4, 5, 10, 20, 30, 40,         // DEFINICAO DE VALORES DA PONTUACAO GERAL DO JOGO
                   50, 100, 200, 300, 400, 500
                  };
    int errou[] = {0, 500, 1, 1500, 2, 2500, 5, 10, 15,
                   20, 25, 50, 100, 150, 200, 0
                  };
    if(errou[dados->acerto]==0 && parou[dados->acerto]==0) // primeiro caso da tabela
        printf("[ACERTAR : R$ %d mil] [PARAR (r) : R$ %d] [ERRAR : R$ %d]\n", acerto[dados->acerto], parou[dados->acerto], errou[dados->acerto]);
    else if(errou[dados->acerto]==500) // segundo caso da tabela
        printf("[ACERTAR : R$ %d mil] [PARAR (r) : R$ %d mil] [ERRAR : R$ %d]\n", acerto[dados->acerto], parou[dados->acerto], errou[dados->acerto]);
    else if(errou[dados->acerto]==0 && acerto[dados->acerto]==1) // ultimo caso da tabela
        printf("[ACERTAR : R$ %d milhão] [PARAR (r) : R$ %d mil] [ERRAR : R$ %d]\n", acerto[dados->acerto], parou[dados->acerto], errou[dados->acerto]);
    else // para os demais
        printf("[ACERTAR : R$ %d mil] [PARAR (r) : R$ %d mil] [ERRAR : R$ %d mil]\n", acerto[dados->acerto], parou[dados->acerto], errou[dados->acerto]);
}
