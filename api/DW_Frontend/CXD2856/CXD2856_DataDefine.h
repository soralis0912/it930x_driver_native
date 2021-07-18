#ifndef __CXD2856_DATADEFINE_H__
#define __CXD2856_DATADEFINE_H__

#include "sony_integ.h"
#include "sony_demod.h"
#include "sony_i2c.h"
#include "sony_helene.h"

typedef struct
{
    void* pDeviceContext;
    unsigned char TunerDeviceID;

}CXD2856_USER_DATA,*PCXD2856_USER_DATA;

#endif // __CXD2856_DATADEFINE_H__