
/*
Por tudo que e mais sagrado nao use os structs diretamente,
se voce precisa de uma variavel do tipo de algum desses structs,
utilize sua respectiva funcao responsavel por criar um struct desse
em memoria, novoPaciente(), novoMedico(), etc

Nao esqueca de usar suas respectivas funcoes free apos o uso, freePaciente(), freeMedico(), etc

Nao seguir essas regras pode levar a problemas como vazamento de memoria e erros inesperados

Se uma variavel de um struct for um ponteiro, ele sera alocado com malloc, voce absolutamente
nao deve mudar o ponteiro ou ira causar os mesmos problemas ja citados

exemplo:

medico *medico = novomedico(12, "josemir", "pediatria", 1232);
medico->nome = "joseir";


Parece que nao tem nada de errado, mas ao fazer isso voce esta mudando o ponteiro
de nome para o endereco da primeira letra da string, 'J', enquanto deixa um array alocado
com malloc em algum lugar da heap, gerando um vazamento de memoria
a forma correta seria

medico *medico = novomedico(12, "josemir", "pediatria", 1232);
strcpy(medico->nome ,"joseir");

Voce deve usar como se fosse um array, nao um ponteiro, porque na verdade e isso mesmo,
na implementacao todos esses ponteiros apontam pra arrays de tamanhos definidos em constantes
que serao informados com um comentario ao lado de cada um
*/

#include <sqlite3.h>
#define SMALL_BUFFER_SIZE 32
#define BUFFER_SIZE 64
#define BIG_BUFFER_SIZE 256
#define ERROR_CODE -1
#define OK_CODE 0

enum Genero { masculino, feminino };
typedef struct {
	int id;
	char *nome; // BUFFER_SIZE
	char *cpf; //SMALL_BUFFER_SIZE
	char *telefone; //SMALL_BUFFER_SIZE
	char *cep; //SMALL_BUFFER_SIZE
	char *alergias; // BIG_BUFFER_SIZE
	char *deficiencias; //BIG_BUFFER_SIZE
	int idade;
	enum Genero genero;
} Paciente;

typedef struct {
	int id;
	char *nome; // BUFFER_SIZE
	char *especialidade; //BUFFER_SIZE
	int cod;
} Medico;

enum Status { agendado, finalizado, cancelado };
typedef struct {
	int id;
	Paciente *paciente;
	Medico *medico;
	char *dataHora; //SMALL_BUFFER_SIZE
	enum Status status;
} Agendamento;

typedef struct PacienteNode {
	Paciente *paciente;
	struct PacienteNode *next;
} PacienteNode;

typedef struct MedicoNode {
	Medico *medico;
	struct MedicoNode *next;
} MedicoNode;

typedef struct AgendamentoNode {
	Agendamento *agendamento;
	struct AgendamentoNode *next;
} AgendamentoNode;

typedef struct {
	PacienteNode *head;
} PacienteLista;

typedef struct {
	MedicoNode *head;
} MedicoLista;

typedef struct {
	AgendamentoNode *head;
} AgendamentoLista;

PacienteLista *buscarPacienteLista();
MedicoLista *buscarMedicoLista();
AgendamentoLista *buscarAgendamentoLista();

void freePacienteLista(PacienteLista *);
void freeMedicoLista(MedicoLista *);
void freeAgendamentoLista(AgendamentoLista *);
/*
As funcoes desse grupo podem gerar memory leak pois retornam ponteiros de structs
alocados com malloc, nao esqueca de usar suas respectivas funcoes free quando nao
precisar mais da variavel as funcoes desse grupo retornam um ponteiro para o struct alocado
com malloc caso tudo ocorra bem ou NULL caso  algo de errado
*/

// As funcoes com prefixo novo recebem um id inteiro positivo como primeiro parametro
// se voce nao sabe o que passar como parametro, passe 0
// Caso nao saiba o que passar como parametro para um ponteiro, passe NULL
//
// Essa funcao ira retornar um struct preenchido com os dados passados
Paciente *novoPaciente(int id, char *nome, char *cpf, char *telefone, char *cep,
		       char *alergias, char *deficiencias, int idade,
		       enum Genero genero);

Medico *novoMedico(int id, char *nome, char *especialidade, int cod);

Agendamento *novoAgendamento(int id, Paciente *paciente, Medico *medico,
			     char *dataHora, enum Status status);

void freePaciente(Paciente *);
void freeMedico(Medico *);
void freeAgendamento(Agendamento *);

//Todas as funcoes abaixo retornam OK_CODE se tudo ocorrer bem e ERROR_CODE em caso de erro
int createdb();

/*
As funcoes desse grupo recebem um ponteiro para seu respectivo struct com todos os dados
exceto pelo id preenchidos, caso tudo ocorra bem o id sera setado ao longo da execucao
*/
int inserirPaciente(Paciente *);
int inserirMedico(Medico *);
int inserirAgendamento(Agendamento *);

int deletePaciente(int id);
int deleteMedico(int id);
int deleteAgendamento(int id);

/*
As funcoes desse grupo irao atualizar o banco de dados baseado no id do struct passado como parametro
*/
int atualizarPaciente(Paciente *);
int atualizarMedico(Medico *);
int atualizarAgendamento(Agendamento *);

/*
As funcoes desse grupo podem gerar memory leak pois retornam ponteiros de structs
alocados com malloc, nao esqueca de usar suas respectivas funcoes free quando nao
precisar mais da variavel as funcoes desse grupo retornam um ponteiro para o struct alocado
com malloc caso tudo ocorra bem ou NULL caso  algo de errado
*/
Paciente *buscarPaciente(int id);
Medico *buscarMedico(int id);
Agendamento *buscarAgendamento(int id);
