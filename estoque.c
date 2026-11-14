#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "inventory.txt"

//estrtutura para represeentar o item do inventario

typedef struct {
    char name[100];
    int quantity;
} Item;

//funcoes auxiliares (considere como metodos)
void showMenu(){
    printf("==============================\n");
    printf("       Inventory Control      \n");
    printf("==============================\n");

    printf("1. Add Item\n");
    printf("2. Remove Item\n");
    printf("3. List Inventory\n");
    printf("4. Exit\n");
    printf("Option: ");
}

void addItem(){
    
    Item item;

    // Limpa o buffer de entrada para evitar conflitos com o Enter anterior
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    printf("Enter the item name: "); 
    fgets(item.name, sizeof(item.name), stdin);//alterei aqui
    item.name[strcspn(item.name, "\n")] = 0; // Remove o \n do final da string

    printf("Enter the quantity: ");
    scanf("%d", &item.quantity); //le a quantidade

    //abre arquivo e adiciona no fim (append)

    FILE *file = fopen(FILENAME, "a");

    if(file == NULL){
        printf("Error opening file!\n");
        return;
    }

    //gravar os dados - nome na primeira linha e qtd na segunda
    fprintf(file, "%s\n%d\n", item.name, item.quantity);
    fclose(file);

    printf("Item added successfully!\n");

}

void listInventory(){
    FILE *file = fopen(FILENAME, "r");

    if(file == NULL){
        printf("The inventory is empty!\n");
        return;
    }

    char name[100];
    int quantity;
    int hasItems = 0;

    //tenta ler o primeiro item
    if(fscanf(file, " %[^\n]", name) != 1 || fscanf(file, "%d", &quantity) != 1){
        printf("The inventory is empty!\n");
        fclose(file);
        return;
    }

    //se houver item, imprimir o cabecalho 
    printf("==============================\n");
    printf("      Currenty Inventory      \n");
    printf("==============================\n");

    rewind(file);

    while (fscanf(file, " %[^\n]", name) == 1 && fscanf(file, "%d", &quantity) == 1){
        printf("Name: %s\n", name);
        printf("Quantity: %d\n", quantity);
    }

    fclose(file);

}

// remocao de item
void removeItem(){
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        printf("Item not found.\n");
        return;
    }

    char targetName[100];
    int removeQty;

    // Limpa o buffer antes de ler a string
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Enter the item name: ");
    fgets(targetName, sizeof(targetName), stdin);
    targetName[strcspn(targetName, "\n")] = 0;
    
    printf("Enter the quantity to be removed: ");
    scanf("%d", &removeQty);

    Item items[100];
    int count = 0;
    int found = 0;
    int insufficientStock = 0;
    int availableQty = 0;

    // Lê todos os itens do ficheiro para a memória
    while (fscanf(file, " %[^\n]", items[count].name) == 1 && fscanf(file, "%d", &items[count].quantity) == 1) {
        if (strcmp(items[count].name, targetName) == 0) {
            found = 1;
            availableQty = items[count].quantity;
            if (items[count].quantity >= removeQty) {
                items[count].quantity -= removeQty;
            } else {
                insufficientStock = 1;
            }
        }
        count++;
    }
    fclose(file);

    if (!found) {
        printf("Item not found.\n");
        return;
    }

    if (insufficientStock) {
        printf("Insufficient stock. Available quantity: %d\n", availableQty);
        return;
    }

    // Reescreve o ficheiro com os dados atualizados
    file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        
        // Mantendo a quantidade atualizada:
        fprintf(file, "%s\n%d\n", items[i].name, items[i].quantity);
    }
    fclose(file);

    printf("Quantity successfully updated!\n");
}

int main(){
    int option;
    char continueChoice;

    do {
        showMenu();
        if (scanf("%d", &option) != 1) {
            printf("Invalid option! Please try again.\n");
            while (getchar() != '\n'); // Limpa o buffer
            continue;
        }

        switch (option) {
            case 1:
                addItem();
                break;
            case 2:
                removeItem();
                break;
            case 3:
                listInventory();
                break;
            case 4:
                printf("Thank you for using Inventory Control! See you next time.\n");
                return 0;
            default:
                printf("Invalid option! Please try again.\n");
                continue; // Volta ao menu sem perguntar se deseja continuar se a opção for inválida
        }

        // Pergunta se deseja realizar outra operação
        printf("Do you want to perform another operation? (y/n): ");
        scanf(" %c", &continueChoice);

    } while (continueChoice == 'y' || continueChoice == 'Y');

    printf("Thank you for using Inventory Control! See you next time.\n");
    return 0;
}