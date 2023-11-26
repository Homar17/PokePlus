#include <iostream>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Data.Json.h>
#include <winrt/Windows.Web.Http.h>
#include <winrt/Windows.Foundation.Collections.h>


using namespace winrt;
using namespace winrt::Windows::Data::Json;
using namespace winrt::Windows::Web::Http;
using namespace winrt::Windows::Foundation::Collections;

// Assuming you have a Pokemon class defined somewhere
class Pokemon {
public:
    int id;
    hstring name;
    hstring hp;
    hstring attack;

    Pokemon(int _id, hstring _name, hstring _hp, hstring _attack)
        : id(_id), name(_name), hp(_hp), attack(_attack) {}
};
// Definición de la clase Nodo
class Nodo {
public:
    Pokemon pokemon;
    Nodo* siguiente;
    Nodo* anterior;

    Nodo(Pokemon valor) : pokemon(valor), siguiente(nullptr), anterior(nullptr) {}
};

// Definición de la clase ListaDobleEnlazada
class ListaDobleEnlazada {
private:
    Nodo* cabeza;

public:
    ListaDobleEnlazada() : cabeza(nullptr) {}

    // Método para insertar al final de la lista
    void insertarAlFinal(Pokemon valor) {
        Nodo* nuevoNodo = new Nodo(valor);
        if (!cabeza) {
            cabeza = nuevoNodo;
        }
        else {
            Nodo* temp = cabeza;
            while (temp->siguiente) {
                temp = temp->siguiente;
            }
            temp->siguiente = nuevoNodo;
            nuevoNodo->anterior = temp;
        }
    }

    // Método para imprimir la lista en orden
    void imprimirEnOrden() {
        Nodo* temp = cabeza;
        while (temp) {
            std::wcout << temp->pokemon.name.c_str() << " ";
            temp = temp->siguiente;
        }
        std::cout << std::endl;
    }

    // Método para imprimir la lista en orden inverso
    void imprimirEnOrdenInverso() {
        Nodo* temp = cabeza;
        while (temp->siguiente) {
            temp = temp->siguiente;
        }
        while (temp) {
            std::wcout << temp->pokemon.name.c_str() << " ";
            temp = temp->anterior;
        }
        std::cout << std::endl;
    }
    hstring createArray(hstring arr[]) {
        Nodo* temp = cabeza;
        int i = 0;
        while (temp) {
            std::wcout << temp->pokemon.name.c_str() << " ";
            arr[i]= temp->pokemon.name;
            temp = temp->siguiente;
            i++;
        }
    }
    static void quickSort(hstring arr[], int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);

            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }

    static int partition(hstring arr[], int low, int high) {
        hstring pivot = arr[high];
        int i = (low - 1);

        for (int j = low; j <= high - 1; j++) {
            if (arr[j] < pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        return (i + 1);
    }
};

int main() {
    init_apartment();

    // Realizar la solicitud HTTP en un bucle
    HttpClient httpClient;

    // Assuming you have a List class to store Pokemon objects
    ListaDobleEnlazada pokemonList;

    for (int i = 1; i <= 10; ++i) {
        // Construir la URL con el nuevo valor de i
        std::wstring apiUrl = L"https://pokeapi.co/api/v2/pokemon/" + std::to_wstring(i) + L"/";
        Windows::Foundation::Uri uri(apiUrl);

        // Realizar la solicitud HTTP
        auto httpResponse = httpClient.GetAsync(uri).get();
        auto responseString = httpResponse.Content().ReadAsStringAsync().get();

        // Parsear la respuesta JSON
        JsonObject jsonObject = JsonObject::Parse(responseString);

        // Extraer información específica
        hstring name = jsonObject.GetNamedString(L"name");
        std::wcout << L"Para el Pokemon con ID " << i << L", el nombre es: " << name.c_str() << std::endl;

        // Variables para almacenar las stats
        hstring hp;
        hstring attack;

        // Obtener la lista de stats
        auto statsArray = jsonObject.GetNamedArray(L"stats");

        // Mostrar solo las stats de interés
        for (const auto& stat : statsArray) {
            auto statObject = stat.GetObject();
            hstring statName = statObject.GetNamedObject(L"stat").GetNamedString(L"name");

            // Filtrar por las stats de interés
            if (statName == L"hp") {
                int statValue = static_cast<int>(statObject.GetNamedNumber(L"base_stat"));
                hp = to_hstring(statValue);
            }
            else if (statName == L"attack") {
                int statValue = static_cast<int>(statObject.GetNamedNumber(L"base_stat"));
                attack = to_hstring(statValue);
            }
        }

        // Crear el objeto Pokemon y agregarlo a la lista
        Pokemon new_pokemon(i, name, hp, attack);
        pokemonList.insertarAlFinal(new_pokemon);

        std::wcout << std::endl;  // Separador entre Pokémon
    }

    // Haz lo que necesites con la lista de Pokémon, por ejemplo, imprimirlos
    pokemonList.imprimirEnOrden();
    pokemonList.imprimirEnOrdenInverso();

    ListaDobleEnlazada::quickSort(arr, 0, 9);

    std::wcout << L"Lista ordenada: ";
    for (int i = 0; i < 10; i++) {
        std::wcout << arr[i].c_str() << " ";
    }
    return 0;
}