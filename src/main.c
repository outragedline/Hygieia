#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define separator "\n================================================\n"

void clear()
{
	system("clear");
}

void cadastrarPaciente()
{
	clear();
	int idade;
	char nome[BUFFER_SIZE], cpf[SMALL_BUFFER_SIZE],
		telefone[SMALL_BUFFER_SIZE];
	char cep[SMALL_BUFFER_SIZE], alergias[BIG_BUFFER_SIZE],
		deficiencias[BIG_BUFFER_SIZE];
	enum Genero genero;

	Paciente *paciente;

	printf("Informe os dados do paciente:\n");
	printf("Nome: ");
	fgets(nome, BUFFER_SIZE, stdin);
	printf("CPF: ");
	fgets(cpf, SMALL_BUFFER_SIZE, stdin);
	printf("Telefone: ");
	fgets(telefone, SMALL_BUFFER_SIZE, stdin);
	printf("CEP: ");
	fgets(cep, SMALL_BUFFER_SIZE, stdin);
	printf("Alergias: ");
	fgets(alergias, BIG_BUFFER_SIZE, stdin);
	printf("Deficiências: ");
	fgets(deficiencias, BIG_BUFFER_SIZE, stdin);
	printf("Idade: ");
	scanf("%d", &idade);
	printf("Gênero (0 para masculino, 1 para feminino): ");
	scanf("%d", &genero);

	nome[strcspn(nome, "\n")] = 0;
	cpf[strcspn(cpf, "\n")] = 0;
	telefone[strcspn(telefone, "\n")] = 0;
	cep[strcspn(cep, "\n")] = 0;
	alergias[strcspn(alergias, "\n")] = 0;
	deficiencias[strcspn(deficiencias, "\n")] = 0;

	paciente = novoPaciente(0, nome, cpf, telefone, cep, alergias,
				deficiencias, idade, genero);
	if (paciente != NULL) {
		if (inserirPaciente(paciente) == OK_CODE) {
			printf("Paciente cadastrado com sucesso!\n");
			printf("ID: %d\n", paciente->id);
			freePaciente(paciente);
		} else {
			freePaciente(paciente);
			printf("Erro ao cadastrar paciente.\n");
		}
	} else {
		printf("Erro ao criar paciente.\n");
	}
}

void cadastrarMedico()
{
	char nome[BUFFER_SIZE], especialidade[BUFFER_SIZE];
	int cod;

	Medico *medico;
	printf("Informe os dados do médico:\n");
	printf("Nome: ");
	fgets(nome, BUFFER_SIZE, stdin);
	printf("Especialidade: ");
	fgets(especialidade, BUFFER_SIZE, stdin);
	printf("Código: ");
	scanf("%d", &cod);
	getchar(); // Limpar o buffer do teclado

	nome[strcspn(nome, "\n")] = 0;
	especialidade[strcspn(especialidade, "\n")] = 0;
	medico = novoMedico(0, nome, especialidade, cod);
	if (medico != NULL) {
		if (inserirMedico(medico) == OK_CODE) {
			printf("Médico cadastrado com sucesso!\n");
			freeMedico(medico);
		} else {
			freeMedico(medico);
			printf("Erro ao cadastrar médico.\n");
		}
	} else {
		printf("Erro ao criar médico.\n");
	}
}

void cadastrarAgendamento()
{
	int id;
	Paciente *paciente;
	Medico *medico;
	Agendamento *agendamento;
	char dataHora[SMALL_BUFFER_SIZE];
	printf("Informe o ID do paciente: ");
	scanf("%d", &id);
	getchar(); // Limpar o buffer do teclado
	paciente = buscarPaciente(id);
	if (paciente == NULL) {
		printf("Paciente não encontrado.\n");
		return;
	}

	printf("Informe o ID do médico: ");
	scanf("%d", &id);
	getchar(); // Limpar o buffer do teclado
	medico = buscarMedico(id);
	if (medico == NULL) {
		printf("Médico não encontrado.\n");
		return;
	}

	printf("Informe a data e hora da consulta no formato dd/mm/yyyy:HH:MM : ");
	fgets(dataHora, SMALL_BUFFER_SIZE, stdin);
	getchar(); // Limpar o buffer do teclado

	dataHora[strcspn(dataHora, "\n")] = 0;
	agendamento = novoAgendamento(0, paciente, medico, dataHora, agendado);
	if (agendamento != NULL) {
		if (inserirAgendamento(agendamento) == OK_CODE) {
			freeAgendamento(agendamento);
			printf("Consulta agendada com sucesso!\n");
		} else {
			freeAgendamento(agendamento);
			printf("Erro ao agendar consulta.\n");
		}
	} else {
		printf("Erro ao criar consulta.\n");
	}
}

void mostrarPaciente(Paciente *paciente)
{
	printf("Nome: %s\n", paciente->nome);
	printf("CPF: %s\n", paciente->cpf);
	printf("Telefone: %s\n", paciente->telefone);
	printf("CEP: %s\n", paciente->cep);
	printf("Alergias: %s\n", paciente->alergias);
	printf("Deficiências: %s\n", paciente->deficiencias);
	printf("Idade: %d\n", paciente->idade);
	printf("Gênero: %s\n",
	       (paciente->genero == masculino) ? "Masculino" : "Feminino");
}

void buscarPacienteInterface()
{
	int id;
	Paciente *paciente;
	printf("Informe o ID do paciente: ");
	scanf("%d", &id);
	getchar(); // Limpar o buffer do teclado
	paciente = buscarPaciente(id);
	if (paciente != NULL) {
		printf("Paciente encontrado:\n");
		mostrarPaciente(paciente);
		freePaciente(paciente);
		return;
	}
	printf("Paciente não encontrado.\n");
}

void mostrarMedico(Medico *medico)
{
	printf("Nome: %s\n", medico->nome);
	printf("Especialidade: %s\n", medico->especialidade);
	printf("Código: %d\n", medico->cod);
}

void buscarMedicoInterface()
{
	int id;
	Medico *medico;
	printf("Informe o ID do médico: ");
	scanf("%d", &id);
	getchar(); // Limpar o buffer do teclado
	medico = buscarMedico(id);

	if (medico != NULL) {
		printf("Médico encontrado:\n");
		mostrarMedico(medico);
		freeMedico(medico);
		return;
	}
	printf("Medico nao encontrado");
}

void mostrarAgendamento(Agendamento *agendamento)
{
	printf("ID: %d", agendamento->id);
	printf("Data e hora: %s", agendamento->dataHora);
	printf(separator);
	printf("Paciente:\n");
	mostrarPaciente(agendamento->paciente);
	printf(separator);
	printf("Medico:\n");
	mostrarMedico(agendamento->medico);
}

void buscarAgendamentoInterface()
{
	int id;
	Agendamento *agendamento;
	printf("Informe o ID da consulta: ");
	scanf("%d", &id);
	getchar(); // Limpar o buffer do teclado
	agendamento = buscarAgendamento(id);
	if (agendamento != NULL) {
		mostrarAgendamento(agendamento);
		freeAgendamento(agendamento);
		return;
	}
	printf("Consulta não encontrada.\n");
}

int mostrarPacienteLista(PacienteLista *lista)
{
	PacienteNode *next, *current = lista->head;
	int c = 0;
	while (current != NULL) {
		c++;
		next = current->next;
		printf(separator);
		mostrarPaciente(current->paciente);
		current = next;
	}
	if (c > 0) {
		printf(separator);
	}
	return c;
}

void buscarPacienteListaInterface()
{
	clear();
	PacienteLista *lista = buscarPacienteLista();
	int c;
	if (lista != NULL) {
		c = mostrarPacienteLista(lista);
		printf("Total de %d pacientes cadastrados\n", c);
		freePacienteLista(lista);
	}
}

void menu()
{
	printf("\nMenu:\n");
	printf("1. Cadastrar Paciente\n");
	printf("2. Cadastrar Médico\n");
	printf("3. Agendar Consulta\n");
	printf("4. Buscar Paciente\n");
	printf("5. Buscar Médico\n");
	printf("6. Buscar Consulta\n");
	printf("7. Mostrar pacientes cadastrados\n");
	printf("8. Mostrar medicos cadastrados\n");
	printf("9. Mostrar consultas agendadas\n");
	printf("0. Sair\n");
	printf("Escolha uma opção: ");
}

int main()
{
	clear();
	int escolha;
	// Inicializando o banco de dados
	if (createdb() != OK_CODE) {
		printf("Erro ao criar o banco de dados.\n");
		return ERROR_CODE;
	}

	do {
		menu();
		scanf("%d", &escolha);
		getchar(); // Limpar o buffer do teclado

		switch (escolha) {
		case 1:
			cadastrarPaciente();
			break;
		case 2:
			cadastrarMedico();
			break;
		case 3:
			cadastrarAgendamento();
			break;
		case 4:
			buscarPacienteInterface();
			break;
		case 5:
			buscarMedicoInterface();
			break;
		case 6:
			buscarAgendamentoInterface();
			break;

		case 7:
			buscarPacienteListaInterface();
			break;
		case 8:
			break;
		case 9:
			break;
		case 0:
			printf("Saindo...\n");
			break;
		default:
			printf("Opção inválida.\n");
		}
	} while (escolha != 0);

	return 0;
}
