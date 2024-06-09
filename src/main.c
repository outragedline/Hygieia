#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"
int main(int argc, char *argv[])
{
	createdb();
	Medico *medico = (Medico *)malloc(sizeof(Medico));
	medico->nome = "Joseval";
	medico->especialidade = "Pediatria";
	medico->cod = 3321;
	inserirMedico(medico);
	free(medico);
	return EXIT_SUCCESS;
}
