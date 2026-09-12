CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17 -Iinclude -Ialgorithms

SRCS = sorting.cpp \
       algorithms/sort.cpp \
       algorithms/mergesort.cpp \
       algorithms/quicksort.cpp \
       algorithms/patiencesort.cpp

OBJS = $(SRCS:.cpp=.o)
TARGET = sorting

all: $(TARGET) inputs run plots

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

inputs:
	@echo "=== Generando arreglos de prueba ==="
	@mkdir -p data/array_input data/measurements data/array_output data/plots
	@cd scripts && python3 array_generator.py

run: $(TARGET)
	@echo "=== Ejecutando mediciones de sorting ==="
	@mkdir -p data/measurements data/array_output
	@echo "algoritmo,n,tipo,dominio,muestra,tiempo_us,memoria_kb" > data/measurements/sorting_measurements.csv
	@for f in data/array_input/*.txt; do \
		[ -e "$$f" ] || continue; \
		for algo in sort mergeSort quickSort patienceSort; do \
			./$(TARGET) "$$algo" "$$f" >> data/measurements/sorting_measurements.csv; \
		done; \
	done
	@echo "=== Mediciones de sorting completadas ==="

plots:
	@echo "=== Generando graficos de sorting ==="
	@mkdir -p data/plots
	@cd scripts && python3 plot_generator.py

clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf data/array_input/* data/array_output/* data/measurements/* data/plots/*

.PHONY: all clean inputs run plots