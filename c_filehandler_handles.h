#ifndef C_FILEHANDLER_HANDLES_H_INCLUDED
#define C_FILEHANDLER_HANDLES_H_INCLUDED

#include "c_filehandler.h"

typedef struct FIO_File{
    FILE *file;
    FIO_OPEN_ENUM type;
}FIO_File;

typedef struct FIO_FileBuffer{
    FIO_File file;
    FIO_Data data;
}FIO_FileBuffer;


typedef struct FIO_FileReader{
    FIO_FileBuffer buff;
}FIO_FileReader;

typedef struct FIO_FileWriter{
    FIO_FileBuffer buff;
}FIO_FileWriter;

/*
typedef struct FIO_FileReader{
    char *fullFilePath;
    FIO_FileBuffer buff;
}FIO_FileReader;

typedef struct FIO_FileWriter{
    char *fullFilePath;
    FIO_FileBuffer buff;
}FIO_FileWriter;

typedef struct FIO_FileLogger{
    char *fullFilePath;
    FIO_FileBuffer buff;
}FIO_FileLogger;

/*
FIO_Data* FIO_newData(FILE* input);
FIO_ERROR_ENUM FIO_initData(FIO_Data *data, FILE* input);

FIO_Data* FIO_newDataComplex(FILE* input, FIO_SIZE bufferSize);
FIO_ERROR_ENUM FIO_initDataComplex(FIO_Data *data, FILE* input, FIO_SIZE bufferSize);

FIO_Data* FIO_newDataDeferred();
FIO_ERROR_ENUM FIO_initDataDeferred(FIO_Data *data);

FIO_Data* FIO_newDataDeferredComplex(FIO_SIZE bufferSize);
FIO_ERROR_ENUM FIO_initDataDeferredComplex(FILE* input, FIO_SIZE bufferSize);

FIO_ERROR_ENUM FIO_DataNext(FIO_Data *data, FILE* input);


FIO_Data* FIO_newFileDeferred(FILE* input);
FIO_ERROR_ENUM FIO_initFileDeferred(FIO_Data *data, FILE* input);

#define FIO_newStream FIO_newDataDeferred
#define FIO_initStream FIO_initDataDeferred



FIO_Data* FIO_newFileReader(const char* fullFilePath, FIO_OPEN_ENUM readType, size_t readBufferSize);
FIO_ERROR_ENUM FIO_initFileReader(FIO_FileReader *reader, const char* fullFilePath, FIO_OPEN_ENUM readType, size_t readBufferSize);

FIO_Data* FIO_newFileReaderComplex(FIO_FileReader *reader, size_t readBufferSize, FILE* existingHandle, unsigned char* existingBuffer);
FIO_ERROR_ENUM FIO_initFileReaderComplex(FIO_FileReader *reader, size_t readBufferSize, FILE* existingHandle, unsigned char* existingBuffer);
*/


#endif // C_FILEHANDLER_HANDLES_H_INCLUDED
