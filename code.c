#include <stdio.h> 
#include <stdlib.h> 
#include <windows.h>

int glob1 = 1;
int glob2;

int check(char*, char*);

void XOR(char*, const int);

void XOR(char* str, const int key){
	for (size_t i = 0; i < strlen(str); i++){
		str[i] = str[i] ^ key;

	}
}
int after_XOR;
int check(char* str1, char* str2){
	if (strcmp(str1, str2) == 0){
		return 1;
	}
	return 0;
}
int after_CHECK;
int main(){
	SetConsoleOutputCP(65001);
	//asdasadaada
	/*asdadasd
	aslala
	*/

	//asda\
	asdadsa\
	asdadasa
	int max_size_path = 1000;
	int max_size_pass = 1000;
	char* pass_name = malloc(max_size_path * sizeof(char));
	if (pass_name == NULL){
		printf("Ошибка при выделении памяти для pass_name\n");
		return;
	}

	printf("Введите имя/путь файла с паролем:\n");
	fgets(pass_name, max_size_path, stdin);
	if (strchr(pass_name, '\n') != NULL) *strchr(pass_name, '\n') = '\0';
	FILE* f_pass;
	fopen_s(&f_pass, pass_name, "r");
	if (f_pass == NULL){
		printf("Ошибка при открытии файла: %s\n", pass_name);
		free(pass_name);
		return;
	}

	char* correct_pass = malloc(max_size_pass * sizeof(char));
	if (correct_pass == NULL){
		printf("Ошибка при выделении памяти для correct_pass\n");
		free(pass_name);
		fclose(f_pass);
		return;
	}
	fgets(correct_pass, max_size_pass, f_pass);
	int key = 1;
	XOR(correct_pass, key);
	fclose(f_pass);
	fopen_s(&f_pass, pass_name, "w");
	if (f_pass == NULL){
		printf("Ошибка при открытии файла: %s\n", pass_name);
		free(correct_pass);
		free(pass_name);
		return;
	}
	fputs(correct_pass, f_pass);
	fclose(f_pass);


	char* pass = malloc(max_size_pass * sizeof(char));
	if (pass == NULL){
		printf("Ошибка при выделении памяти для pass\n");
		free(pass_name);
		return;
	}
	printf("Введите пароль:\n");
	fgets(pass, max_size_pass, stdin);
	if (strchr(pass, '\n') != NULL) *strchr(pass, '\n') = '\0';
	XOR(pass, key);


	if (check(pass, correct_pass)){
		printf("Пароль верен.\n");
	}
	else{
		printf("Пароль неверен.\n");
	}
	printf("Для выхода нажмите любую кнопку\n");
	int chr = getchar();


	free(pass);
	free(correct_pass);
	free(pass_name);
	return 0;
}