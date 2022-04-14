#include "pa2mm.h"
#include "src/hospital.h"
#include "src/simulador.h"

#include <string.h>
#include <stdbool.h>

bool ignorar_pokemon(pokemon_t* p){
    p = p;
    return true;
}

/* No intenten esto en sus casas */
/* Ya vamos a ver como evitar esto en el TDA Lista */
struct{
    pokemon_t* pokemon[500];
    size_t cantidad;
} acumulados;

void resetear_acumulados(){
    acumulados.cantidad = 0;
}

bool acumular_pokemon(pokemon_t* p){
    acumulados.pokemon[acumulados.cantidad] = p;
    acumulados.cantidad++;
    return true;
}

bool acumular_pokemon_hasta_miltank(pokemon_t* p){
    acumulados.pokemon[acumulados.cantidad] = p;
    acumulados.cantidad++;
    return strcmp(pokemon_nombre(p), "miltank");
}

bool acumulados_en_orden_correcto(){
    if(acumulados.cantidad < 2)
        return true;
    pokemon_t* anterior = acumulados.pokemon[0];
    for(int i=1;i<acumulados.cantidad;i++){
        pokemon_t* actual =  acumulados.pokemon[i];
        if(strcmp(pokemon_nombre(anterior), pokemon_nombre(actual)) > 0)
            return false;
    }
    return true;
}

/* Pruebas */

void puedoCrearYDestruirUnHospital(){
    hospital_t* h=NULL;

    pa2m_afirmar((h=hospital_crear()), "Crear un hospital devuelve un hospital");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==0, "Un hospital se crea con cero entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==0, "Un hospital se crea con cero pokemon");

    pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon)==0, "Recorrer los pokemon resulta en 0 pokemon recorridos");

    hospital_destruir(h);
}

void dadoUnHospitalNULL_lasPuedoAplicarLasOperacionesDelHospitalSinProblema(){
    hospital_t* h=NULL;

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==0, "Un hospital NULL tiene cero entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==0, "Un hospital NULL tiene cero pokemon");

    pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon)==0, "Recorrer los pokemon de un hospital NULL resulta en 0 pokemon recorridos");

    hospital_destruir(h);
}

void dadoUnArchivoVacio_NoSeAgreganPokemonAlHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/archivo_vacio.hospital"), "Puedo leer un archivo vacío");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==0, "Un hospital vacío tiene cero entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==0, "Un hospital vacío tiene tiene cero pokemon");

    pa2m_afirmar(hospital_a_cada_pokemon(h, ignorar_pokemon)==0, "Recorrer los pokemon resulta en 0 pokemon recorridos");

    hospital_destruir(h);
}

void dadoUnArchivoConUnEntrenador_SeAgregaElEntrenadorYSusPokemonAlHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/un_entrenador.hospital"), "Puedo leer un archivo con un entrenador");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==1, "El hospital tiene 1 entrenador");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==3, "El hospital tiene 3 pokemon");

    resetear_acumulados();
    pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon)==3, "Recorrer los pokemon resulta en 3 pokemon recorridos");
    pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

    hospital_destruir(h);
}

void dadoUnArchivoConVariosEntrenadores_SeAgreganLosEntrenadoresYSusPokemonAlHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un archivo con varios entrenadores");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==5, "El hospital tiene 5 entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==24, "El hospital tiene 24 pokemon");

    resetear_acumulados();
    pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon)==24, "Recorrer los pokemon resulta en 24 pokemon recorridos");
    pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

    hospital_destruir(h);
}


void dadosVariosArchivos_puedoAgregarlosTodosAlMismoHospital(){
    hospital_t* h=hospital_crear();

    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un archivo con varios entrenadores");
    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer otro archivo con varios entrenadores");
    pa2m_afirmar(hospital_leer_archivo(h, "ejemplos/varios_entrenadores.hospital"), "Puedo leer un tercer archivo con varios entrenadores");

    pa2m_afirmar(hospital_cantidad_entrenadores(h)==15, "El hospital tiene 15 entrenadores");
    pa2m_afirmar(hospital_cantidad_pokemon(h)==72, "El hospital tiene 72 pokemon");

    resetear_acumulados();
    pa2m_afirmar(hospital_a_cada_pokemon(h, acumular_pokemon)==72, "Recorrer los pokemon resulta en 72 pokemon recorridos");
    pa2m_afirmar(acumulados_en_orden_correcto(), "Los pokemon se recorrieron en orden alfabetico");

    hospital_destruir(h);
}

void pruebas_basicas_simulador() {
    simulador_t* simulador = simulador_crear(NULL);
    pa2m_afirmar(!simulador, "Crear un simulador con hospital NULL no crea el simulador");

    hospital_t* hospital = hospital_crear();
    pa2m_afirmar(hospital != NULL, "Puedo crear un hospital nuevamente");

    simulador = simulador_crear(hospital);
    pa2m_afirmar(simulador != NULL, "Crear un simulador con hospital válido crea el simulador");

    simulador_destruir(simulador);
    pa2m_afirmar(true, "Puedo destruir el simulador");
}

void pruebas_simulador_hospital_con_un_entrenador() {
    hospital_t* hospital = hospital_crear();
    pa2m_afirmar(hospital != NULL, "Puedo crear un hospital nuevamente");

    pa2m_afirmar(hospital_leer_archivo(hospital, "ejemplos/un_entrenador.hospital"), "Puedo cargar un archivo con un entrenador en mi hospital nuevamente.");

    simulador_t* simulador = simulador_crear(hospital);
    pa2m_afirmar(simulador != NULL, "Crear un simulador el hospital con un entrenador");

    EstadisticasSimulacion estadisticas;

    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, (void*) &estadisticas) == ExitoSimulacion, "Puedo obtener las estadísticas de simulación correctamente");
    pa2m_afirmar(estadisticas.cantidad_eventos_simulados == 1, "La cantidad de eventos simulados es 1");
    pa2m_afirmar(estadisticas.entrenadores_totales == 1, "La cantidad de entrenadores totales es 1");
    pa2m_afirmar(estadisticas.entrenadores_atendidos == 0, "La cantidad de entrenadores atendidos es 0");
    pa2m_afirmar(estadisticas.pokemon_totales == 3, "La cantidad de pokemones totales es 3");
    pa2m_afirmar(estadisticas.pokemon_atendidos == 0, "La cantidad de pokemones atendidos es 0");
    pa2m_afirmar(estadisticas.pokemon_en_espera == 0, "La cantidad de pokemones en espera es 0");
    pa2m_afirmar(estadisticas.puntos == 0, "La cantidad de puntos inicial es 0");

    InformacionPokemon informacion;
    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, (void*) &informacion) == ErrorSimulacion, "No puedo obtener informacion del pokemon en tratamiento porque no hay");

    pa2m_afirmar(simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL) == ExitoSimulacion, "Paso a consulta al único entrenador exitosamente");
    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerEstadisticas, (void*) &estadisticas) == ExitoSimulacion, "Puedo obtener las estadísticas nuevamente");
    pa2m_afirmar(estadisticas.cantidad_eventos_simulados == 4, "La cantidad de eventos simulados es 3");
    pa2m_afirmar(estadisticas.entrenadores_totales == 1, "La cantidad de entrenadores totales es 1");
    pa2m_afirmar(estadisticas.entrenadores_atendidos == 1, "La cantidad de entrenadores atendidos es 1");
    pa2m_afirmar(estadisticas.pokemon_totales == 3, "La cantidad de pokemones totales es 3");
    pa2m_afirmar(estadisticas.pokemon_atendidos == 0, "La cantidad de pokemones atendidos es 1");
    pa2m_afirmar(estadisticas.pokemon_en_espera == 2, "La cantidad de pokemones en espera es 2");
    pa2m_afirmar(estadisticas.puntos == 0, "La cantidad de puntos sigue siendo 0");

    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, (void*) &informacion) == ExitoSimulacion, "Puedo informacion del pokemon en tratamiento");
    pa2m_afirmar(strcmp(informacion.nombre_entrenador, "Mariano") == 0, "El entrenador del pokemon en entrenamiento es correcto");
    pa2m_afirmar(strcmp(informacion.nombre_pokemon, "lapras") == 0, "El nombre del pokemon en entrenamiento es correcto");

    pa2m_afirmar(simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL) == ErrorSimulacion, "Atender otro entrenador devuelve error");

    simulador_destruir(simulador);
    pa2m_afirmar(true, "Puedo destruir el simulador");
}

void pruebas_simulador_adivinar_nivel() {
    hospital_t* hospital = hospital_crear();
    pa2m_afirmar(hospital != NULL, "Puedo crear un hospital nuevamente");

    pa2m_afirmar(hospital_leer_archivo(hospital, "ejemplos/un_entrenador.hospital"), "Puedo cargar un archivo con un entrenador en mi hospital nuevamente.");

    simulador_t* simulador = simulador_crear(hospital);
    pa2m_afirmar(simulador != NULL, "Crear un simulador el hospital con un entrenador");
    pa2m_afirmar(simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL) == ExitoSimulacion, "Paso a consulta al único entrenador exitosamente");

    InformacionPokemon informacion;
    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, (void*) &informacion) == ExitoSimulacion, "Obtener la información del pokemon en atención devuelve ExitoSimulacion");
    pa2m_afirmar(informacion.nombre_entrenador != NULL && informacion.nombre_pokemon != NULL, "El nombre del pokemon y entrenador no son NULL");

    Intento intento = {
        .nivel_adivinado = 100,
    };

    pa2m_afirmar(simulador_simular_evento(simulador, AdivinarNivelPokemon, (void*) &intento) == ExitoSimulacion, "Intentar adivinar el nivel de un pokemon devuelve ExitoSimulacion");
    pa2m_afirmar(!intento.es_correcto, "El nivel adivinado no es el correcto");

    InformacionPokemon nueva_informacion;
    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionPokemonEnTratamiento, (void*) &nueva_informacion) == ExitoSimulacion, "Obtener la información del pokemon en atención devuelve ExitoSimulacion nuevamente");

    pa2m_afirmar(strcmp(informacion.nombre_pokemon, nueva_informacion.nombre_pokemon) == 0 && strcmp(informacion.nombre_entrenador, nueva_informacion.nombre_entrenador) == 0, "El pokemón en atención sigue siendo el mismo");

    simulador_destruir(simulador);
    pa2m_afirmar(true, "Puedo destruir el simulador");
}


void pruebas_simulador_hospital_con_varios_entrenadores() {
}

unsigned calcular_puntaje(unsigned cantidad_intentos) {
    return cantidad_intentos < 10 ? 1000 - (cantidad_intentos * 100) : 0;
}

int verificar_nivel(unsigned int nivel_adivinado, unsigned int nivel_pokemon) {
    if (nivel_adivinado == nivel_pokemon) return 0;

    return -1;
}

const char* EXITO = "La suerte está de tu lado";
const char* QUIENSABE = "Quien sabe, esto está redificil";

const char* verificacion_a_string(int puntaje) {
    if (puntaje == 0) return EXITO;

    return QUIENSABE;
}

void pruebas_simulador_dificultades() {
    hospital_t* hospital = hospital_crear();
    hospital_leer_archivo(hospital, "ejemplos/un_entrenador.hospital");

    simulador_t* simulador = simulador_crear(hospital);
    simulador_simular_evento(simulador, AtenderProximoEntrenador, NULL);

    int id = 0;
    pa2m_afirmar(simulador_simular_evento(simulador, SeleccionarDificultad, &id) == ExitoSimulacion, "Seleccionar la dificultad 0 (Fácil) devuelve ExitoSimulacion");

    id = 1;
    pa2m_afirmar(simulador_simular_evento(simulador, SeleccionarDificultad, &id) == ExitoSimulacion, "Seleccionar la dificultad 1 (Normal) devuelve ExitoSimulacion");

    id = 2;
    pa2m_afirmar(simulador_simular_evento(simulador, SeleccionarDificultad, &id) == ExitoSimulacion, "Seleccionar la dificultad 2 (Dificil) devuelve ExitoSimulacion");

    id = 3;
    pa2m_afirmar(simulador_simular_evento(simulador, SeleccionarDificultad, &id) == ErrorSimulacion, "Seleccionar una dificultad que no existe (3) devuelve ErrorSimulacion");

    const char nombre[] = "REDIFICIL";

    DatosDificultad dificultad = {
        .nombre = nombre,
        .calcular_puntaje = NULL,
        .verificar_nivel = NULL,
        .verificacion_a_string = NULL,
    };

    pa2m_afirmar(simulador_simular_evento(simulador, AgregarDificultad, NULL) == ErrorSimulacion, "Agregar una dificultad sin dato devuelve ErrorSimulacion");
    pa2m_afirmar(simulador_simular_evento(simulador, AgregarDificultad, &dificultad) == ErrorSimulacion, "Agregar una dificultad con dato incompleto devuelve ErrorSimulacion");

    dificultad.calcular_puntaje = &calcular_puntaje;
    dificultad.verificar_nivel = &verificar_nivel;
    dificultad.verificacion_a_string = &verificacion_a_string;

    pa2m_afirmar(simulador_simular_evento(simulador, AgregarDificultad, &dificultad) == ExitoSimulacion, "Agregar una dificultad con dato válido devuelve ExitoSimulacion");

    InformacionDificultad informacion = {
        .id = 3,
    };
    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionDificultad, &informacion) == ExitoSimulacion, "Obtener información de la dificultad agregada devuelve ExitoSimulacion");

    strcpy((char*) nombre, "LICIFIDER");
    pa2m_afirmar(strcmp(informacion.nombre_dificultad, nombre) != 0, "El nombre de la dificultad se copia");

    strcpy((char*) nombre, "REDIFICIL");
    pa2m_afirmar(simulador_simular_evento(simulador, AgregarDificultad, &dificultad) == ErrorSimulacion, "Agregar de nuevo la misma dificultad (mismo nombre) devuelve ErrorSimulacion");

    InformacionDificultad informacion_repetido = {
        .id = 4,
    };
    pa2m_afirmar(simulador_simular_evento(simulador, ObtenerInformacionDificultad, &informacion_repetido) == ErrorSimulacion, "Obtener información de la dificultad agregada devuelve ExitoSimulacion");


    simulador_destruir(simulador);
}

int main(){

    pa2m_nuevo_grupo("Pruebas de creación y destrucción");
    puedoCrearYDestruirUnHospital();

    pa2m_nuevo_grupo("Pruebas con NULL");
    dadoUnHospitalNULL_lasPuedoAplicarLasOperacionesDelHospitalSinProblema();

    pa2m_nuevo_grupo("Pruebas con un archivo vacío");
    dadoUnArchivoVacio_NoSeAgreganPokemonAlHospital();

    pa2m_nuevo_grupo("Pruebas con un archivo de un entrenador");
    dadoUnArchivoConUnEntrenador_SeAgregaElEntrenadorYSusPokemonAlHospital();

    pa2m_nuevo_grupo("Pruebas con un archivo de varios entrenadores");
    dadoUnArchivoConVariosEntrenadores_SeAgreganLosEntrenadoresYSusPokemonAlHospital();

    pa2m_nuevo_grupo("Pruebas con mas de un archivo");
    dadosVariosArchivos_puedoAgregarlosTodosAlMismoHospital();

    pa2m_nuevo_grupo("Pruebas básicas simulador");
    pruebas_basicas_simulador();

    pa2m_nuevo_grupo("Pruebas simulador con un hospital de un entrenador");
    pruebas_simulador_hospital_con_un_entrenador();

    pa2m_nuevo_grupo("Pruebas de adivinación de nivel");
    pruebas_simulador_adivinar_nivel();

    pa2m_nuevo_grupo("Pruebas simulador con un hospital de varios entrenadores");
    pruebas_simulador_hospital_con_varios_entrenadores();

    pa2m_nuevo_grupo("Pruebas de dificultades");
    pruebas_simulador_dificultades();

    return pa2m_mostrar_reporte();
}
