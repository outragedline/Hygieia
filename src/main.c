#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void menu()
{
	printf("\nMenu:\n");
	printf("1. Cadastrar Paciente\n");
	printf("2. Cadastrar Médico\n");
	printf("3. Agendar Consulta\n");
	printf("4. Buscar Paciente\n");
	printf("5. Buscar Médico\n");
	printf("6. Buscar Consulta\n");
	printf("0. Sair\n");
	printf("Escolha uma opção: ");
}

int main()
{
	int escolha;
	int id;
	char nome[BUFFER_SIZE], cpf[SMALL_BUFFER_SIZE],
		telefone[SMALL_BUFFER_SIZE];
	char cep[SMALL_BUFFER_SIZE], alergias[BIG_BUFFER_SIZE],
		deficiencias[BIG_BUFFER_SIZE];
	char especialidade[BUFFER_SIZE], dataHora[SMALL_BUFFER_SIZE];
	int idade, cod;
	enum Genero genero;
	enum Status status;

	Paciente *paciente;
	Medico *medico;
	Agendamento *agendamento;

	// Inicializando o banco de dados
	if (createdb() != OK_CODE) {
		printf("Erro ao criar o banco de dados.\n");
		return 1;
	}

	do {
		menu();
		scanf("%d", &escolha);
		getchar(); // Limpar o buffer do teclado

		switch (escolha) {
		case 1:
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

			paciente = novoPaciente(0, nome, cpf, telefone, cep,
						alergias, deficiencias, idade,
						genero);
			if (paciente != NULL) {
				if (inserirPaciente(paciente) == OK_CODE) {
					printf("Paciente cadastrado com sucesso!\n");
					printf("ID: %u\n", paciente->id);
					freePaciente(paciente);
				} else {
					freePaciente(paciente);
					printf("Erro ao cadastrar paciente.\n");
				}
			} else {
				printf("Erro ao criar paciente.\n");
			}
			break;
		case 2:
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
			break;
		case 3:
			printf("Informe o ID do paciente: ");
			scanf("%u", &id);
			getchar(); // Limpar o buffer do teclado
			paciente = buscarPaciente(id);
			if (paciente == NULL) {
				printf("Paciente não encontrado.\n");
				break;
			}

			printf("Informe o ID do médico: ");
			scanf("%u", &id);
			getchar(); // Limpar o buffer do teclado
			medico = buscarMedico(id);
			if (medico == NULL) {
				printf("Médico não encontrado.\n");
				break;
			}

			printf("Informe a data e hora da consulta no formato dd/mm/yyyy:HH:MM : ");
			fgets(dataHora, SMALL_BUFFER_SIZE, stdin);
			printf("Informe o status da consulta (0 para agendado, 1 para "
			       "finalizado, 2 para cancelado): ");
			scanf("%d", &status);
			getchar(); // Limpar o buffer do teclado

			dataHora[strcspn(dataHora, "\n")] = 0;
			agendamento = novoAgendamento(0, paciente, medico,
						      dataHora, status);
			if (agendamento != NULL) {
				if (inserirAgendamento(agendamento) ==
				    OK_CODE) {
					freeAgendamento(agendamento);
					printf("Consulta agendada com sucesso!\n");
				} else {
					freeAgendamento(agendamento);
					printf("Erro ao agendar consulta.\n");
				}
			} else {
				printf("Erro ao criar consulta.\n");
			}
			break;
		case 4:
			printf("Informe o ID do paciente: ");
			scanf("%u", &id);
			getchar(); // Limpar o buffer do teclado
			paciente = buscarPaciente(id);
			if (paciente != NULL) {
				printf("Paciente encontrado:\n");
				printf("Nome: %s\n", paciente->nome);
				printf("CPF: %s\n", paciente->cpf);
				printf("Telefone: %s\n", paciente->telefone);
				printf("CEP: %s\n", paciente->cep);
				printf("Alergias: %s\n", paciente->alergias);
				printf("Deficiências: %s\n",
				       paciente->deficiencias);
				printf("Idade: %u\n", paciente->idade);
				printf("Gênero: %s\n",
				       (paciente->genero == masculino) ?
					       "Masculino" :
					       "Feminino");
				freePaciente(paciente);
			} else {
				printf("Paciente não encontrado.\n");
			}
			break;
		case 5:
			printf("Informe o ID do médico: ");
			scanf("%u", &id);
			getchar(); // Limpar o buffer do teclado
			medico = buscarMedico(id);
			if (medico != NULL) {
				printf("Médico encontrado:\n");
				printf("Nome: %s\n", medico->nome);
				printf("Especialidade: %s\n",
				       medico->especialidade);
				printf("Código: %u\n", medico->cod);
				freeMedico(medico);
			} else {
				printf("Médico não encontrado.\n");
			}
			break;
		case 6:
			printf("Informe o ID da consulta: ");
			scanf("%u", &id);
			getchar(); // Limpar o buffer do teclado
			agendamento = buscarAgendamento(id);
			if (agendamento != NULL) {
				printf("Consulta encontrada:\n");
				printf("Data e Hora:  %s\n",
				       agendamento->dataHora);
				printf("Status: %s\n",
				       (agendamento->status == agendado) ?
					       "Agendado" :
				       (agendamento->status == finalizado) ?
					       "Finalizado" :
					       "Cancelado");
				printf("Paciente:\n");
				printf("Nome: %s\n",
				       agendamento->paciente->nome);
				printf("CPF: %s\n", agendamento->paciente->cpf);
				printf("Médico:\n");
				printf("Nome: %s\n", agendamento->medico->nome);
				printf("Especialidade: %s\n",
				       agendamento->medico->especialidade);
				freeAgendamento(agendamento);
			} else {
				printf("Consulta não encontrada.\n");
			}
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
