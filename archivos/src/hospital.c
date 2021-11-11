#include <stdio.h>
#include <string.h>
#include "hospital.h"
#include "split.h"

struct _hospital_pkm_t{
    size_t cantidad_pokemon;
    size_t cantidad_entrenadores;
    pokemon_t* vector_pokemones;
};


struct _pkm_t{
    char* nombre;
    size_t nivel;
};


hospital_t* hospital_crear(){
    return calloc(1, sizeof(hospital_t));
}


char* leer_linea_dinamico(FILE* archivo){
    size_t bytes_leidos = 0;
    size_t tamanio = 1024;
    char* buffer = malloc(tamanio);

    if(!buffer){
        return NULL;
    }

    while(fgets(buffer + bytes_leidos, (int)(tamanio - bytes_leidos), archivo)){
        size_t leido = strlen(buffer + bytes_leidos);
        if (leido > 0 && *(buffer + bytes_leidos + leido - 1) == '\n'){
            *(buffer + bytes_leidos + leido - 1) = 0;
            return buffer;
        }
        else{
            char* auxiliar = realloc(buffer, tamanio * 2);
            if(!auxiliar){
                free(buffer);
                return NULL;
            }
            buffer = auxiliar;
            tamanio *= 2;
        }
        bytes_leidos += leido;
    }
    if(bytes_leidos == 0){
        free(buffer);
        return NULL;
    }

    return buffer;
}


void imprimir(pokemon_t* pokemon, size_t cantidad_pokemon){
    for (int i = 0; i < cantidad_pokemon; ++i){
        printf("%s , %li \n", pokemon[i].nombre, pokemon[i].nivel);
    }
}


void ordenar_pokemon(pokemon_t* pokemon, size_t cantidad){

    pokemon_t auxiliar;


    for (int x = 1; x < cantidad; x++){

        int indiceActual;

        for (indiceActual = 0; indiceActual < cantidad - 1; indiceActual++){

            int indiceSiguienteElemento = indiceActual + 1;

                    if (strcmp(pokemon[indiceActual].nombre, pokemon[indiceSiguienteElemento].nombre) > 0){
                        // Intercambiar
                        memcpy(&auxiliar, &pokemon[indiceActual], sizeof(pokemon_t));
                        memcpy(&pokemon[indiceActual], &pokemon[indiceSiguienteElemento], sizeof(pokemon_t));
                        memcpy(&pokemon[indiceSiguienteElemento], &auxiliar, sizeof(pokemon_t));
                    }
                }

    }


}


void agregar_pokemones(hospital_t* hospital, pokemon_t pokemon){
    if(!hospital){
        return;
    }

    pokemon_t* auxiliar_pokemon = realloc(hospital->vector_pokemones, sizeof(pokemon_t)*(hospital->cantidad_pokemon+1));

    if (!auxiliar_pokemon){
        return;
    }

    hospital->vector_pokemones = auxiliar_pokemon;
    hospital->vector_pokemones[hospital->cantidad_pokemon] = pokemon;
    hospital->cantidad_pokemon++;

}


void liberar_linea_separada(char** linea_separada, size_t cantidad){
    for (int i = 0; i < cantidad; i++){
        free(linea_separada[i]);
    }
    free(linea_separada);
}

char* devolver_copia_cadena(char* nombre){
    char* auxiliar;
    auxiliar = malloc(sizeof(char) * (strlen(nombre) + 1));
    strcpy(auxiliar, nombre);
    return auxiliar;
}

bool hospital_leer_archivo(hospital_t* hospital, const char* nombre_archivo){
    FILE* archivo = fopen(nombre_archivo, "r");
    if(!archivo){
        return false;
    }

    while(archivo){
        char* linea = NULL;        
        char** linea_separada = NULL;
        size_t cantidad_de_vectores = 2;

        linea = leer_linea_dinamico(archivo);
        linea_separada = split(linea, ';');

        free(linea);

        if (!linea_separada){
            fclose(archivo);
            return true;
        }

        while(linea_separada[cantidad_de_vectores]){

            size_t nivel = (size_t)atoi(linea_separada[cantidad_de_vectores+1]);

            pokemon_t datos_pokemon = {devolver_copia_cadena(linea_separada[cantidad_de_vectores]), nivel};
            
            agregar_pokemones(hospital, datos_pokemon);


            cantidad_de_vectores+=2;

        }

        hospital->cantidad_entrenadores++;
    liberar_linea_separada(linea_separada, cantidad_de_vectores);

    }

    fclose(archivo);
    return true;
}


size_t hospital_cantidad_pokemon(hospital_t* hospital){
    if(!hospital){
        return 0;
    }
    else{
        return hospital->cantidad_pokemon;
    }
}


size_t hospital_cantidad_entrenadores(hospital_t* hospital){
    if (!hospital){
        return 0;
    }
    else{
        return hospital->cantidad_entrenadores;
    }
}


size_t hospital_a_cada_pokemon(hospital_t* hospital, bool (*funcion)(pokemon_t* p)){
    if(!funcion){
        return 0;
    }


    if(!hospital){
        return 0;
    }

    ordenar_pokemon(hospital->vector_pokemones, hospital->cantidad_pokemon);    
    pokemon_t* p = hospital->vector_pokemones;
    size_t contador = 0;

    for (int i=0; i < hospital->cantidad_pokemon; i++){
        if(!funcion(&p[i])){
            contador++;
            return contador;
        }
        contador++;

    }

    return contador;
}

void pokemones_destuir(pokemon_t* pokemon, size_t cantidad){
    for(int i = 0; i < cantidad; i++){
        free(pokemon[i].nombre);
    }
}

void hospital_destruir(hospital_t* hospital){
    if (!hospital){
        return;
    }
    pokemones_destuir(hospital->vector_pokemones, hospital->cantidad_pokemon);
    free(hospital->vector_pokemones);
    free(hospital);
}


size_t pokemon_nivel(pokemon_t* pokemon){
    if (!pokemon){
        return 0;
    }
    else{
        return pokemon->nivel;
    }
}


const char* pokemon_nombre(pokemon_t* pokemon){
    if (!pokemon){
        return NULL;
    }
    else{
        return pokemon->nombre;
    }
}
