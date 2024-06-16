#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"
int main(int argc, char *argv[])
{
	int rc;

	createdb();

	Paciente *paciente = novoPaciente(
		0, "Joseir", "123.456.789-10", "00 00000-0000", "00000-000",
		"dipirona e opioides",
		"Paralisia parcial do lado esquerdo devido AVC", 72, masculino);
	inserirPaciente(paciente);
	printf("Id paciente %d\n", paciente->id);

	freePaciente(paciente);

	Medico *medico = novoMedico(0, "joseval", "Pediatria", 3321);
	inserirMedico(medico);

	printf("Id medico %d\n", medico->id);
	freeMedico(medico);

	Agendamento *agendamento =
		novoAgendamento(0, buscarPaciente(1), buscarMedico(1),
				"19/06/2024:15:00", agendado);

	if (agendamento->medico != NULL && agendamento->paciente != NULL) {
		rc = inserirAgendamento(agendamento);
		if (rc == OK_CODE) {
			printf("Agendamento feito\n");
		}
	}
	Medico *m = buscarMedico(1);
	if (m != NULL) {
		printf("\nbuscarMedico\n");
		printf("%d\n", m->id);
		printf("%s\n", m->nome);
		printf("%s\n", m->especialidade);
		printf("%d\n", m->cod);
	}

	Paciente *p = buscarPaciente(1);
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

	Agendamento *a = buscarAgendamento(1);
	if (a != NULL) {
		printf("Agendamento\n");
		printf("ID %d\n", a->id);
		printf("Medico id %d\n", a->medico->id);
		printf("Paciente id %d\n", a->paciente->id);
		printf("status %d\n", a->status);
	}

	PacienteLista *pacienteLista = buscarPacienteLista();
	freePacienteLista(pacienteLista);

	MedicoLista *medicoLista = buscarMedicoLista();
	freeMedicoLista(medicoLista);

	AgendamentoLista *agendamentoLista = buscarAgendamentoLista();
	freeAgendamentoLista(agendamentoLista);
	return EXIT_SUCCESS;
}
