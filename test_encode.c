
#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "decode.h"
#include <string.h>

int main(int argc, char *argv[])
{
    // 1.........TODO........(argc)
    // validate for atleast -e/-d , be.bmp , secrete.txt
    if (argc < 3)
    {
        printf("ERROR : Please Pass CLA\n");
        printf("Usage: \n");
        printf("For Encoding : ./a.out -e beautiful.bmp  secret.txt [stego.bmp]\n");
        printf("For Decoding : ./a.out -d stego.bmp [decode.txt]\n");
        return 1;
    }

    // 2. check operation type (-e or -d) (argv)
    if (check_operation_type(argv) == e_encode)
    {
        printf("Seleceted Encoding\n");
        EncodeInfo encode; //structure variable declaration
        if (read_and_validate_encode_args(argv, &encode) == e_success)
        {
            printf("Read and validation of encode arguments is done successfully\n");
            printf("*****************Encoding started*****************\n");
            do_encoding(&encode); //do encoding function calling
        }
        else
        {
            printf("Read and validation of encode arguments is failed\n");
            return -1;
        }
    }
    else if (check_operation_type(argv) == e_decode)
    {
        printf("Seleceted Decoding\n");
        DecodeInfo decode;//structure variable created
        if (read_and_validate_decode_args(argv, &decode) == e_success)
        {
            printf("Read and validation of decode arguments is done successfully\n");
            printf("*****************Decoding started*****************\n");
            do_decoding(&decode);
        }
    }
    else
    {
        printf("Invalid Input\n");
        printf("For Encoding : ./a.out -e beautiful.bmp  secrete.txt [stego.bmp]\n");
        printf("For Decoding : ./a.out -d stego.bmp [decode.txt]\n");
    }

    return 0; 
}

OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}
