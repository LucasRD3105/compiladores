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
- Operadores de um e dois caracteres
- Delimitadores
- Comentários de linha (`//`)

Também são detectados erros léxicos, como identificadores com mais de 31 caracteres, números malformados, literais inválidos e símbolos não reconhecidos. Após um erro, o analisador continua processando o restante do arquivo.

## Compilação

Compile utilizando GCC:

```bash
gcc -Wall -Wextra -pedantic -std=c11 minilexer.c -o minilexer
```

## Execução

Execute informando um arquivo-fonte:

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

## Decisões de implementação

O arquivo é lido caractere por caractere utilizando `fgetc()`.

Palavras reservadas e identificadores são inicialmente reconhecidos pela mesma regra. Após a leitura do lexema, é feita a comparação com a lista de palavras reservadas.

Para reconhecer operadores de dois caracteres e diferenciar `/` de comentários iniciados por `//`, é feita a leitura antecipada do próximo caractere. Quando necessário, `ungetc()` é utilizado para devolvê-lo ao fluxo do arquivo.

A linha e a coluna são atualizadas durante toda a leitura para registrar a posição inicial de cada token.

## Limitações

O analisador implementa somente os elementos definidos para esta atividade. Não são reconhecidos:

- comentários de múltiplas linhas;
- strings;
- sequências de escape em literais de caractere;
- números em notação científica.

O programa também não realiza análise sintática, análise semântica ou execução do código.

## Testes

Os arquivos utilizados nos testes estão disponíveis na pasta `testes/`.

Foram testados:

- todas as palavras reservadas;
- identificadores válidos;
- identificador com mais de 31 caracteres;
- números inteiros;
- números reais;
- números reais malformados;
- todos os operadores;
- todos os delimitadores;
- comentários;
- literais de caractere válidos e inválidos;
- caracteres inválidos;
- arquivo vazio;
- arquivo contendo somente espaços e comentários;
- tokens sem espaços entre eles.

Também foram utilizados testes adicionais com diferentes erros léxicos para verificar se o analisador continua processando o arquivo após encontrar um erro.

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
