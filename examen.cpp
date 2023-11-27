#include <iostream>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Data.Json.h>
#include <winrt/Windows.Web.Http.h>
#include <winrt/Windows.Foundation.Collections.h>


using namespace winrt;
using namespace winrt::Windows::Data::Json;
using namespace winrt::Windows::Web::Http;
using namespace winrt::Windows::Foundation::Collections;

void quickSort(int [], int, int);
int partition(int[], int, int);

class Pokemon {
public:
    int id;
    hstring name;
    hstring hp;
    hstring attack;

    Pokemon(int _id, hstring _name, hstring _hp, hstring _attack)
        : id(_id), name(_name), hp(_hp), attack(_attack) {}
};

class Nodo {
public:
    Pokemon pokemon;
    Nodo* next;
    Nodo* prev;

    Nodo(Pokemon valor) : pokemon(valor), next(nullptr), prev(nullptr) {}
};


class DoublyLinkedList {
private:
    Nodo* head;

public:
    DoublyLinkedList() : head(nullptr) {}


    void insertarAlFinal(Pokemon valor) {
        Nodo* nuevoNodo = new Nodo(valor);
        if (!head) {
            head = nuevoNodo;
        }
        else {
            Nodo* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = nuevoNodo;
            nuevoNodo->prev = temp;
        }
    }

    void imprimirEnOrden() {
        Nodo* temp = head;
        while (temp) {
            std::wcout << temp->pokemon.name.c_str() << " " << "hp: "<< temp->pokemon.hp.c_str()<<"\n";
            temp = temp->next;
        }
        std::cout << std::endl;
    }


    void imprimirEnOrdenInverso() {
        Nodo* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        while (temp) {
            std::wcout << temp->pokemon.name.c_str() << " ";
            temp = temp->prev;
        }
        std::cout << std::endl;
    }

    int* createArray() {
        int* arr = new int[10];  // Utilizar 'new' para asignar memoria en el heap
        if (head != nullptr) {

            Nodo* temp = head;
            int i = 0;
            while (temp && i < 10) {
                
                // Convertir la cadena 'hp' a entero y almacenarla en el array
                arr[i] = _wtoi(temp->pokemon.hp.c_str());
                temp = temp->next;
                i++;
            }
            return arr;
        }
        return nullptr;
    }


};

int main() {
    init_apartment();

    // Realizar la solicitud HTTP en un bucle
    HttpClient httpClient;

    // Assuming you have a List class to store Pokemon objects
    DoublyLinkedList  pokemonList;

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

    
    }

    pokemonList.imprimirEnOrden();

    int* poke_hps = pokemonList.createArray();
    int longitud = 10;  // Tamaño conocido del array, ajusta según sea necesario



    quickSort(poke_hps, 0, longitud - 1);

    // Imprimir el resultado del quick sort
    std::wcout << "\nhp ordenados: ";
    for (int i = 0; i < longitud; ++i) {
        std::cout << poke_hps[i] << " ";
    }
    std::wcout << std::endl;

    return 0;
}



void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
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