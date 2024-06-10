#include "db.h"
#include <stdlib.h>
#include <stdio.h>
#define DB_STRING "./db.sqlite3"

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
		    "idade INTEGER,"
		    "alergias TEXT,"
		    "deficiencias TEXT,"
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
