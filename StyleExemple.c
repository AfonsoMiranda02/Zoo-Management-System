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
} animal;

typedef struct {
    int animal_id;
    char treating_name[100];
    char treating_start[20];
    char treating_end[20];
    int duration;
} treatment;

animal animals[MAX_ANIMALS];
treatment treatments[MAX_TREATMENTS];
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
void DrawButton(HDC hdc, RECT* rect, LPCSTR text, BOOL isHovered);

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
    static BOOL isHovered[6] = {FALSE};

    switch (uMsg) {
        case WM_CREATE:
            // We'll draw buttons manually, so we don't create them here
            break;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Draw background
            HBRUSH hBrush = CreateSolidBrush(RGB(173, 216, 230));  // Light blue
            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect(hdc, &rect, hBrush);
            DeleteObject(hBrush);

            // Draw styled buttons
            RECT buttonRects[6] = {
                {10, 10, 160, 40},
                {170, 10, 320, 40},
                {330, 10, 480, 40},
                {10, 50, 160, 80},
                {170, 50, 320, 80},
                {330, 50, 480, 80}
            };
            LPCSTR buttonTexts[6] = {
                "Adicionar Animal",
                "Listar Animais",
                "Listar Tratamentos",
                "Filtrar Animais",
                "Adicionar Tratamento",
                "Modificar Idade"
            };

            for (int i = 0; i < 6; i++) {
                DrawButton(hdc, &buttonRects[i], buttonTexts[i], isHovered[i]);
            }

            EndPaint(hwnd, &ps);
        }
        break;

        case WM_MOUSEMOVE:
        {
            int xPos = LOWORD(lParam);
            int yPos = HIWORD(lParam);

            RECT buttonRects[6] = {
                {10, 10, 160, 40},
                {170, 10, 320, 40},
                {330, 10, 480, 40},
                {10, 50, 160, 80},
                {170, 50, 320, 80},
                {330, 50, 480, 80}
            };

            for (int i = 0; i < 6; i++) {
                BOOL newHovered = PtInRect(&buttonRects[i], (POINT){xPos, yPos});
                if (newHovered != isHovered[i]) {
                    isHovered[i] = newHovered;
                    InvalidateRect(hwnd, &buttonRects[i], FALSE);
                }
            }
        }
        break;

        case WM_LBUTTONDOWN:
        {
            int xPos = LOWORD(lParam);
            int yPos = HIWORD(lParam);

            RECT buttonRects[6] = {
                {10, 10, 160, 40},
                {170, 10, 320, 40},
                {330, 10, 480, 40},
                {10, 50, 160, 80},
                {170, 50, 320, 80},
                {330, 50, 480, 80}
            };

            for (int i = 0; i < 6; i++) {
                if (PtInRect(&buttonRects[i], (POINT){xPos, yPos})) {
                    SendMessage(hwnd, WM_COMMAND, ID_ADD_ANIMAL + i, 0);
                    break;
                }
            }
        }
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

void DrawButton(HDC hdc, RECT* rect, LPCSTR text, BOOL isHovered) {
    // Draw button background
    HBRUSH brush = CreateSolidBrush(isHovered ? RGB(0, 102, 204) : RGB(0, 120, 215));
    FillRect(hdc, rect, brush);
    DeleteObject(brush);

    // Draw button text
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));
    DrawText(hdc, text, -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
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

                GetDlgItemText(hwnd, 100, name, sizeof(name));
                GetDlgItemText(hwnd, 101, family, sizeof(family));
                GetDlgItemText(hwnd, 102, species, sizeof(species));
                GetDlgItemText(hwnd, 103, ageStr, sizeof(ageStr));
                age = atoi(ageStr);

                animal newAnimal;
                newAnimal.id = animal_count + 1;
                strncpy(newAnimal.name, name, sizeof(newAnimal.name) - 1);
                newAnimal.name[sizeof(newAnimal.name) - 1] = '\0';

                if (_stricmp(family, "Ave") == 0) {
                    strncpy(newAnimal.family, "Ave", sizeof(newAnimal.family) - 1);
                } else if (_stricmp(family, "Reptil") == 0) {
                    strncpy(newAnimal.family, "Reptil", sizeof(newAnimal.family) - 1);
                } else if (_stricmp(family, "Mamifero") == 0) {
                    strncpy(newAnimal.family, "Mamifero", sizeof(newAnimal.family) - 1);
                } else {
                    MessageBox(hwnd, "A familia inserida não é valida! Use Ave, Reptil ou Mamifero.", "Erro", MB_OK | MB_ICONERROR);
                    return 0; // Return without adding the animal
                }
                newAnimal.family[sizeof(newAnimal.family) - 1] = '\0';
                strncpy(newAnimal.species, species, sizeof(newAnimal.species) - 1);
                newAnimal.species[sizeof(newAnimal.species) - 1] = '\0';
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
        snprintf(buffer, sizeof(buffer), "ID: %d, Nome: %s, Família: %s, Espécie: %s, Idade: %d",
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
        snprintf(buffer, sizeof(buffer), "ID Animal: %d, Nome: %s, Início: %s, Fim: %s, Duração: %d dias",
                treatments[i].animal_id, treatments[i].treating_name, treatments[i].treating_start,
                treatments[i].treating_end, treatments[i].duration);
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
                GetDlgItemText(hwnd, 100, family, sizeof(family));

                HWND hwndList = CreateWindow("LISTBOX", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_NOTIFY,
                                             10, 100, 780, 400, (HWND)GetWindowLongPtr(hwnd, GWLP_USERDATA), NULL, NULL, NULL);

                char buffer[256];
                for (int i = 0; i < animal_count; i++) {
                    if (_stricmp(animals[i].family, family) == 0) {
                        snprintf(buffer, sizeof(buffer), "ID: %d, Nome: %s, Família: %s, Espécie: %s, Idade: %d",
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
                treatment newTreatment = {0};

                GetDlgItemText(hwnd, 100, animalIdStr, sizeof(animalIdStr));
                GetDlgItemText(hwnd, 101, treatmentName, sizeof(treatmentName));
                GetDlgItemText(hwnd, 102, treating_start, sizeof(treating_start));
                GetDlgItemText(hwnd, 104, treating_end, sizeof(treating_end));
                GetDlgItemText(hwnd, 103, durationStr, sizeof(durationStr));

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
                    strncpy(newTreatment.treating_start, treating_start, sizeof(newTreatment.treating_start) - 1);
                    newTreatment.treating_start[sizeof(newTreatment.treating_start) - 1] = '\0';
                }

                if (strlen(treating_end) == 0) {
                    // Se a data não for fornecida, use a data atual
                    time_t t = time(NULL);
                    struct tm *tm = localtime(&t);
                    strftime(newTreatment.treating_end, sizeof(newTreatment.treating_end), "%d/%m/%Y", tm);
                } else {
                    strncpy(newTreatment.treating_end, treating_end, sizeof(newTreatment.treating_end) - 1);
                    newTreatment.treating_end[sizeof(newTreatment.treating_end) - 1] = '\0';
                }

                // Verify if the duration is positive
                if (duration <= 0) {
                    MessageBox(hwnd, "A duração do tratamento deve ser um número positivo!", "Erro", MB_OK | MB_ICONERROR);
                    return 0;
                }

                newTreatment.animal_id = animalId;
                newTreatment.duration = duration;

                strncpy(newTreatment.treating_name, treatmentName, sizeof(newTreatment.treating_name) - 1);
                newTreatment.treating_name[sizeof(newTreatment.treating_name) - 1] = '\0';

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

                GetDlgItemText(hwnd, 100, animalIdStr, sizeof(animalIdStr));
                GetDlgItemText(hwnd, 101, newAgeStr, sizeof(newAgeStr));

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
        return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWDEFAULT);
    } else if (option == 2) {
        // Implementar o modo de console aqui
        animal animals[MAX_ANIMALS];
        treatment treatings[MAX_TREATMENTS];
        int animal_count = 0, treating_count = 0;
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
            printf("Opção selecionada: ");
            scanf("%d", &option);

            if(option==1) {
                if (animal_count < MAX_ANIMALS) {
                    animal new_animal;
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
                    treatment treatingDump = treatings[i];
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
                    treatment new_treatment;
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
            else if(option==6){
                int id, newAge;
                printf("Insira o ID do animal a modificar:\n");
                scanf("%d", &id);
                printf("Insira a nova idade:\n");
                scanf("%d", &newAge);
                for (int i = 0; i < animal_count; i++) {
                    if (animals[i].id == id) {
                        animals[i].age = newAge;
                        printf("Idade modificada com sucesso!\n");
                        break;
                    }
                }
            }
            else if(option==7){
                int total_duration = 0;
                for (int i = 0; i < treating_count; i++) {
                    total_duration += treatings[i].duration;
                }
                printf("Somatório total dos tratamentos: %d dias\n", total_duration);
            }
            else if(option==8){
                int ave_duration = 0, reptil_duration = 0, mamifero_duration = 0;
                for (int i = 0; i < treating_count; i++) {
                    for (int j = 0; j < animal_count; j++) {
                        if (treatings[i].animal_id == animals[j].id) {
                            if (strcmp(animals[j].family, "Ave") == 0) {
                                ave_duration += treatings[i].duration;
                            } else if (strcmp(animals[j].family, "Reptil") == 0) {
                                reptil_duration += treatings[i].duration;
                            } else if (strcmp(animals[j].family, "Mamifero") == 0) {
                                mamifero_duration += treatings[i].duration;
                            }
                            break;
                        }
                    }
                }
                printf("Tempo total de tratamentos por família:\n");
                printf("Ave: %d dias\n", ave_duration);
                printf("Reptil: %d dias\n", reptil_duration);
                printf("Mamifero: %d dias\n", mamifero_duration);
            }
            else if(option==9){
                int ave_duration = 0, reptil_duration = 0, mamifero_duration = 0;
                for (int i = 0; i < treating_count; i++) {
                    for (int j = 0; j < animal_count; j++) {
                        if (treatings[i].animal_id == animals[j].id) {
                            if (strcmp(animals[j].family, "Ave") == 0) {
                                ave_duration += treatings[i].duration;
                            } else if (strcmp(animals[j].family, "Reptil") == 0) {
                                reptil_duration += treatings[i].duration;
                            } else if (strcmp(animals[j].family, "Mamifero") == 0) {
                                mamifero_duration += treatings[i].duration;
                            }
                            break;
                        }
                    }
                }
                if (ave_duration <= reptil_duration && ave_duration <= mamifero_duration) {
                    printf("A família com menor tempo de tratamento é Ave com %d dias.\n", ave_duration);
                } else if (reptil_duration <= ave_duration && reptil_duration <= mamifero_duration) {
                    printf("A família com menor tempo de tratamento é Reptil com %d dias.\n", reptil_duration);
                } else {
                    printf("A família com menor tempo de tratamento é Mamifero com %d dias.\n", mamifero_duration);
                }
            }
            else if(option==10){
                int min_duration = INT_MAX;
                for (int i = 0; i < treating_count; i++) {
                    if (treatings[i].duration < min_duration) {
                        min_duration = treatings[i].duration;
                    }
                }
                printf("Tratamentos com menor tempo de duração (%d dias):\n", min_duration);
                for (int i = 0; i < treating_count; i++) {
                    if (treatings[i].duration == min_duration) {
                        printf("ID do animal: %d, Nome: %s, Data Começo: %s\n",
                               treatings[i].animal_id, treatings[i].treating_name, treatings[i].treating_start);
                    }
                }
            }
            else if(option==11){
                printf("Apresentação do custo total de cada tratamento:\n");
                for (int i = 0; i < treating_count; i++) {
                    // Assumindo um custo diário de 50 unidades monetárias
                    int custo_total = treatings[i].duration * 50;
                    printf("ID do animal: %d, Nome: %s, Custo total: %d unidades monetárias\n",
                           treatings[i].animal_id, treatings[i].treating_name, custo_total);
                }
            }
            else if(option==12) {
                break;
            }else {
                printf("Ocorreu um erro.");
                break;
            }
        }
    } else {
        printf("Opção inválida.\n");
    }

    return 0;
}