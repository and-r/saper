#include <iostream>
#include <conio.h>
#include <ctime>
#include "saper.h"
using namespace std;
//deklaracja zmiennych statycznych klasy cGra
STAN_GRY cGra::StanGry = PYTANIA;
cGra* cGra::wskGra = NULL; //wskaünik statyczny do obiektu klasy cGra ustawia na null 
cGra::~cGra()
{
	delete wskWyswietlacz;
	wskWyswietlacz = NULL;
	delete wskObiektPlansza;
	wskObiektPlansza = NULL;
	wskGra = NULL;
}
bool cWyswietlacz::Init()
{
	if ((iRozmX < 1 || iRozmX > 70) || (iRozmY < 1 || iRozmY > 70))
	{
#ifdef PL
		cout << "nieprawidlowy rozmiar matrycy"<<endl;
#else
		cout << "invalid matrix size"<<endl;
#endif
		Sleep(1000);
		return false;
	}
	wskMatryca = new char*[iRozmX];
	for (int cntr = 0; cntr < iRozmX; ++cntr) { wskMatryca[cntr] = new char[iRozmY]; }
	//wypenianie tablicy
	for (int cntr2 = 0; cntr2 < iRozmY; ++cntr2)
		for (int cntr = 0; cntr < iRozmX; ++cntr)
		{
			wskMatryca[cntr][cntr2] = static_cast<char>(219);
		}
	return true;
}
void cWyswietlacz::Odswiez(bool StanKursora, int Counter)
{
	if ((wskMatryca == NULL) || (iRozmX == 0 || iRozmY == 0))
	{
		return;
	}
	for (int cntr2 = 0; cntr2 < iRozmY; ++cntr2)
	{
		for (int cntr = 0; cntr < iRozmX; ++cntr)
		{
			if ((cntr == kursorX - 1) && (cntr2 == kursorY - 1) && (StanKursora == true)) cout << static_cast<char>(35);
			else cout << wskMatryca[cntr][cntr2];
		}
		cout << endl;
	}
	cout << endl;
	if (Counter > 0)
	{

#ifdef PL
		cout << "MINY: " << Counter << endl;
#else
		cout << "MINES: " << Counter << endl;
#endif
	}
}
bool cWyswietlacz::RuszKursor(int iKlawisz)
{
	switch (iKlawisz)
	{
	case 72:
	{
		if (kursorY > 1) --kursorY;
		return true; }
	case 80:
	{
		if (kursorY < iRozmY) ++kursorY;
		return true; }
	case 75:
	{
		if (kursorX > 1) --kursorX;
		return true; }
	case 77:
	{
		if (kursorX < iRozmX) ++kursorX;
		return true; }
	default: return false;
	}
}
bool cObiektPlansza::Zaminuj(int LiczbaMin)
{
	int randomX;
	int randomY;
	if (wskPlansza == NULL || LiczbaMin > (iRozmX * iRozmY) || LiczbaMin <=0)
	{
#ifdef PL
		cout << "Nieprawidlowa liczba min lub inne dane sa nieprawidlowe" << endl;
#else
		cout << "Invalid number of mines or other data is invalid" << endl;
#endif
		Sleep(1000);
		return false;
	}
	for (int cntr = 1; cntr <= LiczbaMin; ++cntr)
	{
		for (;;)
		{
			randomX = rand() % iRozmX;
			randomY = rand() % iRozmY;
			if (wskPlansza[randomX][randomY].Zawartosc != MINA)
			{
				wskPlansza[randomX][randomY].Zawartosc = MINA;
				break;
			}
		}
	}
	return true;
}
void cGra::Translacja()
{
	int iZakryteLicznik = iPlanszaRozmiarX * iPlanszaRozmiarY;
	FULL_STAN_POLA* wskPoleInfo;
	if ((iPlanszaRozmiarX == 0 || iPlanszaRozmiarY == 0) ||
		(wskWyswietlacz == NULL) || (wskObiektPlansza == NULL))
	{
		return;
	}
	for (int cntr2 = 1; cntr2 <= iPlanszaRozmiarY; ++cntr2)
		for (int cntr = 1; cntr <= iPlanszaRozmiarX; ++cntr)
		{
			wskPoleInfo = wskObiektPlansza->Get(cntr, cntr2);//pobranie danych z ObiektPlansza
			if (StanGry == DETONACJA) wskPoleInfo->Odkryte = ODKRYTE;//jesli to koniec - pokaz wszystkie pola
			if ((wskPoleInfo->Zawartosc != PUSTE) && (wskPoleInfo->Odkryte == ODKRYTE))
			{
				wskWyswietlacz->Set(cntr, cntr2, static_cast<int>(wskPoleInfo->Zawartosc) + 48);//pokazuje liczby i miny
			}
			if ((wskPoleInfo->Odkryte == ODKRYTE) && (wskPoleInfo->Zawartosc == PUSTE))
				wskWyswietlacz->Set(cntr, cntr2, 176);//puste i odkryte pole
			if (wskPoleInfo->Odkryte == ZAKRYTE)
				wskWyswietlacz->Set(cntr, cntr2, 219);//zakryte
			if (wskPoleInfo->Odkryte == FLAGA)
				wskWyswietlacz->Set(cntr, cntr2, 39);//flaga

			if (wskPoleInfo->Odkryte == ODKRYTE)
			{
				--iZakryteLicznik;
			}
		}
	if (iZakryteLicznik == iMinyNum)
		StanGry = UDALOSIE;
}
FULL_STAN_POLA* cObiektPlansza::Get(int PoleX, int PoleY)
{
	if (PoleX <= 0 || PoleX > iRozmX || PoleY <= 0 || PoleY > iRozmY)
	{
#ifdef PL
		cout << "niewlasciwe zapytanie o pole planszy" << endl;
#else
		cout << "invalid request for a field of the board" << endl;
#endif
		return NULL;
	}
	return &wskPlansza[PoleX - 1][PoleY - 1];
}
void cWyswietlacz::Set(int PoleX, int PoleY, int Znak)
{
	if (wskMatryca == NULL || (PoleX <= 0 || PoleX > iRozmX || PoleY <= 0 || PoleY > iRozmY))
		return;
	wskMatryca[PoleX - 1][PoleY - 1] = static_cast<char>(Znak);
}
void cObiektPlansza::Sasiedzi()
{
	int TotalMiny = 0;
	if (wskPlansza == NULL || iRozmX < 1 || iRozmY <1) return;
	for (int cntr2 = 0; cntr2 < iRozmY; ++cntr2)
		for (int cntr = 0; cntr < iRozmX; ++cntr)
		{
			TotalMiny = 0;
			if (cntr != 0)
				if (wskPlansza[cntr - 1][cntr2].Zawartosc == MINA)//jesli mina z lewej
					++TotalMiny;
			if ((cntr != 0) && (cntr2 < iRozmY - 1))
				if (wskPlansza[cntr - 1][cntr2 + 1].Zawartosc == MINA)//jesli mina z lewej i ponizej
					++TotalMiny;
			if (cntr2 < iRozmY - 1)
				if (wskPlansza[cntr][cntr2 + 1].Zawartosc == MINA)//jesli mina ponizej
					++TotalMiny;
			if ((cntr < iRozmX - 1) && (cntr2 < iRozmY - 1))
				if (wskPlansza[cntr + 1][cntr2 + 1].Zawartosc == MINA)//jesli mina z prawej i ponizej
					++TotalMiny;
			if (cntr < iRozmX - 1)
				if (wskPlansza[cntr + 1][cntr2].Zawartosc == MINA)//jesli mina z prawej
					++TotalMiny;
			if ((cntr < iRozmX - 1) && (cntr2 != 0))
				if (wskPlansza[cntr + 1][cntr2 - 1].Zawartosc == MINA)//jesli mina z prawej i powyzej
					++TotalMiny;
			if (cntr2 != 0)
				if (wskPlansza[cntr][cntr2 - 1].Zawartosc == MINA)//jesli mina powyzej
					++TotalMiny;
			if ((cntr != 0) && (cntr2 != 0))
				if (wskPlansza[cntr - 1][cntr2 - 1].Zawartosc == MINA)//jesli mina z lewej i powyzej
					++TotalMiny;
			if ((wskPlansza[cntr][cntr2].Zawartosc != MINA) && (TotalMiny != 0))//jesli nie ma w biezacym polu
			{
				wskPlansza[cntr][cntr2].Zawartosc = static_cast<STAN_POLA>(TotalMiny);
			}

		}
}
bool cObiektPlansza::Akcja(int PoleX, int PoleY)
{
	if (wskPlansza[PoleX - 1][PoleY - 1].Odkryte == FLAGA)
		return false;
	if (wskPlansza[PoleX - 1][PoleY - 1].Zawartosc == MINA)
	{
		return true;
	}
	if (wskPlansza[PoleX - 1][PoleY - 1].Zawartosc == PUSTE)
	{
		Burn(PoleX, PoleY);
		return false;//rozpocznij Burn
	}

	wskPlansza[PoleX - 1][PoleY - 1].Odkryte = ODKRYTE;
	return false;
}
void cObiektPlansza::Burn(int PoleX, int PoleY)
{
	wskPlansza[PoleX - 1][PoleY - 1].Odkryte = ODKRYTE;//biezace pole
													   //pola sasiednie:
	if (PoleX > 1 && PoleY > 1)//pole z lewej i powyzej
	{
		if (wskPlansza[PoleX - 2][PoleY - 2].Odkryte == ZAKRYTE)
		{
			if (wskPlansza[PoleX - 2][PoleY - 2].Zawartosc == PUSTE)
				Burn(PoleX - 1, PoleY - 1);
			else
				wskPlansza[PoleX - 2][PoleY - 2].Odkryte = ODKRYTE;
		}
	}
	if (PoleX > 1)//pole z lewej
	{
		if (wskPlansza[PoleX - 2][PoleY - 1].Odkryte == ZAKRYTE)
		{
			if (wskPlansza[PoleX - 2][PoleY - 1].Zawartosc == PUSTE)
				Burn(PoleX - 1, PoleY);
			else
				wskPlansza[PoleX - 2][PoleY - 1].Odkryte = ODKRYTE;
		}
	}
	if (PoleX > 1 && PoleY < iRozmY)//pole z lewej i ponizej
	{
		if (wskPlansza[PoleX - 2][PoleY].Odkryte == ZAKRYTE)
		{
			if (wskPlansza[PoleX - 2][PoleY].Zawartosc == PUSTE)
				Burn(PoleX - 1, PoleY + 1);
			else
				wskPlansza[PoleX - 2][PoleY].Odkryte = ODKRYTE;
		}
	}
	if (PoleX < iRozmX && PoleY < iRozmY)//pole z prawej i ponizej
	{
		if (wskPlansza[PoleX][PoleY].Odkryte == ZAKRYTE)
		{
			if (wskPlansza[PoleX][PoleY].Zawartosc == PUSTE)
				Burn(PoleX + 1, PoleY + 1);
			else
				wskPlansza[PoleX][PoleY].Odkryte = ODKRYTE;
		}
	}
	if (PoleX < iRozmX)//pole z prawej
	{
		if (wskPlansza[PoleX][PoleY - 1].Odkryte == ZAKRYTE)
		{
			if (wskPlansza[PoleX][PoleY - 1].Zawartosc == PUSTE)
				Burn(PoleX + 1, PoleY);
			else
				wskPlansza[PoleX][PoleY - 1].Odkryte = ODKRYTE;
		}
	}
	if (PoleX < iRozmX && PoleY > 1)//pole z prawej i powyzej
	{
		if (wskPlansza[PoleX][PoleY - 2].Odkryte == ZAKRYTE)
		{
			if (wskPlansza[PoleX][PoleY - 2].Zawartosc == PUSTE)
				Burn(PoleX + 1, PoleY - 1);
			else
				wskPlansza[PoleX][PoleY - 2].Odkryte = ODKRYTE;
		}
	}
	if (PoleY > 1)//pole powyzej
	{
		if (wskPlansza[PoleX - 1][PoleY - 2].Odkryte == ZAKRYTE)
		{
			if (wskPlansza[PoleX - 1][PoleY - 2].Zawartosc == PUSTE)
				Burn(PoleX, PoleY - 1);
			else
				wskPlansza[PoleX - 1][PoleY - 2].Odkryte = ODKRYTE;
		}
	}
	if (PoleY < iRozmY)//pole ponizej
	{
		if (wskPlansza[PoleX - 1][PoleY].Odkryte == ZAKRYTE)
		{
			if (wskPlansza[PoleX - 1][PoleY].Zawartosc == PUSTE)
				Burn(PoleX, PoleY + 1);
			else
				wskPlansza[PoleX - 1][PoleY].Odkryte = ODKRYTE;
		}
	}
}
ODKRYTOSC cObiektPlansza::ZmienFlage(int PoleX, int PoleY)
{
	if (wskPlansza[PoleX - 1][PoleY - 1].Odkryte == ZAKRYTE)
	{
		wskPlansza[PoleX - 1][PoleY - 1].Odkryte = FLAGA;
		return FLAGA;
	}
	else
		if (wskPlansza[PoleX - 1][PoleY - 1].Odkryte == FLAGA)
		{
			wskPlansza[PoleX - 1][PoleY - 1].Odkryte = ZAKRYTE;
			return ZAKRYTE;
		}
	return ODKRYTE;
}
void cGra::Start() //START tu przebiegaja kolejne kroki dzialania programu
{
	bool PokazKursor = true;
	StanGry = PYTANIA;
	cout << "**********          SAPER          **********" << endl << endl;
#ifdef PL
	cout << "Podaj rozmiar X planszy: ";
	cin >> iPlanszaRozmiarX;
	cout << "Podaj rozmiar Y planszy: ";
	cin >> iPlanszaRozmiarY;
	cout << "Podaj ilosc min: ";
	cin >> iMinyNum;
	cout << endl;
	iLicznik = iMinyNum;
	cout << "Ruch kursora: strzalki, oczyszczanie pola: spacja lub enter," << endl;
	cout << "flaga: 'F', wyjscie: ESC" << endl;
	cout << "Przycisnij dowolny klawisz aby rozpoczac..." << endl;
#else
	cout << "Type horizontal size of the board: ";
	cin >> iPlanszaRozmiarX;
	cout << "Type vertical size of the board: ";
	cin >> iPlanszaRozmiarY;
	cout << "Type number of mines: ";
	cin >> iMinyNum;
	cout << endl;
	iLicznik = iMinyNum;
	cout << "Cursor control: arrow keys, field clearing: space or enter," << endl;
	cout << "flag: 'F', exit: ESC" << endl;
	cout << "Press any key to begin..." << endl;
#endif
	_getch();
	cGra::StanGry = ROZBRAJANIE;
	wskWyswietlacz = new cWyswietlacz(iPlanszaRozmiarX, iPlanszaRozmiarY);
	if (!wskWyswietlacz->Init())
	{
		return;  //nieprawidlowe dane, konczymy program
	}
	
	// zainicjowanie generatora liczb pseudolosowych aktualnym czasem
	srand(static_cast<unsigned int>(time(NULL)));
	//-
	// rozpoczecie naliczania czasu
	CzasStart = time(NULL);
	///
	wskObiektPlansza = new cObiektPlansza(iPlanszaRozmiarX, iPlanszaRozmiarY);
	if (!wskObiektPlansza->Zaminuj(iMinyNum))
	{
		return;  //nieprawidlowe dane, konczymy program
	}
	wskObiektPlansza->Sasiedzi();
	Translacja();
	do //obsluga klawiatury
	{
		system("cls");
		wskWyswietlacz->Odswiez(PokazKursor, iLicznik);
		Znak = _getch();//tutaj gra sie zatrzymuje i czeka na sygnal z klawiatury
		iZnak = static_cast<int>(Znak);
		//cout <<"znak: "<< Znak <<" kod: "<< iZnak;
		while (_kbhit())
		{
			Znak = _getch();
			iZnak = static_cast<int>(Znak);
			//cout <<" znak: "<< Znak <<" kod: "<< iZnak;
		}
		cout << endl;
		PokazKursor = wskWyswietlacz->RuszKursor(iZnak);
		if (iZnak == 102 || iZnak == 70)//postaw lub usun flage
		{
			switch (wskObiektPlansza->ZmienFlage(wskWyswietlacz->GetKursorX(), wskWyswietlacz->GetKursorY()))
			{
			case FLAGA:
				if (iLicznik > 0) { --iLicznik; break; }
			case ZAKRYTE:
				++iLicznik;
			}
		}
		else
		{
			if (iZnak == 13 || iZnak == 32)
			{
				if (wskObiektPlansza->Akcja(wskWyswietlacz->GetKursorX(), wskWyswietlacz->GetKursorY()) == true)
					cGra::StanGry = DETONACJA;
			}//wywolanie akcji - odkrycie pola
		}
		Translacja();
	} while ((Znak != 27) && (StanGry != DETONACJA) && (StanGry != UDALOSIE)); //ESC
	system("cls");
	wskWyswietlacz->Odswiez(false, -1);
#ifdef PL
	if (StanGry == DETONACJA) cout << "MINA WYBUCHLA!!!" << endl;
	else
		if (StanGry == UDALOSIE) cout << "ZWYCIESTWO!!!" << endl;
	CzasStop = time(NULL);
	cout << "czas: " << static_cast<int>(CzasStop - CzasStart) << " s" << endl;
	cout << endl;
	cout << "autor gry: Andrzej Rzoska, 2013-2018" << endl;
#else
	if (StanGry == DETONACJA) cout << "MINE HAS EXPLODED!!!" << endl;
	else
		if (StanGry == UDALOSIE) cout << "VICTORY!!!" << endl;
	CzasStop = time(NULL);
	cout << "time: " << static_cast<int>(CzasStop - CzasStart) << " s" << endl;
	cout << endl;
	cout << "game by Andrzej Rzoska, 2013-2018" << endl;
#endif
	_getch();
}
void main()
{
	cGra::ObiektGra()->Start();
	delete cGra::ObiektGra();
	//_getch();
}