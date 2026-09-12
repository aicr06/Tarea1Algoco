// matrix_multiplication.cpp — Programa orquestador de mediciones para los
// algoritmos de multiplicación de matrices.
//
// Mismo diseño que sorting.cpp: una sola combinación (algoritmo, par de
// matrices) por ejecución, para que ru_maxrss no se contamine entre
// algoritmos distintos corridos en el mismo proceso.
//
// Uso: ./matrix_exec <algoritmo> <matriz_A> <matriz_B>
// Algoritmos válidos: naive, strassen
// Salida (stdout): algoritmo,n,tipo,dominio,muestra,tiempo_us,memoria_kb

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <cmath>
#include <stdexcept>
#include <sys/resource.h>

using Matrix = std::vector<std::vector<int>>;

// Declaraciones de funciones implementadas en algorithms/*.cpp
Matrix matrixMultNaive(const Matrix& M1, const Matrix& M2, int n);
Matrix matrixMultStrassen(const Matrix& M1, const Matrix& M2, int n);

// Lee una matriz cuadrada de un archivo con n filas de n números cada una,
// sin línea de encabezado con la dimensión (formato real de matrix_generator.py).
// Se leen todos los números del archivo sin importar los saltos de línea,
// y n se infiere como sqrt(cantidad_total_de_numeros).
Matrix leer_matriz(const std::string& ruta_archivo, int& n) {
    std::ifstream archivo(ruta_archivo);
    if (!archivo.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + ruta_archivo);
    }

    std::vector<int> plano;
    int valor;
    while (archivo >> valor) {
        plano.push_back(valor);
    }
    archivo.close();

    n = static_cast<int>(std::round(std::sqrt(plano.size())));
    if (static_cast<size_t>(n) * static_cast<size_t>(n) != plano.size()) {
        throw std::runtime_error("El archivo no contiene una matriz cuadrada valida: " + ruta_archivo);
    }

    Matrix matriz(n, std::vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            matriz[i][j] = plano[i * n + j];
        }
    }
    return matriz;
}

// Guarda la matriz resultante con formato separado por espacios
void escribir_matriz(const std::string& ruta_archivo, const Matrix& matriz) {
    std::ofstream archivo(ruta_archivo);
    if (!archivo.is_open()) return;
    int n = static_cast<int>(matriz.size());
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            archivo << matriz[i][j];
            if (j + 1 < n) archivo << " ";
        }
        archivo << "\n";
    }
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

// Extrae n, tipo, dominio y muestra desde el nombre base del archivo
// (ignora el sufijo _1/_2 que distingue matriz A de matriz B)
struct metadatos_matriz {
    std::string n, tipo, dominio, muestra;
};

metadatos_matriz extraer_metadatos(const std::string& nombre_base) {
    metadatos_matriz metadatos;
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
    if (argc != 4) {
        std::cerr << "Uso: " << argv[0] << " <algoritmo> <matriz_A> <matriz_B>\n";
        std::cerr << "Algoritmos disponibles: naive, strassen\n";
        return 1;
    }

    std::string algoritmo = argv[1];
    std::string ruta_a = argv[2];
    std::string ruta_b = argv[3];

    int n_a = 0, n_b = 0;
    Matrix matriz_a, matriz_b;

    try {
        matriz_a = leer_matriz(ruta_a, n_a);
        matriz_b = leer_matriz(ruta_b, n_b);
    } catch (const std::exception& e) {
        std::cerr << "Error de lectura: " << e.what() << "\n";
        return 1;
    }

    if (n_a != n_b || n_a == 0) {
        std::cerr << "Dimensiones incompatibles: A tiene " << n_a << " y B tiene " << n_b << "\n";
        return 1;
    }

    Matrix resultado;
    auto inicio = std::chrono::steady_clock::now();

    if (algoritmo == "naive") {
        resultado = matrixMultNaive(matriz_a, matriz_b, n_a);
    } else if (algoritmo == "strassen") {
        resultado = matrixMultStrassen(matriz_a, matriz_b, n_a);
    } else {
        std::cerr << "Algoritmo desconocido: " << algoritmo << "\n";
        return 1;
    }

    auto fin = std::chrono::steady_clock::now();
    auto duracion_us = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
    long memoria_kb = obtener_memoria_kb();

    // Metadatos extraídos ANTES de armar el nombre de salida, para no
    // arrastrar el sufijo _1 (de matriz A) al nombre del resultado
    metadatos_matriz metadatos = extraer_metadatos(obtener_nombre_base(ruta_a));
    std::string base = metadatos.n + "_" + metadatos.tipo + "_" + metadatos.dominio + "_" + metadatos.muestra;
    std::string ruta_salida = "data/matrix_output/" + algoritmo + "_" + base + ".txt";
    escribir_matriz(ruta_salida, resultado);

    std::cout << algoritmo << ","
              << metadatos.n << ","
              << metadatos.tipo << ","
              << metadatos.dominio << ","
              << metadatos.muestra << ","
              << duracion_us << ","
              << memoria_kb << "\n";

    return 0;
}