#include "c_filehandler.h"

FIO_Data* FIO_newData(){
    FIO_Data *data = calloc(1, sizeof(FIO_Data));
    if(data == NULL){
        return NULL;
    }

    data->dataBuffer = NULL;

    if(FIO_initData(data) == FIO_ERROR_FAILURE){
        free(data);
        return NULL;
    }

    return data;
}

FIO_ERROR_ENUM FIO_initData(FIO_Data *data){

    if(data->dataBuffer != NULL){

        FIO_freeDataData(data);
        data->dataBuffer = NULL;
        data->dataSize = 0;
        data->bufferSize = 0;

    }

    data->dataSize = 0;
    data->bufferSize = FIO_BUFF_SIZE;
    data->dataBuffer = calloc(1, data->bufferSize);

    return FIO_ERROR_SUCCESS;
}

void FIO_freeData(FIO_Data *data){
    FIO_freeDataData(data);
    free(data);
}
void FIO_freeDataData(FIO_Data *data){
    free(data->dataBuffer);
}
