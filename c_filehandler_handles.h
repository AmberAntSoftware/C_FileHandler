#ifndef C_FILEHANDLER_HANDLES_H_INCLUDED
#define C_FILEHANDLER_HANDLES_H_INCLUDED

#include "c_filehandler.h"

typedef struct FIO_File{
    FILE *file;
    FIO_OPEN_ENUM type;
    FIO_bool ownsFILE;
}FIO_File;

typedef struct FIO_FileBuffer{
    FIO_File file;
    FIO_Data data;
}FIO_FileBuffer;

FIO_File* FIO_newFile(FILE* input, FIO_OPEN_ENUM openType, FIO_bool ownsFILE);
FIO_File* FIO_newFileFromPath(const char *filePath, FIO_OPEN_ENUM action);

FIO_ERROR_ENUM FIO_initFile(FIO_File* metaStore, FILE* input, FIO_OPEN_ENUM openType, FIO_bool ownsFILE);
FIO_ERROR_ENUM FIO_initFileFromPath(FIO_File* metaStore, const char *filePath, FIO_OPEN_ENUM action);

void FIO_freeFile(FIO_File *file);
void FIO_freeFileData(FIO_File *file);
void FIO_freeFileNotFILE(FIO_File *file);
void FIO_freeFileDataNotFILE(FIO_File *file);

FIO_bool FIO_FileHasError(FIO_File *file);
FIO_bool FIO_FileAtEnd(FIO_File *file);

/**
Reads the next available data into the buffer.
Does not clear data from previous read, only overwrites to as much as read.
Returns FIO_ERROR_FAILURE is an error is detected; not necessarily
    unrecoverable, only that a read error occurred on the current read cycle.
**/
FIO_ERROR_ENUM FIO_FileReadNextData(FIO_File *file, FIO_Data *buffer);


FIO_FileBuffer* FIO_newFileBuffer(FILE* input, FIO_OPEN_ENUM openType);
FIO_FileBuffer* FIO_newFileBufferFromPath(const char *filePath, FIO_OPEN_ENUM action);
FIO_FileBuffer* FIO_newFileBufferFromFile(FIO_File* file, FIO_OPEN_ENUM action);

FIO_ERROR_ENUM FIO_initFileBuffer(FIO_FileBuffer* metaStore, FILE* input, FIO_OPEN_ENUM openType);
FIO_ERROR_ENUM FIO_initFileBufferFromPath(FIO_FileBuffer* metaStore, const char *filePath, FIO_OPEN_ENUM action);
FIO_ERROR_ENUM FIO_initFileBufferFromFile(FIO_FileBuffer* metaStore, FIO_File* file, FIO_OPEN_ENUM action);

void FIO_freeFileBuffer(FIO_File *file);
void FIO_freeFileBufferData(FIO_File *file);
void FIO_freeFileBufferNotFILE(FIO_File *file);
void FIO_freeFileBufferDataNotFILE(FIO_File *file);

#endif // C_FILEHANDLER_HANDLES_H_INCLUDED
