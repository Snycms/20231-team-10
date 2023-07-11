
#include "../include/Baralho.h"
using namespace std;
using namespace Baralhos;

Baralhos::Carta::Carta() {
    this->naipe = "";
    this->valor = "";
    this->codigo = -1;
    this->indice = -1;
}

Baralhos::Carta::Carta(string naipe, string valor, int codigo, int indice) {
    this->naipe = naipe;
    this->valor = valor;
    this->codigo = codigo;
    this->indice = indice;
}

string Baralhos::Carta::getNaipe() {
    return this->naipe;
}

string Baralhos::Carta::getValor() {
    return this->valor;
}

int Baralhos::Carta::getCodigo() {
    return this->codigo;
}

int Baralhos::Carta::getIndice() {
    return this->indice;
}

void Baralhos::Carta::setNaipe(string naipe) {
    this->naipe = naipe;
}

void Baralhos::Carta::setValor(string valor) {
    this->valor = valor;
}

void Baralhos::Carta::setCodigo(int codigo) {
    this->codigo = codigo;
}

void Baralhos::Carta::setIndice(int valor) {
    this->indice = valor;
}

string Baralhos::Carta::getCarta(){ 
    return this->valor + " de " + this->naipe;
}

string Baralhos::Carta::getSimboloCarta(){ 
    string simbolo =  "";
    if (this->naipe == "Copas") {
        simbolo = "♥";
    } else if (this->naipe == "Espadas") {
        simbolo = "♠";
    } else if (this->naipe == "Ouros") {
        simbolo = "♦";
    } else if (this->naipe == "Paus") {
        simbolo = "♣";
    }
    return this->valor + " " + simbolo + " | " + this->valor + " de " + this->naipe;
}

bool Baralhos::Carta::operator > (const Carta& str) const {
    return (this->codigo > str.codigo);

};

bool Baralhos::Carta::operator == (const Carta& str) const {
    return (this->codigo == str.codigo);

};

bool Baralhos::Carta::operator < (const Carta& str) const {
    return (this->codigo < str.codigo);
};

Baralhos::BaralhoTotal::BaralhoTotal() {
    this->iniciarBaralho();
}

Baralhos::BaralhoTotal::~BaralhoTotal() {
    this->todas_cartas.clear();
    this->cartas_usadas = 0;
}

void Baralhos::BaralhoTotal::iniciarBaralho() {
    this->todas_cartas = vector<Carta>();
    this->cartas_usadas = 0;

    string naipes[4] = {"Copas", "Espadas", "Ouros", "Paus"};
    int valores_indices[13] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    string valores[13] = {"2", "3", "4", "5", "6", "7", "8","9", "10", "J", "Q", "K", "A"};
    int codigo = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++, codigo++) {
            this->todas_cartas.push_back(Carta(naipes[i], valores[j], codigo, valores_indices[j]));
        }
    }
    reverse(this->todas_cartas.begin(), this->todas_cartas.end());
}

void Baralhos::BaralhoTotal::embaralhar() {
    random_device rd;
    mt19937 generator(rd());
    shuffle(this->todas_cartas.begin(), this->todas_cartas.end(), generator);
}

int Baralhos::BaralhoTotal::getNumeroCartas()
{
    return this->numero_cartas;
}
vector<Carta> Baralhos::BaralhoTotal::getCartas(int quantidade)
{
    vector<Carta> cartas = this->todas_cartas;

try {
        if (quantidade > this->todas_cartas.size()) {
            throw std::runtime_error("Não há cartas suficientes no baralho.");
        }
    
    // Resize the vector to the desired sample size
    cartas.resize(quantidade);

    auto removePredicate = [cartas](const Carta& c) {
            return std::find(cartas.begin(), cartas.end(), c) != cartas.end();
        };
    
    for (Carta carta : cartas) {
        int code = carta.getCodigo();
        this->todas_cartas.erase(remove_if(this->todas_cartas.begin(), this->todas_cartas.end(), [code](Carta c) {
            return c.getCodigo() == code;
        }), this->todas_cartas.end());
    }
    this->cartas_usadas += quantidade;
} catch (const std::exception& e) {
        std::cerr << "Ocorreu um erro ao obter as cartas: " << e.what() << std::endl;
    }
    return cartas;
}

void Baralhos::BaralhoTotal::printBaralho() {
    for (int i = 0; i < this->todas_cartas.size(); i++) {
        cout << this->todas_cartas[i].getCarta() << endl;
    }
}

int Baralhos::BaralhoTotal::getCartasRestantes() {
    return this->numero_cartas - this->cartas_usadas; 
}
