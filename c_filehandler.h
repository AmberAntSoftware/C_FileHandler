#ifndef C_FILEHANDLER_H_INCLUDED
#define C_FILEHANDLER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define DEBUG
#ifdef DEBUG
    #define debugf(...) printf(__VA_ARGS__)
    #define debug(str) puts(str)
#else
    #define debugf(...) do{}while(0)
    #define debug(str) do{}while(0)
#endif // DEBUG

#define FIO_BUFF_SIZE (1024*1024)
///TODO construct with win32 functions


typedef enum FIO_OPEN_ENUM{
    /** Read only, the file must exist, starts at beginning of file **/
    FIO_OPEN_R_EXISTING,
    /** Read only, creates file if it does not exist, starts at end of file **/
    FIO_OPEN_R_END_CREATE,
    /** Read and write, creates file if it does not exist, writes always go to the end of the file, starts at the beginning of file **/
    FIO_OPEN_R_APPEND_CREATE,
    /** Write only, creates file if it does not exist, clears existing file, starts at the beginning of file **/
    FIO_OPEN_W_CLEAR_CREATE,
    /** Read and write, the file must exist, starts at the beginning of file **/
    FIO_OPEN_RW_EXISTING,
    /** Read and write, creates file if it does not exist, clears existing file, starts at the beginning of file **/
    FIO_OPEN_RW_CLEAR_CREATE

    /** custom compounded operations ontop of the default available ones **/
    ,
    /** Write only, if the file has no data or does not exist, creates file, starts at beginning **/
    FIO_OPEN_W_IF_NOT_EXIST

}FIO_OPEN_ENUM;

typedef unsigned long FIO_SIZE;
///stream failure check
#define FIO_SIZE_ERROR -1L
///used in internal memory allocation operations to caller
#define FIO_SIZE_ERROR_NO_MEMORY -2L

typedef enum FIO_ERROR_ENUM{
    FIO_ERROR_FAILURE = 0,
    FIO_ERROR_SUCCESS,
    FIO_ERROR_NO_OP
}FIO_ERROR_ENUM;

typedef enum FIO_bool{
    FIO_FALSE = 0,
    FIO_TRUE = 1
}FIO_bool;

typedef struct FIO_Data{
    unsigned char *dataBuffer;
    FIO_SIZE dataSize;

    FIO_SIZE bufferSize;
}FIO_Data;

typedef struct FIO_File{
    FILE *file;
    FIO_OPEN_ENUM type;
}FIO_File;

typedef struct FIO_FileBuffer{
    FIO_File file;
    FIO_Data data;
}FIO_FileBuffer;

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




FILE* FIO_openFile(const char *fullFilePath, FIO_OPEN_ENUM action);
FIO_ERROR_ENUM FIO_write(FILE *handle, void *data, size_t byteLength);
FIO_ERROR_ENUM FIO_close(FILE *input);

FIO_ERROR_ENUM FIO_isSeekAtEnd(FILE *input);
FIO_ERROR_ENUM FIO_isInError(FILE *input);

FIO_ERROR_ENUM FIO_writeSeekOffset(FILE *input, FIO_SIZE offsetFromCurrent);
FIO_ERROR_ENUM FIO_writeSeekPosition(FILE *input, FIO_SIZE offsetFromZero);
FIO_ERROR_ENUM FIO_writeSeekToEnd(FILE *input);
FIO_ERROR_ENUM FIO_writeSeekToStart(FILE *input);

FIO_SIZE FIO_readSeekPositionRaw(FILE *input);
FIO_ERROR_ENUM FIO_readSeekPosition(FILE *input, FIO_SIZE *resultStorage);

FIO_ERROR_ENUM FIO_readFileToMemory(const char *fullFilePath, FIO_Data *store);
FIO_ERROR_ENUM FIO_writeMemoryToFile(const char *fullFilePath, void *data, size_t byteLength);

FIO_ERROR_ENUM FIO_readFull(FILE *input, FIO_Data *store);
FIO_SIZE FIO_readFullRaw(FILE *input, unsigned char **addressOfBufferPointer);

FIO_ERROR_ENUM FIO_readFullBestAttempt(FILE *input, FIO_Data *data);
FIO_SIZE FIO_readFullBestAttemptRaw(FILE *input, unsigned char **addressOfBufferPointer, FIO_SIZE *readLength);








FIO_Data* FIO_newData(FILE* input);
FIO_ERROR_ENUM FIO_initData(FIO_Data *data, FILE* input);

FIO_Data* FIO_newDataComplex(FILE* input, FIO_SIZE bufferSize);
FIO_ERROR_ENUM FIO_initDataComplex(FIO_Data *data, FILE* input, FIO_SIZE bufferSize);

FIO_Data* FIO_newDataDeferred();
FIO_ERROR_ENUM FIO_initDataDeferred(FIO_Data *data);

FIO_Data* FIO_newDataDeferredComplex(FIO_SIZE bufferSize);
FIO_ERROR_ENUM FIO_initDataDeferredComplex(FILE* input, FIO_SIZE bufferSize);




FIO_ERROR_ENUM FIO_DataNext(FIO_Data *data, FILE* input);




void FIO_freeData(FIO_Data *data);
void FIO_freeDataBuffer(FIO_Data *data);



FIO_Data* FIO_newFileDeferred(FILE* input);
FIO_ERROR_ENUM FIO_initFileDeferred(FIO_Data *data, FILE* input);

#define FIO_newStream FIO_newDataDeferred
#define FIO_initStream FIO_initDataDeferred









FIO_Data* FIO_newFileReader(const char* fullFilePath, FIO_OPEN_ENUM readType, size_t readBufferSize);
FIO_ERROR_ENUM FIO_initFileReader(FIO_FileReader *reader, const char* fullFilePath, FIO_OPEN_ENUM readType, size_t readBufferSize);

FIO_Data* FIO_newFileReaderComplex(FIO_FileReader *reader, size_t readBufferSize, FILE* existingHandle, unsigned char* existingBuffer);
FIO_ERROR_ENUM FIO_initFileReaderComplex(FIO_FileReader *reader, size_t readBufferSize, FILE* existingHandle, unsigned char* existingBuffer);







#endif // C_FILEHANDLER_H_INCLUDED
