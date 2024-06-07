#include <sqlite3.h>
#define SMALL_BUFFER_SIZE 16
#define BUFFER_SIZE 64
#define BIG_BUFFER_SIZE 256

typedef struct {
	unsigned int id;
	char *nome;
	char *cpf;
	char *rg;
	char *telefone;
	char *rua;
	char *bairro;
	char *numero;
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

typedef struct {
	Paciente *p;
	Medico *m;
	char *dataHora;
	unsigned short status;
} Agendamento;

int createdb();
