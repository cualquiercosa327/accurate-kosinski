// Copyright (c) 2018 Clownacy

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/kosinski_compress.h"

#include "load_file_to_buffer.h"

int main(int argc, char *argv[])
{
	int success = EXIT_FAILURE;

	if (argc < 2)
	{
		printf(
		"This tool compresses a supplied file in the Kosinski format. It tries to produce files accurate to Sega's original compressor.\n"
		"Made by Clownacy.\n"
		"\n"
		"Usage: kosinski_compress.exe [in_file] [out_file](optional)\n"
		);
	}
	else
	{

		unsigned char *file_buffer;
		long file_size;

		if (LoadFileToBuffer(argv[1], &file_buffer, &file_size))
		{
			#ifdef DEBUG
			printf("File '%s' with size %lX loaded\n", argv[1], file_size);
			#endif

			unsigned char *out_buffer;
			size_t out_size = KosinskiCompress(file_buffer, file_size, &out_buffer);

			char *out_filename = (argc > 2) ? argv[2] : "out.kos";

			FILE *out_file = fopen(out_filename, "wb");

			if (out_file)
			{
				fwrite(out_buffer, out_size, 1, out_file);
				free(out_buffer);
				fclose(out_file);

				success = EXIT_SUCCESS;
			}
			else
			{
				printf("Could not open '%s'\n", out_filename);
			}

			free(file_buffer);
		}
		else
		{
			printf("Could not open '%s'\n", argv[1]);
		}
	}

	return success;
}
