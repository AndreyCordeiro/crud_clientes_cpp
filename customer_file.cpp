#include <cstdio>
#include <iostream>
#include <cstring>

using namespace std;

struct cliente {
    char nome[50];
    int codigo;
    int idade;
    int titular = 0;
    bool ativo = false;
};

FILE *fp_cliente;
FILE *fp_ultimo_codigo;

bool verificar_se_cliente_possui_dependente(int codigo_cliente, FILE* fp_cliente) {
    cliente cliente;
    fseek(fp_cliente, 0, SEEK_SET);

    while (fread(&cliente, sizeof(cliente), 1, fp_cliente) == 1) {
        if (cliente.ativo && cliente.titular == codigo_cliente) {
            return true;
        }
    }
    return false;
}

bool verificar_se_cliente_e_titular(int codigo_cliente, FILE* fp_cliente) {
    cliente cliente;
    fseek(fp_cliente, 0, SEEK_SET);

    while (fread(&cliente, sizeof(cliente), 1, fp_cliente) == 1) {
        if (cliente.ativo && cliente.titular == codigo_cliente) {
            if (verificar_se_cliente_possui_dependente(cliente.codigo, fp_cliente)) {
                return true;
            }
        }
    }
    return false;
}

void cadastrar_cliente() {
    cout << "=-=-=-=-=-= CADASTRO DE CLIENTE =-=-=-=-=-=" << endl;

    int ultimo_codigo;
    char ehTitular = 'T';
    int codigoTitular;
    cliente cliente, novo_cliente;

    fseek(fp_ultimo_codigo, 0, SEEK_SET);
    fread(&ultimo_codigo, sizeof(int), 1, fp_ultimo_codigo);

    novo_cliente.codigo = ++ultimo_codigo;

    cout << "Titular ou Dependente? (T/D) \n";
    scanf(" %c", &ehTitular);

    if (ehTitular == 'D') {
        bool titular_encontrado = false;
        bool titular_com_dependentes = false;

        do {
            cout << "Informe o código do titular: \n";
            scanf("%i", &codigoTitular);

            fseek(fp_cliente, 0, SEEK_SET);
            while (fread(&cliente, sizeof(cliente), 1, fp_cliente) == 1) {
                if (cliente.codigo == codigoTitular && !verificar_se_cliente_possui_dependente(cliente.codigo, fp_cliente)) {
                    novo_cliente.titular = codigoTitular;
                    titular_encontrado = true;
                    break;
                }
            }

            if(codigoTitular == 0) {
                return;
            }

            if (!titular_encontrado) {
                cout << "Titular não encontrado!\n";
            }
        } while (!titular_encontrado);
    }

    cout << "Digite o nome:\n";
    scanf("%s", novo_cliente.nome);

    cout << "Digite a idade:\n";
    scanf("%i", &novo_cliente.idade);

    novo_cliente.ativo = true;

    fseek(fp_cliente, 0, SEEK_END);
    fwrite(&novo_cliente, sizeof(cliente), 1, fp_cliente);

    fseek(fp_ultimo_codigo, 0, SEEK_SET);
    fwrite(&ultimo_codigo, sizeof(int), 1, fp_ultimo_codigo);
}

void listar_clientes () {
    cout << "=-=-=-=-=-= LISTAR CLIENTES =-=-=-=-=-=" << endl;

	cliente cliente;

	fseek(fp_cliente, 0, SEEK_SET);

	printf("\n");

	while (fread(&cliente, sizeof(cliente), 1, fp_cliente)) {
		if (cliente.ativo == false)
			continue;

		printf("Codigo: %i\n", cliente.codigo);
		printf("Nome: %s\n",  cliente.nome);
		printf("Idade: %i\n",  cliente.idade);
		cout << "Código titular: " << cliente.titular << "\n";
		printf("\n");
	}
	// fseek(fp_cliente, 0, SEEK_END);
}

void alterar_cliente() {
    cout << "=-=-=-=-=-= ALTERAR DE CLIENTE =-=-=-=-=-=" << endl;

	cliente cliente;
	int codigo;
    
	fseek(fp_cliente, 0, SEEK_SET);

	printf("Digite o codigo do Cliente a ser editado: ");
	scanf(" %i", &codigo);
    
	while(fread(&cliente, sizeof(cliente), 1, fp_cliente) == 1) { 
		if(cliente.codigo == codigo) {
			fseek(fp_cliente, -sizeof(cliente), SEEK_CUR);
			
			printf("Digite seu nome:\n");
			scanf("%s", cliente.nome);

			printf("Digite a Idade:\n");
			scanf("%i", &cliente.idade);
		    
			fwrite(&cliente, sizeof(cliente), 1, fp_cliente);
						
			return;
		} 
	}
	printf("Nenhum cliente encontrado...\n");
}

void excluir_cliente() {
    cout << "=-=-=-=-=-= EXCLUIR DE CLIENTE =-=-=-=-=-=" << endl;

    cliente cliente;
    int codigo;

    printf("Digite o codigo do Cliente a ser excluido:\n");
    scanf("%i", &codigo);

    if (verificar_se_cliente_e_titular(codigo, fp_cliente)) {
        printf("O cliente nao pode ser excluido porque é titular de algum cliente com dependentes.\n");
        return;
    }

    fseek(fp_cliente, 0, SEEK_SET);

    while (fread(&cliente, sizeof(cliente), 1, fp_cliente) == 1) {
        if (cliente.codigo == codigo) {
            cliente.ativo = false;

            fseek(fp_cliente, -sizeof(cliente), SEEK_CUR);
            fwrite(&cliente, sizeof(cliente), 1, fp_cliente);

            printf("Cliente excluido com sucesso!\n");
            return;
        }
    }
    printf("Nao foi encontrado nenhum cliente com esse codigo...\n");
}

FILE* abrir_criar_arquivo (char *fname) {
	FILE* fp = fopen(fname, "r+b");

	if (!fp) {
		fp = fopen(fname, "wb");

		if (!fp) {
			printf("Não foi possível criar o arquivo %s\n", fname);
			exit(1);
		}

		fclose(fp);

		fp = fopen(fname, "r+b");

		if (!fp) {
			printf("Não foi possível abrir o arquivo %s\n", fname);
			exit(1);
		}

		printf("Arquivo %s criado com sucesso!\n", fname);
	}

	printf("Arquivo %s aberto\n", fname);

	return fp;
}

FILE* criar_codigo(char *fname) {
	
	FILE* fp = fopen(fname, "r+b");

	if (!fp) {
		fp = fopen(fname, "wb");
		int ultimo_codigo = 0;

		if (!fp) {
			printf("Não foi possível criar o arquivo %s\n", fname);
			exit(1);
		}
		
		fwrite(&ultimo_codigo, sizeof(int), 1, fp);
		fclose(fp);

		fp = fopen(fname, "r+b");

		if (!fp) {
			printf("Não foi possível abrir o arquivo %s\n", fname);
			exit(1);
		}

		printf("Arquivo %s criado com sucesso\n", fname);
	}

	printf("Arquivo %s aberto\n", fname);

	return fp;
}

void menu() {
	int opcao;

	do {
		printf("\n =-=-=-=-=-= MENU =-=-=-=-=-= \n"); 
		printf(" 1 - Cadastrar Cliente \n"); 
		printf(" 2 - Listar todos os Clientes \n");
		printf(" 3 - Alterar Cliente \n");
		printf(" 4 - Excluir Cliente \n");
		printf(" 5 - Sair\n"); 

		printf("Digite uma opção: \n");
		scanf("%i", &opcao);
        
	switch(opcao) {
		case 1:
			cadastrar_cliente();
			break;
		case 2:
			listar_clientes();
			break;
		case  3:
			alterar_cliente();
			break;
		case  4:
			excluir_cliente();
			break;
		case 5:
        printf("Fechando aplicação!\n");
			break;
		}  
	} while (opcao != 5);

}

int main () {
	char fcliente[] = "arquivo-clientes.txt";
	char fultimo_codigo[] = "arquivo-codigo.txt";

	fp_cliente = abrir_criar_arquivo((char*)fcliente);
	fp_ultimo_codigo = criar_codigo((char*)fultimo_codigo);
    
	menu();

	fclose(fp_cliente);
	fclose(fp_ultimo_codigo);

	return 0;
}
