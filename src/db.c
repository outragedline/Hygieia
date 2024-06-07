#include "db.h"
#include <stdlib.h>
#include <stdio.h>
#define DB_STRING "./db.sqlite3"
sqlite3 *db;

int createdb()
{
	sqlite3_stmt *res;
	int rc = sqlite3_open(DB_STRING, &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Erro ao criar banco: rc %d\n", rc);
		return 1;
	}

	char *err_msg = 0;
	char *sql = "CREATE TABLE IF NOT EXISTS Paciente("
		    "id INTEGER PRIMARY KEY,"
		    "nome TEXT,"
		    "cpf TEXT,"
		    "rg TEXT,"
		    "telefone TEXT,"
		    "rua TEXT,"
		    "bairro TEXT,"
		    "numero TEXT,"
		    "cep TEXT,"
		    "idade INTEGER"
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
		return 1;
	}
	sqlite3_close(db);
	return 0;
}
