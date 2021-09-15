#include "pa2mm.h"
#include "split.h"

#include "string.h"
#include <stdbool.h>

void dadoUnStringNULL_devuelveNULL(){
    pa2m_afirmar(split(NULL, ',')==NULL, "Split de un string NULL resulta en NULL");
}

void dadoUnStringVacío_devuelveUnVectorConElStringYUnNULL(){
    char** vector = NULL;
    char* string = "";

    pa2m_afirmar((vector = split(string,','))!= NULL, "Puedo hacer split de un string vacío");

    pa2m_afirmar(vector[0] != string, "El primer elemento del vector no es el string original");
    pa2m_afirmar(strcmp(vector[0],string)==0, "El primer elemento del vector es un string vacío");
    pa2m_afirmar(vector[1] == NULL, "El segundo elemento del vector es NULL");

    free(vector[0]);
    free(vector);
}

void dadoUnStringSeparadoPorComasYUnaComa_devuelveUnVectorConLosStringsDelimitadosPorLasComas(){
    char** vector = NULL;
    char* string = "1,2,3";

    pa2m_afirmar((vector = split(string,','))!= NULL, "Puedo hacer split del string 1,2,3 separando por ,");

    pa2m_afirmar(vector && vector[0] && strcmp(vector[0],"1")==0, "El primer elemento del vector es \"1\"");
    pa2m_afirmar(vector && vector[1] && strcmp(vector[1],"2")==0, "El segundo elemento del vector es \"2\"");
    pa2m_afirmar(vector && vector[2] && strcmp(vector[2],"3")==0, "El tercer elemento del vector es \"3\"");
    pa2m_afirmar(vector && vector[3] == NULL, "El cuarto elemento del vector es NULL");

    free(vector[0]);
    free(vector[1]);
    free(vector[2]);
    free(vector);
}

void dadoUnStringDeComasYUnaComa_devuelveUnVectorConStringsVacios(){
    char** vector = NULL;
    char* string = ",,,,";

    pa2m_afirmar((vector = split(string,','))!= NULL, "Puedo hacer split del string ,,,, separando por ,");

    pa2m_afirmar(vector && vector[0] && strcmp(vector[0],"")==0, "El primer elemento del vector es \"\"");
    pa2m_afirmar(vector && vector[1] && strcmp(vector[1],"")==0, "El segundo elemento del vector es \"\"");
    pa2m_afirmar(vector && vector[2] && strcmp(vector[2],"")==0, "El tercer elemento del vector es \"\"");
    pa2m_afirmar(vector && vector[3] && strcmp(vector[3],"")==0, "El cuarto elemento del vector es \"\"");
    pa2m_afirmar(vector && vector[4] && strcmp(vector[4],"")==0, "El quinto elemento del vector es \"\"");

    pa2m_afirmar(vector && vector[5] == NULL, "El sexto elemento del vector es NULL");

    free(vector[0]);
    free(vector[1]);
    free(vector[2]);
    free(vector[3]);
    free(vector[4]);
    free(vector);
}

void dadoUnStringLargoSeparadoPorEspacios_devuelveUnVectorConLosStringsSeparadosPorEspacios(){
    char** vector = NULL;
    char string[32768];

    memset(string, '?', 32767);
    string[1024] = ' ';
    string[8192] = ' ';
    string[16384] = ' ';
    string[32767] = 0;


    pa2m_afirmar((vector = split(string,' '))!= NULL, "Puedo hacer split de un string muy grande");

    pa2m_afirmar(vector && vector[0] && vector[0][0]=='?', "El primer elemento del vector no es nulo ni un vector vacío");
    pa2m_afirmar(vector && vector[1] && vector[1][0]=='?', "El segundo elemento del vector no es nulo ni un vector vacío");
    pa2m_afirmar(vector && vector[2] && vector[2][0]=='?', "El tercer elemento del vector no es nulo ni un vector vacío");
    pa2m_afirmar(vector && vector[3] && vector[3][0]=='?', "El cuarto elemento del vector no es nulo ni un vector vacío");
    pa2m_afirmar(vector && vector[4]==NULL, "El quinto elemento del vector es NULL");

    free(vector[0]);
    free(vector[1]);
    free(vector[2]);
    free(vector[3]);
    free(vector);

}

int main(){

    pa2m_nuevo_grupo("Split de strings vacíos o nulos");
    dadoUnStringNULL_devuelveNULL();
    dadoUnStringVacío_devuelveUnVectorConElStringYUnNULL();

    pa2m_nuevo_grupo("Split de strings separados por comas");
    dadoUnStringSeparadoPorComasYUnaComa_devuelveUnVectorConLosStringsDelimitadosPorLasComas();

    pa2m_nuevo_grupo("Split de un strings solo de separadores");
    dadoUnStringDeComasYUnaComa_devuelveUnVectorConStringsVacios();

    pa2m_nuevo_grupo("Split de un string largo");
    dadoUnStringLargoSeparadoPorEspacios_devuelveUnVectorConLosStringsSeparadosPorEspacios();


    return pa2m_mostrar_reporte();
}
