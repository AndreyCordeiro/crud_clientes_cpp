#include <iostream>
#include <string>
#include <list>
#include <limits>

using namespace std;

class Customer {
public:
    int id = 0;
    int age = 0;
    string name;
};

std::list<Customer> customersList;

void insert_customer() {
    cout << "=-=-=-=-=-= CADASTRO DE CLIENTE =-=-=-=-=-=" << endl;

    Customer customer;
    int aux_id = customer.id + 1;
    customer.id = aux_id;

    cout << "Informe o nome: ";
    cin >> customer.name;

    customersList.push_back(customer);
}

void delete_customer() {
    cout << "=-=-=-=-=-= EXCLUIR CLIENTE =-=-=-=-=-=" << endl;
}


void update_customer() {
    cout << "=-=-=-=-=-= MODIFICAR CLIENTE =-=-=-=-=-=" << endl;
}

void list_customer() {
    cout << "=-=-=-=-=-= LISTAR CLIENTES =-=-=-=-=-=" << endl;
}

int exit_program() {
    cout << "Fim do programa!" << endl;
    return 0;
}

int menu() {
    cout << "=-=-=-=-=-= MENU =-=-=-=-=-=" << endl;

    cout << "1 - Cadastrar Cliente" << endl;
    cout << "2 - Excluir Cliente" << endl;
    cout << "3 - Modificar Cliente" << endl;
    cout << "4 - Listar Clientes" << endl;
    cout << "5 - Sair" << endl;

    int option;

    cout << "Escolha uma opção: ";
    std::cin >> option;

    if (option == 1) {
        insert_customer();
    } else if (option == 2) {
        delete_customer();
    } else if (option == 3) {
        update_customer();
    } else if (option == 4) {
        list_customer();
    } else if (option == 5) {
        exit_program();
    } else {
        cout << "Opção inválida" << endl;
        return menu();
    }

    return 0;
}

int main() {
    menu();
    return 0;
}


