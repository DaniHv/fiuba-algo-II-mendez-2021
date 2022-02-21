#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "split.h"

/*
 * Pre: El puntero string deberá ser un puntero válido (no NULL)
 * Post: devolverá la cantidad de carácteres pedidos encontrados en el string recibido.
 */
size_t contar_caracteres(const char* string, char caracter) {
    size_t caracteres = 0;
    size_t i = 0;

    while(string[i]) {
        if (string[i] == caracter) {
            caracteres++;
        }
        i++;
    }

    return caracteres;
}

/*
 * Pre: El puntero string deberá ser un puntero válido (no NULL)
 * Post: Copiará el contenido del string que se encuentra dentro de la posición solicitada
 *       (considerando la división entre cada separador), y la devolverá en un string nuevo.
 */
char* separar_string(const char* string, char separador, size_t posicion) {
    size_t i = 0;
    size_t actual_posicion = 0;
    size_t actual_caracter = 0;

    char* substring = (char*) malloc(sizeof(char));

    if (!substring) return NULL;

    while (string[i] && actual_posicion <= posicion) {
        if (string[i] == separador) {
            actual_posicion++;
        } else if(actual_posicion == posicion) {
            substring = (char*) realloc(substring, sizeof(char) * (actual_caracter + 2));
            if (!substring) return NULL;

            substring[actual_caracter] = string[i];
            actual_caracter++;
        }

        i++;
    }

    if (substring) {
        substring[actual_caracter] = '\0';
    }

    return substring;
}

/*
 * Pre: -
 * Post: Dividirá el string recibido separando por el caracter separador indicado,
 *       y devolverá un array de strings con el resultado.
 */
char** split(const char* string, char separador) {
    if (!string) return NULL;

    size_t caracteres = contar_caracteres(string, separador);
    size_t substrings = caracteres + 1;

    char** splited_string = (char**) calloc(substrings + 1, sizeof(char*));

    if (!splited_string) return NULL;

    for (size_t i = 0; i < substrings; i++) {
        char* substring = separar_string(string, separador, i);

        if (!substring) return NULL;

        splited_string[i] = substring;
    }
    
    return splited_string;
}
