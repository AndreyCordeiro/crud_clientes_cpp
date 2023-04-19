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

void insert_customer() {
    cout << "=-=-=-=-=-= CADASTRO DE CLIENTE =-=-=-=-=-=" << endl;

    Customer *customer = new Customer;
    int lastId;
    char isHolder = 'T';
    int holderIdDig;
    bool found = false;

    fseek(customer_last_id, 0, SEEK_SET);
    fread(&lastId, sizeof(int), 1, customer_last_id);

    cout << "Cadastrar Titular ou Dependente? (T/D): " << endl;
    scanf("%c", &isHolder);

    if (isHolder == 'D') {
        cout << "Informe o código do Titular: " << endl;
        scanf("%i", &holderIdDig);

        for (auto it: customers) {
            if (it->id == holderIdDig) {
                if (it->holder != nullptr) {
                    cout << "\nInforme o ID de um cliente que não seja Dependente!\n" << endl;
                    delete customer;
                    return;
                } else {
                    found = true;

                    it->qtdDependents++;
                    customer->holder = it;
                    break;
                }
            }
        }

        if (!found) {
            cout << "Não foi encontrado um Titular com esse código." << endl;
            return;
        }
    } else {
        customer->holder = nullptr;
    }

    customer->id = ++lastId;
    customer->role = isHolder;
	customer ->holderId = holderIdDig;;
    cout << "Informe o nome: " << endl;
    scanf("%s", customer->name);

    cout << "Informe a idade: " << endl;
    scanf("%i", &customer->age);
	
    customers.push_back(customer);
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

    Customer *customer = new Customer;
    fseek(customer_file, 0, SEEK_SET);

    while (fread(&customer, sizeof(customer), 1, customer_file)) {
        string customerRole = (customer->role == 'T') ? "Titular" : "Dependente";

        cout << "Id: " << customer->id << endl;
        cout << "Nome: " << customer->name << endl;
        cout << "Idade: " << customer->age << endl;
        cout << "Tipo Cadastro: " << customerRole << endl;

        if (customer->role == 'T') {
            cout << "Qtd Dependentes: " << customer->qtdDependents << endl;
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
