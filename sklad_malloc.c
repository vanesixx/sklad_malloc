#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definice struktury - základní stavební kámen
typedef struct {
    char nazev[50];
    long long ean;
    float nakupniCena;
    float prodejniCena;
    int pocetKs;
    int dphLevel;
} Produkt;

// Globální proměnné pro správu dynamického pole
Produkt *sklad = NULL;
int pocetProduktu = 0;

// Funkce pro výpočet koeficientu DPH (pomocná)
float ziskejDph(int level) {
    if (level == 1) return 1.12f; // 12% daň
    if (level == 2) return 1.21f; // 21% daň
    return 1.0f;                  // 0% daň
}

void pridejProdukt() {
    // V čistém C se realloc nepřetypovává, void* se převede samo
    // Paměť se zvětšuje s každým novým prvkem o velikost jedné struktury
    sklad = realloc(sklad, (pocetProduktu + 1) * sizeof(Produkt));

    if (sklad == NULL) {
        printf("Chyba: Nepodarilo se alokovat pamet!\n");
        return;
    }

    printf("Zadej nazev: ");
    scanf("%49s", sklad[pocetProduktu].nazev); // %49s zabrání přetečení paměti
    printf("Zadej EAN: ");
    scanf("%lld", &sklad[pocetProduktu].ean);
    printf("Zadej nakupni cenu: ");
    scanf("%f", &sklad[pocetProduktu].nakupniCena);
    printf("Zadej prodejni cenu: ");
    scanf("%f", &sklad[pocetProduktu].prodejniCena);
    printf("Zadej pocet kusu: ");
    scanf("%d", &sklad[pocetProduktu].pocetKs);
    printf("Zadej DPH level (0, 1, 2): ");
    scanf("%d", &sklad[pocetProduktu].dphLevel);

    pocetProduktu++;
    printf("Produkt byl uspesne pridan.\n");
}

void vypisSklad() {
    if (pocetProduktu == 0) {
        printf("\nSklad je prazdny.\n");
        return;
    }
    printf("\n--- SKLADOVE ZASOBY ---\n");
    for (int i = 0; i < pocetProduktu; i++) {
        float cenaSDph = sklad[i].prodejniCena * ziskejDph(sklad[i].dphLevel);
        printf("%d. %-15s | EAN: %lld | Skladem: %d ks\n", i + 1, sklad[i].nazev, sklad[i].ean, sklad[i].pocetKs);
        printf("   Cena bez DPH: %.2f Kc | Cena s DPH: %.2f Kc\n", sklad[i].prodejniCena, cenaSDph);
    }
}

void vyhledej() {
    char hledany[50];
    printf("Zadej nazev produktu pro vyhledani: ");
    scanf("%49s", hledany);

    int nalezeno = 0;
    for (int i = 0; i < pocetProduktu; i++) {
        // strcmp porovnává texty, vrací 0 pokud jsou identické
        if (strcmp(sklad[i].nazev, hledany) == 0) {
            printf("\nNalezeno: %s\nEAN: %lld\nCena bez DPH: %.2f Kc\nPocet: %d\n",
                   sklad[i].nazev, sklad[i].ean, sklad[i].prodejniCena, sklad[i].pocetKs);
            nalezeno = 1;
            break;
        }
    }
    if (!nalezeno) printf("Produkt s timto nazvem nebyl nalezen.\n");
}

int main() {
    int volba;

    // Nekonečná smyčka aplikace
    while (1) {
        printf("\n--- MENU SKLADU ---\n");
        printf("1. Pridat produkt\n");
        printf("2. Vypis vse\n");
        printf("3. Vyhledat produkt\n");
        printf("4. Ukoncit program\n");
        printf("Vase volba: ");

        // Ošetření vstupu, aby se program nezacyklil při zadání písmena
        if (scanf("%d", &volba) != 1) {
            printf("Neplatny vstup!\n");
            while(getchar() != '\n'); // vyčištění bufferu
            continue;
        }

        if (volba == 1) {
            pridejProdukt();
        } else if (volba == 2) {
            vypisSklad();
        } else if (volba == 3) {
            vyhledej();
        } else if (volba == 4) {
            // Důležité: Uvolnění paměti před koncem
            free(sklad);
            printf("Ukoncovani a uvolnovani pameti...\n");
            break;
        } else {
            printf("Neplatna volba, zkus to znovu.\n");
        }
    }

    return 0;
}