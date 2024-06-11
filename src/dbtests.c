#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"
int main(int argc, char *argv[])
{
	int rc;

	createdb();
	Medico *medico = (Medico *)malloc(sizeof(Medico));
	medico->nome = "Joseval";
	medico->especialidade = "Pediatria";
	medico->cod = 3321;
	inserirMedico(medico);

	printf("Id medico %d\n", medico->id);
	free(medico);

	Paciente *paciente = (Paciente *)malloc(sizeof(Paciente));
	paciente->nome = "Joseir";
	paciente->cpf = "123.456.789-10";
	paciente->telefone = "00 00000-0000";
	paciente->cep = "00000-000";
	paciente->alergias = "dipirona e opioides";
	paciente->deficiencias = "";
	paciente->idade = 67;
	paciente->genero = masculino;
	inserirPaciente(paciente);
	printf("Id paciente %d\n", paciente->id);

	free(paciente);

	Medico *m = buscarMedico(5);
	if (m != NULL) {
		printf("\nbuscarMedico\n");
		printf("%d\n", m->id);
		printf("%s\n", m->nome);
		printf("%s\n", m->especialidade);
		printf("%d\n", m->cod);
	}

	Paciente *p = buscarPaciente(5);
	if (p != NULL) {
		printf("\nbuscarPaciente\n");
		printf("%d\n", p->id);
		printf("%s\n", p->nome);
		printf("%s\n", p->cpf);
		printf("%s\n", p->telefone);
		printf("%s\n", p->cep);
		printf("%s\n", p->alergias);
		printf("%s\n", p->deficiencias);
		printf("%d\n", p->idade);
		printf("%d\n", p->genero);
	}

	Agendamento *agendamento = (Agendamento *)malloc(sizeof(Agendamento));
	agendamento->medico = buscarMedico(5);
	agendamento->paciente = buscarPaciente(5);
	agendamento->dataHora = "18/06/2024:15:00";
	if (agendamento->medico != NULL && agendamento->paciente != NULL) {
		rc = inserirAgendamento(agendamento);
		if (rc == OK_CODE) {
			printf("Agendamento feito\n");
		}
	}

	// Memory leak
	free(agendamento);
	return EXIT_SUCCESS;
}
