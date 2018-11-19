#include <stdio.h>
#include <mysql.h>
#include <stdlib.h>
#include <string.h>

// Criação do banco
/*
  create database Banco;
  use Banco;
	create table Cliente(idCliente int primary key auto_increment, nome varchar(50) not null, sobrenome varchar(50) not null, telefone varchar(11) not null, saldo float not null);
*/

#define HOST "localhost"
#define USER "root"
#define PASS "december13"
#define DB "Banco"

int menu();
void listarClientes(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas, MYSQL_FIELD *campos);
void insereCliente(MYSQL *conexao);
void saldo(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas);
void saque(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas);
void deposito(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas);
float consultaSaldo(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas, int idCliente);
void retirarCliente(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas);
void lp();


int main(){
	lp();

	MYSQL conexao;
	MYSQL_RES *resp;
	MYSQL_ROW linhas;
	MYSQL_FIELD *campos;
	int oper;
	mysql_init(&conexao);
	if(!mysql_real_connect(&conexao, HOST, USER, PASS, DB, 0, NULL, 0)){
		printf("Erro durante a conexao! %s\n\n", mysql_error(&conexao));
	}
	else{
		do{
			oper=menu();
			while(oper<1 || oper>8){
				system("clear || cls");
				printf("Numero invalido!\n\n");
				oper=menu();
			}

			switch(oper){
				case 1:
					insereCliente(&conexao);
				break;
				case 2:
					saldo(&conexao, resp, linhas);
				break;
				
				case 3:
					saque(&conexao, resp, linhas);
				break;
				case 4:
					deposito(&conexao, resp, linhas);
				break;
				case 5:
					listarClientes(&conexao, resp, linhas, campos);
				break;
				case 6:
					retirarCliente(&conexao, resp, linhas);
				break;
			}
		}while(oper!=8);
		mysql_close(&conexao);
	}
}
int menu(){
	int oper;
	printf("===============Banco==============\n");
	printf("%-30s \t 1\n", "Inserir cliente");
	printf("%-30s \t 2\n", "Consultar saldo do cliente");
	printf("%-30s \t 3\n", "Saque");
	printf("%-30s \t 4\n", "Deposito");
	printf("%-30s \t 5\n", "Listar Clientes");
	printf("%-30s \t 6\n", "Retirar Cliente");
	printf("%-30s \t 7\n", "Sair");
	scanf("%d", &oper);
	getchar();
	return oper;
}
void insereCliente(MYSQL *conexao){
	lp();

	char nome[50], sobrenome[50], telefone[11], query[200];
	float saldo;

	printf("Informe o nome do cliente:");
	scanf("%[^\n]s", nome);
	getchar();
	printf("Informe o sobrenome nome do cliente:");
	scanf("%[^\n]s", sobrenome);
	getchar();
	printf("Informe o telefone do cliente:");
	scanf("%s", telefone);
	printf("Informe o saldo inicial do cliente:");
	scanf("%f", &saldo);
	
	sprintf(query, "INSERT INTO Cliente(nome, sobrenome, telefone, saldo) VALUES('%s', '%s', '%s', %f);", nome, sobrenome, telefone, saldo);		

	if(!mysql_query(conexao, query)){	
		lp();
		printf("Cliente inserido com sucesso!\n\n");
	}
	else{
		lp();
		printf("Erro ao inserir cliente! %s\n\n", mysql_error(conexao));	
	}
}

void saldo(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas){
	lp();

	int idProcura, cont;
	char query[100];
	printf("Informe o id do cliente:");
	scanf("%d", &idProcura);
	getchar();
	lp();
	sprintf(query, "SELECT saldo FROM Cliente WHERE idCliente=%d;", idProcura);
	if(mysql_query(conexao, query)){
		printf("Erro durante a consulta! %s\n\n",mysql_error(conexao));
	}
	else{
		resp=mysql_store_result(conexao);
		if(!resp){
			printf("Erro durante a consulta! %s", mysql_error(conexao));
		}
		else{
			if((linhas=mysql_fetch_row(resp))==0){
				printf("Este id nao existe no banco!\n\n");
			}
			else{
				printf("Saldo do cliente: R$%s", linhas[0]);
			}
			printf("\n\n");
		}
		mysql_free_result(resp);
	}
}
void saque(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas){
	lp();

	float valorSaque, saldo;
	int idCliente, cont;
	char query[100];
	printf("Informe o valor a ser sacado:");
	scanf("%f", &valorSaque);
	getchar();
	printf("Informe o id do cliente:");
	scanf("%d", &idCliente);
	getchar();
	saldo=consultaSaldo(conexao, resp, linhas, idCliente);
	if(saldo==-1){

	}
	else{
		if(valorSaque>saldo){
			lp();
			printf("Valor de saque maior que o saldo do cliente!\n\n");
		}
		else{
			sprintf(query, "UPDATE Cliente SET saldo=%.2f WHERE idCliente=%d", (saldo-valorSaque), idCliente);
			if(mysql_query(conexao, query)){
				lp();
				printf("Erro ao sacar!%s\n\n", mysql_error(conexao));				
			}
			else{
			lp();
				printf("Saque efetuado!\n\n");			
			}
		}
	}
}	
void deposito(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas){
	lp();

	float valorDeposito, saldo;
	int idCliente, cont;
	char query[100];
	printf("Informe o valor a ser depositado:");
	scanf("%f", &valorDeposito);
	getchar();
	printf("Informe o id do cliente:");
	scanf("%d", &idCliente);
	saldo=consultaSaldo(conexao, resp, linhas, idCliente);
	if(saldo==-1){

	}
	else{
		sprintf(query, "UPDATE Cliente SET saldo=%.2f WHERE idCliente=%d;", saldo+valorDeposito, idCliente);
		if(mysql_query(conexao, query)){
			lp();
			printf("Erro ao depositar!%s\n\n", mysql_error(conexao));
		}
		else{
			lp();
			printf("Valor Depositado!\n\n");
		}
	}
}
void listarClientes(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas, MYSQL_FIELD *campos){
	lp();

	char query[100];
	int cont;
	printf("=============Cliente=============\n");
	sprintf(query, "SELECT * FROM Cliente;");
	if(mysql_query(conexao, query)){
		printf("Erro ao consultar banco de dados! %s\n\n", mysql_error(conexao));	
	}
	else{
		resp=mysql_store_result(conexao);
		if(!resp){
			printf("Erro ao consultar banco de dados! %s\n\n", mysql_error(conexao));
		}
		else{
			if((linhas=mysql_fetch_row(resp))==0){
				printf("Banco de Dados vazio!");
			}
			else{
				campos=mysql_fetch_fields(resp);
				for(cont=0; cont<mysql_num_fields(resp); cont++){
					printf("%-15s", (campos[cont]).name);
					if(mysql_num_fields(resp)>1){
						printf("\t");
					}
				}
				printf("\n");
				while((linhas=mysql_fetch_row(resp))!=NULL){
					for(cont=0; cont<mysql_num_fields(resp);cont++){
						printf("%-15s", linhas[cont]);
						if(mysql_num_fields(resp)>1){
							printf("\t");
						}
					}
					printf("\n");
				}
				printf("\n");
			}	
		}
		mysql_free_result(resp);
	}
}
void retirarCliente(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas){
	lp();

	char query[100];
	int cont, idCliente;
	float saldo;

	printf("Informe o id do cliente a ser retirado:");
	scanf("%d", &idCliente);

	saldo=consultaSaldo(conexao, resp, linhas, idCliente);
	if(saldo==-1){

	}
	else if(saldo!=0){
		printf("Conta nao se encontra vazia e nao pode ser apagada!\n\n");
	}
	else{
		sprintf(query, "DELETE FROM Cliente WHERE idCliente=%d;", idCliente);
		if(mysql_query(conexao, query)){
			printf("Nao foi possivel apagar o cliente!");
		}
		else{
			printf("Cliente apagado com sucesso!\n\n");
		}
	}
}
float consultaSaldo(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas, int idCliente){
	lp();

	char query[50];
	
	sprintf(query, "SELECT saldo FROM Cliente WHERE idCliente=%d", idCliente);
	if(mysql_query(conexao, query)){
		printf("Erro ao consultar banco de dados! %s\n\n", mysql_error(conexao));
		return -1;
	}
	else{
		resp=mysql_store_result(conexao);
		if(!resp){
			printf("Erro ao consultar banco de dados! %s\n\n", mysql_error(conexao));
			return -1;
		}
		else{
			if((linhas=mysql_fetch_row(resp))==0){
				printf("Este id nao existe no banco!\n\n");
				return -1;
			}
			else{
				return strtof(linhas[0], NULL);
			}
		}
	}
	mysql_free_result(resp);
}
void lp(){
	system("clear || cls");
}
