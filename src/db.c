#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DB_STRING "./db.sqlite3"

Paciente *novoPaciente(int id, char *nome, char *cpf, char *telefone, char *cep,
		       char *alergias, char *deficiencias, int idade,
		       enum Genero genero)
{
	Paciente *paciente = (Paciente *)malloc(sizeof(Paciente));
	paciente->id = id;
	paciente->nome = (char *)malloc(sizeof(char) * BUFFER_SIZE);
	paciente->cpf = (char *)malloc(sizeof(char) * SMALL_BUFFER_SIZE);
	paciente->telefone = (char *)malloc(sizeof(char) * SMALL_BUFFER_SIZE);
	paciente->cep = (char *)malloc(sizeof(char) * SMALL_BUFFER_SIZE);
	paciente->alergias = (char *)malloc(sizeof(char) * BIG_BUFFER_SIZE);
	paciente->deficiencias = (char *)malloc(sizeof(char) * BIG_BUFFER_SIZE);
	paciente->idade = idade;
	paciente->genero = genero;

	if (nome != NULL)
		strcpy(paciente->nome, nome);
	if (cpf != NULL)
		strcpy(paciente->cpf, cpf);
	if (telefone != NULL)
		strcpy(paciente->telefone, telefone);
	if (cep != NULL)
		strcpy(paciente->cep, cep);
	if (alergias != NULL)
		strcpy(paciente->alergias, alergias);
	if (deficiencias != NULL)
		strcpy(paciente->deficiencias, deficiencias);

	return paciente;
}

void freePaciente(Paciente *paciente)
{
	free(paciente->nome);
	free(paciente->cpf);
	free(paciente->telefone);
	free(paciente->cep);
	free(paciente->alergias);
	free(paciente->deficiencias);
	free(paciente);
	return;
}

Medico *novoMedico(int id, char *nome, char *especialidade, int cod)
{
	Medico *medico = (Medico *)malloc(sizeof(Medico));
	medico->id = id;
	medico->nome = (char *)malloc(sizeof(char) * BUFFER_SIZE);
	medico->especialidade = malloc(sizeof(char) * BUFFER_SIZE);
	medico->cod = cod;

	if (nome != NULL)
		strcpy(medico->nome, nome);
	if (especialidade != NULL)
		strcpy(medico->especialidade, especialidade);

	return medico;
}

void freeMedico(Medico *medico)
{
	free(medico->nome);
	free(medico->especialidade);
	return;
}

Agendamento *novoAgendamento(int id, Paciente *paciente, Medico *medico,
			     char *dataHora, enum Status status)
{
	Agendamento *agendamento = (Agendamento *)malloc(sizeof(Agendamento));
	agendamento->id = id;
	agendamento->paciente = NULL;
	agendamento->medico = NULL;
	agendamento->dataHora =
		(char *)malloc(sizeof(char) * SMALL_BUFFER_SIZE);
	agendamento->status = status;

	if (paciente != NULL)
		agendamento->paciente = paciente;
	if (medico != NULL)
		agendamento->medico = medico;
	if (dataHora != NULL)
		strcpy(agendamento->dataHora, dataHora);

	return agendamento;
}

void freeAgendamento(Agendamento *agendamento)
{
	freeMedico(agendamento->medico);
	freePaciente(agendamento->paciente);
	free(agendamento->dataHora);
	free(agendamento);
	return;
}

int createdb()
{
	sqlite3 *db;
	int rc = sqlite3_open(DB_STRING, &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Erro ao criar banco: rc %d\n", rc);
		return ERROR_CODE;
	}

	char *err_msg = 0;
	char *sql =
		"CREATE TABLE IF NOT EXISTS Paciente("
		"id INTEGER PRIMARY KEY,"
		"nome TEXT,"
		"cpf TEXT,"
		"telefone TEXT,"
		"cep TEXT,"
		"alergias TEXT,"
		"deficiencias TEXT,"
		"idade INTEGER,"
		"genero INTEGER"
		");"

		"CREATE TABLE IF NOT EXISTS Medico("
		"id INTEGER PRIMARY KEY,"
		"nome TEXT,"
		"especialidade TEXT,"
		"cod INTEGER"
		");"

		"CREATE TABLE IF NOT EXISTS Agendamento("
		"id INTEGER PRIMARY KEY,"
		"paciente_id,"
		"medico_id,"
		"dataHora TEXT,"
		"status INTEGER,"
		"FOREIGN KEY (medico_id) REFERENCES Medico(id) ON DELETE CASCADE,"
		"FOREIGN KEY (paciente_id) REFERENCES Paciente(id) ON DELETE CASCADE"
		")";
	rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "Erro ao criar banco: %s\n", err_msg);
		sqlite3_free(err_msg);
		sqlite3_close(db);
		return ERROR_CODE;
	}

	sqlite3_free(err_msg);
	sqlite3_close(db);
	return OK_CODE;
}

Medico *buscarMedico(int id)
{
	Medico *medico = (Medico *)malloc(sizeof(Medico));
	sqlite3 *db;
	sqlite3_stmt *stmt;
	int rc = sqlite3_open(DB_STRING, &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Erro %d: %s\n", rc, sqlite3_errmsg(db));
		return NULL;
	}
	char *sql = "SELECT * FROM Medico WHERE id = :id;";
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	rc = sqlite3_bind_int(stmt, 1, id);

	// A busca e baseada no id, entao vou assumir que nao vai ter mais
	// de uma linha de resultado
	// se tiver vai ser bem estranho...
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_ROW) {
		return NULL;
	}

	medico->id = sqlite3_column_int(stmt, 0);
	medico->nome = (char *)malloc(sizeof(char) * BUFFER_SIZE);

	strcpy(medico->nome, (char *)sqlite3_column_text(stmt, 1));
	medico->especialidade = (char *)malloc(sizeof(char) * BUFFER_SIZE);

	strcpy(medico->especialidade, (char *)sqlite3_column_text(stmt, 2));
	medico->cod = sqlite3_column_int(stmt, 3);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return medico;
}

Paciente *buscarPaciente(int id)
{
	Paciente *paciente = (Paciente *)malloc(sizeof(Paciente));
	sqlite3 *db;
	sqlite3_stmt *stmt;
	int rc = sqlite3_open(DB_STRING, &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Erro %d: %s\n", rc, sqlite3_errmsg(db));
		return NULL;
	}

	char *sql = "SELECT * FROM Paciente WHERE id = :id;";
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	rc = sqlite3_bind_int(stmt, 1, id);

	// A busca e baseada no id, entao vou assumir que nao vai ter mais
	// de uma linha de resultado
	// se tiver vai ser bem estranho...
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_ROW) {
		return NULL;
	}

	paciente->id = sqlite3_column_int(stmt, 0);

	paciente->nome = (char *)malloc(sizeof(char) * BUFFER_SIZE);
	strcpy(paciente->nome, (char *)sqlite3_column_text(stmt, 1));

	paciente->cpf = (char *)malloc(sizeof(char) * SMALL_BUFFER_SIZE);
	strcpy(paciente->cpf, (char *)sqlite3_column_text(stmt, 2));

	paciente->telefone = (char *)malloc(sizeof(char) * SMALL_BUFFER_SIZE);
	strcpy(paciente->telefone, (char *)sqlite3_column_text(stmt, 3));

	paciente->cep = (char *)malloc(sizeof(char) * SMALL_BUFFER_SIZE);
	strcpy(paciente->cep, (char *)sqlite3_column_text(stmt, 4));

	paciente->alergias = (char *)malloc(sizeof(char) * BIG_BUFFER_SIZE);
	strcpy(paciente->alergias, (char *)sqlite3_column_text(stmt, 5));

	paciente->deficiencias = (char *)malloc(sizeof(char) * BIG_BUFFER_SIZE);
	strcpy(paciente->deficiencias, (char *)sqlite3_column_text(stmt, 6));

	paciente->idade = sqlite3_column_int(stmt, 7);
	paciente->genero = sqlite3_column_int(stmt, 8);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return paciente;
}

Agendamento *buscarAgendamento(int id)
{
	Agendamento *agendamento = (Agendamento *)malloc(sizeof(Agendamento));
	sqlite3 *db;
	sqlite3_stmt *stmt;
	int rc = sqlite3_open(DB_STRING, &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Erro %d: %s\n", rc, sqlite3_errmsg(db));
		return NULL;
	}

	char *sql = "SELECT * FROM Agendamento WHERE id = :id;";
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	rc = sqlite3_bind_int(stmt, 1, id);

	// A busca e baseada no id, entao vou assumir que nao vai ter mais
	// de uma linha de resultado
	// se tiver vai ser bem estranho...
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_ROW) {
		return NULL;
	}

	agendamento->id = sqlite3_column_int(stmt, 0);
	agendamento->medico = buscarMedico(sqlite3_column_int(stmt, 1));
	agendamento->paciente = buscarPaciente(sqlite3_column_int(stmt, 2));
	if (agendamento->medico == NULL || agendamento->paciente == NULL) {
		return NULL;
	}

	agendamento->dataHora =
		(char *)malloc(sizeof(char) * SMALL_BUFFER_SIZE);
	strcpy(agendamento->dataHora, (char *)sqlite3_column_text(stmt, 3));

	agendamento->status = sqlite3_column_int(stmt, 4);

	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return agendamento;
}

PacienteNode *novoPacienteNode(Paciente *paciente)
{
	PacienteNode *node = (PacienteNode *)malloc(sizeof(PacienteNode));
	node->paciente = NULL;
	node->next = NULL;
	if (paciente != NULL) {
		node->paciente = paciente;
	}

	return node;
}

void pacienteListaAppend(PacienteLista *lista, PacienteNode *node)
{
	if (lista->head == NULL) {
		lista->head = node;
		return;
	}

	PacienteNode *lastNode = lista->head;
	while (lastNode->next != NULL) {
		lastNode = lastNode->next;
	}
	lastNode->next = node;
}
PacienteLista *novoPacienteLista()
{
	PacienteLista *lista = (PacienteLista *)malloc(sizeof(PacienteLista));
	lista->head = NULL;
	return lista;
}

int buscarPacienteListaCallback(void *lista, int argc, char **argv,
				char **azColName)
{
	PacienteLista *pacienteLista = (PacienteLista *)lista;
	PacienteNode *newNode = novoPacienteNode(NULL);
	newNode->paciente = novoPaciente(strtol(argv[0], NULL, 10), argv[1],
					 argv[2], argv[3], argv[4], argv[5],
					 argv[6], strtol(argv[7], NULL, 10),
					 strtol(argv[8], NULL, 10));
	pacienteListaAppend(pacienteLista, newNode);
	return 0;
}
PacienteLista *buscarPacienteLista()
{
	PacienteLista *pacienteLista = novoPacienteLista();

	sqlite3 *db;
	char *err_msg;
	int rc = sqlite3_open(DB_STRING, &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Erro %d: %s\n", rc, sqlite3_errmsg(db));
		return NULL;
	}
	char *sql = "SELECT * FROM Paciente;";
	sqlite3_exec(db, sql, buscarPacienteListaCallback, pacienteLista,
		     &err_msg);
	sqlite3_close(db);
	sqlite3_free(err_msg);
	return pacienteLista;
}

void freePacienteLista(PacienteLista *lista)
{
	PacienteNode *next, *current = lista->head;
	while (current != NULL) {
		next = current->next;
		freePaciente(current->paciente);
		free(current);
		current = next;
	}
	free(lista);
}

MedicoNode *novoMedicoNode(Medico *medico)
{
	MedicoNode *node = (MedicoNode *)malloc(sizeof(MedicoNode));
	node->medico = NULL;
	node->next = NULL;
	if (medico != NULL) {
		node->medico = medico;
	}

	return node;
}

void medicoListaAppend(MedicoLista *lista, MedicoNode *node)
{
	if (lista->head == NULL) {
		lista->head = node;
		return;
	}

	MedicoNode *lastNode = lista->head;
	while (lastNode->next != NULL) {
		lastNode = lastNode->next;
	}
	lastNode->next = node;
}
MedicoLista *novoMedicoLista()
{
	MedicoLista *lista = (MedicoLista *)malloc(sizeof(MedicoLista));
	lista->head = NULL;
	return lista;
}

int buscarMedicoListaCallback(void *lista, int argc, char **argv,
			      char **azColName)
{
	MedicoLista *medicoLista = (MedicoLista *)lista;
	MedicoNode *newNode = novoMedicoNode(NULL);
	newNode->medico = novoMedico(strtol(argv[0], NULL, 10), argv[1],
				     argv[2], strtol(argv[3], NULL, 10));
	medicoListaAppend(medicoLista, newNode);
	return 0;
}
MedicoLista *buscarMedicoLista()
{
	MedicoLista *medicoLista = novoMedicoLista();

	sqlite3 *db;
	char *err_msg;
	int rc = sqlite3_open(DB_STRING, &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Erro %d: %s\n", rc, sqlite3_errmsg(db));
		return NULL;
	}
	char *sql = "SELECT * FROM Medico;";
	sqlite3_exec(db, sql, buscarMedicoListaCallback, medicoLista, &err_msg);
	sqlite3_close(db);
	sqlite3_free(err_msg);
	return medicoLista;
}

void freeMedicoLista(MedicoLista *lista)
{
	MedicoNode *next, *current = lista->head;
	while (current != NULL) {
		next = current->next;
		freeMedico(current->medico);
		free(current);
		current = next;
	}
	free(lista);
}

AgendamentoNode *novoAgendamentoNode(Agendamento *agendamento)
{
	AgendamentoNode *node =
		(AgendamentoNode *)malloc(sizeof(AgendamentoNode));
	node->agendamento = NULL;
	node->next = NULL;
	if (agendamento != NULL) {
		node->agendamento = agendamento;
	}

	return node;
}

void agendamentoListaAppend(AgendamentoLista *lista, AgendamentoNode *node)
{
	if (lista->head == NULL) {
		lista->head = node;
		return;
	}

	AgendamentoNode *lastNode = lista->head;
	while (lastNode->next != NULL) {
		lastNode = lastNode->next;
	}
	lastNode->next = node;
}
AgendamentoLista *novoAgendamentoLista()
{
	AgendamentoLista *lista =
		(AgendamentoLista *)malloc(sizeof(AgendamentoLista));
	lista->head = NULL;
	return lista;
}

int buscarAgendamentoListaCallback(void *lista, int argc, char **argv,
				   char **azColName)
{
	AgendamentoLista *agendamentoLista = (AgendamentoLista *)lista;
	AgendamentoNode *newNode = novoAgendamentoNode(NULL);
	newNode->agendamento =
		novoAgendamento(strtol(argv[0], NULL, 10),
				buscarPaciente(strtol(argv[1], NULL, 10)),
				buscarMedico(strtol(argv[2], NULL, 10)),
				argv[3], strtol(argv[4], NULL, 10));
	agendamentoListaAppend(agendamentoLista, newNode);
	return 0;
}
AgendamentoLista *buscarAgendamentoLista()
{
	AgendamentoLista *agendamentoLista = novoAgendamentoLista();

	sqlite3 *db;
	char *err_msg;
	int rc = sqlite3_open(DB_STRING, &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Erro %d: %s\n", rc, sqlite3_errmsg(db));
		return NULL;
	}
	char *sql = "SELECT * FROM Agendamento;";
	sqlite3_exec(db, sql, buscarAgendamentoListaCallback, agendamentoLista,
		     &err_msg);
	sqlite3_close(db);
	sqlite3_free(err_msg);
	return agendamentoLista;
}

void freeAgendamentoLista(AgendamentoLista *lista)
{
	AgendamentoNode *next, *current = lista->head;
	while (current != NULL) {
		next = current->next;
		freeAgendamento(current->agendamento);
		free(current);
		current = next;
	}
	free(lista);
}

int inserirPaciente(Paciente *paciente)
{
	sqlite3 *db;
	sqlite3_stmt *stmt;
	int rc = sqlite3_open(DB_STRING, &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Erro %d: %s\n", rc, sqlite3_errmsg(db));
		return ERROR_CODE;
	}

	char *sql =
		"INSERT INTO "
		"Paciente(nome,cpf,telefone,cep,alergias,deficiencias,genero,idade) "
		"VALUES(:nome,:cpf,:telefone,:cep,:alergias,:deficiencias,:genero,:"
		"idade)";
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	rc = sqlite3_bind_text(stmt, 1, paciente->nome, -1, SQLITE_TRANSIENT);
	rc = sqlite3_bind_text(stmt, 2, paciente->cpf, -1, SQLITE_TRANSIENT);
	rc = sqlite3_bind_text(stmt, 3, paciente->telefone, -1,
			       SQLITE_TRANSIENT);
	rc = sqlite3_bind_text(stmt, 4, paciente->cep, -1, SQLITE_TRANSIENT);
	rc = sqlite3_bind_text(stmt, 5, paciente->alergias, -1,
			       SQLITE_TRANSIENT);
	rc = sqlite3_bind_text(stmt, 6, paciente->deficiencias, -1,
			       SQLITE_TRANSIENT);
	rc = sqlite3_bind_int(stmt, 7, paciente->genero);
	rc = sqlite3_bind_int(stmt, 8, paciente->idade);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_OK && rc != SQLITE_DONE) {
		fprintf(stderr, "Erro %d: %s\n", rc, sqlite3_errmsg(db));
		sqlite3_close(db);
		return ERROR_CODE;
	}

	rc = sqlite3_finalize(stmt);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Erro ao inserir paciente, rc: %d\n", rc);
		sqlite3_close(db);
		return ERROR_CODE;
	}

	paciente->id = sqlite3_last_insert_rowid(db);
	sqlite3_close(db);
	return OK_CODE;
}

int deleteAgendamento(int id)
{
	sqlite3 *db;
	sqlite3_stmt *stmt;
	int rc = sqlite3_open(DB_STRING, &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Erro %d: %s\n", rc, sqlite3_errmsg(db));
		return ERROR_CODE;
	}

	char *sql = "DELETE FROM Agendamento WHERE id = :id;";
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	rc = sqlite3_bind_int(stmt, 1, id);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_OK && rc != SQLITE_DONE) {
		fprintf(stderr, "Erro %d: %s\n", rc, sqlite3_errmsg(db));
		sqlite3_close(db);
		return ERROR_CODE;
	}

	rc = sqlite3_finalize(stmt);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Erro ao deletar agendamento, rc: %d\n", rc);
		sqlite3_close(db);
		return ERROR_CODE;
	}

	sqlite3_close(db);
	return OK_CODE;
}

int inserirMedico(Medico *medico)
{
	sqlite3 *db;
	sqlite3_stmt *stmt;
	int rc = sqlite3_open(DB_STRING, &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Erro %d: %s\n", rc, sqlite3_errmsg(db));
		return ERROR_CODE;
	}

	char *sql = "INSERT INTO Medico(nome,especialidade,cod) "
		    "VALUES(:nome,:especialidade,:cod)";
	rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	rc = sqlite3_bind_text(stmt, 1, medico->nome, -1, SQLITE_TRANSIENT);
	rc = sqlite3_bind_text(stmt, 2, medico->especialidade, -1,
			       SQLITE_TRANSIENT);
	rc = sqlite3_bind_int(stmt, 3, medico->cod);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_OK && rc != SQLITE_DONE) {
		fprintf(stderr, "Erro %d: %s\n", rc, sqlite3_errmsg(db));
		sqlite3_close(db);
		return ERROR_CODE;
	}

	rc = sqlite3_finalize(stmt);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Erro %d: %s\n", rc, sqlite3_errmsg(db));
		sqlite3_close(db);
		return ERROR_CODE;
	}

	medico->id = sqlite3_last_insert_rowid(db);
	sqlite3_close(db);
	return OK_CODE;
}

int deleteMedico(int id)
{
	sqlite3 *db;
	sqlite3_stmt *stmt;
	int rc = sqlite3_open(DB_STRING, &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Erro %d: %s\n", rc, sqlite3_errmsg(db));
		return ERROR_CODE;
	}

	char *sql = "DELETE FROM Medico WHERE id = :id;";
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	rc = sqlite3_bind_int(stmt, 1, id);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_OK && rc != SQLITE_DONE) {
		fprintf(stderr, "Erro %d: %s\n", rc, sqlite3_errmsg(db));
		sqlite3_close(db);
		return ERROR_CODE;
	}

	rc = sqlite3_finalize(stmt);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Erro ao deletar medico, rc: %d\n", rc);
		sqlite3_close(db);
		return ERROR_CODE;
	}

	sqlite3_close(db);
	return OK_CODE;
}

int inserirAgendamento(Agendamento *agendamento)
{
	sqlite3 *db;
	sqlite3_stmt *stmt;
	int rc = sqlite3_open(DB_STRING, &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Erro %d: %s\n", rc, sqlite3_errmsg(db));
		return ERROR_CODE;
	}
	char *sql =
		"INSERT INTO Agendamento(medico_id,paciente_id,dataHora,status) "
		"VALUES(:medico_id,:paciente_id,:dataHora,:status)";
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	rc = sqlite3_bind_int(stmt, 1, agendamento->medico->id);
	rc = sqlite3_bind_int(stmt, 2, agendamento->paciente->id);
	rc = sqlite3_bind_text(stmt, 3, agendamento->dataHora,
			       sizeof(char) * SMALL_BUFFER_SIZE,
			       SQLITE_TRANSIENT);
	rc = sqlite3_bind_int(stmt, 4, agendamento->status);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_OK && rc != SQLITE_DONE) {
		fprintf(stderr, "Erro %d: %s\n", rc, sqlite3_errmsg(db));
		sqlite3_close(db);
		return ERROR_CODE;
	}

	rc = sqlite3_finalize(stmt);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Erro ao inserir paciente, rc: %d\n", rc);
		sqlite3_close(db);
		return ERROR_CODE;
	}
	agendamento->id = sqlite3_last_insert_rowid(db);
	sqlite3_close(db);
	return OK_CODE;
}

int deletePaciente(int id)
{
	sqlite3 *db;
	sqlite3_stmt *stmt;
	int rc = sqlite3_open(DB_STRING, &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Erro %d: %s\n", rc, sqlite3_errmsg(db));
		return ERROR_CODE;
	}

	char *sql = "DELETE FROM Paciente WHERE id = :id;";
	sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
	rc = sqlite3_bind_int(stmt, 1, id);

	rc = sqlite3_step(stmt);
	if (rc != SQLITE_OK && rc != SQLITE_DONE) {
		fprintf(stderr, "Erro %d: %s\n", rc, sqlite3_errmsg(db));
		sqlite3_close(db);
		return ERROR_CODE;
	}

	rc = sqlite3_finalize(stmt);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Erro ao deletar paciente, rc: %d\n", rc);
		sqlite3_close(db);
		return ERROR_CODE;
	}

	sqlite3_close(db);
	return OK_CODE;
}
