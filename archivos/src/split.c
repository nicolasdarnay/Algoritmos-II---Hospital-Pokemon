#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <split.h>

size_t contar_separador(const char* string, char separador){
    if(*string==0){
        return 0;
    }

    if(*string == separador){
        return 1+contar_separador(string+1, separador);
    }

    return contar_separador(string+1, separador);
}

size_t buscar_posicion_separador(const char* string, char separador){
    size_t i=0;
    while (string[i] != '\0' && string[i] != separador){
        i++;
    }

    return i;
}

char* duplicar_string(const char* string, size_t cantidad){
    char* nuevo_string = malloc((1+cantidad)*sizeof(char));
    if(!nuevo_string){
        return NULL;
    }

    for (size_t i=0; i < cantidad; i++){
        nuevo_string[i] = string[i];
    }

    nuevo_string[cantidad] = 0;
    return nuevo_string;
}

void liberar_vector(char** vector){
    while(*vector){
        free(*vector);
        vector++;
    }
}

char** split(const char* string, char separador){
    if(!string){
        return NULL;
    }

    size_t separadores = contar_separador(string, separador);
    size_t cantidad_substrings = separadores+1;
    
    char** vector = calloc(cantidad_substrings+1, sizeof(void*));
    if(!vector){
        return NULL;
    }

    for (size_t n = 0; n < cantidad_substrings; n++){
        size_t tamanio_substring = buscar_posicion_separador(string, separador);
        char *substring = duplicar_string(string, tamanio_substring);
        
        if (!substring){
            liberar_vector(vector);
            return NULL;
        }

        vector[n] = substring;
        string += tamanio_substring+1;
    }

    return vector;
}
