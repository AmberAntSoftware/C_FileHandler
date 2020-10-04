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

    /** custom compounded operations built ontop of the default modes **/
    ,

    /** Write only, if the file has no data or does not exist, creates file, starts at beginning **/
    FIO_OPEN_W_IF_NOT_EXIST

}FIO_OPEN_ENUM;

typedef enum FIO_ERROR_ENUM{
    FIO_ERROR_FAILURE = 0,
    FIO_ERROR_SUCCESS,
    FIO_ERROR_NO_OP
}FIO_ERROR_ENUM;

typedef unsigned long FIO_SIZE;
/** stream failure check **/
#define FIO_SIZE_ERROR -1L
/** NMEM failure check **/
#define FIO_SIZE_ERROR_NO_MEMORY -2L

typedef enum FIO_bool{
    FIO_FALSE = 0,
    FIO_TRUE = 1
}FIO_bool;

typedef struct FIO_Data{

    unsigned char *dataBuffer;
    FIO_SIZE dataSize;
    FIO_SIZE bufferSize;

}FIO_Data;


FILE* FIO__openPath(const char *filePath, FIO_OPEN_ENUM action);
FIO_ERROR_ENUM FIO__write(FILE *handle, void *data, size_t byteLength);
FIO_ERROR_ENUM FIO__close(FILE *input);

FIO_ERROR_ENUM FIO__isSeekAtEnd(FILE *input);
FIO_ERROR_ENUM FIO__isInError(FILE *input);

FIO_ERROR_ENUM FIO__writeSeekOffset(FILE *input, FIO_SIZE offsetFromCurrent);
FIO_ERROR_ENUM FIO__writeSeekPosition(FILE *input, FIO_SIZE offsetFromZero);
FIO_ERROR_ENUM FIO__writeSeekToEnd(FILE *input);
FIO_ERROR_ENUM FIO__writeSeekToStart(FILE *input);

FIO_SIZE FIO__readSeekPositionRaw(FILE *input);
FIO_ERROR_ENUM FIO__readSeekPosition(FILE *input, FIO_SIZE *resultStorage);

FIO_ERROR_ENUM FIO__readPathToMemory(const char *filePath, FIO_Data *metaStore);
FIO_ERROR_ENUM FIO__writeMemoryToPath(const char *filePath, void *data, size_t byteLength);

FIO_ERROR_ENUM FIO__readFull(FILE *input, FIO_Data *metaStore);
FIO_SIZE FIO__readFullRaw(FILE *input, unsigned char **addressOfBufferPointer);

FIO_ERROR_ENUM FIO__readFullBestAttempt(FILE *input, FIO_Data *data);
FIO_SIZE FIO__readFullBestAttemptRaw(FILE *input, unsigned char **addressOfBufferPointer, FIO_SIZE *readLength);


FIO_Data* FIO_newData();
FIO_ERROR_ENUM FIO_initData(FIO_Data *data);

void FIO_freeData(FIO_Data *data);
void FIO_freeDataData(FIO_Data *data);








#endif // C_FILEHANDLER_H_INCLUDED
