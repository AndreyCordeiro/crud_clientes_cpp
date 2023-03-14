#include <iostream>
#include <string>
#include <list>
#include <limits>

using namespace std;

struct Customer {
    int id;
    int age;
    char name[75];

    Customer *holder;
};

std::list<Customer *> customers;

void insert_customer() {
    cout << "=-=-=-=-=-= CADASTRO DE CLIENTE =-=-=-=-=-=" << endl;

    Customer *customer = new Customer;
    int lastId = 0;
    customer->id = ++lastId; //TODO corrigir a geração de ID

    cout << "Informe o nome: " << endl;
    scanf("%s", customer->name);

    cout << "Informe a idade: " << endl;
    scanf("%i", &customer->age);

    customers.push_back(customer);
}

void delete_customer() {
    cout << "=-=-=-=-=-= EXCLUIR CLIENTE =-=-=-=-=-=" << endl;

    int id;

    cout << "Informe o ID do cliente: " << endl;
    cin >> id;

    auto it = customers.begin();

    while (it != customers.end()) {
        if ((*it)->id == id) {
            customers.erase(it);
            cout << "Cliente excluido com sucesso!" << endl;
            return;
        } else {
            ++it;
        }
        cout << "Cliente não encontrado" << endl;
    }
}

//TODO corrigir a função de atualizar
void update_customer() {
    cout << "=-=-=-=-=-= MODIFICAR CLIENTE =-=-=-=-=-=" << endl;
    int id;
    cout << "Informe o id do cliente que será modificado: ";
    cin >> id;
    bool found = false;

    for (const auto customer: customers) {
        if (customer->id == id) {
            found = true;

            cout << "Informe o nome: " << endl;
            scanf("%s", customer->name);

            cout << "Informe a idade: " << endl;
            scanf("%i", customer->age);

            cout << "Cliente atualizado!" << endl;
        }
    }

    if (!found) {
        cout << "Cliente não encontrado" << endl;
    }
}

void list_customer() {
    cout << "=-=-=-=-=-= LISTAR CLIENTES =-=-=-=-=-=" << endl;

    if (!customers.empty()) {
        for (const auto customer: customers) {
            cout << "Id: " << customer->id << endl;
            cout << "Nome: " << customer->name << endl;
            cout << "Idade: " << customer->age << endl;
            cout << '\n' << endl;
        }
    } else {
        cout << "Nenhum cliente cadastrado." << endl;
    }
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
                break;
        }
    } while (option != 5);

    return 0;
}

int main() {
    menu();

    return 0;
}