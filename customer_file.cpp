#include <iostream>
#include <string>
#include <list>
#include <limits>
#include <fstream>

using namespace std;

struct Customer {
    int id;
    int age;
    char name[75];
    char role;
    int qtdDependents;
	int holderId;
    Customer *holder;
    bool active = false;
};

FILE *customer_file;
FILE *customer_last_id;

bool check_customer_with_dependent(int customerId, FILE* customer_file) {
    Customer customer;

    fseek(customer_file, 0, SEEK_SET);

    while (fread(&customer, sizeof(Customer), 1, customer_file) == 1) {
        if (customer.active && customer.holderId == customerId) {
            return true;
        }
    }
    return false;
}

bool check_holder_with_dependente(int customerId, FILE* customer_file) {
    Customer customer;
    fseek(customer_file, 0, SEEK_SET);

    while (fread(&customer, sizeof(Customer), 1, customer_file) == 1) {
        if (customer.active && customer.holderId == customerId) {
            if (check_customer_with_dependent(customer.id, customer_file)) {
                return true;
            }
        }
    }
    return false;
}

void insert_customer() {
    cout << "=-=-=-=-=-= CADASTRO DE CLIENTE =-=-=-=-=-=" << endl;

    Customer customer, new_customer;
    int lastId;
    char isHolder = 'T';
    int holderIdDig;

    fseek(customer_last_id, 0, SEEK_SET);
    fread(&lastId, sizeof(int), 1, customer_last_id);

    new_customer.id = ++lastId;
    
    cout << "Cadastrar Titular ou Dependente? (T/D): " << endl;
    scanf("%c", &isHolder);

    if (isHolder == 'D') {
        bool holderFound = false;
        bool holderHasDependent = false;

        do {
            cout << "Informe o código do Titular: " << endl;
            scanf("%i", &holderIdDig);

            fseek(customer_file, 0, SEEK_SET);
            while(fread(&customer, sizeof(Customer), 1, customer_file) == 1) {
                if (customer.id == holderIdDig && !check_customer_with_dependent(customer.id, customer_file)) {
                    new_customer.holderId = holderIdDig;
                    holderFound = true;
                    break;
                }
            }

            if (holderIdDig == 0) {
                return;
            }

            if(!holderFound) {
            cout << "Não foi encontrado um Titular com esse código." << endl;
            return;
            }
        } while (!holderFound);
    } else {
        new_customer.holder = nullptr;
    }

    new_customer.role = isHolder;
	new_customer.holderId = holderIdDig;
    new_customer.active = true;

    cout << "Informe o nome: " << endl;
    scanf("%s", new_customer.name);

    cout << "Informe a idade: " << endl;
    scanf("%i", &new_customer.age);

    fseek(customer_file, 0, SEEK_END);
    fwrite(&new_customer, sizeof(Customer), 1, customer_file);

    fseek(customer_last_id, 0, SEEK_SET);
    fwrite(&lastId, sizeof(int), 1, customer_last_id);
}

void delete_customer() {
    cout << "=-=-=-=-=-= EXCLUIR CLIENTE =-=-=-=-=-=" << endl;

    Customer customer;
    int id;

    cout << "Informe o ID do cliente: " << endl;
    cin >> id;

    fseek(customer_file, 0, SEEK_SET);

    if (check_holder_with_dependente(id, customer_file)) {
        printf("O cliente não pode ser excluído pois ele está sendo usado como titular!\n");
        return;
    }

    while (fread(&customer, sizeof(Customer), 1, customer_file) == 1) {
        if (customer.id == id) {
            if (customer.qtdDependents > 0) {
                cout << "Não é possível excluir este cliente pois ele possui " << customer.qtdDependents << " dependentes!" << endl;
                return;
            }
            
            customer.active;
            fseek(customer_file, -sizeof(Customer), SEEK_CUR);
            fwrite(&customer, sizeof(Customer), 1, customer_file);

            cout << "Cliente excluido com sucesso!" << endl;
        }
    }
    cout << "Cliente não encontrado" << endl;
}

void update_customer() {
    cout << "=-=-=-=-=-= MODIFICAR CLIENTE =-=-=-=-=-=" << endl;

    int id;

    cout << "Informe o id do cliente que será modificado: ";
    cin >> id;
    bool found = false;

    // for (auto customer: customers) {
    //     if (customer->id == id) {
    //         found = true;

    //         cout << "Informe o nome: " << endl;
    //         scanf("%s", customer->name);

    //         cout << "Informe a idade: " << endl;
    //         scanf("%i", &customer->age);

    //         cout << "Cliente atualizado!" << endl;
    //     }
    // }

    if (!found) {
        cout << "Cliente não encontrado" << endl;
    }
}

void list_customers() {
    cout << "=-=-=-=-=-= LISTAR CLIENTES =-=-=-=-=-=" << endl;

    Customer customer;
    fseek(customer_file, 0, SEEK_SET);

    while (fread(&customer, sizeof(customer), 1, customer_file)) {
        if (customer.active == false) continue;

        string customerRole = (customer.role == 'T') ? "Titular" : "Dependente";

        cout << "Id: " << customer.id << endl;
        cout << "Nome: " << customer.name << endl;
        cout << "Idade: " << customer.age << endl;
        cout << "Tipo Cadastro: " << customerRole << endl;

        if (customer.role == 'T') {
            cout << "Qtd Dependentes: " << customer.qtdDependents << endl;
        }
        
        cout << '\n' << endl;
    }

    fseek(customer_file, 0, SEEK_END);
}

FILE* create_customer_file (char *fileName) {
	FILE* file = fopen(fileName, "r+b");

	if (!file) {
		file = fopen(fileName, "wb");

		if (!file) {
			printf("Não foi possível criar o arquivo de clientes!");
			exit(1);
		}

		fclose(file);

		file = fopen(fileName, "r+b");

		if (!file) {
			printf("Não foi possível criar o arquivo de clientes!");
			exit(1);
		}
	}

	printf("Arquivo de clientes criado!\n");

	return file;
}

FILE* create_id_file(char *fileName) {
	FILE* file = fopen(fileName, "r+b");

	if (!file) {
		file = fopen(fileName, "wb");
		int lastId = 0;

		if (!file) {
			printf("Não foi possível criar o arquivo que armazena o código!");
			exit(1);
		}
		
		fwrite(&lastId, sizeof(int), 1, file);
		fclose(file);

		file = fopen(fileName, "r+b");

		if (!file) {
			printf("Não foi possível criar o arquivo que armazena o código!");
			exit(1);
		}
	}
	
    printf("Arquivo do código criado com sucesso!\n");

	return file;
}

void close_file() {
    cout << "Arquivos fechados!" << endl;

    fclose(customer_file);
    fclose(customer_last_id);
}

int exit_program() {
    cout << "Fim do programa!" << endl;
    return 0;
}

int menu() {
    int option;

    do {
        cout << "=-=-=-=-=-= MENU =-=-=-=-=-=" << endl;

        cout << "1 - Cadastrar Cliente" << endl;
        cout << "2 - Excluir Cliente" << endl;
        cout << "3 - Modificar Cliente" << endl;
        cout << "4 - Listar Clientes" << endl;
        cout << "5 - Sair" << endl;

        cout << "Escolha uma opção: ";
        std::cin >> option;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (option) {
            case 1:
                insert_customer();
                break;
            case 2:
                delete_customer();
                break;
            case 3:
                update_customer();
                break;
            case 4:
                list_customers();
                break;
            case 5:
                exit_program();
                close_file();
                break;
        }
    } while (option != 5);

    return 0;
}

int main() {
    char fileCustomer[] = "file_customers.txt";
    char idFile[] = "file_last_id.txt";

    customer_file = create_id_file((char*)idFile);
    customer_last_id = create_customer_file((char*)fileCustomer);
    
    menu();

    return 0;
}