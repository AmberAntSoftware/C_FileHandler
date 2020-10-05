#ifndef C_FILEHANDLER_H_INCLUDED
#define C_FILEHANDLER_H_INCLUDED

#include "c_filehandler_globals.h"

FIO_Data* FIO_newData();
FIO_Data* FIO_newDataNone();
FIO_Data* FIO_newDataComplex(FIO_SIZE bufferSize);
FIO_Data* FIO_newDataCopy(FIO_Data *dataToCopy);

FIO_ERROR_ENUM FIO_initData(FIO_Data *data);
FIO_ERROR_ENUM FIO_initDataNone(FIO_Data *data);
FIO_ERROR_ENUM FIO_initDataComplex(FIO_Data *data, FIO_SIZE bufferSize);
FIO_ERROR_ENUM FIO_initDataCopy(FIO_Data *metaStore, FIO_Data *dataToCopy);

void FIO_freeData(FIO_Data *data);
void FIO_freeDataData(FIO_Data *data);

#include "c_filehandler_handles.h"

#endif // C_FILEHANDLER_H_INCLUDED
