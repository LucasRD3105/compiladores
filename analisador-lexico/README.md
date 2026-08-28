# Analisador Léxico MiniC

Analisador léxico simplificado para a linguagem MiniC, desenvolvido em C como atividade prática da disciplina de Compiladores.

O programa realiza a leitura de um arquivo-fonte caractere por caractere e identifica os tokens encontrados, mostrando sua linha, coluna, categoria e lexema.

## Funcionalidades

O analisador reconhece:

- Palavras reservadas: `int`, `float`, `char`, `if`, `else`, `while`, `return` e `print`
- Identificadores
- Números inteiros
- Números reais
- Literais de caractere
- Operadores simples e compostos
- Delimitadores
- Comentários de linha (`//`)

Também são detectados erros léxicos, como identificadores muito longos, números malformados, literais inválidos e símbolos não reconhecidos.

## Compilação

```bash
gcc -Wall -Wextra -pedantic -std=c11 minilexer.c -o minilexer
```

## Execução

```bash
./minilexer <arquivo-fonte>
```

Exemplo:

```bash
./minilexer testes/exemplo.mc
```

## Exemplo

Para a entrada:

```c
if(x>=10){x=x+1;}
```

A saída é:

```text
1:1 | PALAVRA_RESERVADA | if
1:3 | DELIMITADOR | (
1:4 | IDENTIFICADOR | x
1:5 | OPERADOR | >=
1:7 | NUMERO_INTEIRO | 10
1:9 | DELIMITADOR | )
1:10 | DELIMITADOR | {
1:11 | IDENTIFICADOR | x
1:12 | OPERADOR | =
1:13 | IDENTIFICADOR | x
1:14 | OPERADOR | +
1:15 | NUMERO_INTEIRO | 1
1:16 | DELIMITADOR | ;
1:17 | DELIMITADOR | }
Total de tokens: 14
Total de erros léxicos: 0
```

## Testes

Os arquivos utilizados para testar o analisador estão disponíveis na pasta `testes/`.

Foram testados casos envolvendo palavras reservadas, identificadores, números, operadores, delimitadores, comentários, literais de caractere, caracteres inválidos, arquivo vazio e código sem espaços.

## Estrutura

```text
analisador-lexico/
├── minilexer
├── minilexer.c
├── README.md
├── relatorio.pdf
└── testes/
```

## Autor

Lucas Rocha Dantas
