#include "c_filehandler.h"

FIO_Data* FIO_newData(){
    return FIO_newDataComplex(FIO_BUFF_SIZE);
}

FIO_Data* FIO_newDataNone(){
    return FIO_newDataComplex(0);
}

FIO_Data* FIO_newDataComplex(FIO_SIZE bufferSize){
    FIO_Data *data = calloc(1, sizeof(FIO_Data));
    if(data == NULL){
        return NULL;
    }

    if(FIO_initDataComplex(data, bufferSize) == FIO_ERROR_FAILURE){
        free(data);
        return NULL;
    }

    return data;
}

FIO_Data* FIO_newDataCopy(FIO_Data *dataToCopy){
    FIO_Data *copy = FIO_newDataComplex(dataToCopy->bufferSize);
    if(copy == NULL){
        return NULL;
    }

    unsigned char* tmp = copy->dataBuffer;
    /** todo handle memcpy error for some reason **/
    memcpy(copy, dataToCopy, sizeof(*dataToCopy));
    copy->dataBuffer = tmp;
    memcpy(copy->dataBuffer, copy->dataBuffer, dataToCopy->bufferSize);

    return copy;
}


FIO_ERROR_ENUM FIO_initData(FIO_Data *data){
    return FIO_initDataComplex(data, FIO_BUFF_SIZE);
}

FIO_ERROR_ENUM FIO_initDataNone(FIO_Data *data){
    return FIO_initDataComplex(data, 0);
}

FIO_ERROR_ENUM FIO_initDataComplex(FIO_Data *data, FIO_SIZE bufferSize){

    data->dataSize = 0;
    data->bufferSize = bufferSize;

    if(bufferSize > 0){

        data->dataBuffer = malloc(data->bufferSize);
        if(data->dataBuffer == NULL){
            return FIO_ERROR_FAILURE;
        }

    }else{
        data->dataBuffer = NULL;
    }

    return FIO_ERROR_SUCCESS;
}

FIO_ERROR_ENUM FIO_initDataCopy(FIO_Data *metaStore, FIO_Data *dataToCopy){

    /** todo handle memcpy error for some reason **/
    memcpy(metaStore, dataToCopy, sizeof(*dataToCopy));

    size_t dataSize = sizeof(unsigned char)*dataToCopy->bufferSize;
    unsigned char *buff = malloc(dataSize);
    if(buff == NULL){
        return FIO_ERROR_FAILURE;
    }
    memcpy(buff, dataToCopy->dataBuffer, dataSize);

    metaStore->dataBuffer = buff;

    return FIO_ERROR_SUCCESS;
}



void FIO_freeData(FIO_Data *data){
    FIO_freeDataData(data);
    free(data);
}

void FIO_freeDataData(FIO_Data *data){

    if(data->dataBuffer != NULL){
        free(data->dataBuffer);
    }
}



