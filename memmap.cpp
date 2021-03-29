#include "memmap.h"

MemMap::MemMap()
{
   TCHAR memmapfile[] = TEXT("Local\\YawVRGEFile");

    hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // use paging file
        NULL,                    // default security
        PAGE_READWRITE,          // read/write access
        0,                       // maximum object size (high-order DWORD)
       sizeof(yawvr_data),                // maximum object size (low-order DWORD)
        memmapfile);                 // name of mapping object

    if (hMapFile == NULL)
    {
      //  _tprintf(TEXT("Could not create file mapping object (%d).\n"),
        //         GetLastError());
    }
    pBuf = (yawvr_data*) MapViewOfFile(hMapFile,   // handle to map object
                                  FILE_MAP_ALL_ACCESS, // read/write permission
                                  0,
                                  0,
                                      sizeof(yawvr_data));

    if (pBuf == NULL)
    {
        //_tprintf(TEXT("Could not map view of file (%d).\n"),
          //       GetLastError());

        CloseHandle(hMapFile);

    }
}

void MemMap::pushMemMap(yawvr_data data){
    CopyMemory(pBuf, &data, sizeof(data));
    _getch();


}

MemMap::~MemMap(){
    qDebug() << "Closing MemMap";
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
}
