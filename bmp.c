#include "bmp.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define HEADER_BMP 54

imagen_bmp* cargar_bmp(const char* nombre_archivo)
{
	uint8_t header[HEADER_BMP];
	uint32_t total_pixeles;
	uint32_t offset;
	uint32_t bits;
	uint32_t compression;
	int32_t height;
	uint32_t bytes_por_pixel;
	uint32_t padding;
	int top_down = 0;
	imagen_bmp* imagen;
	FILE* archivo = fopen(nombre_archivo, "rb");

	if(archivo == NULL)
	{
		printf("Error al abrir el file %s\n", nombre_archivo);
		return NULL;
	}

	fread(header, sizeof(uint8_t), HEADER_BMP, archivo);

	if(header[0] != 'B' || header[1] != 'M')
	{
		printf("Ese file no es un BMP valido\n");
		fclose(archivo);
		return NULL;
	}

	offset = header[10] | (header[11] << 8) | (header[12] << 16) | (header[13] << 24);
	bits = header[28] | (header[29] << 8);
	compression = header[30] | (header[31] << 8) | (header[32] << 16) | (header[33] << 24);

	if(bits != 24 && bits != 32)
	{
		printf("Solo soporto BMP de 24 o 32 bits\n");
		fclose(archivo);
		return NULL;
	}

	if(compression != 0 && compression != 3)
	{
		printf("Solo soporto BMP sin compresion (BI_RGB/BI_BITFIELDS)\n");
		fclose(archivo);
		return NULL;
	}

	imagen = malloc(sizeof(imagen_bmp));
	imagen->ancho = header[18] | (header[19] << 8) | (header[20] << 16) | (header[21] << 24);
	height = header[22] | (header[23] << 8) | (header[24] << 16) | (header[25] << 24);

	if(height < 0)
	{
		imagen->alto = -height;
		top_down = 1;
	}
	else
	{
		imagen->alto = height;
	}

	imagen->pixeles = NULL;
	total_pixeles = imagen->ancho * imagen->alto;
	imagen->pixeles = malloc(total_pixeles * sizeof(color));

	bytes_por_pixel = bits / 8;
	padding = (4 - ((imagen->ancho * bytes_por_pixel) % 4)) % 4;
	fseek(archivo, offset, SEEK_SET);

	for(uint32_t fila = 0; fila < imagen->alto; fila++)
	{
		uint32_t y;
		if(top_down)
			y = fila;
		else
			y = imagen->alto - 1 - fila;

		for(uint32_t x = 0; x < imagen->ancho; x++)
		{
			color pixel;
			uint8_t bgr[4];

			fread(bgr, sizeof(uint8_t), bytes_por_pixel, archivo);

			pixel.azul = bgr[0];
			pixel.verde = bgr[1];
			pixel.rojo = bgr[2];

			imagen->pixeles[y * imagen->ancho + x] = pixel;
		}

		if(padding > 0)
			fseek(archivo, padding, SEEK_CUR);
	}

	fclose(archivo);
	return imagen;
}

void flip_horizontal(imagen_bmp* imagen)
{
	color temp;
	uint32_t x;
	uint32_t y;

	for(y = 0; y < imagen->alto; y++)
	{
		for(x = 0; x < imagen->ancho / 2; x++)
		{
			temp = imagen->pixeles[y * imagen->ancho + x];
			imagen->pixeles[y * imagen->ancho + x] = imagen->pixeles[y * imagen->ancho + (imagen->ancho - 1 - x)];
			imagen->pixeles[y * imagen->ancho + (imagen->ancho - 1 - x)] = temp;
		}
	}
}

void destruir_bmp(imagen_bmp* imagen)
{
	if(imagen == NULL)
	{
		return;
	}

	free(imagen->pixeles);
	free(imagen);
}
