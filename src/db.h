#include <sqlite3.h>
#define SMALL_BUFFER_SIZE 16
#define BUFFER_SIZE 64
#define BIG_BUFFER_SIZE 256
#define ERROR_CODE -1
#define OK_CODE 0

enum Genero { masculino, feminino };
typedef struct {
	unsigned int id;
	char *nome;
	char *cpf;
	char *telefone;
	char *cep;
	char *alergias;
	char *deficiencias;
	unsigned int idade;
	enum Genero genero;
} Paciente;

typedef struct {
	unsigned int id;
	char *nome;
	char *especialidade;
	unsigned int cod;
} Medico;

enum Status { agendado, finalizado, cancelado };
typedef struct {
	unsigned int id;
	Paciente *paciente;
	Medico *medico;
	char *dataHora;
	enum Status status;
} Agendamento;

//Todas as funções abaixo retornam OK_CODE se tudo ocorrer bem e ERROR_CODE em caso de erro
int createdb();

/*
 * As funcoes desse grupo podem gerar memory leak pois retornam ponteiros de structs
 * alocados com malloc, nao esqueca de usar suas respectivas funcoes free quando nao
 * precisar mais da variavel as funcoes desse grupo sao excecao as regras impostas 
 * acima e retornam um ponteiro para o struct alocado caso tudo ocorra bem ou NULL caso 
 * algo de errado
*/
// TODO: Funcao de free pra cada struct
Medico *buscarMedico(unsigned int id);

/*
 * As funcoes desse grupo recebem um ponteiro para seu respectivo struct com todos os dados
 * exceto pelo id preenchidos, caso tudo ocorra bem o id sera setado ao longo da execucao
 */
int inserirMedico(Medico *);
int inserirPaciente(Paciente *);
