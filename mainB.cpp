/*********************************************************************************
* File:   mainB.cpp
* Author: Alicia Lázaro Huerta  820574
*         Inés Román Gracia  820731
* Date:  10/05/2022
* Coms: programa principal que lee los par ametros de entrada y llama a las 
*       funciones descritas en betris.hpp para resolver el juego de betris
*        ./mainB para ejecutar
*********************************************************************************/

#include<iostream>
#include<iomanip>
#include "betris.hpp"
#include <chrono>
#include <random>
using namespace std;

void crearPiezas(const int n, int v[]) {
    time_t semilla = time(nullptr);
    srand(semilla);
    for (int i = 0; i < n; i++) {
        v[i] = rand() % (PIEZASDEF);
    }
    v[n] = -1;
    /*random_device rd;
    default_random_engine eng(rd());
    uniform_int_distribution<int> distr(0,PIEZASDEF);
    for (int i = 0; i < n; i++) {
        v[i] = distr(eng);
    }*/

}

int main(){
    cout << "Introduzca el número de filas, columnas, objetivo de filas a llenar, el retardo (si deseado) y las piezas a utilizar: " << endl; 
    tpTablero tablero;
    cin >> tablero.nfils;
    tablero.nfils = 5;
    cin >> tablero.ncols;
    tablero.ncols = 5;
    tablero.matriz[tablero.nfils][tablero.ncols];

    int objetivo;
    cin >> objetivo;
    objetivo = 4;

    int retardo;
    cin >> retardo;
    retardo = 100;
    if (retardo < 0) {                              // si el retardo es menor que 0, no habra retardo
        retardo = 0;
    }

    int vEntrada[MAXENTRADA];

    int numPiezas = 0;

    cin >> vEntrada[0];
    
    if (vEntrada[0] < 0) {
        numPiezas = (-1) * vEntrada[0];
        crearPiezas(numPiezas, vEntrada);           // en caso de no recibir piezas las genera aleatoriamente
    } else {
        for (int i = 1; i < MAXENTRADA; i++) {
            cin >> vEntrada[i];                         // se guardan las piezas en el vector de entrada
            if (vEntrada[i] == -1) {                    // en el momento en que la pieza sea "-1" se sale del bucle
                numPiezas = i;                           
                i = MAXENTRADA;            
            }
        }
    }    
    


    inicializarTablero(tablero);
    

    int vSalida[MAXENTRADA];
    int numPiezasSolucion = buscaSolucion(tablero, vEntrada, vSalida, objetivo, 0, retardo);

    cout << "Entrada (números de piezas): ";

    for (int i = 0; i < numPiezas; i++) {
        cout << vEntrada[i] << " ";
    }
    cout << endl;


    cout << "Número de piezas colocadas: " << numPiezasSolucion << endl;

    cout << "Salida (números de columnas): ";

    for (int i = 0; i < numPiezasSolucion; i++) {
        cout << vSalida[i] << " ";
    }
    cout << endl;

}
