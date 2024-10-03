# Hashing Methods Comparison Program

Este projeto implementa e compara diferentes métodos de hashing, utilizando **Linear Probing**, **Double Hashing**, **Encadeamento Explícito** e **Encadeamento Explícito sem Junção**. O objetivo do programa é gerar dados de exemplo, armazená-los em arquivos e avaliar a eficiência de cada técnica de hashing, medindo o número de acessos necessários para encontrar um registro.

## Funcionalidades

- **Linear Probing**: Resolve colisões de hash encontrando a próxima posição livre na tabela.
- **Double Hashing**: Resolve colisões com uma segunda função hash que é usada para calcular um incremento.
- **Encadeamento Explícito**: Usa listas encadeadas para lidar com colisões, onde cada posição de hash tem uma lista associada.
- **Encadeamento Explícito sem Junção**: Versão modificada do encadeamento explícito que trata os ponteiros entre registros de forma diferente.

## Arquivos

- **database.dat**: Arquivo de banco de dados binário que armazena os registros de hashing.
- **file.txt**: Arquivo de entrada com números inteiros a serem inseridos na tabela hash.
- **linearProbing.txt**: Arquivo de saída que contém os registros gerados utilizando o método de Linear Probing.
- **doubleHashing.txt**: Arquivo de saída que contém os registros gerados utilizando o método de Double Hashing.
- **explicitChaining.txt**: Arquivo de saída que contém os registros gerados utilizando o método de Encadeamento Explícito.
- **explicitChainingNoJoining.txt**: Arquivo de saída que contém os registros gerados utilizando o método de Encadeamento Explícito sem Junção.

## Estrutura dos Registros

Cada registro armazenado no banco de dados possui a seguinte estrutura (definida no arquivo `record.h`):

- **`int pos`**: A posição do registro na tabela hash.
- **`int value`**: O valor inteiro armazenado.
- **`int ptr`**: Um ponteiro para o próximo registro (usado para encadeamento).
- **`bool occupied`**: Indica se o registro está ocupado ou não.

## Como Usar

1. **Compilação**: Compile o programa com o seguinte comando:
    ```bash
    gcc -o hash_comparison main.c -lm
    ```

2. **Execução**: Execute o programa, que irá gerar os arquivos de saída com os resultados dos métodos de hashing:
    ```bash
    ./hash_comparison
    ```

3. **Resultados**: Os arquivos de saída (`linearProbing.txt`, `doubleHashing.txt`, `explicitChaining.txt`, `explicitChainingNoJoining.txt`) conterão os registros de hash e o número de acessos necessários para cada técnica.

## Funções Principais

- **populateFile(FILE* d, int size, int percent)**: Preenche o arquivo `file.txt` com valores aleatórios.
- **initializeFile(FILE* f, int m)**: Inicializa o arquivo `database.dat` com registros vazios.
- **linearProbing(FILE* f, FILE* d, int m)**: Aplica o método de Linear Probing.
- **doubleHashing(FILE* f, FILE* d, int m)**: Aplica o método de Double Hashing.
- **explicitChaining(FILE* f, FILE* d, int m)**: Aplica o método de Encadeamento Explícito.
- **explicitChainingNoJoining(FILE* f, FILE* d, int m)**: Aplica o método de Encadeamento Explícito sem Junção.
- **evaluateOpenAddressingTotalAccesses(FILE* f, FILE* d, char type)**: Calcula o número total de acessos para métodos de endereçamento aberto (Linear Probing e Double Hashing).
- **evaluateChainedHashingTotalAccesses(FILE* f, FILE* d)**: Calcula o número total de acessos para métodos de encadeamento.

## Licença

Este projeto está licenciado sob a [MIT License](LICENSE).
