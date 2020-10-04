#include "c_filehandler_globals.h"

static FIO_SIZE FIO_X_closeFailuresSize = 1;
static FIO_SIZE FIO_X_closeFailuresMax = 0;
static FILE** FIO_X_closeFailures = NULL;
static FIO_ERROR_ENUM FIO_X_addToCloseFailures(FILE* failedToClose){

    if(FIO_X_closeFailuresMax+1 >= FIO_X_closeFailuresSize){
        FILE** tmp = NULL;
        if(FIO_X_closeFailures == NULL){
            tmp = malloc(sizeof(FILE*) * FIO_X_closeFailuresSize);
        }else{
            FIO_X_closeFailuresSize *= 2;
            tmp = realloc(FIO_X_closeFailures, sizeof(FILE*) * FIO_X_closeFailuresSize);
        }
        if(tmp == NULL){
            return FIO_ERROR_FAILURE;
        }
        FIO_X_closeFailures = tmp;
    }

    FIO_X_closeFailures[FIO_X_closeFailuresMax] = failedToClose;
    FIO_X_closeFailuresMax++;
    return FIO_ERROR_SUCCESS;
}

FIO_ERROR_ENUM FIO__tryCloseAllFailures(){
    FIO_SIZE i;
    for(i=0; i < FIO_X_closeFailuresMax; i++){
        if(fclose(FIO_X_closeFailures[i]) != EOF){
            FIO_X_closeFailures[i] = FIO_X_closeFailures[FIO_X_closeFailuresMax-1];
            FIO_X_closeFailuresMax--;
            i--;
        }
    }
    if(FIO_X_closeFailuresMax > 0){
        return FIO_ERROR_FAILURE;
    }
    return FIO_ERROR_SUCCESS;
}


FILE* FIO__openPath(const char *filePath, FIO_OPEN_ENUM action){

    FILE* tmp;

    switch(action){
        /** default ops **/
        case(FIO_OPEN_R_EXISTING): return fopen(filePath, "rb");
        case(FIO_OPEN_R_END_CREATE): return fopen(filePath, "ab");
        case(FIO_OPEN_R_APPEND_CREATE): return fopen(filePath, "a+b");
        case(FIO_OPEN_W_CLEAR_CREATE): return fopen(filePath, "wb");
        case(FIO_OPEN_RW_EXISTING): return fopen(filePath, "r+b");
        case(FIO_OPEN_RW_CLEAR_CREATE): return fopen(filePath, "w+b");

        /** custom ops **/
        case(FIO_OPEN_W_IF_NOT_EXIST): {
            tmp = fopen(filePath, "ab");
            /** file could not be created **/
            if(tmp == NULL){
                return NULL;
            }
            /** file is not empty **/
            if(FIO__readSeekPositionRaw(tmp) != 0){
                FIO__close(tmp);
                return NULL;
            }
            return tmp;
        }

        default:
            return NULL;
    }

}

FIO_ERROR_ENUM FIO__write(FILE *handle, void *data, size_t byteLength){

    size_t write = fwrite(data, byteLength, 1, handle);

    if(write != 1){
        return FIO_ERROR_FAILURE;
    }

    return FIO_ERROR_SUCCESS;
}

FIO_ERROR_ENUM FIO__close(FILE *input){

    unsigned int i;
    FIO_SIZE result = 0;
    for(i = 0; ((result = fclose(input)) == EOF) && i < 3; i++){
        debug("FAILED TO CLOSE FILE POINTER");
    }
    if(result == EOF){
        debug("Close Failure, stream no longer exists, or is full");
        FIO_X_addToCloseFailures(input);
        return FIO_ERROR_FAILURE;
    }

    return FIO_ERROR_SUCCESS;
}

FIO_ERROR_ENUM FIO__isSeekAtEnd(FILE *input){
    if(feof(input) != 0){
        return FIO_ERROR_SUCCESS;
    }
    return FIO_ERROR_FAILURE;
}

FIO_ERROR_ENUM FIO__isInError(FILE *input){
    if (ferror(input) != 0){
        return FIO_ERROR_FAILURE;
    }
    return FIO_ERROR_SUCCESS;
}


FIO_ERROR_ENUM FIO__writeSeekOffset(FILE *input, FIO_SIZE offsetFromCurrent){

    if(fseek(input, offsetFromCurrent, SEEK_CUR) != 0){
        return FIO_ERROR_FAILURE;
    }
    return FIO_ERROR_SUCCESS;
}

FIO_ERROR_ENUM FIO__writeSeekPosition(FILE *input, FIO_SIZE offsetFromZero){

    if(fseek(input, offsetFromZero, SEEK_SET) != 0){
        return FIO_ERROR_FAILURE;
    }
    return FIO_ERROR_SUCCESS;
}

FIO_ERROR_ENUM FIO__writeSeekToEnd(FILE *input){

    if(fseek(input, 0, SEEK_END) != 0){
        return FIO_ERROR_FAILURE;
    }
    if(feof(input) != 0){
        return FIO_ERROR_FAILURE;
    }
    return FIO_ERROR_SUCCESS;
}

FIO_ERROR_ENUM FIO__writeSeekToStart(FILE *input){
    return FIO__writeSeekPosition(input, 0);
}


FIO_SIZE FIO__readSeekPositionRaw(FILE *input){

    FIO_SIZE position = 0;
    FIO__readSeekPosition(input, &position);
    return position;
}

FIO_ERROR_ENUM FIO__readSeekPosition(FILE *input, FIO_SIZE *resultStorage){

    if(input == NULL){
        return FIO_ERROR_FAILURE;
    }

    *resultStorage = ftell(input);
    if(*resultStorage == FIO_SIZE_ERROR){
        return FIO_ERROR_FAILURE;
    }

    return FIO_ERROR_SUCCESS;
}



FIO_ERROR_ENUM FIO__writeMemoryToPath(const char *filePath, void *data, size_t byteLength){

    FILE *output = FIO__openPath(filePath, FIO_OPEN_W_CLEAR_CREATE);
    if(output == NULL){
        return FIO_ERROR_FAILURE;
    }

    FIO_ERROR_ENUM result = FIO__write(output, data, byteLength);
    FIO__close(output);

    return result;
}

FIO_ERROR_ENUM FIO__readPathToMemory(const char *filePath, FIO_Data *metaStore){

    FILE *input = FIO__openPath(filePath, FIO_OPEN_R_EXISTING);
    if(input == NULL){
        debug("Could not open path");
        return FIO_ERROR_FAILURE;
    }

    FIO_ERROR_ENUM checkState = FIO__readFull(input, metaStore);
    FIO__close(input);

    return checkState;
}

FIO_ERROR_ENUM FIO__readFull(FILE *input, FIO_Data *metaStore){

    if(FIO__readFullBestAttempt(input, metaStore) == FIO_ERROR_FAILURE){
        if(metaStore->dataBuffer != NULL){
            free(metaStore->dataBuffer);
        }
        metaStore->dataSize = 0;
        metaStore->bufferSize = 0;
        return FIO_ERROR_FAILURE;
    }
    return FIO_ERROR_SUCCESS;
}

FIO_SIZE FIO__readFullRaw(FILE *input, unsigned char **addressOfBufferPointer){

    FIO_SIZE size = 0;
    size = FIO__readFullBestAttemptRaw(input, addressOfBufferPointer, &size);
    if(size == FIO_SIZE_ERROR || size == FIO_SIZE_ERROR_NO_MEMORY){
        if(*addressOfBufferPointer != NULL){
            free(*addressOfBufferPointer);
            *addressOfBufferPointer = NULL;
        }
    }
    return size;
}

FIO_ERROR_ENUM FIO__readFullBestAttempt(FILE *input, FIO_Data *metaStore){
    metaStore->bufferSize = FIO__readFullBestAttemptRaw(input, &metaStore->dataBuffer, &metaStore->dataSize);
    if(metaStore->bufferSize == FIO_SIZE_ERROR || metaStore->bufferSize == FIO_SIZE_ERROR_NO_MEMORY){
        return FIO_ERROR_FAILURE;
    }
    return FIO_ERROR_SUCCESS;
}


FIO_SIZE FIO__readFullBestAttemptRaw(FILE *input, unsigned char **addressOfBufferPointer, FIO_SIZE *readLength){
    const FIO_SIZE STARTING_SIZE = 1024;

    size_t readCount = 0;
    FIO_SIZE size = STARTING_SIZE;
    FIO_SIZE prevSize = size;

    unsigned char *buffer = malloc(sizeof(unsigned char)*STARTING_SIZE);
    unsigned char *mask = buffer;
    if(buffer == NULL){
        debug("FAILED INIT BUFFER ALLOCATE");
        *addressOfBufferPointer = NULL;
        *readLength = 0;
        return FIO_SIZE_ERROR_NO_MEMORY;
    }

    for(;;){

        readCount = fread(mask, sizeof(unsigned char), prevSize, input);

        if (FIO__isInError(input) == FIO_ERROR_FAILURE){
            *addressOfBufferPointer = buffer;
            *readLength = size;
            debug("STREAM ERROR");
            return FIO_SIZE_ERROR;
        }

        debugf("READ COUNT: %lu  SIZE_AVAILABLE: %li\n", readCount, size);
        if(readCount != prevSize){

            if(size == STARTING_SIZE){
                size = (size_t)(unsigned long int)readCount;
                debugf("READ UNDER COUNT: %lu \n", readCount);
            }else{
                size /= 2;
                size += (size_t)(unsigned long int)readCount;
                debugf("READ OVER COUNT: %lu \n", readCount);
            }

            mask = realloc(buffer, sizeof(unsigned char)*size);
            if(mask != NULL){
                buffer = mask;
            }

            debugf("READ COUNT SIZE: %lu\n", size);

            debug("OVER READ FINISH");
            *addressOfBufferPointer = buffer;
            *readLength = size;
            return size;
        }

        if(FIO__isSeekAtEnd(input) == FIO_ERROR_SUCCESS){
            debug("AT END OF FILE");
            *addressOfBufferPointer = buffer;
            *readLength = size;
            return size;
        }

        mask = realloc(buffer, sizeof(unsigned char)*size*2);
        if(mask == NULL){
            debug("FAILED REALLOCATE");
            *addressOfBufferPointer = buffer;
            *readLength = size;
            return FIO_SIZE_ERROR_NO_MEMORY;
        }

        buffer = mask;

        prevSize = size;
        mask = buffer+prevSize;
        size*=2;
    }

}




