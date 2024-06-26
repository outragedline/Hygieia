#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define separator "\n================================================\n"

void clear()
{
#ifdef _WIN32
	system("cls");
#elif __linux__
	system("clear");
#endif
}

Paciente *cadastrarPaciente()
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
	return paciente;
}

void inserirPacienteInterface()
{
	Paciente *paciente = cadastrarPaciente();
	if (paciente != NULL) {
		if (inserirPaciente(paciente) == OK_CODE) {
			printf(separator);
			printf("Paciente cadastrado com sucesso!\n");
			printf("ID: %d\n", paciente->id);
			printf(separator);
			freePaciente(paciente);
			return;
		}
		freePaciente(paciente);
		printf(separator);
		printf("Erro ao cadastrar paciente.\n");
		printf(separator);
		return;
	}
	printf(separator);
	printf("Erro ao criar paciente.\n");
	printf(separator);
}

Medico *cadastrarMedico()
{
	clear();
	char nome[BUFFER_SIZE], especialidade[BUFFER_SIZE];
	int cod;

	Medico *medico;
	printf("Informe os dados do medico:\n");
	printf("Nome: ");
	fgets(nome, BUFFER_SIZE, stdin);
	printf("Especialidade: ");
	fgets(especialidade, BUFFER_SIZE, stdin);
	printf("Codigo: ");
	scanf("%d", &cod);
	getchar(); // Limpar o buffer do teclado

	nome[strcspn(nome, "\n")] = 0;
	especialidade[strcspn(especialidade, "\n")] = 0;
	medico = novoMedico(0, nome, especialidade, cod);
	return medico;
}
void inserirMedicoInterface()
{
	Medico *medico = cadastrarMedico();

	if (medico != NULL) {
		if (inserirMedico(medico) == OK_CODE) {
			printf(separator);
			printf("Medico cadastrado com sucesso!\n");
			printf("ID: %d\n", medico->id);
			printf(separator);

			freeMedico(medico);
			return;
		}
		freeMedico(medico);
		printf(separator);
		printf("Erro ao cadastrar medico.\n");
		printf(separator);
		return;
	}

	printf(separator);
	printf("Erro ao criar medico.\n");
	printf(separator);
}

void inserirAgendamentoInterface()
{
	clear();
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
		printf(separator);
		printf("Paciente nao encontrado.\n");
		printf(separator);
		return;
	}

	printf("Informe o ID do medico: ");
	scanf("%d", &id);
	getchar(); // Limpar o buffer do teclado
	medico = buscarMedico(id);
	if (medico == NULL) {
		printf(separator);
		printf("Medico nao encontrado.\n");
		printf(separator);
		return;
	}

	printf("Informe a data e hora da consulta no formato dd/mm/yyyy:HH:MM : ");
	fgets(dataHora, SMALL_BUFFER_SIZE, stdin);

	dataHora[strcspn(dataHora, "\n")] = 0;
	agendamento = novoAgendamento(0, paciente, medico, dataHora, agendado);
	if (agendamento != NULL) {
		if (inserirAgendamento(agendamento) == OK_CODE) {
			printf(separator);
			printf("Consulta agendada com sucesso!\n");
			printf("ID: %d\n", agendamento->id);
			printf(separator);
			freeAgendamento(agendamento);
			return;
		}
		freeAgendamento(agendamento);

		printf(separator);
		printf("Erro ao agendar consulta.\n");
		printf(separator);
		return;
	}
	printf(separator);
	printf("Erro ao criar consulta.\n");
	printf(separator);
}

void mostrarPaciente(Paciente *paciente)
{
	printf("ID: %d\n", paciente->id);
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
	clear();
	int id;
	Paciente *paciente;
	printf("Informe o ID do paciente: ");
	scanf("%d", &id);
	getchar(); // Limpar o buffer do teclado
	paciente = buscarPaciente(id);
	if (paciente != NULL) {
		printf(separator);
		printf("Paciente encontrado:\n");
		mostrarPaciente(paciente);
		printf(separator);
		freePaciente(paciente);

		return;
	}
	printf(separator);
	printf("Paciente nao encontrado.\n");
	printf(separator);
}

void mostrarMedico(Medico *medico)
{
	printf("ID: %d\n", medico->id);
	printf("Nome: %s\n", medico->nome);
	printf("Especialidade: %s\n", medico->especialidade);
	printf("Codigo: %d\n", medico->cod);
}

void buscarMedicoInterface()
{
	clear();
	int id;
	Medico *medico;
	printf("Informe o ID do medico: ");
	scanf("%d", &id);
	getchar(); // Limpar o buffer do teclado
	medico = buscarMedico(id);

	if (medico != NULL) {
		printf(separator);
		printf("Medico encontrado:\n");
		mostrarMedico(medico);
		printf(separator);

		freeMedico(medico);
		return;
	}
	printf(separator);
	printf("Medico nao encontrado");
	printf(separator);
}

void mostrarAgendamento(Agendamento *agendamento)
{
	printf("ID: %d\n", agendamento->id);
	printf("Data e hora: %s\n", agendamento->dataHora);
	printf("Status: %s\n",

	       (agendamento->status == agendado)   ? "Agendado" :
	       (agendamento->status == finalizado) ? "Finalizado" :
						     "Cancelado");
	printf(separator);
	printf("Paciente:\n");
	mostrarPaciente(agendamento->paciente);
	printf(separator);
	printf("Medico:\n");
	mostrarMedico(agendamento->medico);
}

void buscarAgendamentoInterface()
{
	clear();
	int id;
	Agendamento *agendamento;
	printf("Informe o ID da consulta: ");
	scanf("%d", &id);
	getchar(); // Limpar o buffer do teclado
	agendamento = buscarAgendamento(id);
	if (agendamento != NULL) {
		printf(separator);
		mostrarAgendamento(agendamento);
		printf(separator);
		freeAgendamento(agendamento);
		return;
	}
	printf("Consulta nao encontrada.\n");
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

int mostrarMedicoLista(MedicoLista *lista)
{
	MedicoNode *next, *current = lista->head;
	int c = 0;
	while (current != NULL) {
		c++;
		next = current->next;
		printf(separator);
		mostrarMedico(current->medico);
		current = next;
	}
	if (c > 0) {
		printf(separator);
	}
	return c;
}

void buscarMedicoListaInterface()
{
	clear();
	MedicoLista *lista = buscarMedicoLista();
	int c;
	if (lista != NULL) {
		c = mostrarMedicoLista(lista);
		printf("Total de %d medicos cadastrados\n", c);
		freeMedicoLista(lista);
	}
}
int mostrarAgendamentoLista(AgendamentoLista *lista)
{
	AgendamentoNode *next, *current = lista->head;
	int c = 0;
	while (current != NULL) {
		c++;
		next = current->next;
		printf(separator);
		mostrarAgendamento(current->agendamento);
		printf(separator);
		current = next;
	}
	if (c > 0) {
		printf(separator);
	}
	return c;
}

void buscarAgendamentoListaInterface()
{
	clear();
	AgendamentoLista *lista = buscarAgendamentoLista();
	int c;
	if (lista != NULL) {
		c = mostrarAgendamentoLista(lista);
		printf("Total de %d agendamentos cadastrados\n", c);
		freeAgendamentoLista(lista);
	}
}

void deletePacienteInterface()
{
	clear();
	int id, rc;
	printf("Informe o ID do paciente: ");
	scanf("%d", &id);
	getchar(); // Limpar o buffer do teclado
	rc = deletePaciente(id);
	if (rc == OK_CODE) {
		printf("Paciente deletado com sucesso!\n");
		return;
	}
	printf("O paciente nao pode ser deletado!\n");
}

void deleteMedicoInterface()
{
	clear();
	int id, rc;
	printf("Informe o ID do medico: ");
	scanf("%d", &id);
	getchar(); // Limpar o buffer do teclado
	rc = deleteMedico(id);
	if (rc == OK_CODE) {
		printf("Medico deletado com sucesso!\n");
		return;
	}
	printf("O medico nao pode ser deletado!\n");
}
void deleteAgendamentoInterface()
{
	clear();
	int id, rc;
	printf("Informe o ID do agendamento: ");
	scanf("%d", &id);
	getchar(); // Limpar o buffer do teclado
	rc = deleteAgendamento(id);
	if (rc == OK_CODE) {
		printf("Agendamento deletado com sucesso!\n");
		return;
	}
	printf("O agendamento nao pode ser deletado!\n");
}

void atualizarPacienteInterface()
{
	Paciente *paciente = cadastrarPaciente();
	printf("Id: ");
	scanf("%d", &paciente->id);
	if (paciente != NULL) {
		if (atualizarPaciente(paciente) == OK_CODE) {
			printf(separator);
			printf("Paciente atualizado com sucesso!\n");
			mostrarPaciente(paciente);
			printf(separator);
			freePaciente(paciente);
			return;
		}
		freePaciente(paciente);
		printf(separator);
		printf("Erro ao cadastrar paciente.\n");
		printf(separator);
		return;
	}
	printf(separator);
	printf("Erro ao criar paciente.\n");
	printf(separator);
}

void atualizarMedicoInterface()
{
	Medico *medico = cadastrarMedico();
	printf("Id: ");
	scanf("%d", &medico->id);
	if (medico != NULL) {
		if (atualizarMedico(medico) == OK_CODE) {
			printf(separator);
			printf("Medico atualizado com sucesso!\n");
			mostrarMedico(medico);
			printf(separator);
			freeMedico(medico);
			return;
		}
		freeMedico(medico);
		printf(separator);
		printf("Erro ao cadastrar medico.\n");
		printf(separator);
		return;
	}
	printf(separator);
	printf("Erro ao criar medico.\n");
	printf(separator);
}

void atualizarAgendamentoInterface()
{
	clear();
	int opt, id;
	Agendamento *agendamento;
	printf("Id: ");
	scanf("%d", &id);
	agendamento = buscarAgendamento(id);
	getchar(); // Limpar o buffer do teclado
	printf("1. Reagendar\n");
	printf("2. Finalizar\n");
	printf("3. Cancelar\n");
	printf("O que deseja fazer? ");
	scanf("%d", &opt);
	getchar(); // Limpar o buffer do teclado
	switch (opt) {
	case 1:
		printf("Informe a data e hora da consulta no formato dd/mm/yyyy:HH:MM : ");
		fgets(agendamento->dataHora, SMALL_BUFFER_SIZE, stdin);
		break;
	case 2:
		agendamento->status = finalizado;
		break;
	case 3:
		agendamento->status = cancelado;
		break;

	default:
		printf("Opcao invalida\n");
	}

	agendamento->dataHora[strcspn(agendamento->dataHora, "\n")] = 0;
	atualizarAgendamento(agendamento);
}
void menu()
{
	printf("\nMenu:\n");
	printf("1. Cadastrar Paciente\n");
	printf("2. Cadastrar Medico\n");
	printf("3. Agendar Consulta\n");
	printf("4. Buscar Paciente\n");
	printf("5. Buscar Medico\n");
	printf("6. Buscar Consulta\n");
	printf("7. Mostrar pacientes cadastrados\n");
	printf("8. Mostrar medicos cadastrados\n");
	printf("9. Mostrar consultas agendadas\n");
	printf("10. Deletar paciente cadastrado\n");
	printf("11. Deletar medico cadastrado\n");
	printf("12. Deletar consulta cadastrada\n");
	printf("13. Atualizar paciente cadastrado\n");
	printf("14. Atualizar medico cadastrado\n");
	printf("15. Atualizar agendamento cadastrado\n");
	printf("0. Sair\n");
	printf("Escolha uma opcao: ");
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
			inserirPacienteInterface();
			break;
		case 2:
			inserirMedicoInterface();
			break;
		case 3:
			inserirMedicoInterface();
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
			buscarMedicoListaInterface();
			break;
		case 9:
			buscarAgendamentoListaInterface();
			break;

		case 10:
			deletePacienteInterface();
			break;
		case 11:
			deleteMedicoInterface();
			break;

		case 12:
			deleteAgendamentoInterface();
			break;
		case 13:
			atualizarPacienteInterface();
			break;
		case 14:
			atualizarMedicoInterface();
			break;

		case 15:
			atualizarAgendamentoInterface();
			break;
		case 0:
			printf("Saindo...\n");
			break;
		default:
			clear();
			printf("Opcao invalida.\n");
		}
	} while (escolha != 0);

	return 0;
}
