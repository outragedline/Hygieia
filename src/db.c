#include "db.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define DB_STRING "./db.sqlite3"

Paciente *novoPaciente(unsigned int id, char *nome, char *cpf, char *telefone,
		       char *cep, char *alergias, char *deficiencias,
		       unsigned int idade, enum Genero genero)
{
	Paciente *paciente = (Paciente *)malloc(sizeof(Paciente));
	paciente->id = id;
	paciente->nome = (char *)malloc(sizeof(char) * BUFFER_SIZE);
	paciente->cpf = (char *)malloc(sizeof(char) * SMALL_BUFFER_SIZE);
	paciente->telefone = (char *)malloc(sizeof(char) * SMALL_BUFFER_SIZE);
	paciente->cep = (char *)malloc(sizeof(char) * SMALL_BUFFER_SIZE);
	paciente->alergias = (char *)malloc(sizeof(char) * BIG_BUFFER_SIZE);
	paciente->deficiencias = (char *)malloc(sizeof(char) * BIG_BUFFER_SIZE);
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

Medico *novoMedico(unsigned int id, char *nome, char *especialidade,
		   unsigned int cod)
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

Agendamento *novoAgendamento(unsigned int id, Paciente *paciente,
			     Medico *medico, char *dataHora, enum Status status)
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
	char *sql = "CREATE TABLE IF NOT EXISTS Paciente("
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
		    "medico_id,"
		    "paciente_id,"
		    "dataHora TEXT,"
		    "status INTEGER,"
		    "FOREIGN KEY (medico_id) REFERENCES Medico(id),"
		    "FOREIGN KEY (paciente_id) REFERENCES Paciente(id)"
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

Medico *buscarMedico(unsigned int id)
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

Paciente *buscarPaciente(unsigned int id)
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

Agendamento *buscarAgendamento(unsigned int id)
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

int buscarPacientesListaCallback(void *head, int argc, char **argv,
				 char **azColName)
{
	PacientesLista *current = (PacientesLista *)head;
	while (current != NULL) {
		current = current->next;

	}
	current = (PacientesLista *)malloc(sizeof(PacientesLista));
	current->paciente = novoPaciente(strtoul(argv[0], NULL, 10), argv[1],
					 argv[2], argv[3], argv[4], argv[5],
					 argv[6], strtoul(argv[7], NULL, 10),
					 strtoul(argv[8], NULL, 10));
	current->next = NULL;
	return 0;
}
PacientesLista *buscarPacientesLista()
{
	PacientesLista *head = (PacientesLista *)malloc(sizeof(PacientesLista));
	head->paciente = NULL;
	head->next = NULL;
	sqlite3 *db;
	char *err_msg;
	int rc = sqlite3_open(DB_STRING, &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Erro %d: %s\n", rc, sqlite3_errmsg(db));
		return NULL;
	}
	char *sql = "SELECT * FROM Paciente;";
	sqlite3_exec(db, sql, buscarPacientesListaCallback, head, &err_msg);
	sqlite3_close(db);
	sqlite3_free(err_msg);
	return head;
}

void freePacientesLista(PacientesLista *head)
{
	PacientesLista *current = head;
	while (current != NULL) {
		PacientesLista *temp = current;
		current = current->next;
		freePaciente(temp->paciente);
		free(temp);
	}
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

	char *sql =
		"INSERT INTO Medico(nome,especialidade,cod) VALUES(:nome,:especialidade,:cod)";
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
		"INSERT INTO Paciente(nome,cpf,telefone,cep,alergias,deficiencias,genero,idade) "
		"VALUES(:nome,:cpf,:telefone,:cep,:alergias,:deficiencias,:genero,:idade)";
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
	rc = sqlite3_bind_int(stmt, 7, paciente->idade);
	rc = sqlite3_bind_int(stmt, 8, paciente->genero);

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
