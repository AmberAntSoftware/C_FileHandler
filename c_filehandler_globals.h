#ifndef C_FILEHANDLER_GLOBALS_H_INCLUDED
#define C_FILEHANDLER_GLOBALS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIO_DEBUG
#ifdef FIO_DEBUG
    #define debugf(...) printf(__VA_ARGS__)
    #define debug(str) puts(str)
#else
    #define debugf(...) do{}while(0)
    #define debug(str) do{}while(0)
#endif

#define FIO_BUFF_SIZE (1024*1024)

/** TODO construct handling with win32 functions **/

/**
Enumeration for encapsulated fopen types
**/
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
    FIO_OPEN_W_IF_NOT_EXIST,

    /** enum error reporter **/
    FIO_OPEN_ENUM_ERROR

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



/*************************************
Internal Operations and Conversions
*************************************/



/**
Any FILE* that failed to close in FIO__close(FILE*) will be stored in this.
Internal errors may also happen, periodically attempt to reclose them with this.
Low penalty if no FILE* failed to close.
Recommended to call at the end of every program. ex: atexit(FIO__tryCloseAllFailures);

returns FIO_ERROR_SUCCESS if all stored handles closed
returns FIO_ERROR_FAILURE if any handle(s) did not close
**/
FIO_ERROR_ENUM FIO__tryCloseAllFailures();

/**
converts a known fopen argument to FIO_OPEN_ENUM open type.
returns FIO_OPEN_ENUM_ERROR on error
**/
FIO_OPEN_ENUM FIO__fopenArgsToEnum(const char* fopenArgument, size_t argsByteLength);




/*************************************
FILE* manipuation
*************************************/




/**
Opens a FILE* from a path with the specified action.
See FIO_OPEN_ENUM documentation
**/
FILE* FIO__openPath(const char *filePath, FIO_OPEN_ENUM action);

/**
Attemps to read the full quantity specified.
If the full quantity cannot be read, errors.
**/
FIO_ERROR_ENUM FIO__read(FILE *handle, void *storage, size_t byteLength);

/**
Attemps to write the full quantity specified.
If the full quantity cannot be written, errors.
**/
FIO_ERROR_ENUM FIO__write(FILE *handle, void *data, size_t byteLength);

/**
Reads the maximum amount specified.
If the max amount cannot be read, specifies how much was given.
Check for errors in FIO_SIZE sentinel values.
**/
FIO_SIZE FIO__readMax(FILE *handle, void *storage, size_t byteLength);

/**
Reads the maximum amount specified.
If the max amount cannot be read, specifies how much was read.
Check for errors in FIO_SIZE sentinel values.
**/
FIO_SIZE FIO__writeMax(FILE *handle, void *data, size_t byteLength);

/**
Attempts to close the FILE* 3 times.
If the FILE* cannot be closed, it is added to the internal fail queue.
Returns FIO_ERROR_FAILURE if the handle was not closed.
**/
FIO_ERROR_ENUM FIO__close(FILE *input);



/*************************************
FILE* Seek operations
*************************************/



FIO_bool FIO__isSeekAtEnd(FILE *input);
FIO_bool FIO__isInError(FILE *input);

FIO_ERROR_ENUM FIO__setSeekOffset(FILE *input, FIO_SIZE offsetFromCurrent);
FIO_ERROR_ENUM FIO__setSeekPosition(FILE *input, FIO_SIZE offsetFromZero);
FIO_ERROR_ENUM FIO__setSeekToEnd(FILE *input);
FIO_ERROR_ENUM FIO__setSeekToStart(FILE *input);

FIO_SIZE FIO__getSeekPositionRaw(FILE *input);
FIO_ERROR_ENUM FIO__getSeekPosition(FILE *input, FIO_SIZE *resultStorage);



/*************************************
FILE* Batch Operations
*************************************/



/**
Attempts to read the full data of the file specified by filePath into metaStore.
Frees data if an error is encountered.
**/
FIO_ERROR_ENUM FIO__readPathToMemory(const char *filePath, FIO_Data *metaStore);

/**
Attempts to write the full data of the given length into the path specified by filePath.
If a path given already has data in it, that data is not overwritten, and no action occurs -- reported as error.
Reports if the write failed. Data should not be written at all if there is any failure, but depends on platform behavior.
**/
FIO_ERROR_ENUM FIO__writeMemoryToPathSafe(const char *filePath, void *data, size_t byteLength);
FIO_ERROR_ENUM FIO__writeMemoryToPath(const char *filePath, void *data, size_t byteLength);
/**
Attempts to write the full data of the given length into the path specified by filePath.
If the path given already has data in it, that data is cleared.
Reports if the write failed. Data should not be written at all if there is any failure, but depends on platform behavior.
**/
FIO_ERROR_ENUM FIO__writeMemoryToPathType(const char *filePath, void *data, size_t byteLength, FIO_OPEN_ENUM openType);

/**
Attempts to read the full data into metaStore.
Frees data if an error is encountered.
**/
FIO_ERROR_ENUM FIO__readFull(FILE *input, FIO_Data *metaStore);
/**
Attempts to read the full data into the addressOfBufferPointer.
Frees data if an error is encountered; determined by the code in FIO_SIZE
**/
FIO_SIZE FIO__readFullRaw(FILE *input, unsigned char **addressOfBufferPointer);

/**
Attempts to read the full data into metaStore.
Keeps data if and reports if error is encountered
**/
FIO_ERROR_ENUM FIO__readFullBestAttempt(FILE *input, FIO_Data *data);
/**
Attempts to read the full data into addressOfBufferPointer.
Keeps data if and reports if error is encountered; determined by the code in FIO_SIZE
**/
FIO_SIZE FIO__readFullBestAttemptRaw(FILE *input, unsigned char **addressOfBufferPointer, FIO_SIZE *readLength);


#endif // C_FILEHANDLER_GLOBALS_H_INCLUDED
