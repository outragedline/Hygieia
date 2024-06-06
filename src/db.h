#include <sqlite3.h>
#define SMALL_BUFFER_SIZE 16
#define BUFFER_SIZE 64
#define BIG_BUFFER_SIZE 256

typedef struct Paciente {
	struct Paciente *next;
	struct Paciente *prev;
	struct Paciente *child;
	char nome[BUFFER_SIZE];
	char nasc[BUFFER_SIZE];
	unsigned short gender;
	unsigned short tipoSanguineo;
	char endereco[BUFFER_SIZE];
	char telefone[SMALL_BUFFER_SIZE];
	char email[BUFFER_SIZE];
	char alergias[BIG_BUFFER_SIZE];
	char deficiencias[BIG_BUFFER_SIZE];
	unsigned short tipoAtendimento;
} Paciente;
