#include "../simlib/simlib.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


/* MOJE */
int maxKapacita = 24; //97 / 2 (simulujeme jen jednu stranu)
int vyuzitaKapacita = 0;
int intervalNoveLode = 51;

Facility Vjezd("Vjezd do kanálu");
Facility Blokace("Zablokování kanálu");
Facility GenerujLode("Token pro generování lodí");
Store Kapacita("Denní kapacita kanálu", maxKapacita);


//proces lodi proplouvající kanálem
class Lod : public Process {
	void Behavior() {
		double time = Time;
		Seize(Vjezd); // zaber vjezd do kanálu
		//printf("seize \t%f\t%f\n", time, Time);
		Enter(Kapacita, 1); //vezmi 1 denní kapacitu
		vyuzitaKapacita++;
		
		
		Wait(28);//po 28 minutách uvolni vjezd do kanálu
		//printf("release\t%f\t%f\n", time, Time);
		Release(Vjezd);
	
		Wait(Uniform((12*60)-28, (16*60)-28)); // doba průplavu 12-16 hodin (-28 minut protože tolik trvá vrácení vjezdu o pár řádků výš) 
		
		
		float casPlavby = Time-time;
		//printf("čas\t%f\t%f\n",time, casPlavby);
		//printf("%d\t%f\t%f\n", int(time/intervalNoveLode), time ,casPlavby); //hezčí výpis
		printf("%f\t%f\n", time ,(casPlavby/60)); //výpis pro excel
	}
} ;

//proces přístavu
class Zaseknuti : public Process {
	void Behavior() {
		double time = Time;
		Seize(Vjezd); // bere vjezd aby nebyl na začátku dostupný pro loď
		Seize(Blokace); //zablokování přístavu
		Wait(Uniform(8*60, 151*60));//rozmezí opravy uváznuté lodi
		float casZaseknuti = Time-time;
		printf("---Zaseknutí: %f\n", casZaseknuti);
		Release(Vjezd); //vrať vjezd, ať můžou lodě frčet
		Seize(GenerujLode); //zastavení generování nových lodí
	}
} ;

//generátor lodí
class GeneratorLodi : public Event {
public:
	GeneratorLodi(double interv) : Event() {
		Interval = interv;
	};
	
	void Behavior() {
		if(!GenerujLode.Busy())
		{
			(new Lod())->Activate();
			Activate(Time+Interval);
		}
	}
	
	double	Interval;
} ;


//proces resetu denní kapacity
class ResetKapacity : public Process {
	void Behavior() {
		
		Enter(Kapacita, maxKapacita - vyuzitaKapacita);
		Leave(Kapacita, maxKapacita);
		vyuzitaKapacita = 0;	  
	}
} ;

//generátor kapacity
class GeneratorKapacity : public Event {
public:
	GeneratorKapacity(double interv) : Event() {
		Interval = interv;
	};
	
	void Behavior() {
		(new ResetKapacity())->Activate();
		Activate(Time+Interval);
	}
	
	double	Interval;
} ;


int	main()
{
	RandomSeed(time(NULL));
	SetOutput("ims2022.dat");
	Init(0, 300 * 60); //doba běhu simulace (300 hodin)
	(new Zaseknuti())->Activate();
	(new GeneratorKapacity(24*60))->Activate(); //1x za 24 hodin
	(new GeneratorLodi(intervalNoveLode))->Activate();//každých 51 minut novou loď
	
	Run();

	Blokace.Output();
	Vjezd.Output();
	Kapacita.Output();
}

