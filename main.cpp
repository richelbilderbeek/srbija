#include <iostream>
#include <string>
#include <algorithm>
#include <csignal>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>
using namespace std;

void sleep(const int n_secs)
{
  std::this_thread::sleep_for(std::chrono::seconds(n_secs));
}

#define Helper cout<<helperNaam<<":"<<
#define Speler cout<<spelerNaam<<":"<<
#define Vijar cout<<m_Naam<<": "<<
#define SpelerN <<spelerNaam<<
#define HelperN <<helperNaam<<

int wilget(const int min, const int max) {
  const int denominator = max-min+1;
  assert(denominator != 0);
  return min + (std::rand()% denominator);
}

//#define slaap cout<<"\n";usleep(100000)
#define slaap { std::this_thread::sleep_for(std::chrono::milliseconds(100)); }
#define	SP cout<<speler.toonNaam()<<":"<<
#define	MIR cout<<teg.toonNaam()<<":"<<

typedef short int kint;
void INVOERFOUT(kint &x);
bool BMG = false;

/* Globale variabelen */
const kint
AANTAL_VIJANDEN 	= 7,
GEZONDHEID_SPELER 	= 100,
GENEESKRACHT_SPELER	= 30,
ARMKRACHT_SPELER 	= 20,
BEENKRACHT_SPELER 	= 20,
SNELHEID_SPELER 	= 17,
CODE_SPELER 		= -5;
/* Verander deze instellingen voor een andere speelervaring! */

class Persoon
{
	public:
		Persoon(string naam="Onbekend", kint vijnr = 1, kint gez=100, kint genez=30,
		kint arm=25, kint been=35, kint over=60, kint snelheid=12, bool moordlustig = false,
		bool godmode = false, bool aangevallen = false);
		void instellen(string naam="Onbekend", kint vijnr = 1, kint gez=100, kint genez=30,
		kint arm=25, kint been=35, kint over=60, kint snelheid=12, bool moordlustig = false,
		bool aangevallen = false);
		friend void speelMenu(Persoon &tegenstander, Persoon &speler);
		friend void INVOERFOUT(kint &x);
		friend void controle(Persoon &teg, Persoon &speler);
		
//		void stoot(Persoon &v){}   // dit komt nog wel!
		void vraagGenezing();
		void stoot(Persoon &v);
		void trapt(Persoon &v);
		void niksdoen();
		void keuzeVijand(Persoon &speler, Persoon &vijand);
		void vluchtwegvan(Persoon &v, Persoon &lafaard);
		void geeftzichoveraan(Persoon &v, string &helperNaam);
		void overgaveVijand(Persoon &teg, Persoon &speler);
		
		void toonStatistieken();
		void valsspeelMode();
		bool levend(kint i){return m_Levend[i];}
		bool gevangen(kint i){return m_Gevangen[i];}
		string toonNaam()const{return m_Naam;}
		kint toonGezondheid()const{return m_Gezondheid;}
		kint toonOverwinningskans()const{return m_Overwinningskans;}
		kint toonArmkracht()const{return m_Armkracht;}
		kint toonBeenkracht()const{return m_Beenkracht;}
	protected:
		kint m_Gezondheid, m_VijandNummer, m_Armkracht, m_Beenkracht, m_Overwinningskans,
		m_Snelheid, m_Geneeskracht;
		string m_Naam;
		bool m_Levend[AANTAL_VIJANDEN], m_Gevangen[AANTAL_VIJANDEN];
		bool m_Moordlustig, m_Valsspeelmode, m_Aangevallen;
	private:
        kint m_min, m_max, m_var;
};

Persoon::Persoon(string naam, kint vijnr, kint gez, kint genez, kint arm, kint been,
			kint over, kint snelheid, bool moordlustig, bool godmode, bool aangevallen)
{
	if((gez > 80 && (arm >= 35 || been >= 45) && over >= 60) || godmode)
	{
//		cout << "\nEen machtige vijand is opgestaan! Bereid je maar voor!\n";
	}
	else
	{
//		cout << "\nEen nieuwe vijand is opgestaan, maar ik ben ervan overtuigd "
//		<< "dat jij hem/haar wel aankunt!" << endl;
	}
		for(kint i = 0; i < AANTAL_VIJANDEN; i++) {m_Levend[i] = true;}
		for(kint i = 0; i < AANTAL_VIJANDEN; i++) {m_Gevangen[i] = false;}
		m_Naam = naam;
		m_Gezondheid = gez;
		if(m_Gezondheid > 100){m_Gezondheid = 100;}
		m_Geneeskracht = genez;
		m_Armkracht = arm;
		m_Beenkracht = been;
		m_Overwinningskans = over;
		if(m_Overwinningskans > 100){m_Overwinningskans = 100;}
		m_Snelheid = snelheid;
		m_Moordlustig = moordlustig;
		m_Valsspeelmode = godmode;
		m_Aangevallen = aangevallen;
		m_VijandNummer = vijnr;
		
		if(m_Valsspeelmode)
		{
			m_Gezondheid = 900;
			m_Armkracht = 60;
			m_Beenkracht = 60;
			m_Overwinningskans = 100;
			m_Snelheid = 25;
		}
}

void Persoon::instellen(string naam, kint vijnr, kint gez, kint genez, kint arm, kint been,
                      kint over, kint snelheid, bool moordlustig, bool aangevallen)
{
	m_Naam = naam;
	m_Gezondheid = gez;
	if(m_Gezondheid > 100){m_Gezondheid = 100;}
	m_Geneeskracht = genez;
	m_Armkracht = arm;
	m_Beenkracht = been;
	m_Overwinningskans = over;
	m_Snelheid = snelheid;
	m_Moordlustig = moordlustig;
	m_Aangevallen = aangevallen;
	m_VijandNummer = vijnr;

	if(m_Valsspeelmode)
	{
		m_Gezondheid = 900;
		m_Geneeskracht = 70;
		m_Armkracht = 60;
		m_Beenkracht = 60;
		m_Overwinningskans = 100;
		m_Snelheid = 25;
	}
}

void Persoon::stoot(Persoon &v)
{
    m_min = 0, m_max = 5; v.m_Aangevallen = true;
	kint spreiding = 0, var = 0;
	if(m_VijandNummer == CODE_SPELER){cout << endl;}
    switch (wilget(m_min, m_max))
	{
		case 0: cout << m_Naam << " geeft een vuiststoot tegen de borst van " <<
		v.m_Naam << ".   "; spreiding = 0; break;
		case 1: cout << m_Naam << " geeft een stomp in de maag van " <<
		v.m_Naam << ".   "; spreiding = 2; break;
		case 2: cout << m_Naam << " geeft een dreun tegen de mond van " <<
		v.m_Naam << ".   "; spreiding = 4; break;
		case 3: cout << m_Naam << " geeft een dreun tegen de neus van " <<
		v.m_Naam << ".   "; spreiding = 6; break;
		case 4: cout << m_Naam << " geeft een meshand tegen de nek van " <<
		v.m_Naam << ".   "; spreiding = 8; break;
		case 5: cout << m_Naam << " geeft een stoot tegen de keel van " <<
		v.m_Naam << ".   "; spreiding = 10; break;
		default: exit(1);
	}
	if(spreiding > m_Armkracht){spreiding = m_Armkracht;}
    m_min = m_Armkracht - spreiding;
    m_max = m_Armkracht + spreiding;
    var = wilget(m_min, m_max); v.m_Gezondheid = v.m_Gezondheid - var;
	cout << "-" << var << "\n";
}

void Persoon::trapt(Persoon &v)
{
    m_min = 0, m_max = 5; v.m_Aangevallen = true;
	kint spreiding = 0, var = 0;
	if(m_VijandNummer == CODE_SPELER){cout << endl;}
    switch (wilget(m_min, m_max))
	{
		case 0: cout << m_Naam << " geeft een trap tegen de zij van " <<
		v.m_Naam << ".   "; spreiding = 5; break;
		case 1: cout << m_Naam << " geeft een voorwaartse trap tegen de buik van " <<
		v.m_Naam << ".   "; spreiding = 7; break;
		case 2: cout << m_Naam << " geeft een zijwaartse trap tegen de buik van " <<
		v.m_Naam << ".   "; spreiding = 9; break;
		case 3: cout << m_Naam << " geeft een voetstomp tegen het bovenbeen van " <<
		v.m_Naam << ".   "; spreiding = 11; break;
		case 4: cout << m_Naam << " geeft een zijwaartse trap tegen de kin van " <<
		v.m_Naam << ".   "; spreiding = 13; break;
		case 5: cout << m_Naam << " geeft een voetstomp tegen de wang van " <<
		v.m_Naam << ".   "; spreiding = 15; break;
		default: exit(1);
	}
	if(spreiding > m_Beenkracht){spreiding = m_Beenkracht;}
    m_min = m_Beenkracht - spreiding;
    m_max = m_Beenkracht + spreiding;
    var = wilget(m_min, m_max); v.m_Gezondheid = v.m_Gezondheid - var;
	cout << "-" << var << "\n";
}

void Persoon::keuzeVijand(Persoon &vijand, Persoon &speler)
{
	long double kans = m_Overwinningskans, arm = m_Armkracht, been = m_Beenkracht,
	gezondheid = m_Gezondheid, arm2 = speler.m_Armkracht, been2 = speler.m_Beenkracht,
	gezondheid2 = speler.m_Gezondheid, geneeskracht = m_Geneeskracht,
	MIN=0, MIN2=0, MAX=0, MAX2=0; kans = kans / 100;
	if(!(m_Aangevallen)){niksdoen();}
	else
	{
		/* Potentie vijand */
		((been - 15) <= (arm - 10)) ? MIN = been - 15: MIN = arm - 10;
		((been + 15) >= (arm + 10)) ? MAX = been + 15: MAX = arm + 10;
		
		/* Potentie speler */
		((been2 - 15) <= (arm2 - 10)) ? MIN2 = been2 - 15: MIN2 = arm2 - 10;
		((been2 + 15) >= (arm2 + 10)) ? MAX2 = been2 + 15: MAX2 = arm2 + 10;
		
//		cout << "Minimale slagkracht " << speler.m_Naam << ": " << MIN2 << endl;
//		cout << "Maximale slagkracht " << speler.m_Naam << ": " << MAX2 << endl;
//		cout << "Minimale slagkracht " << vijand.m_Naam << ": " << MIN << endl;
//		cout << "Maximale slagkracht " << vijand.m_Naam << ": " << MAX << endl;
		
		/* Berekent of de kans dat de vijand de speler doodt hoog genoeg is. */
		if(((kans - 1) * (MAX - MIN) + MIN) >= gezondheid2)
		{
			cout << endl << "Desper aanval" << endl;
            m_min = 0, m_max = 2;
            switch (wilget(m_min, m_max))
			{
				case 0: stoot(speler); break;
				case 1: if(m_Armkracht > m_Beenkracht){stoot(speler);}
				   else if(m_Beenkracht > m_Armkracht){trapt(speler);}
                   else{m_max=1;if(wilget(m_min, m_max) == 1){stoot(speler);}else{trapt(speler);}}
				   break;
				case 2: trapt(speler); break;
				default: exit(1);
			}
		}
		/* Berekent of de kans dat de vijand straks gedood wordt hoog genoeg is. */
		else if((kans * (MAX2 - MIN2) + MIN2) >= gezondheid)
		{	/* Berekent of de kans dat de vijand na genezing gedoodt wordt te groot is */
			if((kans*(MAX2-MIN2)+MIN2) >= (gezondheid+((1-kans)*20+(geneeskracht-10))))
			{
				(m_Snelheid > speler.m_Snelheid) ? vluchtwegvan(speler, vijand):
													overgaveVijand(vijand, speler);
			}
			else
			{
				vraagGenezing();
			}
		}
		else
		{
            m_min = 0, m_max = 2;
            switch (wilget(m_min, m_max))
			{
				case 0: stoot(speler); break;
				case 1: if(m_Armkracht > m_Beenkracht){stoot(speler);}
				   else if(m_Beenkracht > m_Armkracht){trapt(speler);}
                   else{m_max=1;if(wilget(m_min, m_max) == 1){stoot(speler);}else{trapt(speler);}}
				   break;
				case 2: trapt(speler); break;
				default: exit(1);
			}
		}
	}
}

void Persoon::overgaveVijand(Persoon &teg, Persoon &speler)
{
    m_min = 0, m_max = 5; cout << endl; kint keuze = 0;
    switch (wilget(m_min, m_max))
	{
		case 0:Vijar"Ik... ik geef het op. Ik kan niet meer. Jij hebt mij verslagen.\n";break;
		case 1: Vijar "Alsjeblieft, ik kan niet meer! Stop alsjeblieft!\n"; break;
		case 2: Vijar "Ik ben op... Accepteer mijn overgave, alsjeblieft.\n"; break;
		case 3: Vijar "Stop... alsjeblieft... ik smeek het je... \n"; break;
		case 4: Vijar "Alsjeblieft, stop! Wees genadig! Ik trek het niet meer!\n"; break;
		case 5: Vijar "Stop het gevecht, ik kan niet meer. Spaar mij alsjeblieft!\n"; break;
		default: exit(1);
	}
	cout << "\n Wat doe jij nu?\n"; slaap;
	cout << "1) " << m_Naam << " sparen en vrijlaten." <<
	" (jij zult " << m_Naam << " later opnieuw moeten bevechten)"; slaap;
	cout << "2) " << m_Naam << " sparen en gevangennemen."; slaap;
	cout << "3) " << m_Naam << " alsnog doden."; slaap;
	cout << "Keuze: "; cin >> keuze;
	while(cin.fail() || keuze < 1 || keuze > 3){INVOERFOUT(keuze);}
	switch (keuze)
	{
        case 1: if (!speler.m_Valsspeelmode) { speler.m_Gezondheid=100; } speelMenu(teg,speler);break;
		case 2: m_Gevangen[m_VijandNummer] = true; controle(teg, speler);
		speelMenu(teg, speler); break;
		case 3: while(m_Gezondheid > 0){speler.trapt(teg);} break;
		default: exit(1);
	}
}

void Persoon::vraagGenezing()
{
	if(m_Gezondheid >= 100 && m_Valsspeelmode == false)
	{
		cout << "\nJij hebt geen genezing nodig! Jij bent kerngezond!\n";
	}
	else if(m_Gezondheid < 100 && m_Valsspeelmode == false)
	{
        m_min = m_Geneeskracht - 10; m_max = m_Geneeskracht + 10;
        m_var = wilget(m_min, m_max); m_Gezondheid = m_Gezondheid + m_var;
        if(m_Gezondheid > 100){m_var = m_var - (m_Gezondheid - 100); m_Gezondheid = 100;}
		if(m_VijandNummer == CODE_SPELER){cout << endl;}
        cout << m_Naam << " geneest!  +" << m_var << "!" << endl;
	}
	else
	{
        m_min = m_Geneeskracht - 10; m_max = m_Geneeskracht + 10;
        m_var = wilget(m_min, m_max); m_Gezondheid = m_Gezondheid + m_var;
        cout << endl << m_Naam << " geneest!  +" << m_var << "!" << endl;
	}
}

void Persoon::vluchtwegvan(Persoon &v, Persoon &lafaard)
{
	if(m_Snelheid > v.m_Snelheid || not v.m_Moordlustig)
	{
        m_min = 0, m_max = 3;
        switch (wilget(m_min, m_max))
		{
			case 0: cout << "\nDe vuile lafaard " << m_Naam << " vlucht weg voor "<< v.m_Naam
			<<"!\n"; break;
			case 1: cout << endl << m_Naam << " is een walgelijke bangeschijterd en vlucht" <<
			" weg voor " << v.m_Naam << "!\n"; break;
			case 2: cout << "\nDe smerige lafbek " << m_Naam << " durft niet meer en rent met"
			<< " de staart tussen de benen weg voor " << v.m_Naam << "!\n"; break;
			case 3: cout << "\nDe landverrader " << m_Naam << " heeft besloten te vluchten" <<
			" voor " << v.m_Naam << "!\n"; break;
			default: exit(1);
		}
		(v.m_VijandNummer == CODE_SPELER) ? speelMenu(lafaard, v): speelMenu(v, lafaard);
	}
	else if(m_Snelheid == v.m_Snelheid && v.m_Moordlustig)
	{
        m_min = 0, m_max = 1;
        switch (wilget(m_min, m_max))
		{
			case 0: cout << "\nDe lafaard " << m_Naam << " weet nog net aan te ontsnappen van"
			<< " " << v.m_Naam << ".\n"; (v.m_VijandNummer == CODE_SPELER) ?
			speelMenu(lafaard, v): speelMenu(v, lafaard); break;
			case 1: cout << "\nDe lafaard " << m_Naam << " probeerde te ontsnappen, maar was "
			<< "niet snel genoeg.\n"; break;
			default: exit(1);
		}
	}
	else if(m_Snelheid < v.m_Snelheid && v.m_Moordlustig)
	{
        m_min = 0, m_max = 3;
        switch (wilget(m_min, m_max))
		{
			case 0: cout << "\nDe vuile socialist " << m_Naam << " probeerde weg te vluchten "
			<< "voor" << v.m_Naam << ", maar was niet snel genoeg!\n"; break;
			case 1: cout << endl << m_Naam << " is een walgelijke bangeschijterd." <<
			m_Naam << " heeft zelfs geprobeerd om te vluchten voor " << v.m_Naam <<
			", maar was te traag!\n"; break;
			case 2: cout << "\nDe smerige lafbek " << m_Naam << " durft niet meer en " <<
			"probeerde met de staart tussen de benen weg te rennen voor " << v.m_Naam <<
			", maar was niet snel genoeg!\n"; break;
			case 3: cout << "\nDe landverrader " << m_Naam << " heeft besloten te vluchten" <<
			" voor " << v.m_Naam << "! Gelukkig was die vluchtpoging mislukt!\n"; break;
			default: exit(1);
		}
	}
	else
	{
		cout << "\nHier zou het programma nooit moeten komen. Rapporteer deze fout:\n" <<
		"void Persoon::vluchtwegvan(Persoon &v, Persoon &lafaard)\n";
	}
	
}

void Persoon::geeftzichoveraan(Persoon & /* v */, string &helperNaam)
{
    m_min = 0, m_max = 6; cout << endl; string spelerNaam = m_Naam;
    switch (wilget(m_min, m_max))
	{
		case 0: Helper " Serven geven nooit op! Vecht door!\n"; break;
		case 1: cout << "Отступать больше некуда. Так бывало не раз!\n"; break;
		case 2: Speler "\nBen je gek? Servië rekent op mij! Ik moet en zal Servië " <<
		"redden!\n"; break;
		case 3: Speler " Ik sterf nog liever dan dat ik toe moet zien hoe " <<
		"de Kroatische heks Mirna Ković Servië vernietigt!\n"; break;
		case 4: Helper " Geef niet op, " SpelerN "! Servië rekent op jou!\n"; break;
		case 5: cout << "Не сдавайся, " SpelerN "! Ты обязан Сербию спасать!\n"; break;
		case 6: cout << "Ратко Младић: Ik heb Servië al een keer gered. Jij zult dit "
		<< "opnieuw moeten doen. Veel succes!\n"; break;
		default: exit(1);
	}
}

void Persoon::niksdoen()
{
	cout << endl << m_Naam << " heeft besloten om niks te doen!" << endl;
}

void Persoon::valsspeelMode()
{
	m_Valsspeelmode ? m_Valsspeelmode = false : m_Valsspeelmode = true;
	instellen(m_Naam, CODE_SPELER, m_Gezondheid, GENEESKRACHT_SPELER, ARMKRACHT_SPELER,
	BEENKRACHT_SPELER, m_Overwinningskans, SNELHEID_SPELER, m_Moordlustig);
	cout << "\nValsspeelmode "; m_Valsspeelmode ? cout << "AAN!\n" : cout << "UIT!\n";
}

void Persoon::toonStatistieken()
{
	cout << endl << "Naam: " << m_Naam << endl;
	cout << "Gezondheid: " << m_Gezondheid << endl;
	cout << "Geneeskracht: " << m_Geneeskracht << endl;
	cout << "Armkracht: " << m_Armkracht << endl;
	cout << "Beenkracht: " << m_Beenkracht << endl;
	cout << "Overwinningskans: " << m_Overwinningskans << endl;
	cout << "Snelheid: " << m_Snelheid << endl;
	cout << "Moordlustig?\t"; m_Moordlustig ? cout << "Ja" : cout << "Nee";
	cout << endl << endl;
}

void INVOERFOUT(kint &x)
{
	cin.clear(); cin.ignore();
	cout << "\nVoer een geldige waarde in: "; cin >> x;
}

void keuzeSpeler(Persoon &teg, Persoon &speler);

void Mirna(Persoon &teg, Persoon &speler)
{
	teg.instellen("Mirna Ković",			-22,120,40,25,25,75,17,0); BMG = true;
    cout << "\n\t In het laboratorium van Mirna Ković te Zadar...\n\n"; sleep(2);
	SP " Jouw dagen zijn geteld, Mirna Ković!\n"; sleep(2);
	MIR " Hoe ben jij binnengekomen?\n"; sleep(2);
	SP " Dat was niet moeilijk. Die " << AANTAL_VIJANDEN << " beschermers kon ik zo " <<
	"uitschakelen.\n"; sleep(2);
	MIR " Heb jij ze alle " << AANTAL_VIJANDEN << " uitgeschakeld?\n"; sleep(2);
	SP " Alle " << AANTAL_VIJANDEN << ", ja.\n"; sleep(2);
	MIR " RAAARGGHH!!! JIJ BENT ER GEWEEST!!!@@@\n"; sleep(2);
	SP " Ik ben gekomen om te voorkomen dat jij Servië vernietigt.\n"; sleep(2);
	MIR " Dan ben jij mooi te laat! De raketten staan klaar voor lancering!\n"; sleep(2);
	SP " Stop die raketten dan, vlug!\n"; sleep(2);
	MIR " Haha, mooi niet. Om die raketten terug te kunnen roepen, heb jij de code nodig " <<
	"die in de kluis zit, maar daar zul jij nooit bijkomen. Daar zorg ik wel voor!\n";
	sleep(4); SP " Als jij het op die manier wilt doen...\n"; sleep(2);
	while(true){keuzeSpeler(teg, speler);}
}

void intro1(string &spelerNaam, string &helperNaam)
{
	sleep(1); cout << endl;
	Helper " Goed dat jij er bent, " SpelerN ". Ik heb jou overal gezocht!\n"; sleep(3);
	Speler " Wat is er dan aan de hand, " HelperN "?\n"; sleep(3);
	Helper " Ik... Ik heb afgrijselijk nieuws!\n"; sleep(2);
	Speler " Vertel...\n"; sleep(2);
	Helper " Het is weer die heks uit Kroatië, Mirna Ković!\n"; sleep(2);
	Speler " Wat is zij dit keer van plan?\n"; sleep(2);
	Helper " Het... het is verschrikkelijk! :'-( \n"; sleep(2);
	Speler " Wat is er dan?\n"; sleep(2);
	Helper " Mirna Ković ontwikkelt een chemisch middel om alle mensen in Servië " <<
	"onvruchtbaar te maken!\n"; sleep(5);
	Speler " Dat meen je niet! Waar is zij nu!?!?\n"; sleep(2);
	Helper " Zij is in het geheime laboratorium van Zadar!\n"; sleep(2);
	Speler " Ik ga er meteen achteraan! Ik moet haar tegenhouden!\n"; sleep(2);
	Helper " Ik reken op jou, " SpelerN ". Het lot van Servië ligt in jouw handen!\n\n"; 
}

void intro2(string &spelerNaam, string &helperNaam)
{
	sleep(1); cout << endl;
	Helper " " SpelerN ", wij hebben jouw hulp dringend nodig! D-8\n"; sleep(2);
	Speler " Wat is er dan, " HelperN "?\n"; sleep(2);
	Helper " Er gebeuren afgrijselijke dingen in Kroatië!\n"; sleep(2);
	Speler " Zoals wat? Waar heb jij het over?\n"; sleep(2);
	Helper " Mirna Ković bouwt daar kernraketten om Servië mee plat te gooien!\n"; sleep(4);
	Speler " WAT!?!?! ZIJ WEER!?!?!?\n"; sleep(2);
	Helper " Ja, zij weer! Mirna is weer eens bezig!\n"; sleep(3);
	Speler " Ik moet haar snel tegenhouden. Weet jij waar zij is?\n"; sleep(3);
	Helper " Jan Pieterszoon Coenstraat, 113, in Zadar!\n"; sleep(2);
	Speler " Ik ga er meteen achter aan, voordat Servië plat is!\n"; sleep(3);
	Helper " Heel Servië rekent op jou, " SpelerN "!\n\n";
}

void intro3(string &spelerNaam, string &helperNaam)
{
	sleep(1); cout << endl;
	Speler " Ah, hallo " HelperN "! Jou heb ik ook al een tijdje niet gezien!\n"; sleep(4);
	Helper " Dat klopt! Maar, ik heb geen tijd om te babbelen. Er is spoed!\n"; sleep(3);
	Speler " Wat is er dan?\n"; sleep(2);
	Helper " Het is weer de Kroatische heks!\n"; sleep(2);
	Speler " Toch niet Mirna Ković, hè?\n"; sleep(2);
	Helper " Ja, toch wel! Mirna Ković is weer bezig!\n"; sleep(2.5);
	Speler " Wat is de diagnose?\n"; sleep(2);
	Helper " Zij ontwikkelt een chemisch wapen om mensen blind te maken. " <<
	"Dat wil zij op Servië gooien!\n"; sleep(4);
	Speler " Dat is verschrikkelijk! Waar is zij nu?\n"; sleep(3);
	Helper " Zadar, in het verlaten laboratorium.\n"; sleep(2);
	Speler " Dan ga ik daar naartoe!\n"; sleep(2);
	Helper " Heel Servië rekent op jou, " SpelerN "!\n\n";
}

string naamGenerator()
{
	kint voorGetal = rand()%74, achterGetal = rand()%54;
	string voor = "Jan", achter = "Willem";
	switch (voorGetal)
	{
		case 0: voor = "Александар"; break;
		case 1: voor = "Адријана"; break;
		case 2: voor = "Александра"; break;
		case 3: voor = "Анастаија"; break;
		case 4: voor = "Анђела"; break;
		case 5: voor = "Андреја"; break;
		case 6: voor = "Андреј"; break;
		case 7: voor = "Аница"; break;
		case 8: voor = "Антонија"; break;
		case 9: voor = "Атанасија"; break;
		case 10: voor = "Биљана"; break;
		case 11: voor = "Богдан"; break;
		case 12: voor = "Богдана"; break;
		case 13: voor = "Борислав"; break;
		case 14: voor = "Бошко"; break;
		case 15: voor = "Божидар"; break;
		case 16: voor = "Божена"; break;
		case 17: voor = "Божидарка"; break;
		case 18: voor = "Бранимир";	break;
		case 19: voor = "Бранислав";	break;
		case 20: voor = "Братислав"; break;
		case 21: voor = "Далиборка"; break;
		case 22: voor = "Даница"; break;
		case 23: voor = "Дамјана"; break;
		case 24: voor = "Данијела"; break;
		case 25: voor = "Димитрије"; break;
		case 26: voor = "Доброслав"; break;
		case 27: voor = "Ђорђе"; break;
		case 28: voor = "Драгица"; break;
		case 29: voor = "Драгомир"; break;
		case 30: voor = "Драгослав"; break;
		case 31: voor = "Драшко"; break;
		case 32: voor = "Душан"; break;
		case 33: voor = "Душица";	break;
		case 34: voor = "Гаврило";	break;
		case 35: voor = "Иван";	break;
		case 36: voor = "Иванка";	break;
		case 37: voor = "Ивана";	break;
		case 38: voor = "Јелисавета";	break;
		case 39: voor = "Јована";	break;
		case 40: voor = "Константин";	break;
		case 41: voor = "Катарина";	break;
		case 42: voor = "Љуба";	break;
		case 43: voor = "Марина";	break;
		case 44: voor = "Милош";	break;
		case 45: voor = "Мирослав";	break;
		case 46: voor = "Мирослава";	break;
		case 47: voor = "Надежда";	break;
		case 48: voor = "Наташа";	break;
		case 49: voor = "Предраг";	break;
		case 50: voor = "Радмило";	break;
		case 51: voor = "Радмила";	break;
		case 52: voor = "Радослава";	break;
		case 53: voor = "Ратко";	break;
		case 54: voor = "Саша";	break;
		case 55: voor = "Слађана";	break;
		case 56: voor = "Славица";	break;
		case 57: voor = "Славомир";	break;
		case 58: voor = "Слободан";	break;
		case 59: voor = "Срећко";	break;
		case 60: voor = "Сњежана";	break;
		case 61: voor = "Станислава";	break;
		case 62: voor = "Светлана";	break;
		case 63: voor = "Тихана";	break;
		case 64: voor = "Томислав";	break;
		case 65: voor = "Урош";	break;
		case 66: voor = "Веселко";	break;
		case 67: voor = "Владислав";	break;
		case 68: voor = "Владимир";	break;
		case 69: voor = "Здравка";	break;
		case 70: voor = "Жељка";	break;
		case 71: voor = "Звездана";	break;
		case 72: voor = "Зорица";	break;
		case 73: voor = "Желимир";	break;
		
		
		default: exit(1);
	}
	
	switch (achterGetal)
	{
		case 0: achter = "Антоновић"; break;
		case 1: achter = "Адашевић"; break;
		case 2: achter = "Белаковић"; break;
		case 3: achter = "Богдановић"; break;
		case 4: achter = "Божовић"; break;
		case 5: achter = "Брозовић"; break;
		case 6: achter = "Бачић"; break;
		case 7: achter = "Глођовић"; break;
		case 8: achter = "Глогињић"; break;
		case 9: achter = "Добриловић"; break;
		case 10: achter = "Дражетић"; break;
		case 11: achter = "Демић"; break;
		case 12: achter = "Ђорђевић"; break;
		case 13: achter = "Ђуровић"; break;
		case 14: achter = "Екмеџић"; break;
		case 15: achter = "Жарковић"; break;
		case 16: achter = "Живковић"; break;
		case 17: achter = "Зубић"; break;
		case 18: achter = "Зубановић"; break;
		case 19: achter = "Југовић"; break;
		case 20: achter = "Јовановић"; break;
		case 21: achter = "Колунџић"; break;
		case 22: achter = "Кнежевић"; break;
		case 23: achter = "Ковачевић"; break;
		case 24: achter = "Љујићи"; break;
		case 25: achter = "Лазић"; break;
		case 26: achter = "Марчић"; break;
		case 27: achter = "Мачић"; break;
		case 28: achter = "Милатовић"; break;
		case 29: achter = "Мишковић"; break;
		case 30: achter = "Мићовић"; break;
		case 31: achter = "Ненадић"; break;
		case 32: achter = "Нишић"; break;
		case 33: achter = "Обрадовић"; break;
		case 34: achter = "Орловић"; break;
		case 35: achter = "Пањковић"; break;
		case 36: achter = "Пешић"; break;
		case 37: achter = "Пушоња"; break;
		case 38: achter = "Радоњић"; break;
		case 39: achter = "Рогановић"; break;
		case 40: achter = "Самарџић"; break;
		case 41: achter = "Стајковић"; break;
		case 42: achter = "Супуровић"; break;
		case 43: achter = "Томашевић"; break;
		case 44: achter = "Ћакић"; break;
		case 45: achter = "Филиповић"; break;
		case 46: achter = "Црнојевић"; break;
		case 47: achter = "Чикић"; break;
		case 48: achter = "Шешељ"; break;
		case 49: achter = "Шкребић"; break;
		case 50: achter = "Чаровић"; break;
		case 51: achter = "Перишић"; break;
		case 52: achter = "Шешум"; break;
		case 53: achter = "Зрнић"; break;
		case 54: achter = "Младић"; break;
		
		
		default: exit(1);
	}
	return voor + " " + achter;
}

void afsluiter(int /* signum */)
{
	kint min = 0, max = 10;
    switch (wilget(min, max))
	{
		case 0: cout << "\nVuile lafaard! Weglopen voor jouw verantwoordelijkheid!\n"; break;
		case 1: cout << "\nDit is diep en diep droevig. Jij laat Servië in de steek!\n"; break;
		case 2: cout << "\nBij terugkomst zul jij geëxecuteerd worden wegens landverraad!\n";
		break;
		case 3: cout << "\nOngelofelijk... Dat jij ons verlaat tijdens zo een belangrijke" <<
		" opdracht... Schaam je diep!\n"; break;
		case 4: cout << "\nMirna Ković: Ik zie dat jij afdruipt met de staart tussen de " <<
		"benen. Dat is mooi!\n"; break;
		case 5: cout << "\nMirna Ković: Ik wist wel dat jij het niet kon. Jij bent een " <<
		"mislukking! \n"; break;
		case 6: cout << "\nMirna Ković: Vluchten kan niet meer! Eerst vernietig ik Servië, "
		<< "en dan ben jij aan de beurt!\n"; break;
		case 7: cout << "\nMirna Ković: hahahahahahahahahaHAAAAAAAAAAAAAAAHAHAHAHAHAHAHAHAHA"
		<< "HAHAHAHAHAHA!!!\n"; break;
		case 8: cout << "\nРатко Младић: Ik ben diep en diep in jou teleurgesteld!\n"; break;
		case 9: cout << "\nDe duivel: Hè, hè, ik dacht dat jij nooit zou gaan!\n"; break;
		case 10: cout << "\nDe duivel: Goedzo! Ga maar weg. Laat Mirna Ković haar werk doen!"
		<< endl; break;
		default: exit(1);
	}
	exit(0);
	
}

void keuzeSpeler(Persoon &teg, Persoon &speler)
{
	kint keuze = 0; string helperNaam = naamGenerator();
	while(true)
	{
	cout << "\nGezondheid " << speler.toonNaam() << ": " << speler.toonGezondheid() << endl;
	cout << "Gezondheid " << teg.toonNaam() << ": " << teg.toonGezondheid() << endl;
	cout << "\nMogelijkheden:" << endl;
	cout << "1) Klap geven" << endl;
	cout << "2) Trap geven" << endl;
	cout << "3) Genezen" << endl;
	cout << "4) Niks doen (niet aan te raden)" << endl;
	cout << "5) Vluchten" << endl;
	cout << "6) Opgeven" << endl;
	cout << "7) Bekijk statistieken (kost geen beurt)" << endl;
	cout << "\nKeuze: "; cin >> keuze;
	while(cin.fail() || keuze > 7 || keuze < 1){INVOERFOUT(keuze);}
	switch (keuze)
	{
		case 1: speler.stoot(teg); controle(teg, speler); teg.keuzeVijand(teg, speler);
		controle(teg, speler); break;
		case 2: speler.trapt(teg); controle(teg, speler); teg.keuzeVijand(teg, speler);
		controle(teg, speler); break;
		case 3: speler.vraagGenezing(); teg.keuzeVijand(teg, speler);
		controle(teg, speler); break;
		case 4: speler.niksdoen(); teg.keuzeVijand(teg, speler); controle(teg, speler); break;
		case 5: speler.vluchtwegvan(teg, speler); teg.keuzeVijand(teg, speler);
		controle(teg, speler); break;
		case 6: speler.geeftzichoveraan(teg, helperNaam); break;
		case 7: speler.toonStatistieken(); teg.toonStatistieken();
		break;
		default: exit(1);
	}
	}
	
}

void speelMenu(Persoon &teg, Persoon &speler)
{
    kint keuze; if (!speler.m_Valsspeelmode) { speler.m_Gezondheid = 100; }
	if(teg.m_VijandNummer == -22){Mirna(teg, speler);}
	cout << "\nChapeau, " << speler.toonNaam() << "! Jij bent nu in Zadar!\n\n"; sleep(1);
	cout << "Om bij Mirna Ković te komen, moet jij eerst de mensen om haar heen " <<
	"uitschakelen. Zij wordt namelijk beschermd door deze mensen hier:\n\n"; sleep(1);
    cout << "\t1) Jacob Rothshild "; if (!teg.levend(0)) { cout<<"(DOOD)"; }
	teg.gevangen(0)?cout<<"(GEVANGEN)":cout << ""; slaap;
    cout << "\t2) André van Kemenade "; if (!teg.levend(1)) { cout<<"(DOOD)"; }
	teg.gevangen(1)?cout<<"(GEVANGEN)":cout << ""; slaap;
    cout << "\t3) Arie Beemsterboer "; if (!teg.levend(2)) { cout<<"(DOOD)"; }
	teg.gevangen(2)?cout<<"(GEVANGEN)":cout << ""; slaap;
    cout << "\t4) Kasperus Smuider "; if (!teg.levend(3)) { cout<<"(DOOD)"; }
	teg.gevangen(3)?cout<<"(GEVANGEN)":cout << ""; slaap;
    cout << "\t5) Frederick van den Berg "; if (!teg.levend(4)) { cout<<"(DOOD)"; }
	teg.gevangen(4)?cout<<"(GEVANGEN)":cout << ""; slaap;
    cout << "\t6) Lihanna Ković "; if (!teg.levend(5)) { cout<<"(DOOD)"; }
	teg.gevangen(5)?cout<<"(GEVANGEN)":cout << ""; slaap;
    cout << "\t7) Kloon speler "; if (!teg.levend(6)) { cout<<"(DOOD)"; }
	teg.gevangen(6)?cout<<"(GEVANGEN)":cout << ""; slaap;
	cout << "\t 999) Spel afsluiten\n";
	cout << "\nMet wie wil jij vechten? Voer een getal in: "; cin >> keuze;
	while((cin.fail() || keuze > AANTAL_VIJANDEN || keuze < 1) && keuze != 1855 &&
	keuze != 999) {INVOERFOUT(keuze);} switch (keuze)
	{
		case 1: teg.instellen("Jacob Rothshild",		0,100,20,5,0,70, 11, 1); break;
		case 2: teg.instellen("André van Kemenade",		1,100,25,10,5,65 ,18, 1); break;
		case 3: teg.instellen("Arie Beemsterboer",		2,100,30,15,10,60 ,17, 1); break;
		case 4: teg.instellen("Kasperus Smuider",		3,100,35,20,15,80 ,16, 0); break;
		case 5: teg.instellen("Frederick van den Berg",	4,100,40,25,20,75 ,15, 0); break;
		case 6: teg.instellen("Lihanna Ković",			5,100,35,20,30,75 ,14, 0); break;
		case 7: teg.instellen("Kloon speler",			6,100,30,20,20,100,17, 0); break;
        case 999: afsluiter(0); break;
		case 1855: speler.valsspeelMode(); speelMenu(teg, speler); break;
		default: exit(1);
	}
	if(teg.levend(teg.m_VijandNummer) == false)
	{
		cout << "\n\nHelaas, maar " << teg.m_Naam << " is al dood. " <<
		"Kies een andere tegenstander uit!"; cout << endl; sleep(3); speelMenu(teg, speler);
	}
	if(teg.gevangen(teg.m_VijandNummer) == true)
	{
		cout << "\n\n" << teg.m_Naam << " leeft in jouw gevangenschap.\n " <<
		"Wat wil jij met " << teg.m_Naam << " doen?\n\n"; slaap;
		cout << "1) Vrijlaten\n2) Vermoorden\n3) Gevangen houden en een andere " <<
		"tegenstander uitzoeken.\n\nKeuze: "; cin >> keuze;
		while(cin.fail() || keuze > 3 || keuze < 1){INVOERFOUT(keuze);}
		switch (keuze)
		{
			case 1: teg.m_Gevangen[teg.m_VijandNummer] = false;  cout << "\nJij hebt " <<
			teg.m_Naam << " vrijgelaten!\n"; speelMenu(teg, speler); break;
			case 2: while(teg.m_Gezondheid > 0){speler.trapt(teg);}
			cout << "\nJij hebt " << teg.m_Naam << " vermoord!\n";
					teg.m_Gevangen[teg.m_VijandNummer] = false;
					teg.m_Levend[teg.m_VijandNummer] = false; speelMenu(teg, speler); break;
			case 3: speelMenu(teg, speler); break;
			default: exit(1);
		}
	}
	keuzeSpeler(teg, speler);
}

void overwinningsscherm(Persoon &speler)
{
	cout << "\n\n"; kint keuze = 0; string codeONT = "lijav9m9ce1krwioiinodavevnletkkes9ink",
	codeDOEL = "emetteaaaiiendwkornkfiuoacnegapvlineehrrosdeoehmeovriatgiklaer", doel = "",
	naam = naamGenerator();
	random_shuffle(codeONT.begin(), codeONT.end());
	random_shuffle(codeDOEL.begin(), codeDOEL.end()); sleep(2);
	cout << "Jij staat snel op om de code in het kastje te lezen.\n";
	SP " Hmmm... Er staan twee codes op... Één voor het ontmantelen van de raketten, en " <<
	"één om een ander doelwit in te stellen... Welke code zal ik doorgeven?\n\n";
	cout << "1) Ontmantelen"; slaap;
	cout << "2) Ander doelwit instellen"; slaap; cout << "Keuze: "; cin >> keuze;
	while(cin.fail() || keuze > 2 || keuze < 1){INVOERFOUT(keuze);} cout << endl;
	if(keuze == 1)
	{
		SP " Attentie! Code " << codeONT << ". Ontmantel alle raketten!" << endl; sleep(2);
		cout << "Commandant: Weet jij het zeker?" << endl; sleep(2);
		SP " ALLE RAKETTEN ONTMANTELEN! ALS IK HET MOET HERHALEN, " <<
		"KOM JIJ VOOR DE KRIJGSRAAD!" << endl; sleep(3);
		cout << "Commandant: Alle raketlanceringen zijn ongedaan gemaakt!" << endl; sleep(2);
		SP " Mooi!" << endl; sleep(1);
	}
	else
	{
		cout << "Welk doel had jij voor ogen?\n\n";
		cout << "1) Het Witte Huis" << endl;
		cout << "2) Het Europees Parlement" << endl;
		cout << "3) Oekraïense RADA" << endl;
		cout << "4) Pyongyang (niet aan te raden)" << endl;
		cout << "5) Kremlin (ook niet aan te raden)" << endl;
		cout << "6) Pieter Janszoon Jongstraat 131, Lutjebroek" << endl;
		cout << "7) Het huis van " << speler.toonNaam() << endl;
		cout << "8) Het huis van George Soros" << endl << endl << "Keuze: "; cin >> keuze;
		while(cin.fail() || keuze > 8 || keuze < 1){INVOERFOUT(keuze);}
		switch (keuze)
		{
			case 1: doel = "Het Witte Huis"; break;
			case 2: doel = "Het Europees Parlement"; break;
			case 3: doel = "Oekraïense RADA"; break;
			case 4: cout << "\nBen je gek? Wij gaan geen oorlogvoeren met Noord-Korea!\n";
			cout << "Ik stel het doel wel in op het Witte Huis!\n"; doel = "Het Witte Huis";
			break;
			case 5: cout << "\nBen je gek? Wij gaan geen oorlogvoeren met Rusland!\n";
			cout << "Ik stel het doel wel in op het huis van George Soros!\n";
			doel = "Het huis van George Soros"; break;
			case 6: doel = "Pieter Janszoon Jongstraat 131, Lutjebroek"; break;
			case 7: doel = "Het huis van " + speler.toonNaam(); break;
			case 8: doel = "Het huis van George Soros"; break;
			default: exit(1);
		}
		
		SP " Attentie! Code " << codeDOEL << ". Stuur alle raketten naar " << doel <<
		"." << endl; sleep(2);
		cout << "Commandant: Weet jij het zeker?" << endl; sleep(2);
		SP " ALLE RAKETTEN OP DOEL " << doel << ". ALS IK HET MOET HERHALEN, " <<
		"KOM JIJ VOOR DE KRIJGSRAAD!" << endl; sleep(3);
		cout << "Commandant: Alle raketlanceringen zijn op " << doel << " gericht!" << endl;
		sleep(2);
		SP " Mooi!" << endl; sleep(1);
		cout << "\n\t Een paar uur later...\n"; sleep(1);
		cout << "\n" << doel << " ligt helemaal in de as!\n"; sleep(2);
		SP " HAHAHAHAHAHAHAHAHAHAHAHAHAHAHAHHA! Dat heb ik gedaan! :-D \n\n"; sleep(2);
	}
	cout << "Van harte gefeliciteerd, " << speler.toonNaam() << "! Jij hebt Servië gered!\n";
	cout << "Jij bent een ware held! Jij zette door in hele moeilijke tijden en hebt het " <<
	"Servische volk een grote dienst bewezen. Jij zult in Servië gehonoreerd worden als grote "
	<< "held!" << endl << endl;
	cout << naam << ": Ik ben blij dat jij het gered hebt, " << speler.toonNaam() << "!\n";
	sleep(1); cout << "Ратко Младић: Ik ben trots op jou, " << speler.toonNaam() << "!\n";
	sleep(1); cout << "Слободан Милошевић: Woorden schieten tekort... Jij bent een held, " <<
	speler.toonNaam() << "!\n";
	cout << "\n Als jij dit spel nog een keer wilt spelen, maar dan in de valsspeelmode, " <<
	"gebruik dan de valsspeelcode 1855 in het hoofdmenu.\n\n"; exit(0);
}

void controle(Persoon &teg, Persoon &speler)
{
	kint aantal = 0;
	
	if(teg.m_Gezondheid <= 0)
	{
		teg.m_Levend[teg.m_VijandNummer] = false;
		for(kint i = 0; i < AANTAL_VIJANDEN; i++)
		{
			teg.levend(i) ? aantal++ : aantal = aantal + 1 - 1;
			teg.gevangen(i) ? aantal-- : aantal = aantal + 1 - 1;
		}
		cout << "Gefeliciteerd! Jij hebt " << teg.toonNaam() << " vermoord!\n";
		if(aantal == 0 && !BMG){cout << "Op naar Mirna Ković!\n"; Mirna(teg, speler);}
		if(aantal == 1 && !BMG){cout << "Nog één vijand te gaan!\n";}
		if(aantal > 2 && !BMG){cout << "Nog " << aantal << " vijanden te gaan!\n";}
		teg.m_VijandNummer == -22 ? overwinningsscherm(speler):speelMenu(teg, speler);
	}
	aantal = 0;
	for(kint i = 0; i < AANTAL_VIJANDEN; i++)
	{
		teg.levend(i) ? aantal++ : aantal = aantal + 1 - 1;
		teg.gevangen(i) ? aantal-- : aantal = aantal + 1 - 1;
	}
	if(aantal == 0 && !BMG){Mirna(teg, speler);}
	if(teg.gevangen(-22)){overwinningsscherm(speler);}
	if(speler.m_Gezondheid <= 0)
	{
		cout << "\n\n\tHelaas, jij bent dood. " << teg.toonNaam() << " heeft jou vermoord.\n";
		cout << "Jij hebt de verwoesting van het Servische volk niet kunnen voorkomen.\n";
		cout << "Jij zult de geschiedenis in gaan als de persoon die Servië niet heeft kunnen"
		 << " redden.\n";
		exit(0);
	}
}

int main()
{
	srand(time(NULL)); signal(SIGINT, afsluiter); kint keuze1 = rand()%3;
	string spelerNaam/* = naamGenerator()*/, helperNaam = naamGenerator(); char keuze2;
	cout << "\n\n\t\t Hallo! Hoe heet jij?    "; getline(cin, spelerNaam);
	cout << "\n\n\t\t Jouw naam is " SpelerN ".\n";
	cout << "\nWil jij het verhaal overslaan? (J/N)    "; cin >> keuze2;
	if(keuze2 == 'j' || keuze2 == 'J')
	{
		cout << "\nVerhaal overgeslagen!\n";
	}
	else
	{
		switch (keuze1)
		{
			case 0: intro1(spelerNaam, helperNaam); break;
			case 1: intro2(spelerNaam, helperNaam); break;
			case 2: intro3(spelerNaam, helperNaam); break;
			default: return 0;
		}
	}
	Persoon teg, speler(spelerNaam, CODE_SPELER, GEZONDHEID_SPELER, GENEESKRACHT_SPELER,
	ARMKRACHT_SPELER, BEENKRACHT_SPELER, 60, SNELHEID_SPELER, false, false, false);
	speelMenu(teg, speler);
	
	
	
	return 0;
}

// rand()%((max)-min)+min;
// 
