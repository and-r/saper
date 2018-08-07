#pragma once
#include<ctime>
#include<windows.h>  //potrzebne do Sleep()

//#define PL  //jezyk polski - przelacznik kompilacji

//deklaracja zapowiadajaca
class cWyswietlacz;
class cObiektPlansza;
enum STAN_GRY { PYTANIA, ROZBRAJANIE, UDALOSIE, DETONACJA };
enum STAN_POLA {
	MINA = -6, PUSTE = 10, OBOK1 = 1, OBOK2 = 2, OBOK3 = 3, OBOK4 = 4, OBOK5 = 5,
	OBOK6 = 6, OBOK7 = 7, OBOK8 = 8
};
enum ODKRYTOSC { ZAKRYTE = 0, ODKRYTE = 1, FLAGA = 2 };
struct FULL_STAN_POLA
{
	STAN_POLA Zawartosc;
	ODKRYTOSC Odkryte;
};
//----------------klasa gry--------------------
//
class cGra
{
private:
	static cGra* wskGra;//wskaznik do obiektu klasy cGra
	cWyswietlacz* wskWyswietlacz{};
	cObiektPlansza* wskObiektPlansza{};
	char Znak;
	int iZnak;
	int iLicznik;
	cGra()
	{
		iPlanszaRozmiarX;
		iPlanszaRozmiarY;
	}//konstruktor po utworzeniu ustawia wskaznik na obiekt

public:
	static STAN_GRY StanGry;
	int iPlanszaRozmiarX{};
	int iPlanszaRozmiarY{};
	int iMinyNum{};
	time_t CzasStart;
	time_t CzasStop;
	~cGra();//destruktor
	static cGra* ObiektGra()
	{
		//if (wskGra == NULL) cGra();
		if (wskGra == NULL) wskGra = new cGra;//tworzy obiekt jesli jeszcze nie istnieje
		return wskGra;//zwraca wskaznik do obiektu klasy cGra a 
	}
	void Start();//kod metody w pliku main
	void Translacja();//kod metody w pliku main, analizuje i przetwarza pola obiektu cObiektPlansza na cWyswietlacz
	//czyli prawdziwa zawartosc planszy na to co powinien zobaczyc gracz

};
//
//---------------klasa cWyswietlacz-----------------
//
class cWyswietlacz
{
private:
	int iRozmX;
	int iRozmY;
	int kursorX;
	int kursorY;
	char** wskMatryca;
public:
	cWyswietlacz() { kursorX = 1; kursorY = 1; }//konstruktor bezparametrowy
	cWyswietlacz(int sizeX, int sizeY)//konstruktor z parametrami
	{
		iRozmX = sizeX;
		iRozmY = sizeY;
		kursorX = 1;
		kursorY = 1;
		wskMatryca = NULL;
	}
	~cWyswietlacz()//destruktor
	{
		//usuniecie matrycy
		if (!wskMatryca == NULL)
		{
			for (int cntr = 0; cntr <iRozmX; ++cntr)
			{
				delete[] wskMatryca[cntr];
			}
			delete[] wskMatryca;
		}
	}
	bool Init();
	void Odswiez(bool StanKursora, int Counter);
	void Set(int PoleX, int PoleY, int Znak);
	bool RuszKursor(int iKlawisz);
	int GetKursorX() { return kursorX; }
	int GetKursorY() { return kursorY; }
};
//
//---------------klasa cObiektPlansza-----------------
//
class cObiektPlansza
{
private:
	FULL_STAN_POLA** wskPlansza;
	int iRozmX;
	int iRozmY;
public:
	cObiektPlansza(int sizeX, int sizeY)//konstruktor
	{
		iRozmX = sizeX;
		iRozmY = sizeY;
		wskPlansza = NULL;
		if ((iRozmX < 1 || iRozmX > 70) || (iRozmY < 1 || iRozmY > 70)) { return; }
		//utworzenie tablicy dynamicznej
		wskPlansza = new FULL_STAN_POLA*[iRozmX];
		for (int cntr = 0; cntr < iRozmX; ++cntr) { wskPlansza[cntr] = new FULL_STAN_POLA[iRozmY]; }
		//wypenianie tablicy
		for (int cntr2 = 0; cntr2 < iRozmY; ++cntr2)
			for (int cntr = 0; cntr < iRozmX; ++cntr)
			{
				wskPlansza[cntr][cntr2].Zawartosc = PUSTE;
				wskPlansza[cntr][cntr2].Odkryte = ZAKRYTE;
			}
	}
	~cObiektPlansza()//destruktor
	{
		//usuniecie tablicy
		if (!wskPlansza == NULL)
		{
			for (int cntr = 0; cntr <iRozmX; ++cntr)
			{
				delete[] wskPlansza[cntr];
			}
			delete[] wskPlansza;
		}
	}
	bool Zaminuj(int LiczbaMin);//prototyp
	FULL_STAN_POLA* Get(int PoleX, int PoleY);//protoryp
	void Sasiedzi();//prototyp, oblicza ilosc min sasiadujacych z polem  i wstawia odpowiednia liczbe na plansze obiektu cObiektPlansza
	bool Akcja(int PoleX, int PoleY);//prototyp
	void Burn(int PoleX, int PoleY);//prototyp
	ODKRYTOSC ZmienFlage(int PoleX, int PoleY);//prototyp
};