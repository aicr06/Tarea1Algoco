// sorting.cpp — Programa orquestador de mediciones para los algoritmos
// de ordenamiento.
//
// Diseño: este programa mide UNA sola combinación (algoritmo, archivo) por
// ejecución. getrusage()/ru_maxrss reporta el pico de memoria de todo el
// proceso desde que arrancó y no se puede "resetear" dentro de un mismo
// main(), así que un script/loop externo (makefile) invoca este ejecutable
// muchas veces, una por cada combinación de algoritmo x archivo, cada vez
// como un proceso nuevo, para que las mediciones de memoria no se contaminen
// entre algoritmos.
//
// Uso: ./sorting_exec <algoritmo> <ruta_archivo_input>
// Algoritmos válidos: sort, mergeSort, quickSort, patienceSort
// Salida (stdout): algoritmo,n,tipo,dominio,muestra,tiempo_us,memoria_kb

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <stdexcept>
#include <sys/resource.h>

std::vector<int> sortArray(std::vector<int>& arr);
void mergeSort(std::vector<int>& A);
void quickSort(std::vector<int>& A);
void patienceSort(std::vector<int>& A);

std::vector<int> leer_arreglo(const std::string& ruta) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + ruta);
    }
    std::vector<int> arreglo;
    int valor;
    while (archivo >> valor) {
        arreglo.push_back(valor);
    }
    archivo.close();
    return arreglo;
}

void escribir_arreglo(const std::string& ruta, const std::vector<int>& arreglo) {
    std::ofstream archivo(ruta);
    if (!archivo.is_open()) return;
    for (size_t i = 0; i < arreglo.size(); ++i) {
        archivo << arreglo[i];
        if (i + 1 < arreglo.size()) archivo << " ";
    }
    archivo << "\n";
    archivo.close();
}

std::string obtener_nombre_base(const std::string& ruta) {
    size_t ultima_barra = ruta.find_last_of("/\\");
    std::string nombre_archivo = (ultima_barra == std::string::npos) ? ruta : ruta.substr(ultima_barra + 1);
    size_t posicion_punto = nombre_archivo.find_last_of('.');
    if (posicion_punto != std::string::npos) {
        nombre_archivo = nombre_archivo.substr(0, posicion_punto);
    }
    return nombre_archivo;
}

struct metadatos_arreglo {
    std::string n, tipo, dominio, muestra;
};

metadatos_arreglo extraer_metadatos(const std::string& nombre_base) {
    metadatos_arreglo metadatos;
    std::stringstream flujo(nombre_base);
    std::getline(flujo, metadatos.n, '_');
    std::getline(flujo, metadatos.tipo, '_');
    std::getline(flujo, metadatos.dominio, '_');
    std::getline(flujo, metadatos.muestra, '_');
    return metadatos;
}

long obtener_memoria_kb() {
    struct rusage uso;
    getrusage(RUSAGE_SELF, &uso);
    return uso.ru_maxrss;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <algoritmo> <ruta_archivo_input>\n";
        std::cerr << "Algoritmos disponibles: sort, mergeSort, quickSort, patienceSort\n";
        return 1;
    }

    std::string algoritmo = argv[1];
    std::string ruta_entrada = argv[2];

    std::vector<int> arreglo;
    try {
        arreglo = leer_arreglo(ruta_entrada);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    auto inicio = std::chrono::steady_clock::now();

    if (algoritmo == "sort") {
        arreglo = sortArray(arreglo);
    } else if (algoritmo == "mergeSort") {
        mergeSort(arreglo);
    } else if (algoritmo == "quickSort") {
        quickSort(arreglo);
    } else if (algoritmo == "patienceSort") {
        patienceSort(arreglo);
    } else {
        std::cerr << "Algoritmo desconocido: " << algoritmo << "\n";
        return 1;
    }

    auto fin = std::chrono::steady_clock::now();
    auto duracion_us = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    long memoria_kb = obtener_memoria_kb();

    std::string nombre_base = obtener_nombre_base(ruta_entrada);
    std::string ruta_salida = "data/array_output/" + algoritmo + "_" + nombre_base + ".txt";
    escribir_arreglo(ruta_salida, arreglo);

    metadatos_arreglo metadatos = extraer_metadatos(nombre_base);
    std::cout << algoritmo << ","
              << metadatos.n << ","
              << metadatos.tipo << ","
              << metadatos.dominio << ","
              << metadatos.muestra << ","
              << duracion_us << ","
              << memoria_kb << "\n";

    return 0;
}