#pragma once

#include "pintor.h"

typedef struct
{
	uint32_t ancho;
	uint32_t alto;
	color* pixeles;
} imagen_bmp;

imagen_bmp* cargar_bmp(const char* nombre_archivo);
void flip_horizontal(imagen_bmp* imagen);
void destruir_bmp(imagen_bmp* imagen);
