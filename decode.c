#include <stdio.h>
#include <string.h>
#include "types.h"
#include "common.h"
#include "decode.h"

/*Function Definations */

/* Check if the Read Command Line Arguments are Valid
 * Input: Command Line Arguments and Structure Pointer
 * Output: Stego Image file
 * Return Value: e_success or e_failure on Invalid Arguments passed
 */

// Function definition for read and validate decode args
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if (strcmp(strstr(argv[2], "."), ".bmp") == 0)
    {
        // printf("INFO : BMP validated successfully.\n");
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        // printf("INFO : ERROR : Not a .bmp file\n");
        return e_failure;
    }
    if (argv[3] != NULL)
    {
        decInfo->secret_data_fname = argv[3];
    }
    else
    {
        // create default decode text file
        decInfo->secret_data_fname = "decode.txt";
        // printf("INFO : Decode file not mentioned. Creating decode.txt as default\n");
    }

    return e_success;
}

/*
 * Get File pointers for i/p and o/p files
 * Inputs: stego image, Secret file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

// open the stego_image and secrete data files in read mode and write mode
Status open_files_2(DecodeInfo *decInfo)
{
    // Stego Image file  (.bmp)
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

        return e_failure;
    }

    // secrete data file
    decInfo->fptr_secret_data = fopen(decInfo->secret_data_fname, "w");

    // Do Error handling
    if (decInfo->fptr_secret_data == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_data_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}

// Function definition for decode magic string
Status decode_magic_string(DecodeInfo *decInfo)
{
    // file pointer of stego.bmp to point to 54rth position
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);

    // decode magic string [16 byte]
    for (int i = 0; i < strlen(MAGIC_STRING); i++)
    {
        fread(decInfo->str, 8, 1, decInfo->fptr_stego_image); // 8 bytes from stego.bmp
        decInfo->decode_data[0] = 0;

        decode_image_to_data(decInfo);

        if (decInfo->decode_data[0] == MAGIC_STRING[i])
        {
            continue; // if decode data and magic string matches then oly proceed furter steps
        }
        else
        {
            return e_failure;
        }
    }
    return e_success;
}

// Function definition for decoding image to data
Status decode_image_to_data(DecodeInfo *decInfo)
{
    for (int i = 0; i < 8; i++)
    {
        decInfo->decode_data[0] |= (decInfo->str[i] & 1) << i;
    }

    return e_success;
}

// Function definition for decode size to data
Status decode_size_to_data(DecodeInfo *decInfo)
{
    fread(decInfo->secret_file_size, 32, 1, decInfo->fptr_stego_image);
    decInfo->image_data_size = 0;

    for (int i = 0; i < 32; i++)
    {
        decInfo->image_data_size |= (decInfo->secret_file_size[i] & 1) << i;
    }

    return e_success;
}

// Function definition for decode file extn size
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    decode_size_to_data(decInfo);

    // printf("INFO : File extension size %d\n", decInfo->image_data_size);

    return e_success;
}

// Function definition for decode secret file extn
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    for (int i = 0; i < decInfo->image_data_size; i++)
    {
        fread(decInfo->str, 8, 1, decInfo->fptr_stego_image);
        decInfo->decode_data[0] = 0;

        decode_image_to_data(decInfo);
    }

    // printf("INFO : File pointer is %ld\n", ftell(decInfo->fptr_stego_image));

    return e_success;
}

// Function definition for decode secret file size
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    decode_size_to_data(decInfo);

    // printf("INFO : File size %d\n", decInfo->image_data_size);
    //  printf("INFO : File pointer is %ld\n", ftell(decInfo->fptr_stego_image));

    return e_success;
}

// Function definition for decode secret file data
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    for (int i = 0; i < decInfo->image_data_size; i++)
    {
        fread(decInfo->str, 8, 1, decInfo->fptr_stego_image);
        decInfo->decode_data[0] = 0;

        decode_image_to_data(decInfo);
        // printf("%c", decInfo->decode_data[0]);
        fprintf(decInfo->fptr_secret_data, "%c", decInfo->decode_data[0]);
    }

    // printf("INFO : File pointer is %ld\n", ftell(decInfo->fptr_stego_image));

    return e_success;
}

// Function definition for do decoding
Status do_decoding(DecodeInfo *decInfo)
{
    if (open_files_2(decInfo) == e_success)
    {
        printf("Open files is a successfully\n");
    }
    else
    {
        printf("failed to open files\n");
        return e_failure;
    }

    if (decode_magic_string(decInfo) == e_success)
    {
        printf("Magic string decoded successfully\n");
    }
    else
    {
        printf("Failed to decode magic string\n");
        return e_failure;
    }

    if (decode_secret_file_extn_size(decInfo) == e_success)
    {
        printf("Secret file extension size decoded successfully\n");
    }
    else
    {
        printf("Failed to decode secret file extension size\n");
        return e_failure;
    }

    if (decode_secret_file_extn(decInfo) == e_success)
    {
        printf("Secret file extension decoded successfully\n");
    }
    else
    {
        printf("Failed to decode file extension\n");
        return e_failure;
    }

    if (decode_secret_file_size(decInfo) == e_success)
    {
        printf("Secret file size decoded successfully\n");
    }
    else
    {
        printf("Failed to decode secret file size\n");
        return e_failure;
    }

    if (decode_secret_file_data(decInfo) == e_success)
    {
        printf("Secret file data decoded successfully.\n");
        printf("===========================================================\n");
        printf("***********Decoding completed successfully***********\n");
        printf("===========================================================\n");
    }
    else
    {
        printf("Failed to secret file data");
        return e_failure;
    }
    fclose(decInfo->fptr_secret_data);
fclose(decInfo->fptr_stego_image);

return e_success;
}