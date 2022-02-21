#include <string.h>
#include <stdio.h>
#include "simulador.h"
#include "hospital_structs.h"
#include "lista.h"
#include "cola.h"
#include "heap.h"

const char MENSAJE_ACERTADO[] = "ACERTASTE!";
const char MENSAJE_CERCA[] = "Estás cerca :D";
const char MENSAJE_LEJOS[] = "Estás lejos :(";

const int ACERTADO = 0;
const int CERCA = 1;
const int LEJOS = -1;

const unsigned PUNTAJE_PERFECTO = 100;

const unsigned DIFICULTAD_DEFAULT_ID = 1;

const char DIFICULTAD_FACIL_NOMBRE[] = "FACIL";
const unsigned DIFICULTAD_FACIL_PENALIZACION_PUNTAJE = 5;
const unsigned DIFICULTAD_FACIL_PORCENTAJE_CERCA= 20;

const char DIFICULTAD_NORMAL_NOMBRE[] = "NORMAL";
const unsigned DIFICULTAD_NORMAL_PENALIZACION_PUNTAJE = 10;
const unsigned DIFICULTAD_NORMAL_PORCENTAJE_CERCA= 10;

const char DIFICULTAD_DIFICIL_NOMBRE[] = "DIFICIL";
const unsigned DIFICULTAD_DIFICIL_PENALIZACION_PUNTAJE = 20;
const unsigned DIFICULTAD_DIFICIL_PORCENTAJE_CERCA= 5;

struct _simulador_t {
    hospital_t* hospital;
    bool simulacion_finalizada;
    unsigned cantidad_eventos_simulados;
    unsigned puntos;
    unsigned dificultad;
    lista_t* dificultades;
    unsigned entrenadores_atendidos;
    cola_t* entrenadores_en_espera;
    heap_t* pokemones_en_espera;
    pokemon_t* pokemon_en_atencion;
    unsigned pokemones_atendidos;
    unsigned cantidad_intentos_adivinar;
};

bool encolar_entrenadores(void* elemento, void* aux) {
    if (!elemento || !aux) return false;

    cola_encolar((cola_t*) aux, elemento);

    return true;
}

unsigned calcular_puntaje_facil(unsigned cantidad_intentos) {
    return PUNTAJE_PERFECTO - ((1 - cantidad_intentos) * DIFICULTAD_FACIL_PENALIZACION_PUNTAJE);
}

unsigned calcular_puntaje_normal(unsigned cantidad_intentos) {
    return PUNTAJE_PERFECTO - ((1 - cantidad_intentos) * DIFICULTAD_NORMAL_PENALIZACION_PUNTAJE);
}

unsigned calcular_puntaje_dificil(unsigned cantidad_intentos) {
    return PUNTAJE_PERFECTO - ((1 - cantidad_intentos) * DIFICULTAD_DIFICIL_PENALIZACION_PUNTAJE);
}

int calcular_porcentaje_diferencia(int num1, int num2) {
    return abs(((num1 - num2) / num2) * 100);
}

int verificar_nivel_facil(unsigned nivel_adivinado, unsigned nivel_pokemon) {
    if (nivel_adivinado == nivel_pokemon) return ACERTADO;

    int diferencia = calcular_porcentaje_diferencia((int) nivel_adivinado, (int) nivel_pokemon);

    return (diferencia <= DIFICULTAD_FACIL_PORCENTAJE_CERCA) ? CERCA : LEJOS;
}

int verificar_nivel_normal(unsigned nivel_adivinado, unsigned nivel_pokemon) {
    if (nivel_adivinado == nivel_pokemon) return ACERTADO;

    int diferencia = calcular_porcentaje_diferencia((int) nivel_adivinado, (int) nivel_pokemon);

    return (diferencia <= DIFICULTAD_NORMAL_PORCENTAJE_CERCA) ? CERCA : LEJOS;
}

int verificar_nivel_dificil(unsigned nivel_adivinado, unsigned nivel_pokemon) {
    if (nivel_adivinado == nivel_pokemon) return ACERTADO;

    int diferencia = calcular_porcentaje_diferencia((int) nivel_adivinado, (int) nivel_pokemon);

    return (diferencia <= DIFICULTAD_DIFICIL_PORCENTAJE_CERCA) ? CERCA : LEJOS;
}

const char* verificacion_a_string_facil(int resultado_verificacion) {
    char* mensaje = NULL;

    if (resultado_verificacion == ACERTADO) {
        mensaje = (char*) &MENSAJE_ACERTADO;
    } else if (resultado_verificacion == CERCA) {
        mensaje = (char*) &MENSAJE_CERCA;
    } else {
        mensaje = (char*) &MENSAJE_LEJOS;
    }

    return (const char*) mensaje;
}

const char* verificacion_a_string_normal(int resultado_verificacion) {
    char* mensaje = NULL;

    if (resultado_verificacion == ACERTADO) {
        mensaje = (char*) &MENSAJE_ACERTADO;
    } else if (resultado_verificacion == CERCA) {
        mensaje = (char*) &MENSAJE_CERCA;
    } else {
        mensaje = (char*) &MENSAJE_LEJOS;
    }

    return (const char*) mensaje;
}

const char* verificacion_a_string_dificil(int resultado_verificacion) {
    char* mensaje = NULL;

    if (resultado_verificacion == ACERTADO) {
        mensaje = (char*) &MENSAJE_ACERTADO;
    } else if (resultado_verificacion == CERCA) {
        mensaje = (char*) &MENSAJE_CERCA;
    } else {
        mensaje = (char*) &MENSAJE_LEJOS;
    }

    return (const char*) mensaje;
}

DatosDificultad* dificultad_crear(const char* nombre, unsigned (*calcular_puntaje)(unsigned cantidad_intentos), int (*verificar_nivel)(unsigned nivel_adivinado, unsigned nivel_pokemon), const char* (*verificacion_a_string)(int resultado_verificacion)) {
    if (!nombre || !calcular_puntaje || !verificar_nivel || !verificacion_a_string) return NULL;

    DatosDificultad* dificultad = malloc(sizeof(DatosDificultad));
    if (!dificultad) return NULL;

    dificultad->nombre = (const char*) malloc(sizeof(char) * (strlen(nombre) + 1));
    if (!dificultad->nombre) {
        free(dificultad);
        return NULL;
    };
    strcpy((char*) dificultad->nombre, nombre);
    dificultad->calcular_puntaje = calcular_puntaje;
    dificultad->verificar_nivel = verificar_nivel;
    dificultad->verificacion_a_string = verificacion_a_string;

    return dificultad;
}

bool dificultad_destruir(void* elemento, void* aux) {
    if (!elemento) return false;

    DatosDificultad* dificultad = (DatosDificultad*) elemento;

    free((char*) dificultad->nombre);
    free(dificultad);

    return true;
}

bool cargar_dificultades_default(lista_t* dificultades) {
    if (!dificultades) return false;

    DatosDificultad* facil = dificultad_crear(DIFICULTAD_FACIL_NOMBRE, &calcular_puntaje_facil, &verificar_nivel_facil, &verificacion_a_string_facil);
    if (!facil) return false;

    if (!lista_insertar(dificultades, facil)) {
        dificultad_destruir(facil, NULL);
        return false;
    }

    DatosDificultad* normal = dificultad_crear(DIFICULTAD_NORMAL_NOMBRE, &calcular_puntaje_normal, &verificar_nivel_normal, &verificacion_a_string_normal);
    if (!normal) return false;

    if (!lista_insertar(dificultades, normal)) {
        dificultad_destruir(normal, NULL);
        return false;
    }

    DatosDificultad* dificil = dificultad_crear(DIFICULTAD_DIFICIL_NOMBRE, &calcular_puntaje_dificil, &verificar_nivel_dificil, &verificacion_a_string_dificil);
    if (!dificil) return false;

    if (!lista_insertar(dificultades, dificil)) {
        dificultad_destruir(dificil, NULL);
        return false;
    }

    return true;
}

int comparador_nivel_pokemones(void* dato1, void* dato2) {
    pokemon_t* pokemon1 = (pokemon_t*) dato1;
    pokemon_t* pokemon2 = (pokemon_t*) dato2;

    return (pokemon1->nivel < pokemon2->nivel) ? 0 : 1;
}

simulador_t* simulador_crear(hospital_t* hospital) {
    if (!hospital) return NULL;

    simulador_t* simulador = malloc(sizeof(simulador_t));
    if (!simulador) return NULL;

    simulador->entrenadores_en_espera = cola_crear();
    if (!simulador->entrenadores_en_espera) {
        free(simulador);
        return NULL;
    }

    if (lista_con_cada_elemento(hospital->entrenadores, &encolar_entrenadores, (void*) simulador->entrenadores_en_espera) != lista_tamanio(hospital->entrenadores)) {
        free(simulador->entrenadores_en_espera);
        free(simulador);
        return NULL;
    }

    simulador->pokemones_en_espera = heap_crear(abb_tamanio(hospital->pokemones_alfabeticamente), &comparador_nivel_pokemones);

    if (!simulador->pokemones_en_espera) {
        free(simulador->entrenadores_en_espera);
        free(simulador);
        return NULL;
    }

    simulador->dificultades = lista_crear();
    if (!simulador->dificultades || !cargar_dificultades_default(simulador->dificultades)) {
        free(simulador);
        cola_destruir(simulador->entrenadores_en_espera);
        lista_destruir(simulador->dificultades);
        return NULL;
    }

    simulador->dificultad = DIFICULTAD_DEFAULT_ID;
    simulador->hospital = hospital;
    simulador->cantidad_eventos_simulados = 0;
    simulador->entrenadores_atendidos = 0;
    simulador->pokemones_atendidos = 0;
    simulador->cantidad_intentos_adivinar = 0;
    simulador->puntos = 0;
    simulador->simulacion_finalizada = false;
    simulador->pokemon_en_atencion = NULL;

    return simulador;
}

bool evento_obtener_estadisticas(simulador_t* simulador, void* datos) {
    if (!simulador || !datos) return false;

    EstadisticasSimulacion* estadisticas = (EstadisticasSimulacion*) datos;
    estadisticas->cantidad_eventos_simulados = simulador->cantidad_eventos_simulados;
    estadisticas->entrenadores_atendidos = simulador->entrenadores_atendidos;
    estadisticas->entrenadores_totales = (unsigned) lista_tamanio(simulador->hospital->entrenadores);
    estadisticas->pokemon_atendidos = simulador->pokemones_atendidos;
    estadisticas->pokemon_en_espera = (unsigned) heap_tamanio(simulador->pokemones_en_espera);
    estadisticas->pokemon_totales = (unsigned) abb_tamanio(simulador->hospital->pokemones_alfabeticamente);
    estadisticas->puntos = simulador->puntos;

    return true;
}

bool agregar_pokemon_a_heap(void* elemento, void* aux) {
    if (!elemento || !aux) return false;

    if (!heap_insertar((heap_t*) aux, elemento)) return false;

    return true;
}

bool atender_proximo_pokemon(simulador_t* simulador) {
    if (!simulador || simulador->pokemon_en_atencion || !heap_tamanio(simulador->pokemones_en_espera)) return false;

    pokemon_t* pokemon = heap_extraer_raiz(simulador->pokemones_en_espera);
    if (!pokemon) return false;

    simulador->pokemon_en_atencion = pokemon;

    return true;
}

bool evento_atender_proximo_entrenador(simulador_t* simulador, void* datos) {
    if (!simulador || !cola_tamanio(simulador->entrenadores_en_espera)) return false;

    entrenador_t* entrenador = cola_desencolar(simulador->entrenadores_en_espera);
    if (!entrenador) return false;

    if (lista_con_cada_elemento(entrenador->pokemones, &agregar_pokemon_a_heap, (void*) simulador->pokemones_en_espera) != lista_tamanio(entrenador->pokemones)) {
        return false;
    }

    simulador->entrenadores_atendidos++;

    if (!simulador->pokemon_en_atencion && heap_tamanio(simulador->pokemones_en_espera)) {
        if (!atender_proximo_pokemon(simulador)) return false;
    }

    return true;
}

bool evento_obtener_informacion_pokemon_en_tratamiento(simulador_t* simulador, void* datos) {
    if (!simulador || !datos) return false;

    InformacionPokemon* informacion = (InformacionPokemon*) datos;
    if (!simulador->pokemon_en_atencion) {
        informacion->nombre_pokemon = NULL;
        informacion->nombre_entrenador = NULL;
        return false;
    }

    informacion->nombre_pokemon = simulador->pokemon_en_atencion->nombre;
    informacion->nombre_entrenador = simulador->pokemon_en_atencion->entrenador->nombre;

    return true;
}

bool evento_adivinar_nivel_pokemon(simulador_t* simulador, void* datos) {
    if (!simulador || !datos || !simulador->pokemon_en_atencion) return false;

    Intento* intento = (Intento*) datos;

    DatosDificultad* dificultad = (DatosDificultad*) lista_elemento_en_posicion(simulador->dificultades, simulador->dificultad);
    if (!dificultad) return false;

    int resultado = dificultad->verificar_nivel(intento->nivel_adivinado, (unsigned) simulador->pokemon_en_atencion->nivel);

    intento->es_correcto = (resultado == ACERTADO);
    intento->resultado_string = dificultad->verificacion_a_string(resultado);
    simulador->cantidad_intentos_adivinar++;

    if (intento->es_correcto) {
        simulador->pokemon_en_atencion = NULL;
        simulador->pokemones_atendidos++;
        simulador->puntos += dificultad->calcular_puntaje(simulador->cantidad_intentos_adivinar);
        simulador->cantidad_intentos_adivinar = 0;

        if (heap_tamanio(simulador->pokemones_en_espera) && !atender_proximo_pokemon(simulador)) return false;
    }

    return true;
}

bool comparar_nombres_dificultad(void* elemento1, void* elemento2) {
    if (!elemento1 || !elemento2) return false;

    return (strcmp(((DatosDificultad*) elemento1)->nombre, ((DatosDificultad*) elemento2)->nombre) == 0);
}

bool evento_agregar_dificultad(simulador_t* simulador, void* datos) {
    if (!simulador || !datos) return false;

    DatosDificultad* dificultad_recibida = (DatosDificultad*) datos;
    DatosDificultad* dificultad_nueva = dificultad_crear(dificultad_recibida->nombre, dificultad_recibida->calcular_puntaje, dificultad_recibida->verificar_nivel, dificultad_recibida->verificacion_a_string);
    if (!dificultad_nueva) return false;

    if (!lista_insertar_si_no_existe(simulador->dificultades, (void*) dificultad_nueva, &comparar_nombres_dificultad)) {
        dificultad_destruir(dificultad_nueva, NULL);
        return false;
    };

    return true;
}

bool evento_seleccionar_dificultad(simulador_t* simulador, void* datos) {
    if (!simulador || !datos) return false;

    int* id = (int*) datos;
    if (*id >= lista_tamanio(simulador->dificultades)) return false;

    simulador->dificultad = (unsigned) *id;

    return true;
}

bool evento_obtener_informacion_dificultad(simulador_t* simulador, void* datos) {
    if (!simulador || !datos) return false;

    InformacionDificultad* informacion = (InformacionDificultad*) datos;

    if (informacion->id >= lista_tamanio(simulador->dificultades)) {
        informacion->nombre_dificultad = NULL;
        informacion->id = -1;
        return false;
    } else {
        DatosDificultad* dificultad = lista_elemento_en_posicion(simulador->dificultades, (size_t) informacion->id);

        if (!dificultad) return false;

        informacion->nombre_dificultad = dificultad->nombre;
        informacion->en_uso = (simulador->dificultad == informacion->id);
    }

    return true;
}

/*
 * Pre: -
 * Post: Establece la simulación como finalizada.
 *       Devuelve true en caso de éxito, false en caso de error (No recibir simulador válido)
 */
bool evento_finalizar_simulacion(simulador_t* simulador, void* datos) {
    if (!simulador) return false;

    simulador->simulacion_finalizada = true;

    return true;
}

ResultadoSimulacion simulador_simular_evento(simulador_t* simulador, EventoSimulacion evento, void* datos) {
    if (!simulador || simulador->simulacion_finalizada) return ErrorSimulacion;
    simulador->cantidad_eventos_simulados++;

    switch (evento) {
        case ObtenerEstadisticas:
            if (!evento_obtener_estadisticas(simulador, datos)) return ErrorSimulacion;
            break;
        
        case AtenderProximoEntrenador:
            if (!evento_atender_proximo_entrenador(simulador, datos)) return ErrorSimulacion;
            break;
        
        case ObtenerInformacionPokemonEnTratamiento:
            if (!evento_obtener_informacion_pokemon_en_tratamiento(simulador, datos)) return ErrorSimulacion;
            break;
        
        case AdivinarNivelPokemon:
            if (!evento_adivinar_nivel_pokemon(simulador, datos)) return ErrorSimulacion;
            break;

        case AgregarDificultad:
            if (!evento_agregar_dificultad(simulador, datos)) return ErrorSimulacion;
            break;
        
        case SeleccionarDificultad:
            if (!evento_seleccionar_dificultad(simulador, datos)) return ErrorSimulacion;
            break;
        
        case ObtenerInformacionDificultad:
            if (!evento_obtener_informacion_dificultad(simulador, datos)) return ErrorSimulacion;
            break;
        
        case FinalizarSimulacion:
            if (!evento_finalizar_simulacion(simulador, datos)) return ErrorSimulacion;
            break;

        default:
            return ErrorSimulacion;
    }

    return ExitoSimulacion;
}

void dificultades_destruir(lista_t* dificultades) {
    if (!dificultades) return;

    lista_con_cada_elemento(dificultades, &dificultad_destruir, NULL);
    
    lista_destruir(dificultades);
}

void simulador_destruir(simulador_t* simulador) {
    if (!simulador) return;

    hospital_destruir(simulador->hospital);
    dificultades_destruir(simulador->dificultades);
    cola_destruir(simulador->entrenadores_en_espera);
    heap_destruir(simulador->pokemones_en_espera);
    free(simulador);
}