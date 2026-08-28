#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int ehPalavraReservada(const char *lexema) {
    const char *palavrasReservadas[] = {
        "int", "float", "char", "if",
        "else", "while", "return", "print"
    };

    int quantidade = 8;

    for (int i = 0; i < quantidade; i++) {
        if (strcmp(lexema, palavrasReservadas[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

int ehInicioIdentificador(char c) {
    return isalpha((unsigned char)c) || c == '_';
}

int ehParteIdentificador(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

int ehDelimitador(char c) {
    return c == '(' || c == ')' ||
           c == '{' || c == '}' ||
           c == '[' || c == ']' ||
           c == ';' || c == ',';
}

int ehOperadorSimples(char c) {
    return c == '+' || c == '-' ||
           c == '*' || c == '/' ||
           c == '%' || c == '=' ||
           c == '<' || c == '>' ||
           c == '!';
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: minilexer <arquivo-fonte>\n");
        return EXIT_FAILURE;
    }

    FILE *arquivo = fopen(argv[1], "r");

    if (arquivo == NULL) {
        printf("Erro: não foi possível abrir o arquivo '%s'.\n", argv[1]);
        return EXIT_FAILURE;
    }

    int c;
    int linha = 1;
    int coluna = 1;
    int totalTokens = 0;
    int totalErros = 0;

    while ((c = fgetc(arquivo)) != EOF) {
        if (c == ' ' || c == '\t') {
            coluna++;
            continue;
        }

        if (c == '\n') {
            linha++;
            coluna = 1;
            continue;
        }

        if (ehInicioIdentificador((char)c)) {
            char lexema[64];
            int tamanho = 0;
            int tamanhoReal = 0;
            int linhaInicio = linha;
            int colunaInicio = coluna;

            while (ehParteIdentificador((char)c)) {
                if (tamanho < 63) {
                    lexema[tamanho++] = (char)c;
                }

                tamanhoReal++;
                coluna++;
                c = fgetc(arquivo);
            }

            lexema[tamanho] = '\0';

            if (c != EOF) {
                ungetc(c, arquivo);
            }

            if (tamanhoReal > 31) {
                printf("ERRO_LEXICO | linha %d, coluna %d | identificador com mais de 31 caracteres: %s\n",
                       linhaInicio, colunaInicio, lexema);
                totalErros++;
            } else if (ehPalavraReservada(lexema)) {
                printf("%d:%d | PALAVRA_RESERVADA | %s\n",
                       linhaInicio, colunaInicio, lexema);
                totalTokens++;
            } else {
                printf("%d:%d | IDENTIFICADOR | %s\n",
                       linhaInicio, colunaInicio, lexema);
                totalTokens++;
            }

            continue;
        }

        if (isdigit((unsigned char)c)) {
            char lexema[64];
            int tamanho = 0;
            int linhaInicio = linha;
            int colunaInicio = coluna;
            int quantidadePontos = 0;
            int digitosDepoisPonto = 0;
            int encontrouPonto = 0;

            while (isdigit((unsigned char)c) || c == '.') {
                if (c == '.') {
                    quantidadePontos++;
                    encontrouPonto = 1;
                    digitosDepoisPonto = 0;
                } else if (encontrouPonto) {
                    digitosDepoisPonto++;
                }

                if (tamanho < 63) {
                    lexema[tamanho++] = (char)c;
                }

                coluna++;
                c = fgetc(arquivo);
            }

            lexema[tamanho] = '\0';

            if (c != EOF) {
                ungetc(c, arquivo);
            }

            if (quantidadePontos == 0) {
                printf("%d:%d | NUMERO_INTEIRO | %s\n",
                       linhaInicio, colunaInicio, lexema);
                totalTokens++;
            } else if (quantidadePontos == 1 && digitosDepoisPonto > 0) {
                printf("%d:%d | NUMERO_REAL | %s\n",
                       linhaInicio, colunaInicio, lexema);
                totalTokens++;
            } else {
                printf("ERRO_LEXICO | linha %d, coluna %d | número malformado: %s\n",
                       linhaInicio, colunaInicio, lexema);
                totalErros++;
            }

            continue;
        }

        if (c == '\'') {
            int linhaInicio = linha;
            int colunaInicio = coluna;
            int primeiro = fgetc(arquivo);

            if (primeiro == EOF) {
                printf("ERRO_LEXICO | linha %d, coluna %d | literal de caractere inválido\n",
                       linhaInicio, colunaInicio);
                totalErros++;
                coluna++;
                continue;
            }

            if (primeiro == '\n') {
                printf("ERRO_LEXICO | linha %d, coluna %d | literal de caractere inválido\n",
                       linhaInicio, colunaInicio);
                totalErros++;
                linha++;
                coluna = 1;
                continue;
            }

            if (primeiro == '\'') {
                printf("ERRO_LEXICO | linha %d, coluna %d | literal de caractere inválido\n",
                       linhaInicio, colunaInicio);
                totalErros++;
                coluna += 2;
                continue;
            }

            int segundo = fgetc(arquivo);

            if (segundo == '\'') {
                printf("%d:%d | LITERAL_CARACTERE | '%c'\n",
                       linhaInicio, colunaInicio, primeiro);
                totalTokens++;
                coluna += 3;
                continue;
            }

            printf("ERRO_LEXICO | linha %d, coluna %d | literal de caractere inválido\n",
                   linhaInicio, colunaInicio);
            totalErros++;

            coluna += 2;

            if (segundo == EOF) {
                continue;
            }

            if (segundo == '\n') {
                linha++;
                coluna = 1;
                continue;
            }

            while (segundo != '\'' &&
                   segundo != '\n' &&
                   segundo != EOF) {

                coluna++;
                segundo = fgetc(arquivo);
            }

            if (segundo == '\'') {
                coluna++;
            } else if (segundo == '\n') {
                linha++;
                coluna = 1;
            }

            continue;
        }

        if (ehDelimitador((char)c)) {
            printf("%d:%d | DELIMITADOR | %c\n",
                   linha, coluna, c);

            totalTokens++;
            coluna++;
            continue;
        }

        if (c == '/') {
            int proximo = fgetc(arquivo);

            if (proximo == '/') {
                coluna += 2;

                while ((c = fgetc(arquivo)) != EOF && c != '\n') {
                    coluna++;
                }

                if (c == '\n') {
                    linha++;
                    coluna = 1;
                }

                continue;
            }

            if (proximo != EOF) {
                ungetc(proximo, arquivo);
            }

            printf("%d:%d | OPERADOR | /\n",
                   linha, coluna);

            totalTokens++;
            coluna++;
            continue;
        }

        if (c == '&' || c == '|') {
            int proximo = fgetc(arquivo);

            if ((c == '&' && proximo == '&') ||
                (c == '|' && proximo == '|')) {

                printf("%d:%d | OPERADOR | %c%c\n",
                       linha, coluna, c, proximo);

                totalTokens++;
                coluna += 2;
            } else {
                if (proximo != EOF) {
                    ungetc(proximo, arquivo);
                }

                printf("ERRO_LEXICO | linha %d, coluna %d | símbolo inválido: %c\n",
                       linha, coluna, c);

                totalErros++;
                coluna++;
            }

            continue;
        }

        if (ehOperadorSimples((char)c)) {
            int proximo = fgetc(arquivo);

            if ((c == '=' && proximo == '=') ||
                (c == '!' && proximo == '=') ||
                (c == '<' && proximo == '=') ||
                (c == '>' && proximo == '=')) {

                printf("%d:%d | OPERADOR | %c%c\n",
                       linha, coluna, c, proximo);

                totalTokens++;
                coluna += 2;
            } else {
                if (proximo != EOF) {
                    ungetc(proximo, arquivo);
                }

                printf("%d:%d | OPERADOR | %c\n",
                       linha, coluna, c);

                totalTokens++;
                coluna++;
            }

            continue;
        }

        printf("ERRO_LEXICO | linha %d, coluna %d | símbolo inválido: %c\n",
               linha, coluna, c);

        totalErros++;
        coluna++;
    }

    printf("Total de tokens: %d\n", totalTokens);
    printf("Total de erros léxicos: %d\n", totalErros);

    fclose(arquivo);

    return EXIT_SUCCESS;
}
