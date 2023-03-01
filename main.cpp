#include <iostream>
#include <limits>

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
    int retorno=0;

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
            retorno = exit_program();
            break;
        default:
            menu();
    }

    return retorno;
}

int main() {
   int option = menu();
	
    while(option == 0){
		menu();
		option =0;
	}

    return 0;
}