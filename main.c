/**
 * Dateiname: main.c
 * Datum: 05.08.2022
 * Beschreibung: Loesen linearer Optimierungsprobleme mit dem primalen und dualen Simplex-Algorithmus
 */

//Inkludierungen benoetigter Bibliotheken
 #include <stdio.h>

//Funktionsdeklarationen
void bildeEinheitsvektor(double* tableau, int pivotSpalte, int pivotZeile, int anzahlZeilen, int anzahlSpalten);
void ausgabeTableau(int anzahlRestriktionen, int anzahlProdukte, double* tableau);
double primalerSimplex(int l_anzahlRestriktionen, int l_anzahlProdukte, double* tableau);
double dualerSimplex(int l_anzahlRestriktionen, int l_anzahlProdukte, double* tableau);

//Globale Variablen
 const char g_alphabet[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

int main()
{
    //Deklarieren benoetigter Variablen
    int anzahlProdukte = 0;
    int anzahlRestriktionen = 0;
    int anzahlKGR = 0;                      //Anzahl kleiner-gleich-Restriktionen
    int anzahlGGR = 0;                      //Anzahl groesser-gleich-Restriktionen
    int anzahlSpalten = 0;
    int anzahlZeilen = 0;
    int statusAlgorithmus = 1;             //0 fuer primalen Simplex-Algorithmus, 1 fuer dualen Simplex-Algorithmus

    /* Deklarieren eines zweidiemensionalen Arrays als Tableau
     * 1. Dimension: Zeilen
     * 2. Dimension: Spalten
     */
    printf("Wie viele Produkte enthaelt Ihr lineares Optimierungsproblem? ");
    scanf("%i", &anzahlProdukte);
    printf("Wie viele '<='-Restriktionen enthaelt Ihr lineares Optimierungsproblem? ");
    scanf("%i", &anzahlKGR);
    printf("Wie viele '>='-Restriktionen enthaelt Ihr lineares Optimierungsproblem? ");
    scanf("%i", &anzahlGGR);
    printf("\n");
    anzahlRestriktionen = anzahlKGR+anzahlGGR;
    anzahlSpalten = anzahlRestriktionen+anzahlProdukte+1;
    anzahlZeilen = anzahlRestriktionen+1;
    double tableau[anzahlZeilen][anzahlSpalten];

    //Eingabe der Daten durch den User
    double h;
    for (int i = 0; i < anzahlZeilen; i++) {
        //Durchlaufen der Zeilen des Tableaus
        printf("\n");
        if (i < anzahlKGR) {
            printf("Geben Sie eine '<='-Restriktion ein: \n");
        }
        else if ((i >= anzahlKGR) && (i < anzahlRestriktionen)) {
            printf("Geben Sie eine '>='-Restriktion ein: \n");
        }
        else {
            printf("Geben Sie die Zielfunktion ein: \n");
        }
        for (int j = 0; j < anzahlSpalten; j++) {
            //Durchlaufen der Spalten des Tableaus
            if (i < anzahlRestriktionen) {
                //Restriktionszeilen
                if (i < anzahlKGR) {
                    //<=
                    if (j < anzahlProdukte) {
                        //Produkte
                        if (j != 0) {
                            printf("+");
                        }
                        printf("\t%c * ", g_alphabet[j]);
                        scanf("%lf", &h);
                        tableau[i][j] = h;
                    } //if (j < anzahlProdukte)
                    else if (j == (anzahlRestriktionen+anzahlProdukte)) {
                        //bi-Zeile
                        printf("<=\t");
                        scanf("%lf", &h);
                        tableau[i][j] = h;
                    } //else if (j == (anzahlRestriktionen+anzahlProdukte))
                    else {
                        //Schlupfvariablen
                        if ((anzahlProdukte+i) == j) {
                            tableau[i][j] = 1;
                        }
                        else {
                            tableau[i][j] = 0;
                        }
                    } //else
                } //if (i < anzahlKGR)
                else {
                    //>=
                    if (j < anzahlProdukte) {
                        //Produkte
                        if (j != 0) {
                            printf("+");
                        }
                        printf("\t%c * ", g_alphabet[j]);
                        scanf("%lf", &h);
                        tableau[i][j] = h*(-1);
                    } //if (j < anzahlProdukte)
                    else if (j == (anzahlRestriktionen+anzahlProdukte)) {
                        //bi-Zeile
                        printf(">=\t");
                        scanf("%lf", &h);
                        tableau[i][j] = h*(-1);
                    } //else if (j == (anzahlRestriktionen+anzahlProdukte))
                    else {
                        //Schlupfvariablen
                        if ((anzahlProdukte+i) == j) {
                            tableau[i][j] = 1;
                        }
                        else {
                            tableau[i][j] = 0;
                        }
                    } //else
                } //else
            } //if (i < anzahlRestriktionen)
            else {
                //F-Zeile
                if (j < anzahlProdukte) {
                    if (j != 0) {
                        printf("+");
                    }
                    printf("\t%c * ", g_alphabet[j]);
                    scanf("%lf", &h);
                    tableau[i][j] = h*(-1);
                } //if (j < anzahlProdukte)
                else {
                    tableau[i][j] = 0;
                } //else
            } //else
        } //for int j = 0; j < anzahlSpalten; j++)
    } //for (int i = 0; i < anzahlZeilen; i++)
    printf("\n");

    //Ausgabe der Eingabe in Form des ersten Simplex-Tableaus
    ausgabeTableau(anzahlSpalten, anzahlZeilen, *tableau);

    //Wahl des Algorithmus
    statusAlgorithmus = 0;
    for (int i = 0; i < anzahlZeilen; i++) {
        if (tableau[i][anzahlRestriktionen+anzahlProdukte] < 0) {
            //keine zulessige Basisloesung => dualer Simplex-Algorithmus
            statusAlgorithmus = 1;
            break;
        }
    }

    //Aufruf der entsprechenden Funktion zur Ausfuehrung des Simplex-Algorithmus
    double h_result = 0;
    double h_lsg;
    if (statusAlgorithmus == 0) {
        //primaler Algorithmus
        printf("Dieses lineare Optimierungsproblem wird mit dem primalen Simplex-Algorithmus geloest.\n");
        h_result = primalerSimplex(anzahlRestriktionen, anzahlProdukte, *tableau);
        if (h_result == -1){
            printf("Fuer das lineare Optimierungsproblem konnte keine optimale Loesung gefunden werden.\n");
        }
        else if (h_result == 1) {
            h_lsg = tableau[anzahlRestriktionen][anzahlRestriktionen+anzahlProdukte];
            printf("Die optimale Loesung lautet: %lf\n\n", h_lsg);
            ausgabeTableau(anzahlSpalten, anzahlZeilen, *tableau);
        }
    } //if (statusAlgorithmus == 0)
    else if (statusAlgorithmus == 1) {
        //dualer Algorithmus
        printf("Dieses lineare Optimierungsproblem wird mit dem dualen Simplex-Algorithmus geloest.\n");
        h_result = dualerSimplex(anzahlRestriktionen, anzahlProdukte, *tableau);
        if (h_result == -1) {
            printf("Fuer das lineare Optimierungsproblem konnte keine zulaessige Loesung gefunden werden.\n");
        }
        else if (h_result == 1) {
            //wenn zulaessige Loesung gefunden, fuehre primalen Simplex aus, um optimale Loesung zu fiden
            h_result = primalerSimplex(anzahlRestriktionen, anzahlProdukte, *tableau);
            if (h_result == -1) {
                printf("Fuer das lineare Optimierungsproblem konnte keine optimale Loesung gefunden werden.\n");
            }
            else if (h_result == 1) {
                h_lsg = tableau[anzahlRestriktionen][anzahlRestriktionen+anzahlProdukte];
                printf("Die optimale Loesung lautet: %lf\n\n", h_lsg);
                ausgabeTableau(anzahlSpalten, anzahlZeilen, *tableau);
            } //else if (h_result == 1)
        }
    }
	return 0;
}


/**
 * Zeilenweise Ausgabe eines Tableaus
 *
 * @param l_anzahlSpalten Anzahl der Spalten des Tableaus
 * @param l_anzahlZeilen Anzahl der Zeilen des Tableaus
 * @param l_tableau Zeiger auf zweidimensionales Array, welches das Tableau enth�lt
 */
void ausgabeTableau(int l_anzahlSpalten, int l_anzahlZeilen, double* tableau)
{
    for (int i = 0; i < l_anzahlZeilen; i++) {
        //Durchlaufen der Zeilen
        for (int j = 0; j < l_anzahlSpalten; j++) {
            //Durchlaufen der Spalten
            printf("%lf\t", tableau[j+i*l_anzahlSpalten]);
        }
        printf("\n");
    }
    printf("\n");
}


/**
 * Bilden eines Einheitsvektors bei gegebenenen Pivotelement 
 * (Dritter Schritt des primalen/dualen Simplex-Algorithmus)
 *
 * @param tableau Zeiger auf zweidimensionales Array, welches das Tableau enth�lt
 * @param l_pivotSpalte Welche Spalte stellt die Pivotspalte dar
 * @param l_pivotZeile Welche Zeile stellt die Pivotzeile dar
 * @param l_anzahlZeilen Wie viele Zeilen hat das Tableau
 * @param l_anzahlSpalten Wie viele Spalten hat das Tableau
 */
void bildeEinheitsvektor(double* tableau, int l_pivotSpalte, int l_pivotZeile, int l_anzahlZeilen, int l_anzahlSpalten)
{
    //Pivotelement auf 1 bringen
    double l_pivotElement = tableau[l_anzahlSpalten*l_pivotZeile+l_pivotSpalte];
    double l_arrayPivotZeile[l_anzahlSpalten];

    for (int i = 0; i < l_anzahlSpalten; i++) {
        //Teile Pivotzeile durch Pivotelement, um Pivotelement auf 1 zu bringen
        tableau[l_anzahlSpalten*l_pivotZeile+i] /= l_pivotElement;
        l_arrayPivotZeile[i] = tableau[l_anzahlSpalten*l_pivotZeile+i];
    }

    //Alle anderen Zeilen auf 0 bringen
    for (int i = 0; i < l_anzahlZeilen; i++) {
        double l_faktor = tableau[i*l_anzahlSpalten+l_pivotSpalte]*(-1);
        for (int j = 0; j < l_anzahlSpalten; j++) {
            if (i == l_pivotZeile) {
                //Die Pivotzeile muss nicht nochmal bearbeitet werden
                break;
            }
            else {
                tableau[j+i*l_anzahlSpalten] = tableau[j+i*l_anzahlSpalten] + l_faktor * l_arrayPivotZeile[j];
            }
        }
    }
}


/**
 * Anwendung des primalen Simplex-Algorithmus auf ein lineares Optimierungsproblem
 *
 * @param l_anzahlRestriktionen Anzahl der Restriktionen des linearen Optimierungsproblems
 * @param l_anzahlProdukte Anzahl der Produkte des linearen Optimierungsproblems
 * @param l_tableau Zeiger auf zweidimensionales Array, welches das Tableau enthaelt
 * @return Entweder 1, wenn optimale Loesung gefunden wurde, oder -1, wenn keine optimale Loesung gefunden wurde
 */
double primalerSimplex(int l_anzahlRestriktionen, int l_anzahlProdukte, double* tableau)
{
    int l_anzahlSpalten = l_anzahlRestriktionen+l_anzahlProdukte+1;
    int l_anzahlZeilen = l_anzahlRestriktionen+1;
    int l_lsg = 0;   //Hilfsvariable; boolean   
    int l_run = 1;   //Solange 1, wende primalen Simplex-Algorithmus an, bei 0: fertig oder Abbruch
    int l_pivotSpalte = -1;
    int l_pivotZeile = -1;

    //Wiederhole den primalen Simplex-Algorithmus solange, bis eine optimale Loesung gefunden wurde
    while (l_run == 1) {
        //Schritt 1: Bestimmung der Pivotspalte
        l_lsg = 1;
        //wenn die F-Zeile nur nichtnegative Werte enthaelt, ist die Basisloesung optimal; ansonsten suche den kleinsten neg. Wert
        for (int i = 0; i < l_anzahlSpalten; i++) {
            if ((tableau[i+l_anzahlRestriktionen*l_anzahlSpalten]) < 0) {
                //min. ein negativer Wert vorhanden
                l_lsg = 0;
                if ((tableau[i + l_anzahlRestriktionen * l_anzahlSpalten]) < (tableau[(l_pivotSpalte) + l_anzahlRestriktionen * l_anzahlSpalten])) {
                    //speichern des kleinsten negativen Wert
                    l_pivotSpalte = i;
                }
            }
        }
        if (l_lsg == 1) {
            //optimale Loesung gefunden, return der Basisloesung
            l_run = 0;
            return 1;
        }

        //Schritt 2: Bestimmung der Pivotzeile mittels Engpassbestimmung
        l_lsg = 1;
        double h_quotient = 0;
        for (int i = 0; i < l_anzahlZeilen; i++) {
            if ((tableau[(l_anzahlRestriktionen+l_anzahlProdukte)+i*l_anzahlSpalten] > 0) && (tableau[l_pivotSpalte+i*l_anzahlSpalten] > 0)) {
                //wenn bi-Wert > 0 & Wert in Pivotspalte > 0 => Engpassbestimmung
                l_lsg = 0;
                if ((h_quotient == 0) || (h_quotient > (tableau[(l_anzahlRestriktionen+l_anzahlProdukte)+i*l_anzahlSpalten]/tableau[l_pivotSpalte+i*l_anzahlSpalten]))) {
                    h_quotient = (tableau[(l_anzahlRestriktionen+l_anzahlProdukte)+i*l_anzahlSpalten])/(tableau[l_pivotSpalte+i*l_anzahlSpalten]);
                    l_pivotZeile = i;
                }
            }
        }
        if (l_lsg == 1) {
            //Kann keine optimale Bedingung finden; kein Koeffizient >= 0
            l_run = 0;
            return -1;
        }

        bildeEinheitsvektor(tableau, l_pivotSpalte, l_pivotZeile, l_anzahlZeilen, l_anzahlSpalten);
    }
}


/**
 * Anwendung des dualen Simplex-Algorithmus auf ein lineares Optimierungsproblem
 *
 * @param l_anzahlRestriktionen Anzahl der Restriktionen des linearen Optimierungsproblems
 * @param l_anzahlProdukte Anzahl der Produkte des linearen Optimierungsproblems
 * @param l_tableau Zeiger auf zweidimensionales Array, welches das Tableau enthaelt
 * @return Entweder 1, wenn zulaessige Loesung gefunden wurde, oder -1, wenn kene zulaessige Loesung gefunden wurde
 */
double dualerSimplex(int l_anzahlRestriktionen, int l_anzahlProdukte, double* tableau)
{
    int l_anzahlSpalten = l_anzahlRestriktionen+l_anzahlProdukte+1;
    int l_anzahlZeilen = l_anzahlRestriktionen+1;  
    int l_run = 1;   //Solange 1, wende dualen Simplex-Algorithmus an, bei 0: fertig oder Abbruch
    int l_pivotSpalte = -1;
    int l_pivotZeile = -1;

    while (l_run == 1) {
        int l_pivotSpalte = -1;
        int l_pivotZeile = -1;
        
        //Schritt 1: Bestimmung der Pivotzeile
        for (int i = 0; i < l_anzahlZeilen; i++) {
            if (tableau[(l_anzahlProdukte+l_anzahlRestriktionen)+i*l_anzahlSpalten] < 0) {
                //wenn der bi-Wert der aktuellen Zeile negativ ist, setze aktuelle Zeile auf Pivotzeile, wenn...
                if (l_pivotZeile == -1) {
                    //...es die erste Zeile ist
                    l_pivotZeile = i;
                }
                else {
                    if (tableau[(l_anzahlProdukte+l_anzahlRestriktionen)+i*l_anzahlSpalten] < tableau[(l_anzahlProdukte+l_anzahlRestriktionen)+l_pivotZeile*l_anzahlSpalten]) {
                        //...der neue bi-Wert kleiner als der aktuelle bi-Wert ist
                        l_pivotZeile = i;
                    }
                }
            }
        }
        if (l_pivotZeile == -1) {
            //zulaessige Basisloesung gefunden
            l_run = 0;
            return 1;
        }

        //Schritt 2: Bestimmung der Pivotspalte
        for (int i = 0; i < l_anzahlSpalten; i++) {
            if (tableau[i+l_pivotZeile*l_anzahlSpalten] < 0) {
                //wenn der Koeffizient in der aktuellen Spalte negativ ist, setze aktuelle Spalte auf Pivotspalte, wenn...
                if (l_pivotSpalte == -1) {
                    //...es die erste Spalte ist
                    l_pivotSpalte = i;
                }
                else {
                    if ((tableau[i+l_anzahlRestriktionen*l_anzahlSpalten] / tableau[i+l_pivotZeile*l_anzahlSpalten]) > (tableau[l_pivotSpalte+l_anzahlRestriktionen*l_anzahlSpalten] / tableau[l_pivotSpalte+l_pivotZeile*l_anzahlSpalten])) {
                        //...der neue Quotient groesser als der aktuelle Quotient ist
                        l_pivotSpalte = i;
                    }
                }
            }
        }
        if (l_pivotSpalte == -1) {
            //es kann keine zulaessige Loesung gefunden werden
            l_run = 0;
            return (-1);
        }

        //Schritt 3: Bilde Einheitsvektor
        bildeEinheitsvektor(tableau, l_pivotSpalte, l_pivotZeile, l_anzahlZeilen, l_anzahlSpalten);
    }
}
