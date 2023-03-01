#include <iostream>
#include <string>
#include <list>

using namespace std;

class Customer {
    int id = 0;
    int age = 0;
    string name;

public:
    static list<Customer> customersList;

    static void insert_customer() {
        cout << "=-=-=-=-=-= CADASTRO DE CLIENTE =-=-=-=-=-=" << endl;

        Customer customer;
        int aux_id = customer.id + 1;
        string customer_age;

        customer.id = aux_id;

        cout << "Informe o nome: ";
        getline(cin, customer.name);

        cout << "Informe a idade: ";
        getline(cin, customer_age);
        customer.age = stoi(customer_age);

        customersList.push_back(customer);
    }

    static void delete_customer() {
        cout << "=-=-=-=-=-= EXCLUIR CLIENTE =-=-=-=-=-=" << endl;

        int id;

        cout << "Informe o ID do cliente: ";
        cin >> id;

        auto it = customersList.begin();

        while (it != customersList.end())
        {

            if (it->id == id)
            {

                it = customersList.erase(it);
                cout << "Cliente excluido com sucesso!" << endl;
                return;
            }

            else
            {

                ++it;
            }
            cout << "Cliente não encontrado" << endl;
        }
    }

    static void update_customer() {
        cout << "=-=-=-=-=-= MODIFICAR CLIENTE =-=-=-=-=-=" << endl;
    }

    static void list_customer() {
        cout << "=-=-=-=-=-= LISTAR CLIENTES =-=-=-=-=-=" << endl;
    }
};