#ifndef C_FILEHANDLER_H_INCLUDED
#define C_FILEHANDLER_H_INCLUDED

#include "c_filehandler_globals.h"

FIO_Data* FIO_newData();
FIO_ERROR_ENUM FIO_initData(FIO_Data *data);

void FIO_freeData(FIO_Data *data);
void FIO_freeDataData(FIO_Data *data);

#include "c_filehandler_handles.h"

#endif // C_FILEHANDLER_H_INCLUDED
