#include <iostream>
#include <limits>
#include "customer.cpp"

using namespace std;

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

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (option) {
        case 1:
            Customer::insert_customer();
            break;
        case 2:
            Customer::delete_customer();
            break;
        case 3:
            Customer::update_customer();
            break;
        case 4:
            Customer::list_customer();
            break;
        case 5:
            exit_program();
            break;
        default:
            menu();
    }

    return 0;
}

int main() {
    menu();
    return 0;
}