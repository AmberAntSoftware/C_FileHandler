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

/**
Returns if the FIO_File had an error in the last called operation
**/
FIO_bool FIO_FileHasError(FIO_File *file);
/**
Returns if the FIO_File FILE* is at the end
**/
FIO_bool FIO_FileAtEnd(FIO_File *file);

/**
Reads the next available data into the buffer.
Does not clear data from previous read, only overwrites to as much as read.
Returns FIO_ERROR_FAILURE is an error is detected; not necessarily
    unrecoverable, only that a read error occurred on the current read cycle.
**/
FIO_ERROR_ENUM FIO_FileReadNextData(FIO_File *file, FIO_Data *buffer);


/**
FIO_FileBuffer
Purely a copy container for operation with data convenience
Simply initialize each separately on the base structure to avoid copying
**/
FIO_FileBuffer* FIO_newFileBuffer(FIO_File* copyFile, FIO_Data* copyData);
FIO_ERROR_ENUM FIO_initFileBuffer(FIO_FileBuffer* metaStore, FIO_File* copyFile, FIO_Data* copyData);

void FIO_freeFileBuffer(FIO_FileBuffer *file);
void FIO_freeFileBufferData(FIO_FileBuffer *file);
/**
Does not free either of the copied data
Intended for copying out of the structure or copying data pointers
**/
void FIO_freeFileBufferNotData(FIO_FileBuffer *file);

/**
See: FIO_FileHasError
**/
FIO_bool FIO_FileBufferHasError(FIO_FileBuffer *fbuff);
/**
See: FIO_FileAtEnd
**/
FIO_bool FIO_FileBufferAtEnd(FIO_FileBuffer *fbuff);
/**
See: FIO_FileReadNextData
**/
FIO_ERROR_ENUM FIO_FileBufferReadNextData(FIO_FileBuffer *fbuff);


#endif // C_FILEHANDLER_HANDLES_H_INCLUDED
