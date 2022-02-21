#include "src/hospital.h"
#include "src/simulador.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define ObtenerEstadisticas_KEY 'e'
#define AtenderProximoEntrenador_KEY 'p'
#define ObtenerInformacionPokemonEnTratamiento_KEY 'i'
#define AdivinarNivelPokemon_KEY 'a'
#define AgregarDificultad_KEY 'n'
#define SeleccionarDificultad_KEY 'd'
#define ObtenerInformacionDificultad_KEY 'o'
#define FinalizarSimulacion_KEY 'q'
#define MostarAyuda_KEY 'h'

typedef struct{
    simulador_t* simulador;
    bool jugando;
} juego_t;

void mostrar_separador() {
    printf("*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n");
}

void mostrar_ayuda() {
    printf("[%c] Obtener estadísticas de simulación. \n", ObtenerEstadisticas_KEY);
    printf("[%c] Atender próximo entrenador. \n", AtenderProximoEntrenador_KEY);
    printf("[%c] Obtener información del pokemon en tratamiento. \n", ObtenerInformacionPokemonEnTratamiento_KEY);
    printf("[%c] Adivinar nivel pokemon. \n", AdivinarNivelPokemon_KEY);
    printf("[%c] Seleccionar dificultad. \n", SeleccionarDificultad_KEY);
    printf("[%c] Obtener informacion dificultad. \n", ObtenerInformacionDificultad_KEY);
    printf("[%c] Finalizar simulación. \n", FinalizarSimulacion_KEY);
    printf("[%c] Ver la ayuda nuevamente. \n", MostarAyuda_KEY);
}

void mostrar_bienvenida() {
    system("clear");
    mostrar_separador();
    printf("Bienvenido al simulador de hospital 9000! \n");
    mostrar_separador();

    printf("\nAntes de empezar, conocé las acciones que puedes realizar en el simulador: \n\n");

    mostrar_ayuda();

    printf("\n");
    mostrar_separador();
    printf("\n");
}


char leer_comando () {
    // char linea[100];
    // char* leido;
    // leido = fgets(linea, 100, stdin);
    // if(!leido)
    //     return 0;
    // while(*leido == ' ')
    //     leido++;

    char comando;
    scanf(" %c", &comando);

    return (char)tolower(comando);
}

void mostrar_estadisticas(juego_t* juego) {
    EstadisticasSimulacion estadisticas;
    simulador_simular_evento(juego->simulador, ObtenerEstadisticas, &estadisticas);
    
    printf("Las estadísticas de la simulación son las siguientes: \n");
    mostrar_separador();
    printf("\n");
    printf("* Cantidad eventos simulados: %i \n", estadisticas.cantidad_eventos_simulados);
    printf("* Entrenadores atendidos: %i \n", estadisticas.entrenadores_atendidos);
    printf("* Entrenadores totales: %i \n", estadisticas.entrenadores_totales);
    printf("* Pokemones atendidos: %i \n", estadisticas.pokemon_atendidos);
    printf("* Pokemones en espera: %i \n", estadisticas.pokemon_en_espera);
    printf("* Pokemones totales: %i \n", estadisticas.pokemon_totales);
    printf("* Puntos conseguidos: %i \n", estadisticas.puntos);
    printf("\n");
}

void atender_proximo_entrenador(juego_t* juego) {
    if (simulador_simular_evento(juego->simulador, AtenderProximoEntrenador, NULL) == ExitoSimulacion) {
        printf("Se ha antedido el próximo entrenador. \n");
    } else {
        printf("No quedan más entrenadores para ser atendidos. \n");
    }
}

void obtener_informacion_pokemon_en_tratamiento(juego_t* juego) {
    InformacionPokemon informacion;
    if (simulador_simular_evento(juego->simulador, ObtenerInformacionPokemonEnTratamiento, &informacion) == ExitoSimulacion) {
        printf("La informacion del pokemon en tratamiento es: \n");

        mostrar_separador();
        printf("\n");

        printf("* Nombre del pokemon: %s \n", informacion.nombre_pokemon);
        printf("* Nombre del entrenador: %s \n", informacion.nombre_entrenador);

        printf("\n");
    } else {
        printf("No hay actualmente un pokemon en tratamiento \n");
    }
}

void adivinar_nivel_pokemon(juego_t* juego) {
    printf("Adivina el nivel del pokemon en tratamiento: \n");
    mostrar_separador();
    printf("\n");

    unsigned nivel;
    printf("> ");
    scanf("%i", &nivel);


    Intento intento = {
        .nivel_adivinado = nivel,
    };

    system("clear");
    mostrar_separador();

    if (simulador_simular_evento(juego->simulador, AdivinarNivelPokemon, &intento) == ExitoSimulacion) {
        if (intento.es_correcto) {
            printf("[%s] - El nivel insertado (%i) es correcto!\n", intento.resultado_string, intento.nivel_adivinado);
        } else {
            printf("[%s] - El nivel insertado (%i) es incorrecto :(\n", intento.resultado_string, intento.nivel_adivinado);
        }
    } else {
        printf("No hay pokemon en tratamiento!\n");
    }
}

void seleccionar_dificultad(juego_t* juego) {
    printf("Selecciona la dificultad que deseas elegir (0 - 4): \n");
    mostrar_separador();
    printf("\n");

    for (int i = 0; i < 5; i++) {
        InformacionDificultad informacion = {
            .id = i,
        };

        simulador_simular_evento(juego->simulador, ObtenerInformacionDificultad, &informacion);

        printf("[%i] %s \n", i, informacion.nombre_dificultad);
    }

    printf("\n");

    unsigned nivel;
    printf("> ");
    scanf("%i", &nivel);


    system("clear");
    mostrar_separador();

    if (simulador_simular_evento(juego->simulador, SeleccionarDificultad, &nivel) == ExitoSimulacion) {
        printf("Haz cambiado la dificultad satisfactoriamente.\n");
    } else {
        printf("Selecciona la dificultad elegida no existe.\n");
    }
}

void obtener_informacion_dificultad(juego_t* juego) {
    printf("Selecciona la dificultad que desear ver: \n");
    mostrar_separador();
    printf("\n");

    int id;
    printf("> ");
    scanf("%i", &id);

    InformacionDificultad informacion = {
        .id = id,
    };

    system("clear");
    mostrar_separador();

    if (simulador_simular_evento(juego->simulador, ObtenerInformacionDificultad, &informacion) == ExitoSimulacion) {
        printf("Los datos del la dificultad son los siguientes:\n");
        mostrar_separador();
        printf("\n");

        printf("* ID de la dificultad: %i \n", informacion.id);
        printf("* Nombre de la dificultad: %s \n", informacion.nombre_dificultad);
        printf("* En uso: %s \n", informacion.en_uso ? "Si" : "No");

        printf("\n");
    } else {
        printf("Selecciona la dificultad elegida no existe.\n");
    }
}

void finalizar_simulacion(juego_t* juego) {
    simulador_simular_evento(juego->simulador, FinalizarSimulacion, NULL);
    juego->jugando = false;
}

void ejecutar_comando(juego_t *juego, char comando) {
    if (!juego) return;

    system("clear");
    mostrar_separador();

    switch (comando) {
        case ObtenerEstadisticas_KEY:
            mostrar_estadisticas(juego);
            break;

        case AtenderProximoEntrenador_KEY:
            atender_proximo_entrenador(juego);
            break;

        case ObtenerInformacionPokemonEnTratamiento_KEY:
            obtener_informacion_pokemon_en_tratamiento(juego);
            break;

        case AdivinarNivelPokemon_KEY:
            adivinar_nivel_pokemon(juego);
            break;

        case SeleccionarDificultad_KEY:
            seleccionar_dificultad(juego);
            break;
        
        case ObtenerInformacionDificultad_KEY:
            obtener_informacion_dificultad(juego);
            break;

        case FinalizarSimulacion_KEY:
            finalizar_simulacion(juego);
            break;

        case MostarAyuda_KEY:
            mostrar_ayuda();
            break;

        default:
            printf("Haz insertado un comando inválido. \n");
            break;
    }

    mostrar_separador();
    printf("\n");
}

unsigned calcular_puntaje_refacil(unsigned intentos) {
    return 1000;
}

unsigned calcular_puntaje_redificil(unsigned intentos) {
    return 1000 - (intentos * 100);
}

#define ACERTADO 0
#define ES_MENOR_LEJOS -1
#define ES_MENOR_CERCA -2
#define ES_MAYOR_LEJOS 1
#define ES_MAYOR_CERCA 2
#define CERCANIA_REFACIL 10
#define QUIEN_SABE -1

int verificar_nivel_refacil(unsigned nivel_adivinado, unsigned nivel_pokemon) {
    if (nivel_adivinado == nivel_pokemon) return ACERTADO;

    if (nivel_adivinado < nivel_pokemon) {
        return (abs((int) nivel_adivinado - (int) nivel_pokemon) < CERCANIA_REFACIL) ? ES_MAYOR_CERCA : ES_MAYOR_LEJOS;
    } else {
        return (abs((int) nivel_adivinado - (int) nivel_pokemon) < CERCANIA_REFACIL) ? ES_MENOR_CERCA : ES_MENOR_LEJOS;
    }
}

int verificar_nivel_redificil(unsigned nivel_adivinado, unsigned nivel_pokemon) {
    if (nivel_adivinado == nivel_pokemon) return ACERTADO;

    return QUIEN_SABE;
}

const char* verificacion_a_string_refacil(int puntaje) {
    switch (puntaje) {
        case ACERTADO:
            return "Bueno estuvo refacil, no?";
        
        case ES_MAYOR_CERCA:
            return "Estás cerca, es mayor";
        
        case ES_MAYOR_LEJOS:
            return "Estás lejos, es mayor";
        
        case ES_MENOR_CERCA:
            return "Estás cerca, es menor";
        
        case ES_MENOR_LEJOS:
            return "Estás lejos, es menor";

        default:
            return "NO SE QUE PASÓ";
    }
}

const char* verificacion_a_string_redificil(int puntaje) {
    return puntaje == ACERTADO ? "Lo conseguiste, usaste hacks?" : "ESTA REDIFICIL";
}

void inicializar_juego(juego_t* juego, hospital_t* hospital) {
    if (!juego) return;

    juego->simulador = simulador_crear(hospital);
    juego->jugando = true;

    DatosDificultad REDIFICIL = {
        .nombre = "REDIFICIL",
        .calcular_puntaje = &calcular_puntaje_redificil,
        .verificar_nivel = &verificar_nivel_redificil,
        .verificacion_a_string = &verificacion_a_string_redificil,
    };

    DatosDificultad REFACIL = {
        .nombre = "REFACIL",
        .calcular_puntaje = &calcular_puntaje_refacil,
        .verificar_nivel = &verificar_nivel_refacil,
        .verificacion_a_string = &verificacion_a_string_refacil,
    };

    simulador_simular_evento(juego->simulador, AgregarDificultad, &REDIFICIL);
    simulador_simular_evento(juego->simulador, AgregarDificultad, &REFACIL);
}

void destruir_juego(juego_t juego) {
    simulador_destruir(juego.simulador);
}

int main(int argc, char *argv[]) {
    hospital_t* hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/varios_entrenadores.hospital");

    juego_t juego;
    inicializar_juego(&juego, hospital);
    if (!juego.simulador) return -1;

    mostrar_bienvenida();

    do {
        printf("Ejecutar acción > ");
        char comando = leer_comando();
        ejecutar_comando(&juego, comando);
    } while(juego.jugando);

    destruir_juego(juego);

    return 0;
}
