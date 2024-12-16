#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ANIMALS 100
#define MAX_TREATMENTS 100

// Definindo os IDs dos botões
#define ID_ADD_ANIMAL 1
#define ID_LIST_ANIMALS 2
#define ID_LIST_TREATMENTS 3
#define ID_FILTER_ANIMALS 4
#define ID_ADD_TREATMENT 5
#define ID_MODIFY_AGE 6

// Definindo o ID do ícone
#define IDI_ICON 101

typedef struct {
    int id;
    char name[50];
    char family[50];
    char species[50];
    int age;
} Animal;

typedef struct {
    int animal_id;
    char treating_name[100];
    char treating_start[20];
    char treating_end[20];
    int duration;
} Treatment;

Animal animals[MAX_ANIMALS];
Treatment treatments[MAX_TREATMENTS];
int animal_count = 0;
int treatment_count = 0;

HWND hwndMain, hwndAddAnimal, hwndListAnimals, hwndListTreatments;
HICON hIcon;

// Protótipos das funções
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void AddAnimal(HWND hwnd);
void ListAnimals(HWND hwnd);
void ListTreatments(HWND hwnd);
void FilterAnimals(HWND hwnd);
void AddTreatment(HWND hwnd);
void ModifyAnimalAge(HWND hwnd);
LRESULT CALLBACK AddAnimalProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FilterAnimalsProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AddTreatmentProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ModifyAnimalAgeProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Registrar a classe da janela
    const char CLASS_NAME[] = "Zoo Management System";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    // Carregar o ícone do recurso
    hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
    if (hIcon) {
        wc.hIcon = hIcon;
    } else {
        // Se falhar ao carregar o ícone personalizado, use o ícone padrão do sistema
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    }

    RegisterClass(&wc);

    // Criar a janela
    hwndMain = CreateWindowEx(
        0,                              // Estilo estendido
        CLASS_NAME,                     // Nome da classe
        "Zoo Management System",        // Título da janela
        WS_OVERLAPPEDWINDOW,            // Estilo da janela
        CW_USEDEFAULT, CW_USEDEFAULT,   // Posição
        800, 600,                       // Tamanho
        NULL,                           // Janela pai
        NULL,                           // Menu
        hInstance,                      // Instância
        NULL                            // Dados adicionais
    );

    if (hwndMain == NULL) {
        return 0;
    }

    ShowWindow(hwndMain, nCmdShow);

    // Loop de mensagens
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Liberar o ícone
    if (hIcon) {
        DestroyIcon(hIcon);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            // Criar botões
            CreateWindow("BUTTON", "Adicionar Animal", WS_VISIBLE | WS_CHILD, 10, 10, 150, 30, hwnd, (HMENU)ID_ADD_ANIMAL, NULL, NULL);
            CreateWindow("BUTTON", "Listar Animais", WS_VISIBLE | WS_CHILD, 170, 10, 150, 30, hwnd, (HMENU)ID_LIST_ANIMALS, NULL, NULL);
            CreateWindow("BUTTON", "Listar Tratamentos", WS_VISIBLE | WS_CHILD, 330, 10, 150, 30, hwnd, (HMENU)ID_LIST_TREATMENTS, NULL, NULL);
            CreateWindow("BUTTON", "Filtrar Animais", WS_VISIBLE | WS_CHILD, 10, 50, 150, 30, hwnd, (HMENU)ID_FILTER_ANIMALS, NULL, NULL);
            CreateWindow("BUTTON", "Adicionar Tratamento", WS_VISIBLE | WS_CHILD, 170, 50, 150, 30, hwnd, (HMENU)ID_ADD_TREATMENT, NULL, NULL);
            CreateWindow("BUTTON", "Modificar Idade", WS_VISIBLE | WS_CHILD, 330, 50, 150, 30, hwnd, (HMENU)ID_MODIFY_AGE, NULL, NULL);
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_ADD_ANIMAL:
                    AddAnimal(hwnd);
                    break;
                case ID_LIST_ANIMALS:
                    ListAnimals(hwnd);
                    break;
                case ID_LIST_TREATMENTS:
                    ListTreatments(hwnd);
                    break;
                case ID_FILTER_ANIMALS:
                    FilterAnimals(hwnd);
                    break;
                case ID_ADD_TREATMENT:
                    AddTreatment(hwnd);
                    break;
                case ID_MODIFY_AGE:
                    ModifyAnimalAge(hwnd);
                    break;
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void AddAnimal(HWND hwnd) {
    if (animal_count >= MAX_ANIMALS) {
        MessageBox(hwnd, "Limite máximo de animais atingido!", "Erro", MB_OK | MB_ICONERROR);
        return;
    }

    HWND hwndDialog = CreateWindow("STATIC", "Adicionar Animal", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 100, 200, 300, 300, hwnd, NULL, NULL, NULL);

    CreateWindow("STATIC", "Nome:", WS_VISIBLE | WS_CHILD, 10, 10, 80, 20, hwndDialog, NULL, NULL, NULL);
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 10, 180, 20, hwndDialog, (HMENU)100, NULL, NULL);

    CreateWindow("STATIC", "Família:", WS_VISIBLE | WS_CHILD, 10, 40, 80, 20, hwndDialog, NULL, NULL, NULL);
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 40, 180, 20, hwndDialog, (HMENU)101, NULL, NULL);
    CreateWindow("STATIC", "(Ave,Reptil ou Mamifero)", WS_VISIBLE | WS_CHILD, 10, 70, 160, 20, hwndDialog, NULL, NULL, NULL);

    CreateWindow("STATIC", "Espécie:", WS_VISIBLE | WS_CHILD, 10, 100, 80, 20, hwndDialog, NULL, NULL, NULL);
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 100, 180, 20, hwndDialog, (HMENU)102, NULL, NULL);
    CreateWindow("STATIC","Gorila,Macaco,etc..." ,WS_VISIBLE | WS_CHILD, 10, 130, 160, 20, hwndDialog, NULL, NULL, NULL);

    CreateWindow("STATIC", "Idade:", WS_VISIBLE | WS_CHILD, 10, 160, 80, 20, hwndDialog, NULL, NULL, NULL);
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 160, 180, 20, hwndDialog, (HMENU)103, NULL, NULL);

    CreateWindow("BUTTON", "Adicionar", WS_VISIBLE | WS_CHILD, 100, 190, 100, 30, hwndDialog, (HMENU)1, NULL, NULL);
    SetWindowLongPtr(hwndDialog, GWLP_USERDATA, (LONG_PTR)hwnd);
    SetWindowLongPtr(hwndDialog, GWLP_WNDPROC, (LONG_PTR)AddAnimalProc);
}

LRESULT CALLBACK AddAnimalProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {
                char name[50], family[50], species[50], ageStr[10];
                int age;

                GetDlgItemText(hwnd, 100, name, 50);
                GetDlgItemText(hwnd, 101, family, 50);
                GetDlgItemText(hwnd, 102, species, 50);
                GetDlgItemText(hwnd, 103, ageStr, 10);
                age = atoi(ageStr);

                Animal newAnimal = {animal_count + 1, "", "", "", 0};
                strcpy(newAnimal.name, name);
                if (strcmp(family, "Ave") == 0 || strcmp(family, "ave") == 0) {
                    strcpy(newAnimal.family, "Ave");
                } else if (strcmp(family, "Reptil") == 0 || strcmp(family, "reptil") == 0) {
                    strcpy(newAnimal.family, "Reptil");
                } else if (strcmp(family, "Mamifero") == 0 || strcmp(family, "mamifero") == 0) {
                    strcpy(newAnimal.family, "Mamifero");
                } else {
                    MessageBox(hwnd, "A familia inserida não é valida! Use Ave, Reptil ou Mamifero.", "Erro", MB_OK | MB_ICONERROR);
                    return 0; // Return without adding the animal
                }
                strcpy(newAnimal.species, species);
                newAnimal.age = age;

                animals[animal_count++] = newAnimal;

                MessageBox(hwnd, "Animal adicionado com sucesso!", "Sucesso", MB_OK | MB_ICONINFORMATION);
                DestroyWindow(hwnd);
            }
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void ListAnimals(HWND hwnd) {
    if (animal_count == 0) {
        MessageBox(hwnd, "Não há animais cadastrados.", "Informação", MB_OK | MB_ICONINFORMATION);
        return;
    }

    HWND hwndList = CreateWindow("LISTBOX", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOTIFY,
                                 10, 100, 780, 400, hwnd, NULL, NULL, NULL);

    char buffer[256];
    for (int i = 0; i < animal_count; i++) {
        sprintf(buffer, "ID: %d, Nome: %s, Família: %s, Espécie: %s, Idade: %d",
                animals[i].id, animals[i].name, animals[i].family, animals[i].species, animals[i].age);
        SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)buffer);
    }
}

void ListTreatments(HWND hwnd) {
    if (treatment_count == 0) {
        MessageBox(hwnd, "Não há tratamentos cadastrados.", "Informação", MB_OK | MB_ICONINFORMATION);
        return;
    }

    HWND hwndList = CreateWindow("LISTBOX", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOTIFY,
                                 10, 100, 780, 400, hwnd, NULL, NULL, NULL);

    char buffer[256];
    for (int i = 0; i < treatment_count; i++) {
        sprintf(buffer, "ID Animal: %d, Nome: %s, Início: %s, Fim: %s, Duração: %d dias",
                treatments[i].animal_id, treatments[i].treating_name, treatments[i].treating_start, treatments[i].treating_end, treatments[i].duration);
        SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)buffer);
    }
}

void FilterAnimals(HWND hwnd) {
    HWND hwndDialog = CreateWindow("STATIC", "Filtrar Animais por Família", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 100, 100, 300, 150, hwnd, NULL, NULL, NULL);

    CreateWindow("STATIC", "Família:", WS_VISIBLE | WS_CHILD, 10, 10, 80, 20, hwndDialog, NULL, NULL, NULL);
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 10, 180, 20, hwndDialog, (HMENU)100, NULL, NULL);

    CreateWindow("BUTTON", "Filtrar", WS_VISIBLE | WS_CHILD, 100, 50, 100, 30, hwndDialog, (HMENU)1, NULL, NULL);

    SetWindowLongPtr(hwndDialog, GWLP_USERDATA, (LONG_PTR)hwnd);
    SetWindowLongPtr(hwndDialog, GWLP_WNDPROC, (LONG_PTR)FilterAnimalsProc);
}

LRESULT CALLBACK FilterAnimalsProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {
                char family[50];
                GetDlgItemText(hwnd, 100, family, 50);

                HWND hwndList = CreateWindow("LISTBOX", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOTIFY,
                                             10, 100, 780, 400, (HWND)GetWindowLongPtr(hwnd, GWLP_USERDATA), NULL, NULL, NULL);

                char buffer[256];
                for (int i = 0; i < animal_count; i++) {
                    if (strcmp(animals[i].family, family) == 0) {
                        sprintf(buffer, "ID: %d, Nome: %s, Família: %s, Espécie: %s, Idade: %d",
                                animals[i].id, animals[i].name, animals[i].family, animals[i].species, animals[i].age);
                        SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)buffer);
                    }
                }

                DestroyWindow(hwnd);
            }
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void AddTreatment(HWND hwnd) {
    if (treatment_count >= MAX_TREATMENTS) {
        MessageBox(hwnd, "Limite máximo de tratamentos atingido!", "Erro", MB_OK | MB_ICONERROR);
        return;
    }

    HWND hwndDialog = CreateWindow("STATIC", "Adicionar Tratamento", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 100, 100, 300, 250, hwnd, NULL, NULL, NULL);

    CreateWindow("STATIC", "ID do Animal:", WS_VISIBLE | WS_CHILD, 10, 10, 100, 20, hwndDialog, NULL, NULL, NULL);
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 10, 160, 20, hwndDialog, (HMENU)100, NULL, NULL);

    CreateWindow("STATIC", "Nome do Tratamento:", WS_VISIBLE | WS_CHILD, 10, 40, 120, 20, hwndDialog, NULL, NULL, NULL);
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 140, 40, 140, 20, hwndDialog, (HMENU)101, NULL, NULL);

    CreateWindow("STATIC", "Data de Início:", WS_VISIBLE | WS_CHILD, 10, 70, 100, 20, hwndDialog, NULL, NULL, NULL);
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 70, 160, 20, hwndDialog, (HMENU)102, NULL, NULL);

    CreateWindow("STATIC", "Data de Fim:", WS_VISIBLE | WS_CHILD, 10, 100, 100, 20, hwndDialog, NULL, NULL, NULL);
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 100, 160, 20, hwndDialog, (HMENU)104, NULL, NULL);

    CreateWindow("STATIC", "Duração (dias):", WS_VISIBLE | WS_CHILD, 10, 130, 100, 20, hwndDialog, NULL, NULL, NULL);
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 130, 160, 20, hwndDialog, (HMENU)103, NULL, NULL);

    CreateWindow("BUTTON", "Adicionar", WS_VISIBLE | WS_CHILD, 100, 170, 100, 30, hwndDialog, (HMENU)1, NULL, NULL);

    SetWindowLongPtr(hwndDialog, GWLP_USERDATA, (LONG_PTR)hwnd);
    SetWindowLongPtr(hwndDialog, GWLP_WNDPROC, (LONG_PTR)AddTreatmentProc);
}

LRESULT CALLBACK AddTreatmentProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {
                char animalIdStr[10], treatmentName[100], treating_start[20], treating_end[20], durationStr[10];
                int animalId, duration;
                Treatment newTreatment = {0}; // Inicialize a estrutura aqui

                GetDlgItemText(hwnd, 100, animalIdStr, 10);
                GetDlgItemText(hwnd, 101, treatmentName, 100);
                GetDlgItemText(hwnd, 102, treating_start, 20);
                GetDlgItemText(hwnd, 104, treating_end, 20);
                GetDlgItemText(hwnd, 103, durationStr, 10);

                animalId = atoi(animalIdStr);
                duration = atoi(durationStr);

                // Verify if the animal exists
                int animalExists = 0;
                for (int i = 0; i < animal_count; i++) {
                    if (animals[i].id == animalId) {
                        animalExists = 1;
                        break;
                    }
                }

                if (!animalExists) {
                    MessageBox(hwnd, "O animal com o ID fornecido não existe!", "Erro", MB_OK | MB_ICONERROR);
                    return 0;
                }

                // Verify if the treatment name is not empty
                if (strlen(treatmentName) == 0) {
                    MessageBox(hwnd, "O nome do tratamento não pode estar vazio!", "Erro", MB_OK | MB_ICONERROR);
                    return 0;
                }

                if (strlen(treating_start) == 0) {
                    // Se a data não for fornecida, use a data atual
                    time_t t = time(NULL);
                    struct tm *tm = localtime(&t);
                    strftime(newTreatment.treating_start, sizeof(newTreatment.treating_start), "%d/%m/%Y", tm);
                } else {
                    // Se a data for fornecida, copie-a diretamente
                    strcpy(newTreatment.treating_start, treating_start);
                }

                if (strlen(treating_end) == 0) {
                    // Se a data não for fornecida, use a data atual
                    time_t t = time(NULL);
                    struct tm *tm = localtime(&t);
                    strftime(newTreatment.treating_end, sizeof(newTreatment.treating_end), "%d/%m/%Y", tm);
                } else {
                    // Se a data for fornecida, copie-a diretamente
                    strcpy(newTreatment.treating_end, treating_end);
                }

                // Verify if the duration is positive
                if (duration <= 0) {
                    MessageBox(hwnd, "A duração do tratamento deve ser um número positivo!", "Erro", MB_OK | MB_ICONERROR);
                    return 0;
                }

                newTreatment.animal_id = animalId;
                newTreatment.duration = duration;

                strcpy(newTreatment.treating_name, treatmentName);

                treatments[treatment_count++] = newTreatment;

                MessageBox(hwnd, "Tratamento adicionado com sucesso!", "Sucesso", MB_OK | MB_ICONINFORMATION);
                DestroyWindow(hwnd);
            }
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void ModifyAnimalAge(HWND hwnd) {
    HWND hwndDialog = CreateWindow("STATIC", "Modificar Idade do Animal", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 100, 100, 300, 200, hwnd, NULL, NULL, NULL);

    CreateWindow("STATIC", "ID do Animal:", WS_VISIBLE | WS_CHILD, 10, 10, 100, 20, hwndDialog, NULL, NULL, NULL);
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 10, 160, 20, hwndDialog, (HMENU)100, NULL, NULL);

    CreateWindow("STATIC", "Nova Idade:", WS_VISIBLE | WS_CHILD, 10, 40, 100, 20, hwndDialog, NULL, NULL, NULL);
    CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 40, 160, 20, hwndDialog, (HMENU)101, NULL, NULL);

    CreateWindow("BUTTON", "Modificar", WS_VISIBLE | WS_CHILD, 100, 80, 100, 30, hwndDialog, (HMENU)1, NULL, NULL);

    SetWindowLongPtr(hwndDialog, GWLP_USERDATA, (LONG_PTR)hwnd);
    SetWindowLongPtr(hwndDialog, GWLP_WNDPROC, (LONG_PTR)ModifyAnimalAgeProc);
}

LRESULT CALLBACK ModifyAnimalAgeProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {
                char animalIdStr[10], newAgeStr[10];
                int animalId, newAge;

                GetDlgItemText(hwnd, 100, animalIdStr, 10);
                GetDlgItemText(hwnd, 101, newAgeStr, 10);

                animalId = atoi(animalIdStr);
                newAge = atoi(newAgeStr);

                int found = 0;
                for (int i = 0; i < animal_count; i++) {
                    if (animals[i].id == animalId) {
                        animals[i].age = newAge;
                        found = 1;
                        break;
                    }
                }

                if (found) {
                    MessageBox(hwnd, "Idade do animal modificada com sucesso!", "Sucesso", MB_OK | MB_ICONINFORMATION);
                } else {
                    MessageBox(hwnd, "Animal não encontrado!", "Erro", MB_OK | MB_ICONERROR);
                }

                DestroyWindow(hwnd);
            }
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int main() {
    int option;
    printf("Selecione o modo em que deseja executar:\n");
    printf("1-Modo Gráfico\n");
    printf("2-Modo de consola\n");
    printf("Opção selecionada: ");
    scanf("%d", &option);

    if (option == 1) {
        WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWDEFAULT);
    } else if (option == 2) {
        // Implementar o modo de console aqui
        printf("Modo de console não implementado nesta versão.\n");
    } else {
        printf("Opção inválida.\n");
    }

    return 0;
}