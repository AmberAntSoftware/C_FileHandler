#ifndef C_FILEHANDLER_HANDLES_H_INCLUDED
#define C_FILEHANDLER_HANDLES_H_INCLUDED

#include "c_filehandler.h"

typedef enum FIO_FILEBUFFER_TYPE_ENUM{
    FIO_FILEBUFFER_TYPE_READ,
    FIO_FILEBUFFER_TYPE_WRITE,
    FIO_FILEBUFFER_TYPE_RW
}FIO_FILEBUFFER_TYPE_ENUM;

typedef struct FIO_File{
    FILE *file;
    FIO_OPEN_ENUM type;
}FIO_File;

typedef struct FIO_FileBuffer{
    FIO_File file;
    FIO_Data data;
}FIO_FileBuffer;

typedef struct FIO_FileStream{
    FIO_FileBuffer buff;
    FIO_FILEBUFFER_TYPE_ENUM control;
}FIO_FileStream;

FIO_Data* FIO_newFileStream(FILE* input);
FIO_Data* FIO_newFileStream(FILE* input, FIO_SIZE bufferSize);
FIO_Data* FIO_newFileStreamDefer();
FIO_Data* FIO_newFileStreamDeferComplex(FIO_SIZE bufferSize);

FIO_ERROR_ENUM FIO_initFileStream(FIO_Data *data, FILE* input);
FIO_ERROR_ENUM FIO_initFileStreamComplex(FIO_Data *data, FILE* input, FIO_SIZE bufferSize);
FIO_ERROR_ENUM FIO_initFileStreamDefer(FIO_Data *data);
FIO_ERROR_ENUM FIO_initFileStreamDeferComplex(FILE* input, FIO_SIZE bufferSize);


#endif // C_FILEHANDLER_HANDLES_H_INCLUDED
