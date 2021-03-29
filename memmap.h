#ifndef MEMMAP_H
#define MEMMAP_H


#include <QObject>
#include <QDebug>
#include "defines.h"

class MemMap : public QObject
{
    Q_OBJECT

private:
    HANDLE hMapFile;
    yawvr_data *pBuf;
public:
    MemMap();
    ~MemMap();
public slots:
    void pushMemMap(yawvr_data);
};

#endif // MEMMAP_H
