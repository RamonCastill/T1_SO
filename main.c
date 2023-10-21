#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <stdbool.h>
#include <string.h>

// Estructura para guardar datos de procesos 
typedef struct{
    double t_llegada;
    double duracion;
} Proceso;
// Estructura para guardar tiempos y otros para sacar informacion
typedef struct{
    double t_espera;
    double t_respuesta;
    double R;
    double duracion;
    int largo_cola;
} Tiempo;

//Declaracion de funciones
void FCFS(Proceso procesos[1000],  Tiempo tiempos[1000],  double *t_procesador, double *t_durmiendo);
void HRRN(Proceso procesos[1000], Tiempo tiempos[1000], double *t_procesador, double *t_durmiendo);
void SPN(Proceso procesos[1000], Tiempo tiempos[1000], double *t_procesador, double *t_durmiendo);
void print_times(Tiempo tiempos[1000], double t_procesador, double t_durmiendo);

int main(int argc, char *argv[]){
    int opt;
    // Lectura del archivo 
    FILE *file_in = fopen("procesos.txt","r");

    if (file_in == NULL) {
        printf("No se pudo abrir el archivo\n");
        return 1;
    }

    // Creacion arrays de estructuras
    char buffer[1024];
    Proceso procesos[1000];
    Tiempo tiempos[1000];
    int i_read = 0;

    // Almacenamiento de cada linea del archivo procesos.txt en la estructura procesos
    while(fgets(buffer,1024,file_in)){
        double t_llegada, duracion;
        if (sscanf(buffer, "%lf %lf", &t_llegada, &duracion) == 2) {
            procesos[i_read].t_llegada = t_llegada;
            procesos[i_read].duracion = duracion;
            i_read++;
        } else {
            printf("Error al leer la línea %d\n", i_read + 1);
        }
    }
    fclose(file_in);

    double t_procesador = 0, t_durmiendo = 0;

    //promp para elegir la funcion a usar
    printf("Ingresa: \n'fcfs' --> First-Come, First-Served\n" );
    printf("'hrrn' --> Highest Response Ratio Next\n" );
    printf("'spn' --> Shortest Process Next\n" );
    printf("aquí --> " );

    char entrada[256];

    fgets(entrada,256,stdin);
    // Se elimina el caracter de termino de lo ingresado por el usuario
    entrada[strcspn(entrada, "\n")] = '\0';

    // se ejecuta la funcion dependiendo de lo que el usuario ingrese
    if (strcmp(entrada, "fcfs") == 0) {
        FCFS(procesos, tiempos, &t_procesador, &t_durmiendo);
    } else if (strcmp(entrada, "hrrn") == 0) {
        HRRN(procesos, tiempos, &t_procesador, &t_durmiendo);
    } else if (strcmp(entrada, "spn") == 0) {
        SPN(procesos, tiempos, &t_procesador, &t_durmiendo);
    } else {
        printf("Opción no válida: %s\n", entrada);
    }

    // Se imprimen los tiempos
    print_times(tiempos, t_procesador, t_durmiendo);


    return 0;     
}

void print_times(Tiempo tiempos[1000], double t_procesador, double t_durmiendo){
    // Declaracion de variables
    double t_espera_min = 1000, t_espera_max = 0, t_espera_promedio = 0;
    double duracion_min = 1000, duracion_max = 0, duracion_promedio = 0;
    double t_respuesta_min = 1000, t_respuesta_max = 0, t_respuesta_promedio = 0;
    double R_min = 1000, R_max = 0, R_promedio = 0;
    int l_cola_min = 1000, l_cola_max = 0,l_cola_promedio = 0;

    // Se recorre el array con estructuras de tipo tiempo para encontrar min y max
    for(int i = 0; i<1000; i++){
        t_espera_min = (tiempos[i].t_espera < t_espera_min) ? tiempos[i].t_espera : t_espera_min;
        t_espera_max = (tiempos[i].t_espera > t_espera_max) ? tiempos[i].t_espera : t_espera_max;
        duracion_min = (tiempos[i].duracion < duracion_min) ? tiempos[i].duracion : duracion_min;
        duracion_max = (tiempos[i].duracion > duracion_max) ? tiempos[i].duracion : duracion_max;
        t_respuesta_min = (tiempos[i].t_respuesta < t_respuesta_min) ? tiempos[i].t_respuesta : t_respuesta_min;
        t_respuesta_max = (tiempos[i].t_respuesta > t_respuesta_max) ? tiempos[i].t_respuesta : t_respuesta_max;
        R_min = (tiempos[i].R < R_min) ? tiempos[i].R : R_min;
        R_max = (tiempos[i].R > R_max) ? tiempos[i].R : R_max;
        l_cola_min = (tiempos[i].largo_cola < l_cola_min) ? tiempos[i].largo_cola : l_cola_min;
        l_cola_max = (tiempos[i].largo_cola > l_cola_max) ? tiempos[i].largo_cola : l_cola_max;

        // Se recorre el array con estructuras de tipo tiempo sumar todo
        t_espera_promedio += tiempos[i].t_espera;
        duracion_promedio += tiempos[i].duracion;
        t_respuesta_promedio += tiempos[i].t_respuesta;
        R_promedio += tiempos[i].R;
        l_cola_promedio += tiempos[i].largo_cola;
    }
    // Se hace el promedio
    t_espera_promedio /= 1000;
    duracion_promedio /= 1000;
    t_respuesta_promedio /= 1000;
    R_promedio /= 1000;
    l_cola_promedio /= 1000;

    // Mostrar por pantalla...
    printf("Mínimo t_espera: %lf\n", t_espera_min);
    printf("Máximo t_espera: %lf\n", t_espera_max);
    printf("Promedio t_espera: %lf\n", t_espera_promedio);

    printf("Mínima duración: %lf\n", duracion_min);
    printf("Máxima duración: %lf\n", duracion_max);
    printf("Promedio duración: %lf\n", duracion_promedio);

    printf("Mínima t_respuesta: %lf\n", t_respuesta_min);
    printf("Máxima t_respuesta: %lf\n", t_respuesta_max);
    printf("Promedio t_respuesta: %lf\n", t_respuesta_promedio);

    printf("Mínimo R: %lf\n", R_min);
    printf("Máximo R: %lf\n", R_max);
    printf("Promedio R: %lf\n", R_promedio);

    printf("Mínimo largo_cola: %i\n", l_cola_min);
    printf("Máximo largo_cola: %i\n", l_cola_max);
    printf("Promedio largo_cola: %i\n", l_cola_promedio);

    printf("Tiempo útil = %lfs, Utilización = %lf%\n", t_procesador, 100 - (t_durmiendo/t_procesador)*100);
}

void SPN(Proceso procesos[1000], Tiempo tiempos[1000], double *t_procesador, double *t_durmiendo){

    double t_espera = 0, t_respuesta = 0, R = 0,t_espera2 = 0, R2 = 0;
    Proceso procesos_ordenados[1000];
    int aux = 1;
    int lc = 0;
    bool compare = true;

    for (int i = 0; i < 1000; i++){
        // Explicaion en lineas 273 a 278...
        if(*t_procesador <= procesos[i].t_llegada){
            *t_durmiendo += procesos[i].t_llegada - *t_procesador;
            *t_procesador = procesos[i].t_llegada + procesos[i].duracion;
            t_espera = 0;
            t_respuesta = procesos[i].duracion;
        }
        else{ // Si ya hay procesos esperando se busca el que tenga menor duracion y se hace un swap para que sea el siguiente procesos en sumar tiempos
            compare = true;
            while(compare){
                if(*t_procesador >= procesos[i + aux].t_llegada && i + aux < 1000){
                    if(procesos[i].duracion > procesos[i + aux].duracion){
                        double a = procesos[i].t_llegada;
                        double b = procesos[i].duracion;
                        procesos[i].t_llegada = procesos[i + aux].t_llegada;
                        procesos[i].duracion = procesos[i + aux].duracion;
                        procesos[i + aux].t_llegada = a;
                        procesos[i + aux].duracion = b;

                       
                    }
                    aux++; // Se suma cada proceso en la cola
                }
                else{
                    lc = aux;
                    aux = 1;
                    compare = false;
                }

            }

            //aqui se actualizan los tiempos
            t_espera = *t_procesador - procesos[i].t_llegada;
            *t_procesador += procesos[i].duracion;
            t_respuesta = *t_procesador - procesos[i].t_llegada;
            
        }

        // R = ...
        R = (t_espera+procesos[i].duracion)/(procesos[i].duracion);
        // Se guardan los datos en el array de estructuras
        tiempos[i].t_espera = t_espera;
        tiempos[i].t_respuesta = t_respuesta;
        tiempos[i].R = R;
        tiempos[i].duracion = procesos[i].duracion;
        tiempos[i].largo_cola = lc;

    }
}

void HRRN(Proceso procesos[1000], Tiempo tiempos[1000], double *t_procesador, double *t_durmiendo){

    double t_espera = 0, t_respuesta = 0, R = 0,t_espera2 = 0, R2 = 0;
    Proceso procesos_ordenados[1000];
    int aux = 1;
    int lc = 0;
    bool compare = true;

    for (int i = 0; i < 1000; i++){
        // Explicaion en lineas 273 a 278...
        if(*t_procesador < procesos[i].t_llegada){
            *t_durmiendo += procesos[i].t_llegada - *t_procesador;
            *t_procesador = procesos[i].t_llegada + procesos[i].duracion;
            t_espera = 0;
            t_respuesta = procesos[i].duracion;
        }
        else{
            compare = true;
            while(compare){// Si ya hay procesos esperando se busca el que tenga mayor R y se hace un swap para que sea el siguiente procesos en sumar tiempos
                if(*t_procesador >= procesos[i + aux].t_llegada && i + aux < 1000){
                    t_espera = *t_procesador - procesos[i].t_llegada;
                    R = (t_espera+procesos[i].duracion)/(procesos[i].duracion);
                    t_espera2 = *t_procesador - procesos[i + aux ].t_llegada;
                    R2 = (t_espera2+procesos[i + aux].duracion)/(procesos[i + aux].duracion);
                    if(R < R2){
                        double a = procesos[i].t_llegada;
                        double b = procesos[i].duracion;
                        procesos[i].t_llegada = procesos[i + aux].t_llegada;
                        procesos[i].duracion = procesos[i + aux].duracion;
                        procesos[i + aux].t_llegada = a;
                        procesos[i + aux].duracion = b;

                       
                    }
                    aux++;
                }
                else{
                    lc = aux; // Se guarda el nro de procesos en la cola en ese momento
                    aux = 1;
                    compare = false;
                }

            }
            //aqui
            t_espera = *t_procesador - procesos[i].t_llegada;
            *t_procesador += procesos[i].duracion;
            t_respuesta = *t_procesador - procesos[i].t_llegada;
            
        }

        R = (t_espera+procesos[i].duracion)/(procesos[i].duracion);
        // Se guardan los datos en el array de estructuras
        tiempos[i].t_espera = t_espera;
        tiempos[i].t_respuesta = t_respuesta;
        tiempos[i].R = R;
        tiempos[i].duracion = procesos[i].duracion;
        tiempos[i].largo_cola = lc;

    }
}

void FCFS(Proceso procesos[1000], Tiempo tiempos[1000], double *t_procesador, double *t_durmiendo){

    double t_espera = 0, t_respuesta = 0, R = 0;
    //Se recorre el array de estrucutras con los datos del proceso
    for (int i = 0; i < 1000; ++i){ 
        // Si el tiempo del procesador es menor que el timpo del siguinte procesos
        if(*t_procesador < procesos[i].t_llegada){ // Se le suma esa diferencia de tiempo para simluar la espera
            *t_durmiendo += procesos[i].t_llegada - *t_procesador; // Se guarda el tiempo de espera
            *t_procesador = procesos[i].t_llegada + procesos[i].duracion; //Se actualiza el tiempo del procesador
            t_espera = 0; // el proceso no espero
            t_respuesta = procesos[i].duracion; //como no espero ...


        }
        else{ // Por otro lado, si hay un proceso esperando
            t_espera = *t_procesador - procesos[i].t_llegada; // Se calcula el tiempo de espera
            *t_procesador += procesos[i].duracion; // Se actualiza el tiempo del procesador
            t_respuesta = *t_procesador - procesos[i].t_llegada; // El riempo de respuesta es la dif entre el tiempo del procesador y el tiempo en el que el proceso llego
        }

        // Se cuenta el numero de procesos en la cola de espera
        bool contar = true;
        int lc = 1;
        while(contar){
            if(*t_procesador >= procesos[i + lc].t_llegada && i + lc < 1000){
                lc++;
            }
            else{
                contar = false;
            }
        }

        // Se calcula R...
        R = (t_espera+procesos[i].duracion)/(procesos[i].duracion);

        // Se guardan los datos en el array de estructuras
        tiempos[i].t_espera = t_espera;
        tiempos[i].t_respuesta = t_respuesta;
        tiempos[i].R = R;
        tiempos[i].duracion = procesos[i].duracion;
        tiempos[i].largo_cola = lc;
    }
}