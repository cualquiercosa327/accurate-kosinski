#include "kosinski_moduled.h"

#include <stdio.h>

#include "kosinski_compress.h"
#include "kosinski_decompress.h"

void KosinskiCompressModuled(unsigned char *file_buffer, size_t file_size, FILE *output_file)
{
	fputc(file_size >> 8, output_file);
	fputc(file_size & 0xFF, output_file);

	for (unsigned int i = 0; i < (file_size - 1) >> 12; ++i)
	{
		KosinskiCompress(file_buffer, 0x1000, output_file);
		file_buffer += 0x1000;
	}

	KosinskiCompress(file_buffer, ((file_size - 1) & 0xFFF) + 1, output_file);
}

void KosinskiDecompressModuled(FILE *in_file, FILE *out_file)
{
	unsigned char byte1 = fgetc(in_file);
	unsigned short size = fgetc(in_file) | (byte1 << 8);

	unsigned int module_count = ((size - 1) >> 12);
	if (module_count == 0xA)
		module_count = 8;

	for (unsigned int i = 0; i < module_count; ++i)
	{
		KosinskiDecompress(in_file, out_file);

		fseek(in_file, (-(ftell(in_file) - 2)) & 0xF, SEEK_CUR);
	}

	KosinskiDecompress(in_file, out_file);
}
