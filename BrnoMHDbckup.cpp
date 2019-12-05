#include<simlib.h>
#include<cstdio>

//#define POCET_OBYVATEL 380681	//obyvatele
#define POCET_OBYVATEL 10000	//obyvatele
#define CO_STOPA_AUTO 144.1		//g/km
#define CO_STOPA_SALINA 35.1	//g/km
#define CO_STOPA_AUTOBUS 104.7	//g/km
//#define PROCENTO_RIDICI 43 		//% obyvatel
//#define PROCENTO_MHD 38			//% obyvatel
#define PROCENTO_PESKY 18 		//% obyvatel
#define KAPACITA_SALINA 162 	//Cestujicich
#define VZDALENOST_PRACE 12 	//KM
#define CESTOVATELE_AUTO 1.6	//cestujicich v aute
#define ROK 30

int PocetAut=0;
int PocetTramvaj=0;
int PocetAutobus=0;
int PocetTrolejbus=0;

int Pocasi=0;

int All_PocetAut=0;
int All_PocetTramvaj=0;
int All_PocetAutobus=0;
int All_PocetTrolejbus=0;
    
Store MistaTramvaje("MistaTramvaje", 0);
Store MistaTrolejbusy("MistaTrolejbusy", 0);
Store MistaAutobusy("MistaAutobusy", 0);

class Cestovatel;

class Cestovatel: public Process{
public:
	void Behavior(){
		if(Uniform(0,100)>=30){
			//GOTO Jedu MHD bez pocasi
            goto jedu_MHD;
		}else{
			//Diva se na pocasi
			if(Uniform(0,100)>Pocasi){
				//GOTO jedu MHD
                goto jedu_MHD;
			}else{
				//nejedu MHD
                PocetAut++;
                All_PocetAut++;
				//goto konec;
				return;
			}
		}
jedu_MHD:
        
        int Vyber_vozidla=Uniform(0,100);//Rozdeleni jakym vozidlem pojede
        if(Vyber_vozidla<54){//Salina
            //MistaTramvaje.Output();
            if(MistaTramvaje.Full()){
                MistaTramvaje.SetCapacity(MistaTramvaje.Capacity()+137);
                PocetTramvaj++;
                //All_PocetTramvaj++;
            }
            Enter(MistaTramvaje, 1);//nasedne
            Wait(Exponential(40));//sedi v MHD
            Leave(MistaTramvaje, 1);//vystoupi
        }else if(Vyber_vozidla>67){//Autobus
            if(MistaAutobusy.Full()){
                MistaAutobusy.SetCapacity(MistaAutobusy.Capacity()+111);
                PocetAutobus++;
                //All_PocetAutobus++;
            }
            Enter(MistaAutobusy, 1);//nasedne
            Wait(Exponential(40));//sedi v MHD
            Leave(MistaAutobusy, 1);//vystoupi
        }else{//Trolejbusy
            if(MistaTrolejbusy.Full()){
                MistaTrolejbusy.SetCapacity(MistaTrolejbusy.Capacity()+137);
                PocetTrolejbus++;
                //All_PocetTrolejbus++;
            }
            Enter(MistaTrolejbusy, 1);//nasedne
            Wait(Exponential(40));//sedi v MHD
            Leave(MistaTrolejbusy, 1);//vystoupi
        }
	}
};

    
    
    
class NovyDen: public Event{
public:
    int CestujiciLidi=POCET_OBYVATEL;
    //generovani pocasi

    void Behavior(){
        Pocasi=Uniform(0,100);
        //int PocetTramvaj=0;
        //int PocetAutobus=0;
        //int PocetTrolejbus=0;
        //int PocetAut=0;
        //Pocasi=100;
        while(CestujiciLidi>0){
    	    CestujiciLidi -=264;
            //vygeneruj 264 lidi
            for(int i=0;264>=i;i++){
                (new Cestovatel)->Activate();
                Activate(Time+1);
                //Wait(1);
            }
        }
/*
        printf("Pocasi: %d\n",Pocasi);
        Pocasi=0;
        */
            
		//Store MistaTramvaje("MistaTramvaje", 0);
		//Store MistaTrolejbusy("MistaTrolejbusy", 0);
		//Store MistaAutobusy("MistaAutobusy", 0);
        printf("Šaliny: %d|",PocetTramvaj);
        printf("Trolejbus: %d|",PocetTrolejbus);
        printf("Autobus: %d|",PocetAutobus);
        printf("Auto: %d\n",PocetAut);
        //MistaTramvaje.Clear();
    }
    
};

class CelyRok: public Event{
    void Behavior(){
        for(int i=0;ROK>=i;i++){
            (new NovyDen)->Activate();
            Activate(Time + (24*60));
        }
    }
};
                    
int main(int argc, char *argv[]){
    Init(0,1000);
    //(new CelyRok)->Activate();
    (new NovyDen)->Activate();
    
    Run();

    printf("Auto: %d\n",PocetAut);
    printf("Šaliny: %d\n",PocetTramvaj);
    printf("Trolejbus: %d\n",PocetTrolejbus);
    printf("Autobus: %d\n",PocetAutobus);

    
	// primitivni zpracovani argumentu, pro vice vystupu s ruznymy hodnotami u 'make run'
	

	//arg 1 - % MHD
	//arg 2 - % Auta
	//arg 3 - % zaplneni vozu mhd
	//arg 4 - pocet cestujicich v aute
/*
	double cestuje_mhd_prc = std::atof(argv[1]);
	double cestuje_autem_prc = std::atof(argv[2]);

 	double ZAPLNENI_MHD=std::atof(argv[3]);
	double ZAPLNENI_AUTO=std::atof(argv[4]);

	double Pocet_aut = (((cestuje_autem_prc/100) * POCET_OBYVATEL)/ZAPLNENI_AUTO);
	double Pocet_mhd = (((cestuje_mhd_prc/100) * POCET_OBYVATEL)/(KAPACITA_SALINA*(ZAPLNENI_MHD/100)));

	double CO_aut=CO_STOPA_AUTO*Pocet_aut*VZDALENOST_PRACE;
	double CO_mhd=CO_STOPA_SALINA*Pocet_mhd*VZDALENOST_PRACE;

	double CO_celkem_den=CO_aut+CO_mhd;
	double CO_celkem_rok=(CO_celkem_den*365)/1000000;

	printf("Pocet_aut: %f, Pocet_mhd: %f\n",Pocet_aut,Pocet_mhd);
	printf("CO za den: %f t, za rok: %f t\n",CO_celkem_den/1000000,CO_celkem_rok);
*/

    printf("Auto: %d\n",All_PocetAut);
    printf("Šaliny: %d\n",All_PocetTramvaj);
    printf("Trolejbus: %d\n",All_PocetTrolejbus);
    printf("Autobus: %d\n",All_PocetAutobus);
    
}