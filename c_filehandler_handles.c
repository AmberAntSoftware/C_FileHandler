#include "c_filehandler_handles.h"

FIO_File* FIO_newFile(FILE* input, FIO_OPEN_ENUM openType, FIO_bool ownsFILE){

    FIO_File *file = malloc(sizeof(FIO_File));
    if(file == NULL){
        return NULL;
    }

    if(FIO_initFile(file, input, openType, ownsFILE) == FIO_ERROR_FAILURE){
        free(file);
        return NULL;
    }

    return file;
}
FIO_File* FIO_newFileFromPath(const char *filePath, FIO_OPEN_ENUM action){

    FIO_File *file = malloc(sizeof(FIO_File));
    if(file == NULL){
        return NULL;
    }

    if(FIO_initFileFromPath(file, filePath, action) == FIO_ERROR_FAILURE){
        free(file);
        return NULL;
    }

    return file;
}

FIO_ERROR_ENUM FIO_initFile(FIO_File* metaStore, FILE* input, FIO_OPEN_ENUM openType, FIO_bool ownsFILE){

    metaStore->file = input;
    metaStore->ownsFILE = ownsFILE;
    metaStore->type = openType;

    return FIO_ERROR_SUCCESS;
}
FIO_ERROR_ENUM FIO_initFileFromPath(FIO_File* metaStore, const char *filePath, FIO_OPEN_ENUM action){
    FILE* input = FIO__openPath(filePath, action);
    if(input == NULL){
        return FIO_ERROR_FAILURE;
    }
    return FIO_initFile(metaStore, input, action, FIO_TRUE);
}

void FIO_freeFile(FIO_File *file){
    FIO_freeFileData(file);
    free(file);
}
void FIO_freeFileData(FIO_File *file){
    if(file->ownsFILE == FIO_TRUE){
        FIO__close(file->file);
    }
    FIO_freeFileNotFILE(file);
}

void FIO_freeFileNotFILE(FIO_File *file){
    FIO_freeFileDataNotFILE(file);
    free(file);
}
void FIO_freeFileDataNotFILE(FIO_File *file){
    return;
}


FIO_bool FIO_FileHasError(FIO_File *file){
    return FIO__isSeekAtEnd(file->file);
}
FIO_bool FIO_FileAtEnd(FIO_File *file){
    return FIO__isSeekAtEnd(file->file);
}

FIO_ERROR_ENUM FIO_FileReadNextData(FIO_File *file, FIO_Data *buffer){

    if(FIO_FileAtEnd(file) == FIO_TRUE){
        buffer->dataSize = 0;
        buffer->bufferSize = 0;
        return FIO_ERROR_SUCCESS;
    }

    buffer->dataSize = FIO__readMax(file->file, buffer->dataBuffer, buffer->bufferSize);

    if(buffer->dataSize != buffer->dataSize && FIO_FileHasError(file) == FIO_TRUE){
        return FIO_ERROR_FAILURE;
    }

    return FIO_ERROR_SUCCESS;
}

