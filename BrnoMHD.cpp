#include<simlib.h>
#include<cstdio>

#define POCET_OBYVATEL 380681	//obyvatele
#define CO_STOPA_AUTO 144.1		//g/km
#define CO_STOPA_SALINA 35.1	//g/km
#define CO_STOPA_AUTOBUS 104.7	//g/km
//#define PROCENTO_RIDICI 43 		//% obyvatel
//#define PROCENTO_MHD 38			//% obyvatel
#define PROCENTO_PESKY 18 		//% obyvatel
#define KAPACITA_SALINA 162 	//Cestujicich
#define VZDALENOST_PRACE 12 	//KM
#define CESTOVATELE_AUTO 1.6	//cestujicich v aute

int main(int argc, char *argv[]){
	// primitivni zpracovani argumentu, pro vice vystupu s ruznymy hodnotami u 'make run'
	

	//arg 1 - % MHD
	//arg 2 - % Auta
	//arg 3 - % zaplneni vozu mhd
	//arg 4 - pocet cestujicich v aute

	double cestuje_mhd_prc = std::atof(argv[1]);
	double cestuje_autem_prc = std::atof(argv[2]);

 	double ZAPLNENI_MHD=std::atof(argv[3]);
	double ZAPLNENI_AUTO=std::atof(argv[4]);
	double auta_prc;
	auta_prc=cestuje_autem_prc / 100;
	double mhd_prc=cestuje_mhd_prc / 100;
	printf("%f\n",auta_prc);
	printf("%f\n",mhd_prc);
	double Pocet_aut = (((cestuje_autem_prc/100) * POCET_OBYVATEL)/ZAPLNENI_AUTO);
	double Pocet_mhd = (((cestuje_mhd_prc/100) * POCET_OBYVATEL)/(KAPACITA_SALINA*(ZAPLNENI_MHD/100)));

	double CO_aut=CO_STOPA_AUTO*Pocet_aut*VZDALENOST_PRACE;
	double CO_mhd=CO_STOPA_SALINA*Pocet_mhd*VZDALENOST_PRACE;

	double CO_celkem_den=CO_aut+CO_mhd;
	double CO_celkem_rok=(CO_celkem_den*365)/1000000;

	printf("Pocet_aut: %f, Pocet_mhd: %f\n",Pocet_aut,Pocet_mhd);
	printf("CO za den: %f t, za rok: %f t\n",CO_celkem_den/1000000,CO_celkem_rok);


		
}