#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <mysql.h>
#include <ctype.h>

#define HOST "localhost"
#define USER "root"
#define PASS "december13"
#define DB "biblioteca"

#define TAMSENHA 15
#define TAMNOMES 51
#define TAMQUERY 500
#define TAMDATA 12
#define TAMMATRICULA 13
#define TAMTELEFONE 12
#define TAMISBN 14

void lp();
int login(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas);
void insereLivro(MYSQL *conexao);
void retirarLivro(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas);
void insereAluno(MYSQL *conexao);
void retirarAluno(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas);
void listarLivros(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas, MYSQL_FIELD *campos);
void alugarLivro(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas);
void devolverLivro(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas);
int menuAdministrador();
int menuAluno();

int main(){
	lp();
	MYSQL conexao;
	MYSQL_RES *resp;
	MYSQL_ROW linhas;
	MYSQL_FIELD *campos;
	int oper, userType;

	mysql_init(&conexao);
	if(!mysql_real_connect(&conexao, HOST, USER, PASS, DB, 0, NULL, 0)){
		printf("Erro ao conectar ao banco de dados! %s", mysql_error(&conexao));
	}
	else{
		do{
			userType=login(&conexao, resp, linhas);
			if(userType==1){
				do{
					oper=menuAdministrador();
					switch(oper){
						case 1:
							insereLivro(&conexao);
						break;
						case 2:
							retirarLivro(&conexao, resp, linhas);
						break;
						case 3:
							insereAluno(&conexao);
						break;
						case 4:
							retirarAluno(&conexao, resp, linhas);
						break;
						case 5:
							listarLivros(&conexao, resp, linhas, campos);
						break;
					}	
				}while(oper!=-2);
			}
			else if(userType==2){
				do{
					oper=menuAluno();	
					switch(oper){
						case 1:
							alugarLivro(&conexao, resp, linhas);
						break;
						case 2:
							devolverLivro(&conexao, resp, linhas);
						break;
						case 3:
							listarLivros(&conexao, resp, linhas, campos);
						break;
					}
				}while(oper!=-2);
			}
			else{
				printf("Acesso Negado!\n");
				continue;
			}
			
		}while(oper!=-1);	
	}
	
}
int login(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas){
	int oper, idUser;
	char query[TAMQUERY], senha[TAMSENHA];
	printf("%-10s \t 1\n", "Atendente");
	printf("%-10s \t 2\n", "Aluno");
	scanf("%d", &oper);
	getchar();
	while(oper!=1 && oper!=2){
		lp();
		printf("Invalido!\n");
		oper=login(conexao, resp, linhas);
	}
	printf("Informe seu id/matricula:");
	scanf("%d", &idUser);
	getchar();
	printf("Informe sua senha:");
	scanf("%[^\n]s", senha);
	getchar();
	lp();
	while(strlen(senha)>TAMSENHA-1){
		lp();
		printf("Tamanho excede o máximo aceitavel! %d Caracteres\n", TAMSENHA-1);
		printf("Informe sua senha:");
		scanf("%[^\n]s", senha);
		getchar();
	}

	if(oper==1){	
		sprintf(query, "SELECT senha FROM Atendente WHERE idAtendente=%d", idUser);	
		if(mysql_query(conexao, query)){
			printf("Erro ao conectar ao banco de dados! %s", mysql_error(conexao));
		}
		else{
			resp=mysql_store_result(conexao);
			if(!resp){
				printf("Erro ao conectar a tabela de Atendente! %s", mysql_error(conexao));
			}
			if((linhas=mysql_fetch_row(resp))==0){
				lp();
				printf("Id fornecido nao esta cadastrado!");
			}
			else{
				if((strcmp(senha, linhas[0]))==0){
					return 1;
				}
				else{
					printf("Senha incorreta!\n");
					return 0;
				}
			}
		}
	}
	else{
		sprintf(query, "SELECT senha FROM Aluno WHERE idAluno=%d", idUser);	
		if(mysql_query(conexao, query)){
			printf("Erro ao conectar ao banco de dados! %s", mysql_error(conexao));
		}
		else{
			resp=mysql_store_result(conexao);
			if(!resp){
				printf("Erro ao conectar a tabela de Aluno! %s", mysql_error(conexao));
			}
			else{
				if((linhas=mysql_fetch_row(resp))==0){
					lp();
					printf("Id fornecido nao esta cadastrado!");
				}
				else{
					if((strcmp(senha, linhas[0]))==0){
						return 2;
					}
					else{
						printf("Senha incorreta!\n");
						return 0;
					}
				}
			}
		}
	}
	mysql_free_result(resp);
}

void insereLivro(MYSQL *conexao){
	lp();
	char titulo[TAMNOMES], autor[TAMNOMES], dataPub[TAMMATRICULA], query[TAMQUERY], ISBN[TAMISBN];
	printf("Informe o titulo do livro:");
	scanf("%[^\n]s", titulo);
	getchar();
	while(strlen(titulo)>TAMNOMES-1){
		lp();
		printf("Tamanho excede o máximo aceitavel! %d Caracteres\n", TAMNOMES-1);
		printf("Informe o titulo do livro:");
		scanf("%[^\n]s", titulo);
		getchar();
	}
	printf("Informe o autor do livro:");
	scanf("%[^\n]s", autor);
	getchar();
	while(strlen(autor)>TAMNOMES-1){
		lp();
		printf("Tamanho excede o máximo aceitavel! %d Caracteres\n", TAMNOMES-1);
		printf("Informe o autor do livro:");
		scanf("%[^\n]s", autor);
		getchar();
	}
	printf("Informe o a data de publicacao do livro[yyyy/mm/dd]:");
	scanf("%[^\n]s", dataPub);
	getchar();
	while(strlen(dataPub)>TAMDATA-1){
		lp();
		printf("Tamanho excede o máximo aceitavel! %d Caracteres\n", TAMNOMES-1);
		printf("Informe o a data de publicacao do livro[yyyy/mm/dd]:");
		scanf("%[^\n]s", dataPub);
		getchar();
	}
	printf("Informe o ISBN do livro:");
	scanf("%[^\n]s", ISBN);
	getchar();
	while(strlen(ISBN)>TAMISBN-1){
		lp();
		printf("Tamanho excede o máximo aceitavel! %d Caracteres\n", TAMISBN-1);
		printf("Informe o ISBN do livro:");
		scanf("%[^\n]s", ISBN);
		getchar();
	}

	sprintf(query, "INSERT INTO Livro(titulo, autor, dataPub, emprestado, ISBN) VALUES('%s', '%s', '%s', 0, '%s');", titulo, autor, dataPub, ISBN);
	if(mysql_query(conexao, query)){
		printf("Erro ao inserir o livro! %s", mysql_error(conexao));
	}
	else{
		printf("Livro inserido com sucesso!\n\n");
	}
}
void retirarLivro(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas){
	char query[TAMQUERY], ISBN[TAMISBN], teste[4];
	int cont;
	printf("Informe o ISBN do livro a ser retirado:");
	scanf("%s", ISBN);

	sprintf(query, "SELECT emprestado FROM Livro WHERE ISBN=%s;", ISBN);
	if(mysql_query(conexao, query)){
		lp();
		printf("Erro ao conectar a tabela de Livros! %s", mysql_error(conexao));
	}
	else{
		resp=mysql_store_result(conexao);
		if(!resp){
			lp();
			printf("Erro ao conectar a tabela de Livros! %s", mysql_error(conexao));
		}
		else{
			if((linhas=mysql_fetch_row(resp))==0){
				lp();
				printf("Este id nao esta relacionado a nenhum livro da biblioteca\n\n");
			}
			else{
				if(strcmp(linhas[0], "0")==0){
					printf("Tem certeza que quer retirar o livro?[SIM/NAO]");
					scanf("%s", teste);
					getchar();
					for(cont=0; cont<strlen(teste); cont++){
						teste[cont]=tolower(teste[cont]);
					}
					while(strcmp(teste, "sim")!=0 && strcmp(teste, "nao")!=0){
						lp();
						printf("Resposta Invalida!\n");
						printf("Tem certeza que quer retirar o livro?[SIM/NAO]:");
						scanf("%s", teste);
						getchar();
						for(cont=0; cont<strlen(teste); cont++){
							teste[cont]=tolower(teste[cont]);
						}
					}
					if(strcmp(teste, "sim")==0){
						sprintf(query, "DELETE FROM Livro WHERE ISBN=%s", ISBN);
						if(mysql_query(conexao, query)){
							lp();
							printf("Nao foi possivel retirar o livro! %s\n\n!", mysql_error(conexao));
						}
						else{
							lp();
							printf("Livro rerirado com sucesso!\n\n");
						}
					}
					else{
						lp();
						printf("Operacao cancelada!\n\n");
					}
				}
				else{
					lp();
					printf("Nao foi possivel retirar o livro! Este se encontra emprestado!\n\n");
				}
			}
		}
	}
	mysql_free_result(resp);
}
void insereAluno(MYSQL *conexao){
	lp();
	printf("===========Insercao de Aluno===========\n");
	char query[TAMQUERY], nome[TAMNOMES], telefone[TAMTELEFONE], matricula[TAMMATRICULA], senha[TAMSENHA], dataNascimento[TAMDATA];
	printf("Informe o nome do aluno:");
	scanf("%[^\n]s", nome);
	getchar();
	while(strlen(nome)>TAMNOMES-1){
		lp();
		printf("Tamanho excede o máximo aceitavel! %d Caracteres\n", TAMNOMES-1);
		printf("Informe o nome do aluno:");
		scanf("%[^\n]s", nome);
		getchar();
	}
	printf("Informe a matricula do aluno:");
	scanf("%[^\n]s", matricula);
	getchar();
	while(strlen(matricula)>TAMMATRICULA-1){
		lp();
		printf("Tamanho excede o máximo aceitavel! %d Caracteres\n", TAMMATRICULA-1);
		printf("Informe a matricula do aluno:");
		scanf("%[^\n]s", matricula);
		getchar();
	}
	printf("Informe a senha para aluno:");
	scanf("%[^\n]s", senha);
	getchar();
	while(strlen(senha)>TAMSENHA-1){
		lp();
		printf("Tamanho excede o máximo aceitavel! %d Caracteres\n", TAMSENHA-1);
		printf("Informe a senha para o aluno:");
		scanf("%[^\n]s", senha);
		getchar();
	}	
	printf("Informe o telefone do aluno:");
	scanf("%[^\n]s", telefone);
	getchar();
	while(strlen(telefone)>TAMTELEFONE-1){
		lp();
		printf("Tamanho excede o máximo aceitavel! %d Caracteres\n", TAMTELEFONE-1);
		printf("Informe a matricula do aluno:");
		scanf("%[^\n]s", telefone);
		getchar();
	}
	printf("Informe a data de nascimento do aluno:");
	scanf("%[^\n]s", dataNascimento);
	getchar();
	while(strlen(dataNascimento)>TAMDATA-1){
		lp();
		printf("Tamanho excede o máximo aceitavel! %d Caracteres\n", TAMDATA-1);
		printf("Informe a matricula do aluno:");
		scanf("%[^\n]s", dataNascimento);
		getchar();
	}
	sprintf(query, "INSERT INTO Aluno(nome, senha, telefone, dataNascimento, numLivros, matricula) VALUES('%s', '%s', '%s', '%s', 0, '%s');", nome, senha, telefone, dataNascimento, matricula);
	if(mysql_query(conexao, query)){
		lp();
		printf("Erro ao inserir o aluno!\n");
		printf("%s\n", mysql_error(conexao));
		printf("Erro numero %d\n", mysql_errno(conexao));
	}
	else{
		printf("Aluno inserido com sucesso!\n");
	}
}	
void retirarAluno(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas){
	char query[TAMQUERY], matricula[TAMMATRICULA], teste[4];
	int cont;
	printf("Informe a matricula do aluno a ser retirado:");
	scanf("%s", matricula);
	while(strlen(matricula)>TAMMATRICULA-1){
		lp();
		printf("Tamanho excede o máximo aceitavel! %d Caracteres\n", TAMDATA-1);
		printf("Informe a matricula do aluno:");
		scanf("%[^\n]s", matricula);
		getchar();
	}
	sprintf(query, "SELECT numLivros FROM Aluno WHERE matricula=%s;", matricula);
	if(mysql_query(conexao, query)){
		lp();
		printf("Erro ao conectar a tabela de Aluno! %s", mysql_error(conexao));
	}
	else{
		resp=mysql_store_result(conexao);
		if(!resp){
			lp();
			printf("Erro ao conectar a tabela de Aluno! %s", mysql_error(conexao));
		}
		else{
			if((linhas=mysql_fetch_row(resp))==0){
				lp();
				printf("Esta matricula nao esta relacionada a nenhum aluno cadastrado\n\n");
			}
			else{
				if(strcmp(linhas[0], "0")==0){
					printf("Tem certeza que quer retirar o aluno?[SIM/NAO]");
					scanf("%s", teste);
					getchar();
					for(cont=0; cont<strlen(teste); cont++){
						teste[cont]=tolower(teste[cont]);
					}
					while(strcmp(teste, "sim")!=0 && strcmp(teste, "nao")!=0){
						lp();
						printf("Resposta Invalida!\n");
						printf("Tem certeza que quer retirar o aluno?[SIM/NAO]:");
						scanf("%s", teste);
						getchar();
						for(cont=0; cont<strlen(teste); cont++){
							teste[cont]=tolower(teste[cont]);
						}
					}
					if(strcmp(teste, "sim")==0){
						sprintf(query, "DELETE FROM Aluno WHERE matricula=%s;", matricula);
						if(mysql_query(conexao, query)){
							lp();
							printf("Nao foi possivel retirar o aluno! %s\n\n!", mysql_error(conexao));
						}
						else{
							lp();
							printf("Aluno rerirado com sucesso!\n\n");
						}
					}
					else{
						lp();
						printf("Operacao cancelada!\n\n");
					}
				}
				else{
					lp();
					printf("Nao foi possivel retirar o aluno! Este possui livros emprestado!\n\n");
				}
			}
		}
	}
	mysql_free_result(resp);

}
void listarLivros(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas, MYSQL_FIELD *campos){
	lp();
	char query[TAMQUERY];
	int cont;
	printf("=============Livros=============\n");
	sprintf(query, "SELECT * FROM Livro;");
	if(mysql_query(conexao, query)){
		printf("Erro ao consultar banco de dados! %s\n\n", mysql_error(conexao));	
	}
	else{
		resp=mysql_store_result(conexao);
		if(!resp){
			printf("Erro ao consultar banco de dados! %s\n\n", mysql_error(conexao));
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
		mysql_free_result(resp);
	}
}
void alugarLivro(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas){
	char dia[3], ano[5], mes[4], data[12];
	int cont;
	strcpy(data, __DATE__);
	printf("%s\n", data);
    
    mes[0]=data[0];
    mes[1]=data[1];
    mes[2]=data[2];
    mes[3]='\0';

    dia[0]=data[4];
    dia[1]=data[5];
    dia[2]='\0';

    ano[0]=data[7];
    ano[1]=data[8];
    ano[2]=data[9];
    ano[3]=data[10];
    ano[4]='\0';

    

    printf("%s %s %s\n", dia, mes, ano);
}
void devolverLivro(MYSQL *conexao, MYSQL_RES *resp, MYSQL_ROW linhas){

	printf("%s\n", __DATE__);

}
int menuAdministrador(){
	int oper;
	printf("============Menu Atendente============\n");
	printf("%-25s \t 1\n", "Inserir Livro");
	printf("%-25s \t 2\n", "Retirar Livro");
	printf("%-25s \t 3\n", "Inserir Aluno");
	printf("%-25s \t 4\n", "Retirar Aluno");
	printf("%-25s \t 5\n", "Listar Livros");
	printf("%-25s \t -1\n", "Sair");
	printf("%-25s \t -2\n", "Voltar ao menu de usuario");
	scanf("%d", &oper);
	getchar();
	return oper;
}	
int menuAluno(){
	int oper;
	printf("============Menu Aluno============\n");
	printf("%-25s \t 1\n", "ALugar Livro");
	printf("%-25s \t 2\n", "Devolver Livro");
	printf("%-25s \t 3\n", "Listar Livros");
	printf("%-25s \t -1\n", "Sair");
	printf("%-25s \t -2\n", "Voltar ao menu de usuario");
	scanf("%d", &oper);
	getchar();
	return oper;
}
void lp(){
	system("clear || cls");
}
