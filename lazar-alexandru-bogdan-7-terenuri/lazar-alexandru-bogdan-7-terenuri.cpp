#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// Clasa de baza
class Teren {
protected:
    string forma;
    double suprafata;
    double pret;
public:
    Teren(string f, double s, double p) : forma(f), suprafata(s), pret(p) {}
    virtual ~Teren() = default;

    virtual void afisare() const {
        cout << "Forma: " << forma << ", Suprafata: " << suprafata << " mp, Pret: " << pret << " RON\n";
    }

    double getSuprafata() const { return suprafata; }
    void setPret(double p) { pret = p; }
    double getPret() const { return pret; }
};

// Clasa derivata: TerenIntravilane
class TerenIntravilan : public Teren {
    bool accesLaStrada;
public:
    TerenIntravilan(string f, double s, double p, bool acces)
        : Teren(f, s, p), accesLaStrada(acces) {}

    void afisare() const override {
        Teren::afisare();
        cout << "Acces la strada: " << (accesLaStrada ? "Da" : "Nu") << "\n";
    }

    bool areAccesLaStrada() const { return accesLaStrada; }
};

// Clasa derivata: TerenExtravilan
class TerenExtravilan : public Teren {
    string tipSol;
public:
    TerenExtravilan(string f, double s, double p, string sol)
        : Teren(f, s, p), tipSol(sol) {}

    void afisare() const override {
        Teren::afisare();
        cout << "Tip sol: " << tipSol << "\n";
    }

    string getTipSol() const { return tipSol; }
};

// Functii utilitare
void adaugaTeren(vector<Teren*>& terenuri) {
    int tip;
    cout << "Adaugare teren: 1 = intravilan, 2 = extravilan: ";
    cin >> tip;

    string forma;
    double suprafata, pret;
    cout << "Forma terenului: ";
    cin >> forma;
    cout << "Suprafata (mp): ";
    cin >> suprafata;
    cout << "Pret (RON): ";
    cin >> pret;

    if (tip == 1) {
        bool acces;
        cout << "Are acces la strada? (1 = Da, 0 = Nu): ";
        cin >> acces;
        terenuri.push_back(new TerenIntravilan(forma, suprafata, pret, acces));
    }
    else if (tip == 2) {
        string tipSol;
        cout << "Tipul solului: ";
        cin >> tipSol;
        terenuri.push_back(new TerenExtravilan(forma, suprafata, pret, tipSol));
    }
    else {
        cout << "Tip invalid.\n";
    }
}

void afiseazaTerenuri(const vector<Teren*>& terenuri) {
    int numarTerenuri = terenuri.size();
    int numarIntravilane = 0;
    int numarExtravilane = 0;
    
    for (const auto& teren : terenuri) {
        if (dynamic_cast<TerenIntravilan*>(teren)) {
            numarIntravilane++;
        }
        else if (dynamic_cast<TerenExtravilan*>(teren)) {
            numarExtravilane++;
        }
    }

    cout << "\nIn registrul de evidenta figureaza " << numarTerenuri
        << " terenuri, din care " << numarIntravilane
        << " intravilan si " << numarExtravilane << " extravilan.\n";
   
    cout << "\nTerenuri intravilane:\n\n";
    for (const auto& teren : terenuri) {
        if (auto intravilan = dynamic_cast<TerenIntravilan*>(teren)) {
            intravilan->afisare();
            cout << "------------------------------------------------\n";
        }
    }
        
    cout << "\nTerenuri extravilane:\n\n";
    for (const auto& teren : terenuri) {
        if (auto extravilan = dynamic_cast<TerenExtravilan*>(teren)) {
            extravilan->afisare();
            cout << "------------------------------------------------\n";
        }
    }
}


void cautaTerenExtravilan(const vector<Teren*>& terenuri) {
    string tipSol;
    cout << "Introdu tipul de sol cautat: ";
    cin >> tipSol;

    bool gasit = false;
    for (const auto& teren : terenuri) {
        auto extravilan = dynamic_cast<TerenExtravilan*>(teren);
        if (extravilan && extravilan->getTipSol() == tipSol) {
            extravilan->afisare();
            gasit = true;
        }
    }

    if (!gasit) {
        cout << "Nu exista terenuri extravilane cu tipul de sol specificat.\n";
    }
}

void stergeTerenDupaSuprafata(vector<Teren*>& terenuri) {
    double suprafata;
    cout << "Introdu suprafata terenului de sters: ";
    cin >> suprafata;

    // Parcurgem vectorul si eliminam terenurile cu suprafata specificata
    auto it = remove_if(terenuri.begin(), terenuri.end(), [suprafata](Teren* teren) {
        if (teren->getSuprafata() == suprafata) {
            delete teren; 
            return true;  
        }
        return false;
        });

    if (it != terenuri.end()) {
        terenuri.erase(it, terenuri.end()); 
        cout << "Terenurile cu suprafata specificata au fost sterse.\n";
    }
    else {
        cout << "Nu exista terenuri cu suprafata specificata.\n";
    }
}

void modificaPretTerenuri(vector<Teren*>& terenuri) {
    int optiune;
    cout << "Doriti sa modificati preturile cu: 1 = +2%, 2 = -2%? ";
    cin >> optiune;

    if (optiune != 1 && optiune != 2) {
        cout << "Optiune invalida. Nu s-au efectuat modificari.\n";
        return;
    }

    double factor = (optiune == 1) ? 1.02 : 0.98;

    for (auto& teren : terenuri) {
        auto intravilan = dynamic_cast<TerenIntravilan*>(teren);
        if (intravilan && intravilan->areAccesLaStrada()) {
            double pretNou = intravilan->getPret() * factor;
            if (pretNou <= 0) {
                cout << "Atentie: Pretul terenului cu suprafata " << intravilan->getSuprafata()
                    << " mp devine invalid (<= 0). Modificarea a fost anulata pentru acest teren.\n";
            }
            else {
                intravilan->setPret(pretNou);
            }
        }
    }

    cout << "Preturile terenurilor intravilane cu acces la strada au fost actualizate.\n";
}


void meniu() {
    vector<Teren*> terenuri;
    int optiune;

    do {
        cout << "\nMeniu:\n\n";
        cout << "1. Adaugare teren\n";
        cout << "2. Afisare terenuri\n";
        cout << "3. Cautare teren extravilan dupa tip sol\n";
        cout << "4. Stergere teren dupa suprafata\n";
        cout << "5. Modificare pret terenuri intravilane (cu acces la strada)\n";
        cout << "6. Iesire\n";
        cout << "\nSelectati o optiune: ";
        cin >> optiune;

        switch (optiune) {
        case 1: adaugaTeren(terenuri); break;
        case 2: afiseazaTerenuri(terenuri); break;
        case 3: cautaTerenExtravilan(terenuri); break;
        case 4: stergeTerenDupaSuprafata(terenuri); break;
        case 5: modificaPretTerenuri(terenuri); break;
        case 6: cout << "Iesire...\n"; break;
        default: cout << "Optiune invalida.\n";
        }
    } while (optiune != 6);

    for (auto teren : terenuri) {
        delete teren;
    }
}

int main() {
    meniu();
    return 0;
}
