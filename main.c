/**
 * Dateiname: main.c
 * Datum: 05.08.2022
 * Beschreibung: Loesen linearer Optimierungsprobleme mit dem primalen und dualen Simplex-Algorithmus
 */

 #include <stdio.h>

//Funktionsdeklarationen
/*
void dual_waehleZeile();
void dual_waehleSpalte();
void dual_pruefeBedingungen();
*/
void bildeEinheitsvektor(double* tableau, int pivotSpalte, int pivotZeile, int anzahlZeilen, int anzahlSpalten);
void ausgabeTableau(int anzahlRestriktionen, int anzahlProdukte, double* tableau);
double primalerSimplex(int l_anzahlRestriktionen, int l_anzahlProdukte, double* tableau);


//Globale Variablen
 const char g_alphabet[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};



int main()
{
    //Deklarieren benötigter Variablen
    int anzahlProdukte = 0;
    int anzahlRestriktionen = 0;
    int anzahlKGR = 0;
    int anzahlGGR = 0;
    int anzahlSpalten = 0;
    int anzahlZeilen = 0;
    char statusZielfunktion = 0;                       //0 bei zu maximierender Zielfunktion, 1 bei zu minimierender Zielfunktion
    char statusAlgorithmus = 1;                        //0 für primalen Simplex-Algorithmus, 1 für dualen Simplex-Algorithmus

    /* Deklarieren eines zweidiemensionalen Arrays als Tableau
     * 1. Dimension: Zeilen
     * 2. Dimension: Spalten
     */
    printf("Wie viele Produkte enthaelt Ihr lineares Optimierungsproblem? ");
    scanf("%i", &anzahlProdukte);
    printf("Geben Sie '0' ein, wenn Ihre Zielfunktion maximiert werden soll, geben Sie '1' ein, wenn Ihre Zielfunktion minimiert werden soll: ");
    fflush(stdin);
    scanf("%c", &statusZielfunktion);
    printf("Wie viele '<='-Restriktionen enthaelt Ihr lineares Optimierungsproblem? ");
    scanf("%i", &anzahlKGR);
    printf("Wie viele '>='-Restriktionen enthaelt Ihr lineares Optimierungsproblem? ");
    scanf("%i", &anzahlGGR);
    printf("\n");
    anzahlRestriktionen = anzahlKGR+anzahlGGR;
    anzahlSpalten = anzahlRestriktionen+anzahlProdukte+1;
    anzahlZeilen = anzahlRestriktionen+1;
    double tableau[anzahlZeilen][anzahlSpalten];

    //Eingabe durch User
    double h;
    for (int i = 0; i < anzahlZeilen; i++)
    {
        printf("\n");
        if (i < anzahlKGR)
        {
            printf("Geben Sie eine '<='-Restriktion ein: \n");
        }
        else if ((i >= anzahlKGR) && (i < anzahlRestriktionen))
        {
            printf("Geben Sie eine '>='-Restriktion ein: \n");
        }
        else
        {
            printf("Geben Sie die Zielfunktion ein: \n");
        }
        for (int j = 0; j < anzahlSpalten; j++)
        {
            if (i < anzahlRestriktionen)
            {
                //Restriktionszeilen
                if (i < anzahlKGR)
                {
                    //<=
                    if (j < anzahlProdukte)
                    {
                        //Produkte
                        if (j != 0)
                        {
                            printf("+");
                        }
                        printf("\t%c * ", g_alphabet[j]);
                        scanf("%lf", &h);
                        tableau[i][j] = h;

                    }
                    else if (j == (anzahlRestriktionen+anzahlProdukte))
                    {
                        //bi-Zeile
                        printf("<=\t");
                        scanf("%lf", &h);
                        tableau[i][j] = h;
                    }
                    else
                    {
                        //Schlupfvariablen
                        if ((anzahlProdukte+i) == j)
                        {
                            tableau[i][j] = 1;
                        }
                        else
                        {
                            tableau[i][j] = 0;
                        }
                    }

                } // if (i < anzahlKGR)
                else
                {
                    //>=
                    if (j < anzahlProdukte)
                    {
                        //Produkte
                        if (j != 0)
                        {
                            printf("+");
                        }
                        printf("\t%c * ", g_alphabet[j]);
                        scanf("%lf", &h);
                        tableau[i][j] = h*(-1);
                    }
                    else if (j == (anzahlRestriktionen+anzahlProdukte))
                    {
                        //bi-Zeile
                        printf(">=\t");
                        scanf("%lf", &h);
                        tableau[i][j] = h*(-1);
                    }
                    else
                    {
                        //Schlupfvariablen
                        if ((anzahlProdukte+i) == j)
                        {
                            tableau[i][j] = 1;
                        }
                        else
                        {
                            tableau[i][j] = 0;
                        }
                    }
                } //else

            } //if (i < anzahlRestriktionen)
            else
            {
                //F-Zeile
                if (statusZielfunktion == '0')
                {
                    //maximierende Zielfunktion
                    if (j < anzahlProdukte)
                    {
                        if (j != 0)
                        {
                            printf("+");
                        }
                        printf("\t%c * ", g_alphabet[j]);
                        scanf("%lf", &h);
                        tableau[i][j] = h*(-1);
                    }
                    else
                    {
                        tableau[i][j] = 0;
                    }
                }
                else
                {
                    //minimierende Zielfunktion
                    if (j < anzahlProdukte)
                    {
                        if (j != 0)
                        {
                            printf("+");
                        }
                        printf("\t%c * ", g_alphabet[j]);
                        scanf("%lf", &h);
                        tableau[i][j] = h;
                    }
                    else
                    {
                        tableau[i][j] = 0;
                    }
                }

            } //else

        } //for int j = 0; j < anzahlSpalten; j++)

    } //for (int i = 0; i < anzahlZeilen; i++)
    printf("\n");

    //Ausgabe der Eingabe in Form des ersten Tableaus
    ausgabeTableau(anzahlSpalten, anzahlZeilen, *tableau);

    //Wahl des Algorithmus
    if ((anzahlGGR == 0) && (statusZielfunktion == '0'))
    {
        //Primaler Algorithmus
        statusAlgorithmus = '0';
        printf("Dieses lineare Optimierungsproblem wird mit dem primalen Simplex-Algorithmus geloest.\n");
        double h_result = primalerSimplex(anzahlRestriktionen, anzahlProdukte, *tableau);
        if (h_result == -1)
        {
            printf("Fuer das lineare Optimierungsproblem konnte keine optimale Loesung gefunden werden.\n");
        }
        else
        {
            printf("Die optimale Lösung lautet: %lf\n\n", h_result);
            ausgabeTableau(anzahlSpalten, anzahlZeilen, *tableau);
        }
    }
    else
    {
        //dualer Algorithmus
        statusAlgorithmus = '1';
        printf("Dieses lineare Optimierungsproblem wird mit dem dualen Simplex-Algorithmus geloest.\n");
    }


	return 0;
}

/**
 * Ausgabe eines Tableaus
 *
 * @param l_anzahlSpalten Anzahl der Spalten des Tableaus
 * @param l_anzahlZeilen Anzahl der Zeilen des Tableaus
 * @param l_tableau Zeiger auf zweidimensionales Array, welches das Tableau enthält
 */
void ausgabeTableau(int l_anzahlSpalten, int l_anzahlZeilen, double* tableau)
{
    for (int i = 0; i < l_anzahlZeilen; i++)
    {
        for (int j = 0; j < l_anzahlSpalten; j++)
        {
            printf("%lf\t", tableau[j+i*l_anzahlSpalten]);
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * Bilden eines Einheitsvektors bei gegebenenen Pivotelement
 *
 * @param tableau Zeiger auf zweidimensionales Array, welches das Tableau enthält
 * @param l_pivotSpalte Welche Spalte stellt die Pivotspalte dar
 * @param l_pivotZeile Welche Zeile stellt die Pivotzeile dar
 * @param l_anzahlZeilen Wie viele Zeilen hat das Tableau
 * @param l_anzahlSpalten Wie viele Spalten hat das Tableau
 */
void bildeEinheitsvektor(double* tableau, int l_pivotSpalte, int l_pivotZeile, int l_anzahlZeilen, int l_anzahlSpalten)
{
    //Pivotelement auf 1 bringen
    //tableau[wunschSpalte + wunschZeile * anzahlSpalten]
    double l_pivotElement = tableau[l_anzahlSpalten*l_pivotZeile+l_pivotSpalte];
    double l_arrayPivotZeile[l_anzahlSpalten];
   // printf("Neue Pivotzeile: ");
    for (int i = 0; i < l_anzahlSpalten; i++)
    {
        //printf("Altes Element: %lf; ",   tableau[anzahlSpalten*(pivotZeile-1)+i]);
        tableau[l_anzahlSpalten*l_pivotZeile+i] /= l_pivotElement;
        //printf("Neues Element: %lf\n",   tableau[anzahlSpalten*(pivotZeile-1)+i]);
        l_arrayPivotZeile[i] = tableau[l_anzahlSpalten*l_pivotZeile+i];
       // printf("%lf; ", arrayPivotZeile[i]);
    }
    //printf("\n");

    //Alle anderen Zeilen auf 0 bringen
    for (int i = 0; i < l_anzahlZeilen; i++)
    {
        double l_faktor = tableau[i*l_anzahlSpalten+l_pivotSpalte]*(-1);
        for (int j = 0; j < l_anzahlSpalten; j++)
        {
            if (i == l_pivotZeile)
            {
                //Die Pivotzeile muss nicht nochmal bearbeitet werden
                break;
            }
            else
            {
                //Gaussche Berechnungen
                //tableau[j+i*anzahlSpalten] j=aktuelleSPalte, i*...=aktuelleZeile

                //printf("\nFaktor: %lf\n", faktor);
                //printf("i: %i\tj: %i\tValue: %lf\tFaktor: %lf\tPivotzeile: %lf\t", i, j,  tableau[j+i*anzahlSpalten], faktor, arrayPivotZeile[j]);
                tableau[j+i*l_anzahlSpalten] = tableau[j+i*l_anzahlSpalten] + l_faktor * l_arrayPivotZeile[j];
               // printf("new Value: %lf\n", tableau[j+i*anzahlSpalten]);
            }
        }
    }
    //printf("\n");
}

/**
 * Anwendung des primalen Simplex-Algorithmus auf ein lineares Optimierungsproblem
 *
 * @param l_anzahlRestriktionen Anzahl der Restriktionen des linearen Optimierungsproblems
 * @param l_anzahlProdukte Anzahl der Produkte des linearen Optimierungsproblems
 * @param l_tableau Zeiger auf zweidimensionales Array, welches das Tableau enthält
 * @return Entweder optimale Lösung oder -1, wenn keine optimale Lösung gefunden werden konnte
 */
double primalerSimplex(int l_anzahlRestriktionen, int l_anzahlProdukte, double* tableau)
{
    int l_anzahlSpalten = l_anzahlRestriktionen+l_anzahlProdukte+1;
    int l_anzahlZeilen = l_anzahlRestriktionen+1;
    int l_lsg = 0;
    char l_run = '1';   //Solange 1, wende primalen Simplex-Algorithmus an, bei 0: fertig oder Abbruch
    int l_pivotSpalte = -1;
    int l_pivotZeile = -1;

    //Wiederhole den primalen Simplex-Algorithmus solange, bis eine optimale Loesung gefunden wurde
    while (l_run == '1')
    {
        //Schritt 1: Bestimmung der Pivotspalte
        l_lsg = 1;
        //wenn die F-Zeile nur nichtnegative Werte enthält, ist die Basislösung optimal; ansonsten suche den kleinsten neg. Wert
        for (int i = 0; i < l_anzahlSpalten; i++)
        {
            if ((tableau[i+l_anzahlRestriktionen*l_anzahlSpalten]) < 0)
            {
                //min. ein negativer Wert vorhanden
                l_lsg = 0;
                if ((tableau[i + l_anzahlRestriktionen * l_anzahlSpalten]) < (tableau[(l_pivotSpalte) + l_anzahlRestriktionen * l_anzahlSpalten]))
                {
                    l_pivotSpalte = i;
                }
            }
        }
        if (l_lsg == 1)
        {
            //optimale Lösung gefunden, return der Basislösung
            l_run = '0';
            return (tableau[(l_anzahlRestriktionen+l_anzahlProdukte)+l_anzahlRestriktionen*l_anzahlSpalten]);
        }

        //Schritt 2: Bestimmung der Pivotzeile mittels Engpassbestimmung
        l_lsg = 1;
        double h_quotient = 0;
        for (int i = 0; i < l_anzahlZeilen; i++)
        {
            if ((tableau[(l_anzahlRestriktionen+l_anzahlProdukte)+i*l_anzahlSpalten] > 0) && (tableau[l_pivotSpalte+i*l_anzahlSpalten] > 0))
            {
                //wenn bi-Wert > 0 & Wert in Pivotspalte > 0 => Engpassbestimmung
                l_lsg = 0;
                if ((h_quotient == 0) || (h_quotient > (tableau[(l_anzahlRestriktionen+l_anzahlProdukte)+i*l_anzahlSpalten]/tableau[l_pivotSpalte+i*l_anzahlSpalten])))
                {
                    h_quotient = (tableau[(l_anzahlRestriktionen+l_anzahlProdukte)+i*l_anzahlSpalten])/(tableau[l_pivotSpalte+i*l_anzahlSpalten]);
                    l_pivotZeile = i;
                }
            }
        }
        if (l_lsg == 1)
        {
            //Kann keine optimale Bedingung finden
            l_run = '0';
            return -1;
        }
        bildeEinheitsvektor(tableau, l_pivotSpalte, l_pivotZeile, l_anzahlZeilen, l_anzahlSpalten);
    }
}

void dual_waehleZeile();
void dual_waehleSpalte();
void dual_pruefeBedingungen();
