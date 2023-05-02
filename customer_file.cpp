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
};

FILE *customer_file;
FILE *customer_last_id;

std::list<Customer *> customers;

bool check_customer_with_dependent(int customerId, FILE* customer_file) {
    Customer customer;

    fseek(customer_file, 0, SEEK_SET);

    while (fread(&customer, sizeof(Customer), 1, customer_file) == 1) {
        if (customer.holderId == customerId) {
            return true;
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
    bool found = false;

    fseek(customer_last_id, 0, SEEK_SET);
    fread(&lastId, sizeof(int), 1, customer_last_id);

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
                cout << "Código de titular não encontrado!";
            }
        } while (!holderFound);

        if (!found) {
            cout << "Não foi encontrado um Titular com esse código." << endl;
            return;
        }
    } else {
        new_customer.holder = nullptr;
    }

    new_customer.id = ++lastId;
    new_customer.role = isHolder;
	new_customer.holderId = holderIdDig;

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

    if (customers.empty()) {
        cout << "\nNão há clientes cadastrados, cadastre um cliente antes de executar essa ação!\n" << endl;
        return;
    }

    int id;

    cout << "Informe o ID do cliente: " << endl;
    cin >> id;

    auto it = customers.begin();

    while (it != customers.end()) {
        if ((*it)->id == id) {
            if ((*it)->qtdDependents > 0) {
                cout << "Não é possível excluir este cliente pois ele possui " << (*it)->qtdDependents << " dependentes!" << endl;
                return;
            }
			//Arrumar para deduzir o dependente quando excluido
			for (const auto customer: customers){
				if((*it)-> holderId == customer -> id){
				customer -> qtdDependents--;
				}
			}

            customers.erase(it);
            cout << "Cliente excluido com sucesso!" << endl;
            
            return;
        } else {
            ++it;
        }
    }
    cout << "Cliente não encontrado" << endl;
}

void update_customer() {
    cout << "=-=-=-=-=-= MODIFICAR CLIENTE =-=-=-=-=-=" << endl;

    if (customers.empty()) {
        cout << "\nNão há clientes cadastrados, cadastre um cliente antes de executar essa ação!\n" << endl;
        return;
    }

    int id;

    cout << "Informe o id do cliente que será modificado: ";
    cin >> id;
    bool found = false;

    for (auto customer: customers) {
        if (customer->id == id) {
            found = true;

            cout << "Informe o nome: " << endl;
            scanf("%s", customer->name);

            cout << "Informe a idade: " << endl;
            scanf("%i", &customer->age);

            cout << "Cliente atualizado!" << endl;
        }
    }

    if (!found) {
        cout << "Cliente não encontrado" << endl;
    }
}

void list_customer() {
    cout << "=-=-=-=-=-= LISTAR CLIENTES =-=-=-=-=-=" << endl;

    Customer customer;
    fseek(customer_file, 0, SEEK_SET);

    while (fread(&customer, sizeof(customer), 1, customer_file)) {
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

void open_file_customer() {
    customer_file = fopen("customer_file.txt", "r+b");

    if (customer_file == NULL) {
        cout << "Erro ao abrir o arquivo 'customer_file.txt!'" << endl;
        return;
    } else {
        cout << "Arquivo 'customer_file.txt' aberto!" << endl;
    }
}

void open_file_last_id() {
    customer_last_id = fopen("customer_last_id.txt", "r+b");

    if (customer_last_id == NULL) {
        cout << "Erro ao abrir o arquivo 'customer_last_id.txt!'" << endl;
        return;
    } else {
        cout << "Arquivo 'customer_last_id.txt' aberto!" << endl;
    }
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
                list_customer();
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
    open_file_customer();
    open_file_last_id();
    
    menu();

    return 0;
}