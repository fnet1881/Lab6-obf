#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#define  max_path_size 1024
int size_str = 8;
int len_name = 8;
int c_names = 8;
int counter_renames = 1;
char* types[] = {"void", "char","short","int","long","float","double","size_t","FILE"};
char* bad_names[] = {"main","const", "unsigned", "signed"};

char* get_p_near_type(char* str);
void move_left(char* begin, int c_of_move){
	if (c_of_move <= 0) return;
	int len_to_move = strlen(begin + c_of_move) + 1;
	memmove(begin, begin + c_of_move, len_to_move);
}
void move_right(char* begin, int c_of_move){
	if (c_of_move <= 0) return;
	int len_to_move = strlen(begin) + 1;
	memmove(begin + c_of_move, begin, len_to_move);
}
int count_funcs(char* str){
	int c_funcs = 0;

	while (str){//итерация = начало типа -- поиск функции
		char* potential_func = str;
		while (*str != ' '){
			if (*str == '\0')return c_funcs;
			str++;
		}
		while (*str == ' ')str++;
		while (*str != ' ' && *str != ';' && *str != '('){
			if (*str == '\0')return c_funcs;
			str++;
		}
		while (*str == ' ' || *str == '\n')str++;
		if (*str == '('){//функция
			str++;
			while (*str != ';' && *str != '{') str++;
			if (*str == '{'){//potential_func указывает на начало функции
				c_funcs++;
			}
		}
		str = get_p_near_type(str);
	}
	return c_funcs;
}
char* get_p_near_type(char* str){
	if (!str) return NULL;

	char* p_near = strrchr(str, '}');
	int c_of_types = sizeof(types) / sizeof(*types);

	for (size_t i = 0; i < c_of_types; i++){
		char* p = strstr(str, types[i]);
		int len_type = strlen(types[i]);

		while (p && p < p_near){
			if ((p == str || isspace(*(p - 1)) || *(p - 1) == '(' || *(p - 1) == '[' || *(p - 1) == '{') &&
				(isspace(*(p + len_type)) || *(p + len_type) == '*')){
				p_near = p;
				break;
			}
			else{
				p = strstr(p + len_type, types[i]);
			}
		}
	}

	if (p_near == strrchr(str, '}')) return NULL;
	return p_near;
}
void shake_arr(int* arr, int size){
	for (int i = 0; i < size - 1; i++){
		int j = i + rand() % (size - i);
		int temp = arr[i];
		arr[i] = arr[j];
		arr[j] = temp;
	}
}
void printstr(char* str){
	for (size_t i = 0; i < 1000; i++){
		printf("=");
	}
	printf("\n");
	puts(str);
	for (size_t i = 0; i < 1000; i++){
		printf("=");
	}
	printf("\n");
}
int char_in_str(char chr, char* str){
	for (size_t i = 0; i < strlen(str); i++){
		if (chr == str[i]){
			return 1;
		}
	}
	return 0;
}

void del_comments(char** str){
	char* old_str = *str;
	char* p = old_str;
	char* new_str = malloc((strlen(*str)+1) * sizeof(char));
	if (!new_str){
		printf("Ошибка при выделении памяти для new_str\n");
		return;
	}
	int inx_new_str = 0;
	int in_str = 0;
	while (*old_str != '\0'){
		while (1){
			if (*old_str == '/' || *old_str == '\n'){
				if(*old_str == '\n') new_str[inx_new_str++] = *old_str++;
				if(in_str == 0 || *old_str == '\0') break;
			}
			if (*old_str == '"') in_str = (in_str == 0) ? 1 : 0;
			new_str[inx_new_str++] = *old_str++;
			if (*old_str == '\0') break;
		}
		if (*old_str == '\0') break;
		if (*old_str == '/' && *(old_str + 1) == '/'){//
			while (*old_str != '\0' && *old_str != '\n'){
				if ((*old_str == '\\' && *(old_str + 1) == '\n')){
					break;
				}
				else old_str++;
			}
			if (*old_str == '\n') old_str++;
			if (*old_str == '\\'){
				old_str += 2;//тут новая строка все еще комент //
				while (1){
					while (*old_str != '\0' && *old_str != '\n' && (*old_str != '\\' && *(old_str + 1) != '\n')) old_str++;
					if (*old_str == '\\'){
						old_str += 2;
					}
					else{
						old_str++;
						break;
					}
				}
			}
		}
		else if (*old_str == '/' && *(old_str + 1) == '*'){
			old_str += 2;// /*
			while (*old_str != '*' && *old_str != '\0') old_str++;
			if (*old_str == '\0')break;
			old_str += 3;// */ и \n
		}
		else{
			if (*old_str == '"') in_str = (in_str == 0) ? 1 : 0;
			new_str[inx_new_str++] = *old_str++;
		}
	}
	new_str[inx_new_str] = '\0';
	free(p);
	*str = new_str;
}
void obf_rename(char** str){
	char* old_str = realloc(*str, (strlen(*str) * 12 + 1) * sizeof(char));
	if (!old_str)return;
	char* p = old_str;
	int inx_new_str = 0;

	//запись всех имен в arr_names
	char** arr_names = malloc(c_names * sizeof(char*));
	for (size_t i = 0; i < c_names; i++){
		*(arr_names + i) = malloc(strlen(old_str) * sizeof(char));
	}
	int inx_names = 0;
	old_str = get_p_near_type(old_str);
	while (old_str){//итерация = запись имени
		while (*old_str != ' ') old_str++;
		while (*old_str == ' ') old_str++;
		char* old_name = (char*)malloc(len_name * sizeof(char));
		if (!old_name){
			printf("Ошибка при выделении памяти для name\n");
			return;
		}
		int inx_name = 0;
		while (1){
			if (!isalpha(*old_str) && !isdigit(*old_str) && *old_str != '_') break;
			if (inx_name + 1 == len_name){
				char* p = realloc(old_name, len_name * 2);
				if (!p){
					printf("Ошибка при выделении памяти realloc для p\n");
					free(old_name);
					return;
				}
				old_name = p;
				len_name *= 2;
			}
			old_name[inx_name++] = *old_str++;
		}
		old_name[inx_name++] = '\0';

		//проверка на корректность замены будущей
		int c_of_types = sizeof(types) / sizeof(*types);
		int c_of_bad_names = sizeof(bad_names) / sizeof(*bad_names);
		int is_bad_name = 0;
		
		for (size_t i = 0; i < c_of_types; i++){
			if (strcmp(old_name, types[i]) == 0){
				is_bad_name = 1;
				break;
			}
		}
		for (size_t i = 0; i < c_of_bad_names; i++){
			if (strcmp(old_name, bad_names[i]) == 0){
				is_bad_name = 1;
				break;
			}
		}
		if (is_bad_name){
			free(old_name);
			continue;
		}

		//запись старого имени в массив
		if (inx_names == c_names){
			char** p = realloc(arr_names, c_names * 2 * sizeof(char*));
			if (!p){
				printf("Ошибка при выделении памяти realloc для p\n");
				free(old_name);
				for (size_t i = 0; i < c_names; i++){
					free(arr_names[i]);
				}
				free(arr_names);
				return;
			}
			arr_names = p;
			for (size_t i = c_names; i < c_names * 2; i++){
				arr_names[i] = malloc(strlen(*str) * sizeof(char));
			}
			c_names *= 2;
		}
		for (size_t i = 0; i < strlen(old_name); i++){
			arr_names[inx_names][i] = old_name[i];
		}
		arr_names[inx_names][strlen(old_name)] = '\0';
		inx_names++;
		free(old_name);
		old_str = get_p_near_type(old_str);
	}


	//for (size_t i = 0; i < inx_names; i++){
	//	puts(arr_names[i]);
	//}
	
	

	//rename
	

	//for (size_t i = 0; i < inx_names; i++){
	//	puts(arr_names[i]);
	//}
	for (size_t i = 0; i < inx_names; i++){
		old_str = p;
		// создание нового имени
		char new_name[13];
		if (arr_names[i][0] != 'a'){
			new_name[0] = 'a';
		}
		else{
			new_name[0] = 'b';
		}
		char n_str[13];
		_itoa_s(counter_renames, n_str, 13, 10);
		for (size_t j = 1; j < strlen(n_str) + 1; j++){
			new_name[j] = n_str[j - 1];
		}
		new_name[strlen(n_str) + 1] = '\0';
		counter_renames++;

		//замена имени старого на нового

		char* p_begin_old_name = strstr(old_str, arr_names[i]);
		if (!p_begin_old_name || old_str == p_begin_old_name) continue;
		p_begin_old_name--;
		int len_old_name = strlen(arr_names[i]);
		int len_new_name = strlen(new_name);
		char* p_mov = p;
		while (p_begin_old_name){//итерация = замена старого на нового
			if ((*(p_begin_old_name) == '*' || *(p_begin_old_name) == '+' || *(p_begin_old_name) == '-' || \
				*(p_begin_old_name) == ',' || *(p_begin_old_name) == '(' || *(p_begin_old_name) == '[' || \
				*(p_begin_old_name) == '{' || *(p_begin_old_name) == '?' || *(p_begin_old_name) == ':' ||\
				* (p_begin_old_name) == '!' || *(p_begin_old_name) == '&' || isspace(*(p_begin_old_name))) && \

				(*(p_begin_old_name + len_old_name + 1) == '*' || *(p_begin_old_name + len_old_name + 1) == '+' || \
					* (p_begin_old_name + len_old_name + 1) == '-' || *(p_begin_old_name + len_old_name + 1) == ',' || \
					* (p_begin_old_name + len_old_name + 1) == ')' || *(p_begin_old_name + len_old_name + 1) == ']' || \
					* (p_begin_old_name + len_old_name + 1) == '}' || *(p_begin_old_name + len_old_name + 1) == '?' || \
					* (p_begin_old_name + len_old_name + 1) == '(' || *(p_begin_old_name + len_old_name + 1) == '{' || \
					* (p_begin_old_name + len_old_name + 1) == '[' || \
					* (p_begin_old_name + len_old_name + 1) == ':' || *(p_begin_old_name + len_old_name + 1) == ';' || isspace(*(p_begin_old_name + len_old_name + 1)))){
				//двигаем строку
				p_begin_old_name++;
				if (len_old_name > len_new_name){
					move_left(p_begin_old_name, len_old_name - len_new_name);
				}
				else{
					move_right(p_begin_old_name, len_new_name - len_old_name);
				}
				//замена
				for (size_t i = 0; i < len_new_name; i++){
					*p_begin_old_name++ = new_name[i];
				}
			}
			else{
				old_str = p_begin_old_name;
				old_str += len_old_name + 1;
			}
			
			p_begin_old_name = strstr(old_str, arr_names[i]);
			if (!p_begin_old_name) break;
			p_begin_old_name--;
		}
	}
	
	*str = p;
}
void shake_funcs(char** str){
	char* old_str = *str;
	char* p = old_str;
	old_str = get_p_near_type(old_str);
	int c_funcs = count_funcs(old_str);
	//--------запись функций в массив строк
	char** funcs = malloc(c_funcs * sizeof(char*));
	if (!funcs){
		printf("Ошибка при funcs\n");
		return;
	}
	for (size_t i = 0; i < c_funcs; i++){
		funcs[i] = malloc(strlen(p)*200 * sizeof(char));
		if (!funcs[i]){
			printf("Ошибка при funcs[i]\n");
			return;
		}
	}
	char** begin_funcs = malloc(c_funcs * sizeof(char*));
	if(!begin_funcs){
		printf("Ошибка при begin_funcs\n");
		return;
	}
	char** end_funcs = malloc(c_funcs * sizeof(char*));
	if (!end_funcs){
		printf("Ошибка при end_funcs\n");
		return;
	}
	int inx_fun = 0;
	int inx_fun_char = 0;
	while (old_str){
		char* potential_func = old_str;
		while (*old_str != ' ')old_str++;
		while (*old_str == ' ')old_str++;
		while (*old_str != ' ' && *old_str != ';' && *old_str != '(')old_str++;
		while (*old_str == ' ' || *old_str == '\n')old_str++;
		if (*old_str == '('){
			old_str++;
			while (*old_str != ';' && *old_str != '{') old_str++;
			if (*old_str == '{'){//potential_func указывает на начало функции
				begin_funcs[inx_fun] = potential_func;//запись адресов начала функций

				if (inx_fun >= c_funcs) return;
				int c_opened = 0;//стек для { чтобы определеить конец функции
				while (*potential_func != '{'){
					funcs[inx_fun][inx_fun_char++] = *potential_func++;
				}
				funcs[inx_fun][inx_fun_char++] = *potential_func++;
				c_opened++;
				while (c_opened != 0){
					if (*potential_func == '{'){
						c_opened++;
					}
					else if (*potential_func == '}'){
						c_opened--;
					}
					funcs[inx_fun][inx_fun_char++] = *potential_func++;
				}
				funcs[inx_fun][inx_fun_char] = '\0';
				end_funcs[inx_fun] = potential_func;//запись концов функций
				//puts(funcs[inx_fun]);
				inx_fun++;
				inx_fun_char = 0;
			}
		}
		old_str = get_p_near_type(old_str);
	}
	
	//---------перемешивание

	//arr_inx_new - новые индексы 
	int* arr_inx_new = malloc(c_funcs * sizeof(int));
	if (!arr_inx_new){
		printf("Ошибка при arr_inx_new\n");
		return;
	}
	for (size_t i = 0; i < c_funcs; i++){
		arr_inx_new[i] = i;
	}
	shake_arr(arr_inx_new, c_funcs);
	//for (size_t i = 0; i < c_funcs; i++){
	//	printf("%d ", arr_inx_new[i]);
	//}
	
	//запись в new_str
	old_str = p;
	char* new_str = malloc((strlen(old_str)+1)*200 * sizeof(char));
	int inx_new_str = 0;
	if (!new_str){
		printf("Ошибка при new_str\n");
		return;
	}
	for (size_t n_foo = 0; n_foo < c_funcs; n_foo++){
		//запись информации до функции
		while (old_str != begin_funcs[n_foo]){
			new_str[inx_new_str++] = *old_str++;
		}

		//запись новой вместо старой
		int new_len_foo = end_funcs[arr_inx_new[n_foo]] - begin_funcs[arr_inx_new[n_foo]]; //длина функции на которую меняем
		for (size_t i = 0; i < new_len_foo; i++){
			new_str[inx_new_str++] = *(begin_funcs[arr_inx_new[n_foo]]+i);
		}
		//пропуск старой
		int old_len_foo = end_funcs[n_foo] - begin_funcs[n_foo]; //длина функции которую поменяли
		old_str += old_len_foo;
		//puts(new_str);
	}
	new_str[inx_new_str] = '\0';
	old_str = p;
	free(old_str);
	*str = new_str;
	//puts(old_str);
}
void add_func_gurbage(char** str){
	char* old_str = *str;
	char* p = old_str;

	//------запись адресов начала и конца функций
	int c_funcs = count_funcs(old_str);
	char** begin_funcs = malloc(c_funcs * sizeof(char*));
	if (!begin_funcs){
		printf("Ошибка при begin_funcs\n");
		return;
	}
	char** end_funcs = malloc(c_funcs * sizeof(char*));
	if (!end_funcs){
		printf("Ошибка при end_funcs\n");
		return;
	}
	int inx_fun = 0;
	int inx_fun_char = 0;
	while (old_str){
		char* potential_func = old_str;
		while (*old_str != ' ')old_str++;
		while (*old_str == ' ')old_str++;
		while (*old_str != ' ' && *old_str != ';' && *old_str != '(')old_str++;
		while (*old_str == ' ' || *old_str == '\n')old_str++;
		if (*old_str == '('){
			old_str++;
			while (*old_str != ';' && *old_str != '{') old_str++;
			if (*old_str == '{'){//potential_func указывает на начало функции
				if (inx_fun >= c_funcs) break;
				begin_funcs[inx_fun] = potential_func;//запись адресов начала функций
				//puts(funcs[inx_fun]);
				inx_fun++;
				inx_fun_char = 0;
			}
		}
		old_str = get_p_near_type(old_str);
	}
	old_str = p;


	//----------------запись в newstr
	if (!old_str)return;
	char* new_str = malloc(200 * strlen(old_str) * sizeof(char));
	if (!new_str){
		printf("Ошибка при new_str\n");
		return;
	}
	int inx_new_str = 0;
	//char* data_gurb = "short f12345678900(){\n\tint g12345678900 = 12345678900;\n\tfor (size_t i = 0; i < 12345678900; i++){\n\t\tint b12345678900 = 12345678900;\n\t}\n\tint n12345678900 = 12345678900;\n\treturn 0;\n}\n";
	//puts(data_gurb_func);
	

	//--------------------------запись

	//запись пропусков
	char* p_near_type = get_p_near_type(old_str);
	while (old_str != begin_funcs[0]){
		new_str[inx_new_str++] = *old_str++;
	}
	old_str = begin_funcs[0];
	for (size_t i = 0; i < c_funcs; i++){
		//Запись мусорной
		while (old_str != begin_funcs[i]){
			char* num_types[] = {"int","short","char","double","float"};
			int len_num_types = sizeof(num_types) / sizeof(*num_types);
			while (*old_str != '\n') new_str[inx_new_str++] = *old_str++;
			new_str[inx_new_str++] = *old_str++;
			if (old_str == begin_funcs[0]) break;

			//название мусорной функции
			char new_f_name[13];
			new_f_name[0] = 'f';
			char n_gurb_func_str[13];
			_itoa_s(counter_renames, n_gurb_func_str, 13, 10);
			for (size_t j = 1; j < strlen(n_gurb_func_str) + 1; j++){
				new_f_name[j] = n_gurb_func_str[j - 1];
			}
			new_f_name[strlen(n_gurb_func_str) + 1] = '\0';
			counter_renames++;

			
			//тип функции
			int inx_type_foo = rand() % len_num_types;
			

			//---------------запись функции
			for (size_t i = 0; i < strlen(num_types[inx_type_foo]); i++){//short
				new_str[inx_new_str++] = num_types[inx_type_foo][i];
			}
			new_str[inx_new_str++] = ' ';
			for (size_t i = 0; i < strlen(new_f_name); i++){//f12345678900
				new_str[inx_new_str++] = new_f_name[i];
			}
			char part1[] = "(){\n\t";
			for (size_t i = 0; i < strlen(part1); i++){//(){\n\t
				new_str[inx_new_str++] = part1[i];
			}
			char part2[] = "return 0;\n}\n";
			for (size_t i = 0; i < strlen(part2); i++){
				new_str[inx_new_str++] = part2[i];
			}
			//new_str[inx_new_str++] = '\0';
			//puts(new_str);

		}

		//запись нормальной функции
		int c_opened = 0;//стек для { чтобы определеить конец функции
		while (*old_str != '{'){
			new_str[inx_new_str++] = *old_str++;
		}
		new_str[inx_new_str++] = *old_str++;
		c_opened++;
		while (c_opened != 0){
			if (*old_str == '{'){
				c_opened++;
			}
			else if (*old_str == '}'){
				c_opened--;
			}
			new_str[inx_new_str++] = *old_str++;
		}
		if (*old_str == '\n') old_str++;
		new_str[inx_new_str++] = '\n';
		//puts(new_str);
	}
	if (c_funcs == 0){
		char* num_types[] = {"int","short","char","double","float"};
		int len_num_types = sizeof(num_types) / sizeof(*num_types);
		while (*old_str != '\n' && *old_str != '\0') new_str[inx_new_str++] = *old_str++;
		new_str[inx_new_str++] = *old_str++;

		//название мусорной функции
		char new_f_name[13];
		new_f_name[0] = 'f';
		char n_gurb_func_str[13];
		_itoa_s(counter_renames, n_gurb_func_str, 13, 10);
		for (size_t j = 1; j < strlen(n_gurb_func_str) + 1; j++){
			new_f_name[j] = n_gurb_func_str[j - 1];
		}
		new_f_name[strlen(n_gurb_func_str) + 1] = '\0';
		counter_renames++;


		//тип функции
		int inx_type_foo = rand() % len_num_types;


		//---------------запись функции
		for (size_t i = 0; i < strlen(num_types[inx_type_foo]); i++){//short
			new_str[inx_new_str++] = num_types[inx_type_foo][i];
		}
		new_str[inx_new_str++] = ' ';
		for (size_t i = 0; i < strlen(new_f_name); i++){//f12345678900
			new_str[inx_new_str++] = new_f_name[i];
		}
		char part1[] = "(){\n\t";
		for (size_t i = 0; i < strlen(part1); i++){//(){\n\t
			new_str[inx_new_str++] = part1[i];
		}
		char part2[] = "return 0;\n}\n";
		for (size_t i = 0; i < strlen(part2); i++){
			new_str[inx_new_str++] = part2[i];
		}
	}
	while (*old_str != '\0'){
		new_str[inx_new_str++] = *old_str++;
	}
	new_str[inx_new_str] = '\0';
	//puts(new_str);
	old_str = p;
	free(old_str);
	*str = new_str;

}
void add_cycl_gurbage(char** str){
	char* old_str = *str;
	char* p = old_str;

	//------запись адресов начала и конца функций
	int c_funcs = count_funcs(old_str);
	char** begin_funcs = malloc(c_funcs * sizeof(char*));
	if (!begin_funcs){
		printf("Ошибка при begin_funcs\n");
		return;
	}
	char** end_funcs = malloc(c_funcs * sizeof(char*));
	if (!end_funcs){
		printf("Ошибка при end_funcs\n");
		return;
	}
	int inx_fun = 0;
	int inx_fun_char = 0;
	while (old_str){
		char* potential_func = old_str;
		while (*old_str != ' '){
			if (*old_str == '\0') return;
			old_str++;
		}
		while (*old_str == ' ')old_str++;
		while (*old_str != ' ' && *old_str != ';' && *old_str != '('){
			if (*old_str == '\0') return;
			old_str++;
		}
		while (*old_str == ' ' || *old_str == '\n')old_str++;
		if (*old_str == '('){
			old_str++;
			while (*old_str != ';' && *old_str != '{') old_str++;
			if (*old_str == '{'){//potential_func указывает на начало функции
				begin_funcs[inx_fun] = potential_func;//запись адресов начала функций
				int c_opened = 0;//стек для { чтобы определеить конец функции
				while (*potential_func != '{')potential_func++;
				potential_func++;
				c_opened++;
				while (c_opened != 0){
					if (*potential_func == '{'){
						c_opened++;
					}
					else if (*potential_func == '}'){
						c_opened--;
					}
					potential_func++;
				}
				end_funcs[inx_fun] = potential_func;//запись концов функций
				//puts(funcs[inx_fun]);
				inx_fun++;
				inx_fun_char = 0;
			}
			
		}
		old_str = get_p_near_type(old_str);
	}
	old_str = p;


	//----------------запись в newstr
	if (!old_str)return;
	char* new_str = malloc(200 * strlen(old_str) * sizeof(char));
	if (!new_str){
		printf("Ошибка при new_str\n");
		return;
	}
	int inx_new_str = 0;
	char* data_cycl_gurb = "for (int i = 0; i < rand; i++){\n\n\t}\n";
	//puts(data_cycl_gurb);


	//запись
	for (size_t n_func = 0; n_func < c_funcs; n_func++){
		while (old_str != begin_funcs[n_func]){//запись инфы до функции
			new_str[inx_new_str++] = *old_str++;
		}
		int c_tabs = 0;
		while (old_str != end_funcs[n_func]){
			int if_flag = 0;
			while (*old_str == '\t')old_str++;
			for (int i = 0; i < c_tabs; i++){
				new_str[inx_new_str++] = '\t';
			}
			if (*old_str == 'i' && *(old_str + 1) == 'f'){
				while (*old_str != '}'){
					new_str[inx_new_str++] = *old_str++;
				}
				if_flag = 1;
			}
			else if (*old_str == 'f' && *(old_str + 1) == 'o' && *(old_str + 2) == 'r'){
				while (*old_str != '{'){
					new_str[inx_new_str++] = *old_str++;
				}
				c_tabs++;
			}
			else{
				while (*old_str != ';' && *old_str != '{' && *old_str != '}'){
					new_str[inx_new_str++] = *old_str++;
				}
				if (*old_str == '{') c_tabs++;
				if (*old_str == '}') c_tabs--;
			}
			new_str[inx_new_str++] = *old_str++;
			//puts(new_str);
			while (isspace(*old_str) && *old_str != '\t'){
				new_str[inx_new_str++] = *old_str++;
			}
			if (old_str + 1 == end_funcs[n_func]) break;
			while (*old_str == '\t')old_str++;
			//puts(new_str);
			if (c_tabs == 0) c_tabs++;
			if (if_flag) continue;
			for (int i = 0; i < c_tabs; i++){
				new_str[inx_new_str++] = '\t';
			}
			
			//ген рандомного счетчика
			int counter = rand();
			char buf_counter[12];
			_itoa_s(counter, buf_counter, 12, 10);

			//запись цикла
			char part1[] = "for (int i = 0; i < ";
			char part2[] = "; i++){\n\n";
			char part3[] = "}\n";
			//puts(new_str);
			
			for (size_t i = 0; i < strlen(part1); i++){
				new_str[inx_new_str++] = part1[i];
			}
			for (size_t i = 0; i < strlen(buf_counter); i++){
				new_str[inx_new_str++] = buf_counter[i];
			}
			for (size_t i = 0; i < strlen(part2); i++){
				new_str[inx_new_str++] = part2[i];
			}
			for (int i = 0; i < c_tabs; i++){
				new_str[inx_new_str++] = '\t';
			}
			for (size_t i = 0; i < strlen(part3); i++){
				new_str[inx_new_str++] = part3[i];
			}
			//puts(new_str);
			while (isspace(*old_str) && *old_str != '\t'){
				new_str[inx_new_str++] = *old_str++;
			}
			if (*old_str == '}') c_tabs--;
			//puts(new_str);
		}
		//new_str[inx_new_str] = '\0';
		//puts(new_str);
	}
	if (*old_str == '}'){
		new_str[inx_new_str++] = *old_str;
	}
	new_str[inx_new_str] = '\0';
	//puts(new_str);
	old_str = p;
	free(old_str);
	*str = new_str;


}
void add_vars_gurbage(char** str){
	char* old_str = *str;
	char* p = old_str;
	if (!old_str)return;
	char* new_str = malloc(200 * strlen(old_str) * sizeof(char));
	if (!new_str){
		printf("Ошибка при new_str\n");
		return;
	}
	int inx_new_str = 0;
	char* num_types[] = {"int","short","char","double","float"};
	int len_num_types = sizeof(num_types) / sizeof(*num_types);
	int c_tabs = 0;
	while (1){
		int if_flag = 0;
		if (*old_str == 'i' && *(old_str + 1) == 'f'){
			while (*old_str != '}'){
				new_str[inx_new_str++] = *old_str++;
			}
			if_flag = 1;
		}
		else if (*old_str == 'f' && *(old_str + 1) == 'o' && *(old_str + 2) == 'r'){
			while (*old_str != '{'){
				new_str[inx_new_str++] = *old_str++;
			}
			c_tabs++;
		}
		else if (*old_str == '\0'){
			break;
		}
		else{
			while (*old_str != ';' && *old_str != '{' && *old_str != '}' && *old_str != '\0'){
				new_str[inx_new_str++] = *old_str++;
			}
			if (*old_str == '{') c_tabs++;
			if (*old_str == '}') c_tabs--;
			if (*old_str == '\0'){
				break;
			}
		}
		new_str[inx_new_str++] = *old_str++;
		while (isspace(*old_str) && *old_str != '\t') new_str[inx_new_str++] = *old_str++;
		while (*old_str == '\t') old_str++;
		if (*old_str == '\0') break;
		for (size_t i = 0; i < c_tabs; i++){
			new_str[inx_new_str++] = '\t';
		}
		if (if_flag) continue;
		//-----------тут указываем на начало выражения
		//название мусорной переменной внутри функции
		char new_arg_name[13];
		new_arg_name[0] = 'g';
		char n_gurb_arg_str[13];
		_itoa_s(counter_renames, n_gurb_arg_str, 13, 10);
		for (size_t j = 1; j < strlen(n_gurb_arg_str) + 1; j++){
			new_arg_name[j] = n_gurb_arg_str[j - 1];
		}
		new_arg_name[strlen(n_gurb_arg_str) + 1] = '\0';
		//puts(new_str);
		counter_renames++;
		//тип переменной внутри
		int inx_type_argin = rand() % len_num_types;

		//число переменной для присваивания
		int arg = rand() % CHAR_MAX;
		if (strcmp(num_types[inx_type_argin], "char") == 0){
			arg = rand() % CHAR_MAX;
		}
		else if (strcmp(num_types[inx_type_argin], "short") == 0){
			arg = rand() % SHRT_MAX;
		}
		else if (strcmp(num_types[inx_type_argin], "int") == 0){
			arg = rand() % INT_MAX;
		}
		else if (strcmp(num_types[inx_type_argin], "float") == 0){
			arg = rand() % INT_MAX;
		}
		else if (strcmp(num_types[inx_type_argin], "double") == 0){
			arg = rand() % INT_MAX;
		}

		char gurb_arg_str[13];
		_itoa_s(arg, gurb_arg_str, 13, 10);

		for (size_t i = 0; i < strlen(num_types[inx_type_argin]); i++){
			new_str[inx_new_str++] = num_types[inx_type_argin][i];
		}
		new_str[inx_new_str++] = ' ';
		for (size_t i = 0; i < strlen(new_arg_name); i++){
			new_str[inx_new_str++] = new_arg_name[i];
		}
		
		char part1[] = " = ";
		for (size_t i = 0; i < strlen(part1); i++){
			new_str[inx_new_str++] = part1[i];
		}
		for (size_t i = 0; i < strlen(gurb_arg_str); i++){
			new_str[inx_new_str++] = gurb_arg_str[i];
		}
		new_str[inx_new_str++] = ';';
		//puts(new_str);
		new_str[inx_new_str++] = '\n';
		if (*old_str == '}'){
			for (int i = 0; i < c_tabs - 1; i++){
				new_str[inx_new_str++] = '\t';
			}
		}
		else{
			for (int i = 0; i < c_tabs; i++){
				new_str[inx_new_str++] = '\t';
			}
		}

	}
	new_str[inx_new_str] = '\0';
	old_str = p;
	free(old_str);
	*str = new_str;

}
void del_spaces(char** str){
	char* old_str = *str;
	char* p = old_str;
	if (!old_str)return;
	char* new_str = malloc((strlen(old_str)+1) * sizeof(char));
	char* p_new_str = new_str;
	if (!new_str){
		printf("Ошибка при new_str\n");
		return;
	}
	int inx_new_str = 0;

	char* p_first_type = get_p_near_type(old_str);
	//запись пропущенного
	while (old_str != p_first_type){
		new_str[inx_new_str++] = *old_str++;
	}
	//--------------------Первичная обработка в строку: удаление \t и \n. 
	while (1){
		while (*old_str != '\n' && *old_str != '\0') new_str[inx_new_str++] = *old_str++;
		if (*old_str == '\0') break;
		new_str[inx_new_str++] = ' ';
		while (*old_str == ' ' || *old_str == '\n' || *old_str == '\t') old_str++;
	}
	new_str[inx_new_str] = '\0';
	//puts(new_str);
	//-------------------------Удаление лишних пробелов
	
	char* res_str = malloc((strlen(new_str) + 1) * sizeof(char));
	int inx = 0;
	if (!res_str) return;
	res_str[inx++] = *new_str++;
	char skip_chars[] = ",;+-*^/{}=<> ";
	while (*new_str != '\0'){
		while (char_in_str(*new_str, skip_chars) == 0 && *new_str != '\0') res_str[inx++] = *new_str++;
		if (*new_str == '\0') break;
		if (*new_str == ' ' && char_in_str(*(new_str + 1), skip_chars)) new_str++;
		res_str[inx++] = *new_str++;
		if (*new_str == ' ') new_str++;

	}
	res_str[inx] = '\0';
	//puts(res_str);
	new_str = p_new_str;
	free(new_str);
	*str = res_str;
}
int main(){
	SetConsoleOutputCP(65001);
	srand(time(0));

	printf("Введите путь к файлу .c для обфускации: \n");
	char obf_path[max_path_size];
	fgets(obf_path, max_path_size, stdin);
	if (strchr(obf_path, '\n') != NULL) *(strchr(obf_path, '\n')) = '\0';
	FILE* f_to_obfuscate;
	fopen_s(&f_to_obfuscate, obf_path, "r");
	if (!f_to_obfuscate){
		printf("Ошибка при открытии файла %s\n", obf_path);
		return;
	}
	char* str = (char*)malloc(size_str * sizeof(char));
	if (!str){
		printf("Ошибка при выделении памяти для str\n");
		fclose(f_to_obfuscate);
		return 0;
	}

	
	int inx_str = 0;
	while (1){
		int chr = fgetc(f_to_obfuscate);
		if (chr == EOF) break;
		if (inx_str + 1 == size_str){
			char* p = realloc(str, size_str * 2);
			if (!p){
				printf("Ошибка при выделении памяти realloc для p\n");
				free(str);
				fclose(f_to_obfuscate);
				return;
			}
			str = p;
			size_str *= 2;
		}
		str[inx_str++] = chr;
	}
	str[inx_str] = '\0';
	fclose(f_to_obfuscate);


	//------------------конфиг

	printf("Введите путь к файлу конфигурации: \n");
	char cfg_path[max_path_size];
	fgets(cfg_path, max_path_size, stdin);
	if (strchr(cfg_path, '\n') != NULL) *(strchr(cfg_path, '\n')) = '\0';

	FILE* f_config;
	fopen_s(&f_config, cfg_path, "r");
	if (!f_config){
		printf("Ошибка в %s\n", cfg_path);
		return;
	}
	/*int cmd_del_comments = 0;
	//int cmd_add_gurbage_funcs = 0;
	//int cmd_add_gurbage_cycles = 0;
	//int cmd_add_gurbage_vars = 0;
	//int cmd_shake = 0;
	//int cmd_rename = 0;
	//int cmd_del_spaces = 0;*/
	int configs[7] = {0};
	char buf[25];
	for (size_t i = 0; i < 7; i++){
		fgets(buf, 25, f_config);
		if (strchr(buf, '1') != NULL) 
			configs[i] = 1;
	}
	fclose(f_config);
	//for (size_t i = 0; i < 7; i++){
	//	printf("%d ", configs[i]);
	//}

	if (configs[0]) del_comments(&str);
	if (configs[1]) add_func_gurbage(&str);
	if (configs[2]) add_cycl_gurbage(&str);
	if (configs[3]) add_vars_gurbage(&str);
	if (configs[4]) shake_funcs(&str);
	if (configs[5]) obf_rename(&str);
	if (configs[6]){
		del_comments(&str);
		del_spaces(&str);
	}
	//printstr(str);





	//запись изменений
	fopen_s(&f_to_obfuscate, obf_path, "w");
	if (!f_to_obfuscate){
		printf("Ошибка при открытии файла %s\n", obf_path);
		free(str);
		return;
	}
	for (size_t i = 0; i < strlen(str); i++){
		fputc(str[i], f_to_obfuscate);
	}
	fclose(f_to_obfuscate);
	free(str);
	printf("Проведена обфускация\n\n");
	printf("Для выхода нажмите enter\n");
	int exit = getchar();
	return 0;
}