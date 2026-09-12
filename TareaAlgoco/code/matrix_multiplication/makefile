CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17 -Iinclude -Ialgorithms

SRCS = matrix_multiplication.cpp \
       algorithms/naive.cpp \
       algorithms/strassen.cpp

OBJS = $(SRCS:.cpp=.o)
TARGET = matrix_multiplication

all: $(TARGET) inputs run plots

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

inputs:
	@echo "=== Generando matrices de prueba ==="
	@mkdir -p data/matrix_input data/measurements data/matrix_output data/plots
	@cd scripts && python3 matrix_generator.py

run: $(TARGET)
	@echo "=== Ejecutando mediciones de matrices ==="
	@mkdir -p data/measurements data/matrix_output
	@echo "algoritmo,n,tipo,dominio,muestra,tiempo_us,memoria_kb" > data/measurements/matrix_measurements.csv
	@for f1 in data/matrix_input/*_1.txt; do \
		[ -e "$$f1" ] || continue; \
		f2="$${f1%_1.txt}_2.txt"; \
		if [ -f "$$f2" ]; then \
			for algo in naive strassen; do \
				./$(TARGET) "$$algo" "$$f1" "$$f2" >> data/measurements/matrix_measurements.csv; \
			done; \
		fi; \
	done
	@echo "=== Mediciones de matrices completadas ==="

plots:
	@echo "=== Generando graficos de matrices ==="
	@mkdir -p data/plots
	@cd scripts && python3 plot_generator.py

clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf data/matrix_input/* data/matrix_output/* data/measurements/* data/plots/*

.PHONY: all clean inputs run plots