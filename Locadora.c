#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#define TAM 200

/*Criação do banco de dados MYsql:
create database Locadora;
use Locadora;
create table Filme(id int primary key not null auto_increment, titulo varchar(50) not null, sinopse varchar(2000) not null, serie varchar(6) not null, atorPrincipal varchar(50) not null, alugado int(2) not null);
*/

#define HOST "localhost"
#define USER "root"
#define PASS "december13"
#define DB "Locadora"

void inserir(MYSQL *conexao);
void remocao(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas, MYSQL_FIELD *campos);
void devolucao(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas, MYSQL_FIELD *campos);
void listarFilmes(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas, MYSQL_FIELD *campos);
void listarFilmesPorAtor(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas, MYSQL_FIELD *campos);
void aluguel(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas, MYSQL_FIELD *campos);

int menu(){
	int oper;
	printf("\t=========Locadora=========\n");
	printf("%-25s \t 1\n", "Inserir Filme");
	printf("%-25s \t 2\n", "Remocao de Filme");
	printf("%-25s \t 3\n", "Devolucao de Filme");
	printf("%-25s \t 4\n", "Listar Filmes Por Ator");
	printf("%-25s \t 5\n", "Listar Filmes");
	printf("%-25s \t 6\n", "Aluguel");
	printf("%-25s \t 7\n", "Sair");
	scanf("%d", &oper);
	getchar();
	return oper;
}

int main(){
	system("clear || cls");
	int oper;
	MYSQL conexao;
	MYSQL_RES *resp;
	MYSQL_ROW linhas;
	MYSQL_FIELD *campos;

	mysql_init(&conexao);
	if(!mysql_real_connect(&conexao, HOST, USER, PASS, DB, 0, NULL, 0)){
		printf("Erro durante a conexao! %s", mysql_error(&conexao));
	}
	else{
		do{
			oper=menu();
			if(oper<1 || oper>7){
				oper=menu();
			}
			switch(oper){
				case 1:
					inserir(&conexao);
				break;
				case 2:
					remocao(&conexao, resp, linhas, campos);
				break;
				case 3:
					devolucao(&conexao, resp, linhas, campos);
				break;

				case 4:
					listarFilmesPorAtor(&conexao, resp, linhas, campos);
				break;

				case 5:
					listarFilmes(&conexao, resp, linhas, campos);
				break;
				case 6:
					aluguel(&conexao, resp, linhas, campos);
				break;
			}
		}while(oper!=7);
		mysql_close(&conexao);
	}
}
void inserir(MYSQL *conexao){
	system("clear || cls");
		long int res;
		char titulo[50], sinopse[2000], serie[6], atorPrincipal[50], query[2260];
		printf("Informe o Titulo do Filme:");
		scanf("%[^\n]s", titulo);
		getchar();
		printf("Informe a Sinopse do Filme:");
		scanf("%[^\n]s", sinopse);
		getchar();
		printf("Informe a Serie do filme (Ouro, Prata ou Bronze):");
		scanf("%[^\n]s", serie);
		getchar();
		while((strcmp(serie, "Prata")!=0) && (strcmp(serie, "Ouro")!=0) && (strcmp(serie, "Bronze")!=0)){
			printf("Serie invalida!\n");
			printf("Informe a Serie do filme (Ouro, Prata ou Bronze):");
			scanf("%[^\n]s", serie);	
			getchar();
		}

		printf("Informe o ator principal do filme:");
		scanf("%[^\n]s", atorPrincipal);
		getchar();
		sprintf(query, "INSERT INTO Filme(titulo, sinopse, serie, atorPrincipal, alugado) VALUES('%s', '%s', '%s', '%s', 0);", titulo, sinopse, serie, atorPrincipal);
		res=mysql_query(conexao, query);
		if(!res){
			printf("Filme Inserido!\n\n");
		}
		else{
			printf("Erro durante a insercao do filme! %s\n\n", mysql_error(conexao));
		}
}

void remocao(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas, MYSQL_FIELD *campos){
	system("clear || cls");

	char query[100];
	int cont, id;
	printf("Informe o id do filme a ser removido:");
	scanf("%d", &id);

	sprintf(query, "SELECT alugado FROM Filme WHERE id=%d", id);
	if(mysql_query(conexao, query)){
		printf("Erro %d\n", mysql_errno(conexao));	
	}
	else{
		resp=mysql_store_result(conexao);
			if(!resp){
				printf("Erro\n");
			}
			else{
				linhas=mysql_fetch_row(resp);
			}
			if(strcmp(linhas[0], "0")==0){
				sprintf(query, "DELETE FROM Filme WHERE id=%d;", id);
				if(!mysql_query(conexao, query)){
					printf("Livro Retirado!\n");
				}
				else{
					printf("Filme nao encontrado!\n");
				}
			}
			else{
				printf("Filme se encontra alugado e nao pode ser retirado no momento!\n");
			}
		mysql_free_result(resp);
	}
}

void devolucao(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas, MYSQL_FIELD *campos){
	system("clear || cls");

	char query[100];
	int cont, id;
	printf("Informe o id do filme a ser devolvido:");
	scanf("%d", &id);

	sprintf(query, "SELECT alugado FROM Filme WHERE id=%d", id);
	if(mysql_query(conexao, query)){
		printf("Erro %d\n", mysql_errno(conexao));	
	}
	else{
		resp=mysql_store_result(conexao);
			if(!resp){
				printf("Erro\n");
			}
			else{
				linhas=mysql_fetch_row(resp);
			}
			if(strcmp(linhas[0], "1")==0){
				sprintf(query, "UPDATE Filme SET alugado=0 WHERE id=%d;", id);
				mysql_query(conexao, query);
				printf("Filme devolvido com sucesso!\n");
			}
			else{
				printf("Filme nao se encontra alugado!\n");
			}
		mysql_free_result(resp);
	}
}

void listarFilmes(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas, MYSQL_FIELD *campos){
	system("clear || cls");

	int cont;
	if(mysql_query(conexao, "SELECT * FROM Filme;")){
		printf("Erro %d\n", mysql_errno(conexao));	
	}
	else{
		printf("==============Lista De Filmes==============\n");
		resp=mysql_store_result(conexao);
		if(!resp){
			printf("Erro\n");
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
				for(cont=0; cont<mysql_num_fields(resp); cont++){
					printf("%-15s\t", linhas[cont]);
				}
				printf("\n");
			}
			mysql_free_result(resp);
		}
	}
	printf("\n");
}

void listarFilmesPorAtor(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas, MYSQL_FIELD *campos){
	system("clear || cls");
	int cont;
	char atorPesquisa[50], query[100];

	printf("Informe o nome do ator:");
	scanf("%s", atorPesquisa);
	sprintf(query, "SELECT * FROM Filme WHERE atorPrincipal='%s';", atorPesquisa);
	if(mysql_query(conexao, query)){
		printf("Erro %d\n", mysql_errno(conexao));	
	}
	else{
		printf("==============Lista De Filmes Por Ator==============\n");
		resp=mysql_store_result(conexao);
		if(!resp){
			printf("Erro\n");
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
				for(cont=0; cont<mysql_num_fields(resp); cont++){
					printf("%-15s\t", linhas[cont]);
				}
				printf("\n");
			}
			mysql_free_result(resp);
		}
	}
}

void aluguel(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas, MYSQL_FIELD *campos){
	system("clear || cls");

	char query[100];
	int cont, id, alugado;
	printf("Informe o id do filme a ser alugado:");
	scanf("%d", &id);

	sprintf(query, "SELECT alugado FROM Filme WHERE id=%d", id);
	if(mysql_query(conexao, query)){
		printf("Erro %d\n", mysql_errno(conexao));	
	}
	else{
		resp=mysql_store_result(conexao);
			if(!resp){
				printf("Erro\n");
			}
			else{
				linhas=mysql_fetch_row(resp);
			}
			if(strcmp(linhas[0], "0")==0){
				sprintf(query, "UPDATE Filme SET alugado=1 WHERE id=%d;", id);
				mysql_query(conexao, query);
				printf("Filme Alugado com sucesso!\n");
			}
			else{
				printf("Filme se encontra alugado!\n");
			}
		mysql_free_result(resp);
	}
}
