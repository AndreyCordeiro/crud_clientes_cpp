#include <iostream>
#include <string>
#include <list>

using namespace std;

class Customer {
    int id = 0;
    int age = 0;
    string name;
    list<Customer> customersList;

public:
    void insert_customer() {
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

    void delete_customer() {
        cout << "=-=-=-=-=-= EXCLUIR CLIENTE =-=-=-=-=-=" << endl;

        int id;

        cout << "Informe o ID do cliente: ";
        cin >> id;

        auto it = customersList.begin();

        while (it != customersList.end()) {
            if (it->id == id) {
                customersList.erase(it);
                cout << "Cliente excluido com sucesso!" << endl;
                return;
            } else {
                ++it;
            }
            cout << "Cliente não encontrado" << endl;
        }
    }

    void update_customer() {
        cout << "=-=-=-=-=-= MODIFICAR CLIENTE =-=-=-=-=-=" << endl;
        int id;
        cout << "Informe o id do cliente que será modificado: ";
        cin >> id;
        bool found = false;

        for (auto &customer: customersList) {
            if (customer.id == id) {
                found = true;
                string customer_age;

                cout << "Informe o nome: " << endl;
                getline(cin, customer.name);

                cout << "Informe a idade: " << endl;
                getline(cin, customer_age);
                customer.age = stoi(customer_age);

                cout << "Cliente atualizado!" << endl;
            }
        }

        if (!found) {
            cout << "Cliente não encontrado" << endl;
        }
    }

    void list_customer() {
        cout << "=-=-=-=-=-= LISTAR CLIENTES =-=-=-=-=-=" << endl;

        if (!customersList.empty()) {
            for (const Customer &customer: customersList) {
                cout << "Id: " << customer.id << endl;
                cout << "Nome: " << customer.name << endl;
                cout << "Idade: " << customer.age << endl;
                cout << '\n' << endl;
            }
        } else {
            cout << "Nenhum cliente cadastrado." << endl;
        }
    }
};