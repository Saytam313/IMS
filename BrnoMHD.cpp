#include<simlib.h>
#include<cstdio>


#define CO_STOPA_AUTO 144.1		//g/km
#define CO_STOPA_SALINA 35.1	//g/km
#define CO_STOPA_AUTOBUS 104.7	//g/km

#define VZDALENOST_PRACE 12 	//KM
#define VZDALENOST_MHD 10.59

int PocetAut=0;
int PocetTramvaj=0;
int PocetAutobus=0;
int PocetTrolejbus=0;
int PocetCestovatelu=0;

int Pocasi=0;
double zaplneni=0;
int autaonly_flag=0;
Store MistaTramvaje("MistaTramvaje", 0);
Store MistaTrolejbusy("MistaTrolejbusy", 0);
Store MistaAutobusy("MistaAutobusy", 0);

int ignore_pocasi;
int kolik_dni;
int pocet_obyvatel;

Stat pocasi;

class Cestovatel: public Process{
	void Behavior(){
        PocetCestovatelu++;
		if(Uniform(0,100)>=ignore_pocasi){
			//GOTO Jedu MHD bez pocasi
            goto jedu_MHD;
		}else{
			//Diva se na pocasi
			if(Uniform(0,100)>Pocasi+autaonly_flag){
				//GOTO jedu MHD
                goto jedu_MHD;
			}else{
				//nejedu MHD
                PocetAut++;
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
    int CestujiciLidi=pocet_obyvatel;
    int a = 0;

    void Behavior(){
        if(CestujiciLidi==pocet_obyvatel){
            Pocasi=Uniform(0,100);
            pocasi(Pocasi);
            a = pocet_obyvatel%3;
        }
        
        if(CestujiciLidi>0){
            if(CestujiciLidi >= 3){
                CestujiciLidi-=3;
                //vygeneruj 264 lidi
                for(int i=0;3>i;i++){
                    (new Cestovatel)->Activate();
                }
            } else {
                CestujiciLidi-=a;
                //vygeneruj 264 lidi
                for(int i=0;a>i;i++){
                    (new Cestovatel)->Activate();
                }
            }
            Activate(Time + 1);
        }
    }
};

class Rok : public Event{
    int dnu=0;
    
	void Behavior(){
        if(dnu < kolik_dni) {
            dnu++;
		    (new NovyDen)->Activate();
		    Activate(Time + 1*60*24);
        }
	}
};
               
int main(int argc, char *argv[]){
    
    // primitivni zpracovani argumentu, pro vice vystupu s ruznymy hodnotami u 'make run'
	
	//arg 1 - Kolik procent lidi ignoruje pocasi
	//arg 2 - Kolik dni simulujeme
	//arg 3 - Kolik lidi simulujeme x100
    //arg 4 - Koefisient zaplneni MHD

    ignore_pocasi=std::atoi(argv[1]);
    kolik_dni=std::atoi(argv[2]);
    pocet_obyvatel=std::atoi(argv[3])*0.8;
    zaplneni=std::atof(argv[4])/100;
    
    if(ignore_pocasi>100){
    	autaonly_flag=10000;
    }

    Init(0);
    (new Rok)->Activate();
    Run();
    printf("Statistika pocasi za simulovane obdobi:\n");
    pocasi.Output();

    PocetTramvaj=PocetTramvaj/zaplneni;
    PocetTrolejbus=PocetTrolejbus/zaplneni;
    PocetAutobus=PocetAutobus/zaplneni;
    
    PocetTramvaj*=kolik_dni;
    PocetTrolejbus*=kolik_dni;
    PocetAutobus*=kolik_dni;
    PocetAut=(PocetAut*100)/1.5;
    
    printf("Auto: %d\n",PocetAut);
    printf("Tramvaje: %d\n",PocetTramvaj);
    printf("Trolejbus: %d\n",PocetTrolejbus);
    printf("Autobus: %d\n",PocetAutobus);
    printf("Cestovatelu: %d\n",PocetCestovatelu*100);
    
    printf("Uhlikova stopa na %d dnu: %f kt\n",kolik_dni,(PocetAut*VZDALENOST_PRACE*CO_STOPA_AUTO + PocetTramvaj*VZDALENOST_MHD*CO_STOPA_SALINA + PocetTrolejbus*VZDALENOST_MHD*CO_STOPA_SALINA + PocetAutobus*VZDALENOST_MHD*CO_STOPA_AUTOBUS)/100000000);
}
