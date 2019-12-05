#include<simlib.h>
#include<cstdio>


#define POCET_OBYVATEL 3800	//obyvatele /100
#define CO_STOPA_AUTO 144.1		//g/km
#define CO_STOPA_SALINA 35.1	//g/km
#define CO_STOPA_AUTOBUS 104.7	//g/km

#define KAPACITA_SALINA 162 	//Cestujicich
#define VZDALENOST_PRACE 12 	//KM
#define VZDALENOST_MHD 10.59
#define CESTOVATELE_AUTO 1.6	//cestujicich v aute
#define ROK 7

int PocetAut=0;
int PocetTramvaj=0;
int PocetAutobus=0;
int PocetTrolejbus=0;

int Pocasi=0;
    
Store MistaTramvaje("MistaTramvaje", 0);
Store MistaTrolejbusy("MistaTrolejbusy", 0);
Store MistaAutobusy("MistaAutobusy", 0);


int ignore_pocasi=0;
int tolerance_pocasi=0;

class Cestovatel: public Process{
	void Behavior(){
		if(Uniform(0,100)>=ignore_pocasi){
			//GOTO Jedu MHD bez pocasi
            goto jedu_MHD;
		}else{
			//Diva se na pocasi
			if(Uniform(0,100)>Pocasi-tolerance_pocasi){
				//GOTO jedu MHD
                goto jedu_MHD;
			}else{
				//nejedu MHD
                PocetAut++;
				//goto konec;
				return;
			}
		}
        
jedu_MHD:
        int Vyber_vozidla=Uniform(0,100);//Rozdeleni jakym vozidlem pojede
        if(Vyber_vozidla<54){//Salina
            if(MistaTramvaje.Full()){
                MistaTramvaje.SetCapacity(MistaTramvaje.Capacity()+2);
                PocetTramvaj++;
            }
            Enter(MistaTramvaje, 1);//nasedne
            Wait(Exponential(40));//sedi v MHD
            Leave(MistaTramvaje, 1);//vystoupi
        }else if(Vyber_vozidla>67){//Autobus
            if(MistaAutobusy.Full()){
                MistaAutobusy.SetCapacity(MistaAutobusy.Capacity()+1);
                PocetAutobus++;
            }
            Enter(MistaAutobusy, 1);//nasedne
            Wait(Exponential(40));//sedi v MHD
            Leave(MistaAutobusy, 1);//vystoupi
        }else{//Trolejbusy
            if(MistaTrolejbusy.Full()){
                MistaTrolejbusy.SetCapacity(MistaTrolejbusy.Capacity()+1);
                PocetTrolejbus++;
            }
            Enter(MistaTrolejbusy, 1);//nasedne
            Wait(Exponential(40));//sedi v MHD
            Leave(MistaTrolejbusy, 1);//vystoupi
        }
	}
};    
    
class NovyDen: public Event{
    int CestujiciLidi=POCET_OBYVATEL;
    //generovani pocasi

    void Behavior(){
        Pocasi=Uniform(0,100);
        while(CestujiciLidi>0){
    	    CestujiciLidi -=3;
            //vygeneruj 264 lidi
            for(int i=0;3>=i;i++){
                (new Cestovatel)->Activate();
                Activate(Time+1);
            }
        }
    }
    
};
               
int main(int argc, char *argv[]){
	ignore_pocasi=std::atoi(argv[1]);
	tolerance_pocasi=std::atoi(argv[2]);

    Init(0,1000);
    for(int i=0;ROK>i;i++){
        (new NovyDen)->Activate();
    }
    Run();

    
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

    printf("Auto: %d\n",PocetAut);
    printf("Šaliny: %d\n",PocetTramvaj);
    printf("Trolejbus: %d\n",PocetTrolejbus);
    printf("Autobus: %d\n",PocetAutobus);
    
    printf("Uhlíková stopa na týden: %f tun\n",(PocetAut*VZDALENOST_PRACE*CO_STOPA_AUTO + PocetTramvaj*VZDALENOST_MHD*CO_STOPA_SALINA + PocetTrolejbus*VZDALENOST_MHD*CO_STOPA_SALINA + PocetAutobus*VZDALENOST_MHD*CO_STOPA_AUTOBUS)/10000);
}