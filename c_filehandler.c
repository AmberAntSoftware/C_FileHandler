#include "c_filehandler.h"

FILE* FIO_openFile(const char *fullFilePath, FIO_OPEN_ENUM action){

    FILE* tmp;

    switch(action){
        /** default ops **/
        case(FIO_OPEN_R_EXISTING): return fopen(fullFilePath, "rb");
        case(FIO_OPEN_R_END_CREATE): return fopen(fullFilePath, "ab");
        case(FIO_OPEN_R_APPEND_CREATE): return fopen(fullFilePath, "a+b");
        case(FIO_OPEN_W_CLEAR_CREATE): return fopen(fullFilePath, "wb");
        case(FIO_OPEN_RW_EXISTING): return fopen(fullFilePath, "r+b");
        case(FIO_OPEN_RW_CLEAR_CREATE): return fopen(fullFilePath, "w+b");

        /** custom ops **/
        case(FIO_OPEN_W_IF_NOT_EXIST): {
            tmp = fopen(fullFilePath, "ab");
            /** file could not be created **/
            if(tmp == NULL){
                return NULL;
            }
            /** file is not empty **/
            if(FIO_readSeekPositionRaw(tmp) != 0){
                FIO_close(tmp);
                return NULL;
            }
            return tmp;
        }

        default:
            return NULL;
    }

}

FIO_ERROR_ENUM FIO_write(FILE *handle, void *data, size_t byteLength){

    size_t write = fwrite(data, byteLength, 1, handle);

    if(write != 1){
        return FIO_ERROR_FAILURE;
    }

    return FIO_ERROR_SUCCESS;
}

FIO_ERROR_ENUM FIO_close(FILE *input){

    unsigned int i;
    FIO_SIZE result = 0;
    for(i = 0; (result = fclose(input)) == EOF && i < 10; i++){
        debug("FAILED TO CLOSE FILE POINTER");
    }
    if(result == EOF){
        debug("Close Failure, stream no longer exists, or is full");
        return FIO_ERROR_FAILURE;
    }

    return FIO_ERROR_SUCCESS;
}

FIO_ERROR_ENUM FIO_isSeekAtEnd(FILE *input){
    if(feof(input) != 0){
        return FIO_ERROR_SUCCESS;
    }
    return FIO_ERROR_FAILURE;
}

FIO_ERROR_ENUM FIO_isInError(FILE *input){
    if (ferror(input) != 0){
        return FIO_ERROR_FAILURE;
    }
    return FIO_ERROR_SUCCESS;
}


FIO_ERROR_ENUM FIO_writeSeekOffset(FILE *input, FIO_SIZE offsetFromCurrent){

    if(fseek(input, offsetFromCurrent, SEEK_CUR) != 0){
        return FIO_ERROR_FAILURE;
    }
    return FIO_ERROR_SUCCESS;
}

FIO_ERROR_ENUM FIO_writeSeekPosition(FILE *input, FIO_SIZE offsetFromZero){

    if(fseek(input, offsetFromZero, SEEK_SET) != 0){
        return FIO_ERROR_FAILURE;
    }
    return FIO_ERROR_SUCCESS;
}

FIO_ERROR_ENUM FIO_writeSeekToEnd(FILE *input){

    if(fseek(input, 0, SEEK_END) != 0){
        return FIO_ERROR_FAILURE;
    }
    if(feof(input) != 0){
        return FIO_ERROR_FAILURE;
    }
    return FIO_ERROR_SUCCESS;
}

FIO_ERROR_ENUM FIO_writeSeekToStart(FILE *input){
    return FIO_writeSeekPosition(input, 0);
}


FIO_SIZE FIO_readSeekPositionRaw(FILE *input){

    FIO_SIZE position = 0;
    FIO_readSeekPosition(input, &position);
    return position;
}

FIO_ERROR_ENUM FIO_readSeekPosition(FILE *input, FIO_SIZE *resultStorage){

    if(input == NULL){
        return FIO_ERROR_FAILURE;
    }

    *resultStorage = ftell(input);
    if(*resultStorage == FIO_SIZE_ERROR){
        return FIO_ERROR_FAILURE;
    }

    return FIO_ERROR_SUCCESS;
}



FIO_ERROR_ENUM FIO_writeMemoryToFile(const char *fullFilePath, void *data, size_t byteLength){

    FILE *output = FIO_openFile(fullFilePath, FIO_OPEN_W_CLEAR_CREATE);
    if(output == NULL){
        return FIO_ERROR_FAILURE;
    }

    FIO_ERROR_ENUM result = FIO_write(output, data, byteLength);
    FIO_close(output);

    return result;
}

FIO_ERROR_ENUM FIO_readFileToMemory(const char *fullFilePath, FIO_Data *store){

    FILE *input = FIO_openFile(fullFilePath, FIO_OPEN_R_EXISTING);
    if(input == NULL){
        return FIO_ERROR_FAILURE;
    }

    FIO_ERROR_ENUM checkState = FIO_readFull(input, store);
    FIO_close(input);

    return checkState;
}

FIO_ERROR_ENUM FIO_readFull(FILE *input, FIO_Data *store){

    if(FIO_readFullBestAttempt(input, store) == FIO_ERROR_FAILURE){
        if(store->dataBuffer != NULL){
            free(store->dataBuffer);
        }
        return FIO_ERROR_FAILURE;
    }
    return FIO_ERROR_SUCCESS;
}

FIO_SIZE FIO_readFullRaw(FILE *input, unsigned char **addressOfBufferPointer){

    FIO_SIZE size = 0;
    size = FIO_readFullBestAttemptRaw(input, addressOfBufferPointer, &size);
    if(size == FIO_SIZE_ERROR || size == FIO_SIZE_ERROR_NO_MEMORY){
        if(*addressOfBufferPointer != NULL){
            free(*addressOfBufferPointer);
        }
    }
    return size;
}

FIO_ERROR_ENUM FIO_readFullBestAttempt(FILE *input, FIO_Data *store){
    store->bufferSize = FIO_readFullBestAttemptRaw(input, &store->dataBuffer, &store->dataSize);
    if(store->bufferSize == FIO_SIZE_ERROR || store->bufferSize == FIO_SIZE_ERROR_NO_MEMORY){
        return FIO_ERROR_FAILURE;
    }
    return FIO_ERROR_SUCCESS;
}


FIO_SIZE FIO_readFullBestAttemptRaw(FILE *input, unsigned char **addressOfBufferPointer, FIO_SIZE *readLength){
    const FIO_SIZE STARTING_SIZE = 32;

    size_t readCount = 0;
    FIO_SIZE size = STARTING_SIZE;
    FIO_SIZE prevSize = size;

    unsigned char *buffer = malloc(sizeof(unsigned char)*STARTING_SIZE);
    unsigned char *mask = buffer;
    if(buffer == NULL){
        debug("FAILED INIT BUFFER ALLOCATE");
        *addressOfBufferPointer = buffer;
        *readLength = size;
        return FIO_SIZE_ERROR_NO_MEMORY;
    }

    for(;;){

        readCount = fread(mask, sizeof(unsigned char), prevSize, input);

        if (FIO_isInError(input) == FIO_ERROR_FAILURE){
            *addressOfBufferPointer = buffer;
            *readLength = size;
            debug("STREAM ERROR");
            return FIO_SIZE_ERROR;
        }

        debugf("READ COUNT: %llu  SIZE: %li\n", readCount, size);
        if(readCount != prevSize){

            size /= 2;
            size += (size_t)(unsigned long int)readCount;

            debugf("READ READ OVER COUNT: %llu \n", readCount);

            mask = realloc(buffer, sizeof(unsigned char)*size);
            if(mask != NULL){
                buffer = mask;
            }

            debugf("READ COUNT SIZE: %li\n", size);

            debug("OVER READ FINISH");
            *addressOfBufferPointer = buffer;
            *readLength = size;
            return size;
        }

        if(FIO_isSeekAtEnd(input) == FIO_ERROR_SUCCESS){
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













void FIO_freeData(FIO_Data *data){
    FIO_freeDataBuffer(data);
    free(data);
}
void FIO_freeDataBuffer(FIO_Data *data){
    free(data->dataBuffer);
}















