/*********************************************************************************
* File:   betris.cpp
* Author: Alicia Lázaro Huerta  820574
*         Inés Román Gracia  820731
* Date:  10/05/2022
* Coms:  Fichero de implementación del módulo betris
*********************************************************************************/


#include<fstream>
#include<iostream>
#include<chrono>
#include<thread>
#include<iomanip>
#include "betris.hpp"
using namespace std;

// Pre: true
// Post: Todas las componentes de tablero.matriz son -1 (casilla vacía)
void inicializarTablero(tpTablero &tablero){
    for(int i=0;i<MAXDIM;i++){
        for(int j =0;j<MAXDIM;j++){
            tablero.matriz[i][j]=-1;
        }
    }
}

// Pre: vEntrada contiene los números de las piezas que entran y tablero contiene el estado actual 
//      del tablero, en el que pueden estar colocadas algunas de dichas piezas 
// Post: Se ha mostrado el tablero por pantalla (cada pieza se ha dibujado con su color)
void mostrarTablero(const tpTablero & tablero, const int vEntrada[MAXENTRADA]){
    system("clear");
    int n;
   for(int i = 0; i < tablero.nfils; i++){
       cout << "\t";
       for(int j = 0; j < tablero.ncols; j++){
           n = tablero.matriz[i][j];
           if(n < 0){ // huecos vacíos
               cout << setw(1) << " ";
           }else if(vEntrada[n] == 0){ // pieza azul
               cout << setw(1) <<"\033[34;44m" << " " << "\033[0m";
           }else if(vEntrada[n] == 1){ // pieza verde
               cout << setw(1) <<  "\033[34;42m" << " " << "\033[0m";
           }else if(vEntrada[n] == 2){ // pieza roja
               cout << setw(1) << "\033[34;41m" << " " << "\033[0m";
           }else if(vEntrada[n] == 3){ // pieza cian
               cout << setw(1) << "\033[34;46m" << " " << "\033[0m";
           }else if(vEntrada[n] == 4){ // pieza amarilla
               cout << setw(1) << "\033[34;43m" << " " << "\033[0m";
           }else{
               cout << setw(1) << " ";
           }
        }
       cout << endl;
    }
}

// Función auxiliar de la función poner
// Pre: se le pasan los parámetros del número de pieza, el número de columna y de fila
// Post: devuelve true si la pieza cabe en esa coordenada y false en caso contrario
bool cabe(const tpTablero tablero, const int pieza, const int columna, const int fila){
    bool sePuede = true;
    int j, k;
    for(int i = 0; i < TAMPIEZA; i++){
        j = fila + vPiezas[pieza].forma[i][0];
        k = columna + vPiezas[pieza].forma[i][1];
        for(int l = j; l >= 0; l--){ // comprueba que no haya piezas por encima en la misma columna
            if(tablero.matriz[l][k] > -1){
                sePuede = false;
            }
        }
        if((j >= tablero.nfils) || (k >= tablero.ncols) || (tablero.matriz[j][k] > -1)){ 
            // comprueba que la pieza no se salga de los límites del tablero o que ocupe
            // el lugar de otra pieza
            sePuede = false;
        } 
    }
    return sePuede;
}

// Pre: se le pasa el número de pieza y el número de columna del tablero
// Post: devuelve true si la pieza cabe en esa columna y f toma el valor del número de fila en el que cabe,
//      y en caso de que no quepa en esa columna devuelve false
bool poner(tpTablero &tablero, const int pieza, const int columna, int &f, const int n){
    int fila = tablero.nfils-1; 
    bool sePuede = cabe(tablero, pieza, columna, fila); // miramos si cabe en el fondo del tablero
    while(!sePuede && fila>0){ // miramos desde el fondo del tablero hasta encontrar una fila en la que
        fila--;                // quepa la pieza o no queden más filas
        sePuede = cabe(tablero, pieza, columna, fila);
    }
    int j, k;
    if(sePuede){ // si la pieza cabe en alguna fila para la columna que le hemos asignado la colocamos
                 // en el tablero
        f = fila;     
        for(int i = 0; i < TAMPIEZA; i++){
            j = fila + vPiezas[pieza].forma[i][0];
            k = columna + vPiezas[pieza].forma[i][1];
            tablero.matriz[j][k] = n;
        }
    }
    return sePuede;
}

// Pre: se le pasa el número de pieza, la columna y la fila donde esta se encuentra en el tablero
// Post: se elimina la pieza del tablero
void quitar(tpTablero &tablero, const int pieza, const int columna, const int fila){
    int j, k;
    for(int i = 0; i < TAMPIEZA; i++){
        j = fila + vPiezas[pieza].forma[i][0];
        k = columna + vPiezas[pieza].forma[i][1];
        tablero.matriz[j][k] = -1;
    }
}

// Pre: se le pasa el número de filas que hay que completar
// Post: se devuelve true si se han completado al menos ese número de filas y false en caso contrario
bool correcto(const tpTablero tablero, const int objetivo){
    int contador;       // num de huecos completos en una fila
    bool hecho = false; // indica si ya se ha completado el objetivo y no hay que seguir buscando
    int completas = 0;  // num de filas completas encontradas hasta el momento
    bool sig = false;   // indica si hay un hueco en una fila para pasar a mirar la siguiente
    for(int i = 0; i < tablero.nfils && !(hecho); i++){
        contador = 0;
        sig = false;
        for(int j = 0; j < tablero.ncols && !(sig); j++){
            if(tablero.matriz[i][j] > -1){
                contador++;
            }else{
                sig = true;
            }
        }
        if(contador == tablero.ncols){
            completas++;
            if(completas == objetivo){
                hecho = true;
            }
        }
    }
    return completas == objetivo;
}

// Pre: En el tablero se han colocada las n primeras piezas de vEntrada, en la columnas indicadas respectivamente en vSalida
// Post: Si las piezas colocadas completan al menos objetivo filas sin huecos,
//       entonces
//           devuelve el número de piezas colocadas, en vSalida las columnas en las que se han colocado
//           las piezas y el tablero con las piezas colocadas
//       si no devuelve -1
int buscaSolucion(tpTablero &tablero, const int vEntrada[MAXENTRADA], int vSalida[MAXENTRADA], const int objetivo, int n, const int retardo){
    bool ponerPieza;
    int colocadas = 0;
    int fila;
    for(int columna=0;columna<tablero.ncols;columna++){ // intentamos colocar cada pieza columna por columna
        ponerPieza = poner(tablero, vEntrada[n], columna, fila, n);
        if(ponerPieza){ // la pieza cabe en la columna en la que la estamos colocando
            if(retardo>0){ // la mostramos si retardo > 0
                mostrarTablero(tablero, vEntrada);
                this_thread::sleep_for(chrono::milliseconds(retardo));
            }
            vSalida[n] = columna;// numero de columna de la pieza colocada
            
            if(correcto(tablero, objetivo)){ // si hemos completado el objetivo al colocar la pieza 
                 return n+1;                 // buscaSolución devuelve el num de piezas colocadas

            }else if(vEntrada[n+1]>-1){ // sino seguimos con la siguiente pieza
                colocadas = buscaSolucion(tablero, vEntrada, vSalida, objetivo, n+1, retardo);
                
                if(colocadas > -1){ // si colocando las siguientes piezas encontramos una solución 
                    return colocadas; // buscaSolución devuelve el num de piezas colocadas
                }
            }
            quitar(tablero, vEntrada[n], columna, fila); 
            // no encontramos ua solución al colocar esta pieza en esta columna: la borramos del
            // tablero y pasamos a intentar ponerla en la siguiente columna
                                                        
        }
    }
    // no hay ninguna solución al poner esta pieza en ninguna columna tal como está puesto el tablero
    return -1; 
}
