#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    char family[50];
    char species[50];
    int age;
} animal;

typedef struct {
    int animal_id;
    char treating_name[100];
    char treating_start[20];
    int duration;
} treating;

const int MAX_ANIMALS = 100;
const int MAX_TREATMENTS = 100;

int main(){
    animal animals[MAX_ANIMALS];
    animal new_animal;
    treating treatings[MAX_TREATMENTS];
    treating new_treatment;
    int animal_count=0, treating_count=0;
    int option;
    char familyOpt[50];

    while (1) {
        printf("\nSelecione a opção que deseja executar: \n");
        printf("1-Adicionar animal\n");
        printf("2-Listar animais\n");
        printf("3-Listar tratamentos\n");
        printf("4-Filtrar animais por familia\n");
        printf("5-Adicionar tratamento\n");
        printf("6-Modificar a idade do animal\n");
        printf("7-Calcular somatório dos tratamentos\n");
        printf("8-Calcular o tempo total de tratamentos por família\n");
        printf("9-Calcular a família com menor tempo de tratamento\n");
        printf("10-Listar tratamentos com o menor tempo de tratamento\n");
        printf("11-Apresentação do custo total de cada tratamento\n");
        printf("12-Sair\n");
        printf("Opção selecionada: \n");
        scanf("%d", &option);

        if(option==1) {
            if (animal_count < MAX_ANIMALS) {
                new_animal.id = animal_count + 1;
                printf("Insira o nome do animal:\n");
                scanf("%s", new_animal.name);
                printf("Insira a familia do animal:\n");
                scanf("%s", new_animal.family);
                printf("Insira a especie do animal:\n");
                scanf("%s", new_animal.species);
                printf("Insira a idade do animal:\n");
                scanf("%d", &new_animal.age);
                animals[animal_count++] = new_animal;
                printf("O animal foi adicionado.\n");
            }else {
                printf("Nao foi adicionado devido a falta de espaço.\n");
            }
        }
        else if(option==2) {
            printf("Listagem de todos os animais:\n");
            for (int i = 0; i < animal_count; i++) {
                animal animalDump = animals[i];
                printf("ID: %d, Nome: %s, Familia: %s, Especie: %s, Idade: %d\n",
                animalDump.id, animalDump.name, animalDump.family, animalDump.species, animalDump.age);
            }
        }
        else if(option==3) {
            printf("Listagens de todos os tratamentos:\n");
            for (int i = 0; i < treating_count; i++) {
                treating treatingDump = treatings[i];
                printf("ID do animal: %d, Nome: %s, Data Começo: %s, Duração(em dias): %d\n",
                       treatingDump.animal_id, treatingDump.treating_name, treatingDump.treating_start, treatingDump.duration);
            }
        }
        else if(option==4) {
            printf("Escolha a familia do animal que quer filtrar: \n");
            scanf("%s", familyOpt);
            for (int i = 0; i < animal_count; i++) {
                if (strcmp(animals[i].family, familyOpt) == 0) {
                    printf("ID: %d, Nome: %s, Familia: %s, Especie: %s, Idade: %d\n",
                            animals[i].id, animals[i].name, animals[i].family, animals[i].species, animals[i].age);
                }
            }
        }
        else if(option==5) {
            if (treating_count < MAX_TREATMENTS) {
                printf("Insira o ID do animal:\n");
                scanf("%d", &new_treatment.animal_id);
                printf("Insira o nome do tratamento:\n");
                scanf("%s", new_treatment.treating_name);
                printf("Insira quando o tratamento começou:\n");
                scanf("%s", new_treatment.treating_start);
                printf("Insira a duração do tratamento (em dias):\n");
                scanf("%d", &new_treatment.duration);
                treatings[treating_count++] = new_treatment;
                printf("O tratamento foi adicionado\n");
            } else {
                printf("Não foi possível adicionar o tratamento, pois o limite foi atingido.\n");
            }
        }
        else if(option==6){}
        else if(option==7){}
        else if(option==8){}
        else if(option==9){}
        else if(option==10){}
        else if(option==11){}
        else if(option==12) {
            break;
        }else {
            printf("Ocorreu um erro.");
            break;
        }
    }
    return 0;
}