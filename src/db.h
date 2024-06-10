#include <sqlite3.h>
#define SMALL_BUFFER_SIZE 16
#define BUFFER_SIZE 64
#define BIG_BUFFER_SIZE 256
#define ERROR_CODE -1
#define OK_CODE 0

typedef struct {
	unsigned int id;
	char *nome;
	char *cpf;
	char *telefone;
	char *cep;
	unsigned int idade;
	char *alergias;
	char *deficiencias;
	unsigned short genero;
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
	Paciente *p;
	Medico *m;
	char *dataHora;
	enum Status status;
} Agendamento;

//Todas as funções abaixo retornam OK_CODE se tudo ocorrer bem e ERROR_CODE em caso de erro
int createdb();

/*
 * As funcoes desse grupo recebem um ponteiro para seu respectivo struct com todos os dados
 * exceto pelo id preenchidos, caso tudo ocorra bem o id sera setado ao longo da execucao
 */
int inserirMedico(Medico *);
