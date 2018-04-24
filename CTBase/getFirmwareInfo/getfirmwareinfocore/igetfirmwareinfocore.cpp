#include "igetfirmwareinfocore.h"

#include "sislog.h"
#include "ctexception/ctexception.h"

#include <typeinfo>

using namespace CT;
using namespace SiS;
using namespace SiS::Procedure;

#define EXCEPTION_TITLE "IGetFirmwareInfoCore Exception : "
#define TAG "IGetFirmwareInfoCore"

IGetFirmwareInfoCore::IGetFirmwareInfoCore(GetFirmwareInfoParameter* getFirmwareInfoParameter) :
    CTBaseCore(getFirmwareInfoParameter),
    m_getFirmwareInfoParameter(getFirmwareInfoParameter)
{

}

IGetFirmwareInfoCore::~IGetFirmwareInfoCore()
{

}

CTBaseReference*
IGetFirmwareInfoCore::generateReference()
{
    return new GetFirmwareInfoReference();
}

GetFirmwareInfoReference*
IGetFirmwareInfoCore::getGetFirmwareInfoReference(ReferenceSource rs, int chipIndex)
{
    CTBaseReference* ctBaseRef = getCTBaseReference(rs, chipIndex);

    try
    {
        GetFirmwareInfoReference* gRef = dynamic_cast<GetFirmwareInfoReference*>(ctBaseRef);
        return gRef;
    }
    catch(bad_cast)
    {
        std::string msg = EXCEPTION_TITLE;
        char errorMsg[1024] = "";
        sprintf(errorMsg, "bad_cast GetFirmwareInfoReference* : rs=%d, chipIndex=%d\n", rs, chipIndex);
        msg.append(errorMsg);
        throw CTException( msg );
    }
}

void
IGetFirmwareInfoCore::showSerialData(SerialData* serialData, bool showEngineerMode)
{
    if(showEngineerMode)
    {
        SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "$%s : %s", serialData->getKeyword().c_str(), serialData->getSerial().c_str());
    }
    else
    {
        int shift = 0;
        unsigned char* dataBuf = serialData->getData();
        int dataSize = serialData->getSize();

        while( shift < dataSize )
        {
            /* 1. address */
            //SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "0x%08x : ", serialData->getAddress() + shift);

            /* 2. hex */
            unsigned char _16Bytes[16] = { 0x0 };
            int count = 0;

            for(int i = 0; i < 16; i++)
            {
                if( shift + i < dataSize )
                {
                    _16Bytes[i] = dataBuf[shift + i];
                    count++;
                }
            }

            std::string _16BytesHex;
            for(int i = 0; i < 16; i++)
            {
                char bData[10] = "";
                if( i < count )
                {
                    sprintf(bData, "%02X ", _16Bytes[i]);
                }
                else
                {
                    sprintf(bData, "   ");
                }
                _16BytesHex.append(bData);
            }
            //SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s", _16BytesHex.c_str());

            /* 3. dump */
            std::string _16BytesASCII;
            for(int i = 0; i < count; i++)
            {
                char bData[10] = "";
                if ( (_16Bytes[i] < 0x20) || (_16Bytes[i] > 0xff) )
                {
                    sprintf(bData, ".");
                }
                else if( _16Bytes[i] == 0xff )
                {
                    sprintf(bData, ".");
                }
                else
                {
                    sprintf(bData, "%c", _16Bytes[i]);
                }
                _16BytesASCII.append(bData);
            }
            //SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "%s\n", _16BytesASCII.c_str());

            SIS_LOG_I(SiSLog::getOwnerSiS(), TAG, "0x%08x : %s %s", serialData->getAddress() + shift, _16BytesHex.c_str(), _16BytesASCII.c_str() );

            /* next */
            shift += 16;
        }
    }
}
