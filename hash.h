#ifndef HASH_H
#define HASH_H

#define PAIR_SIZE (sizeof(int) + sizeof(long int))
#define BLOCK_CAP 10
#define BLOCK_SIZE (PAIR_SIZE * (BLOCK_CAP + 1))

typedef struct par
{
    int     chave;
    long int cont;
} Par;

typedef struct bloco
{
    Par arr[BLOCK_CAP]; // Vetor de (chave, cont)
    int       num_elem; // Número de elementos no bloco
    long int      prox; // Índice no arquivo para bloco extendido desse
} Bloco;

// Função que, dada uma quatidade total esperada de elementos, cria na memória
// secundária os blocos necessários para abriga-los no arquivo hash_path
void inicializa_hash(std::string hash_path, int num_elem);

// Insere um par p na hash do arquivo hash_path
void insere_hash(std::string hash_path, Par p);

// Retorna todos os pares que contém a chave na hash de hash_path
std::vector<Par> busca_hash(std::string hash_path, int chave);


// Funções internas

// Retorna em qual dos baldes iniciais deve ser inserido/buscado
static int hash_func(int chave, int M);

// Insere p no bloco da posição index de arquivo
static void insere_bloco(FILE *arquivo, long int index, Par p);

// Salva bloco blk na posição pos de arquivo, arquivo já deve estar aberto
static long int salva_bloco(FILE *arquivo, long int pos, Bloco blk);

// Retorna o bloco em arquivo a partir de pos, arquivo já deve estar aberto
static Bloco le_bloco(FILE* arquivo, long int pos);

bool operator>(Par a, Par b);
bool operator<(Par a, Par b);

// Função unicamente para DEBUG
void imprime_hash(std::string path);

#endif