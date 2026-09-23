#include <stdio.h> 
#include <stdlib.h> 
#include <windows.h>

int a114 = 1;
float a115 = 23259;
int a116;

float a117 = 18500;
int a118(char*, char*);

double a119 = 12735;
void a120(char*, const int);

int a121 = 16193;
double a122(){
	short a123 = 17375;
	for (int a124 = 0; a124 < 16451; a124++){

		int a125 = 5038;
	}
	double a126 = 26074;
	return 0;
	float a127 = 12799;
}
int a128 = 22527;
int b129;
char a130 = 108;
short a131(){
	int a132 = 8015;
	for (int a124 = 0; a124 < 18509; a124++){

		short a134 = 11445;
	}
	char a135 = 14;
	return 0;
	float a136 = 1630;
}
short a137 = 21360;
int main(){
	float a138 = 9076;
	for (int a124 = 0; a124 < 7138; a124++){

		double a140 = 29666;
	}
	int a141 = 14329;
	SetConsoleOutputCP(65001);
	float a142 = 18717;
	for (int a124 = 0; a124 < 31828; a124++){

		char a144 = 27;
	}

	double a145 = 27983;
	
	int a146 = 1000;
	float a147 = 17202;
	for (int a124 = 0; a124 < 3963; a124++){

		int a149 = 2093;
	}
	double a150 = 10395;
	int a151 = 1000;
	float a152 = 14701;
	for (int a124 = 0; a124 < 19775; a124++){

		double a154 = 1119;
	}
	char a155 = 124;
	char* a156 = malloc(a146 * sizeof(char));
	float a157 = 30490;
	for (int a124 = 0; a124 < 12348; a124++){

		double a159 = 13580;
	}
	char a160 = 77;
	if (a156 == NULL){
		printf("Ошибка при выделении памяти для pass_name\n");
		return;
	}

	printf("Введите имя/путь файла с паролем:\n");
	int a161 = 18272;
	for (int a124 = 0; a124 < 27803; a124++){

		char a163 = 11;
	}
	int a164 = 30042;
	fgets(a156, a146, stdin);
	char a165 = 80;
	for (int a124 = 0; a124 < 6380; a124++){

		double a167 = 9654;
	}
	char a168 = 109;
	if (strchr(a156, '\n') != NULL) *strchr(a156, '\n') = '\0';
	FILE* a169;
	fopen_s(&a169, a156, "r");
	if (a169 == NULL){
		printf("Ошибка при открытии файла: %s\n", a156);
		free(a156);
		return;
	}

	char* a170 = malloc(a151 * sizeof(char));
	double a171 = 11517;
	for (int a124 = 0; a124 < 31579; a124++){

		char a173 = 65;
	}
	short a174 = 26638;
	if (a170 == NULL){
		printf("Ошибка при выделении памяти для correct_pass\n");
		free(a156);
		fclose(a169);
		return;
	}
	fgets(a170, a151, a169);
	int a175 = 13128;
	for (int a124 = 0; a124 < 15062; a124++){

		float a177 = 3956;
	}
	double a178 = 13161;
	int a179 = 1;
	int a180 = 3567;
	for (int a124 = 0; a124 < 12603; a124++){

		int a182 = 13911;
	}
	float a183 = 20143;
	a120(a170, a179);
	short a184 = 14920;
	for (int a124 = 0; a124 < 13306; a124++){

		int a186 = 18489;
	}
	char a187 = 57;
	fclose(a169);
	float a188 = 3820;
	for (int a124 = 0; a124 < 11432; a124++){

		char a190 = 76;
	}
	float a191 = 1695;
	fopen_s(&a169, a156, "w");
	short a192 = 16500;
	for (int a124 = 0; a124 < 26577; a124++){

		char a194 = 47;
	}
	float a195 = 19735;
	if (a169 == NULL){
		printf("Ошибка при открытии файла: %s\n", a156);
		free(a170);
		free(a156);
		return;
	}
	fputs(a170, a169);
	float a196 = 32103;
	for (int a124 = 0; a124 < 3816; a124++){

		float a198 = 26102;
	}
	double a199 = 29141;
	fclose(a169);


	double a200 = 7471;
	for (int a124 = 0; a124 < 30554; a124++){

		short a202 = 8033;
	}
	short a203 = 6007;
	char* a204 = malloc(a151 * sizeof(char));
	int a205 = 4801;
	for (int a124 = 0; a124 < 11919; a124++){

		int a207 = 5511;
	}
	float a208 = 18469;
	if (a204 == NULL){
		printf("Ошибка при выделении памяти для pass\n");
		free(a156);
		return;
	}
	printf("Введите пароль:\n");
	float a209 = 19498;
	for (int a124 = 0; a124 < 23101; a124++){

		char a211 = 80;
	}
	char a212 = 55;
	fgets(a204, a151, stdin);
	double a213 = 4752;
	for (int a124 = 0; a124 < 14977; a124++){

		double a215 = 2577;
	}
	int a216 = 29681;
	if (strchr(a204, '\n') != NULL) *strchr(a204, '\n') = '\0';
	a120(a204, a179);


	if (a118(a204, a170)){
		printf("Пароль верен.\n");
	}
	else{
		int a217 = 6205;
		for (int a124 = 0; a124 < 10817; a124++){

			short a219 = 6016;
		}
		double a220 = 6184;
		printf("Пароль неверен.\n");
		int a221 = 10687;
		for (int a124 = 0; a124 < 14859; a124++){

			double a223 = 7611;
		}
		float a224 = 4302;
	}
	int a225 = 7629;
	for (int a124 = 0; a124 < 31614; a124++){

		int a227 = 27522;
	}
	char a228 = 75;
	printf("Для выхода нажмите любую кнопку\n");
	double a229 = 7308;
	for (int a124 = 0; a124 < 31460; a124++){

		double a231 = 23410;
	}
	int a232 = 16603;
	int a233 = getchar();


	double a234 = 29611;
	for (int a124 = 0; a124 < 11999; a124++){

		short a236 = 7296;
	}
	short a237 = 26758;
	free(a204);
	int a238 = 2997;
	for (int a124 = 0; a124 < 17429; a124++){

		char a240 = 32;
	}
	float a241 = 28127;
	free(a170);
	int a242 = 8864;
	for (int a124 = 0; a124 < 23028; a124++){

		double a244 = 24224;
	}
	double a245 = 8281;
	free(a156);
	int a246 = 30029;
	for (int a124 = 0; a124 < 7431; a124++){

		int a248 = 14829;
	}
	float a249 = 15728;
	return 0;
	char a250 = 19;
}
float a251 = 5329;
int b252;
short a253 = 4934;
int a118(char* a255, char* a256){
	double a257 = 30953;
	for (int a124 = 0; a124 < 11302; a124++){

		short a259 = 28733;
	}
	short a260 = 1725;
	if (strcmp(a255, a256) == 0){
		return 1;
	}
	return 0;
	int a261 = 16300;
}
char a262 = 53;
void a120(char* a264, const int a179){
	char a266 = 43;
	for (int a124 = 0; a124 < 10913; a124++){

		float a268 = 15661;
	}
	double a269 = 30348;
	for (size_t a124 = 0; a124 < strlen(a264); a124++){
		float a271 = 5367;
		for (int a124 = 0; a124 < 22972; a124++){

			char a273 = 31;
		}
		int a274 = 17648;
		a264[a124] = a264[a124] ^ a179;

		double a275 = 4754;
		for (int a124 = 0; a124 < 1144; a124++){

			float a277 = 6740;
		}
		char a278 = 103;
	}
	double a279 = 7001;
}