#include "CalibrationParameter.h"
#include "SiSTouchAdapter.h"
#include "AegisSiSTouchAdapter.h"
#include "version.h"
#include "ExitStatus.h"
#include <cstring>
#include <typeinfo>


void printVersion();
void print_sep();
FILE* open_firmware_bin( const char* filename, int* file_size );

int main( int argc, char** argv )
{
    ApplicationParameter *param_tmp= ApplicationParameter::create(ApplicationParameter::CALIBRATION);
    CalibrationParameter &param = dynamic_cast<CalibrationParameter&>(*param_tmp);
    int exitValue = EXIT_OK;

    printVersion();

    if ( !param.parse( argc, argv ) )
    {
        param.print_usage();
        return EXIT_BADARGU;
    }

    print_sep();

    param.conParameter.setActiveDeviceName();

    ////////////////////////////////////////////////////////////////////////////////
    // update flow

    SiSTouchAdapter* adapter = SiSTouchAdapter::prepareAdapter(param.conParameter);

    if(adapter != 0)
    {
        int multiNum = SiSTouchAdapter::DEFALUT_SINGLE_NUM;

        multiNum = adapter->doDetectSlaveNum(param.conParameter.m_slaveNumber) + 1;

        int ret = 0;
        if(typeid(*adapter) == typeid(AegisMultiSiSTouchAdapter))
        {
            if(param.checkFlag)
            {
                for(int i = 0; i < multiNum; i++)
                {
                    if(i == 0)
                    {
                        printf("master:\n");
                    }
                    else
                    {
                        printf("slave %d:\n", (i-1));
                    }

                    ((AegisMultiSiSTouchAdapter*)adapter)->setSlaveAddr(AegisMultiSiSTouchAdapter::getStaticSlaveAddr(i));

                    ret = adapter->doCheckCalibrationFlag();
                    if(ret == SiSTouchAdapter::RESULT_SAME)
                    {
                        exitValue |= EXIT_OK;
                        printf("calibration flag has been set\n");
                    }
                    else if(ret == SiSTouchAdapter::RESULT_DIFFERENT)
                    {
                        exitValue = EXIT_FAIL;
                        printf("calibration flag has not been set yet\n");
                    }
                    else
                    {
                        exitValue = EXIT_ERR;
                        printf("some error occurs, please check the output. Err = %d.\n", ret);
                        break;
                    }
                }
            }
            else if(param.eraseFlag)
            {
                ret = ((AegisMultiSiSTouchAdapter*)adapter)->doEraseCalibrationFlag(multiNum);

                if(ret == SiSTouchAdapter::SUCCESS)
                {
                    exitValue = EXIT_OK;
                    printf("Erase calibration flag successfully.\n");
                }
                else
                {
                    exitValue = EXIT_ERR;
                    printf("some error occurs, please check the output. Err = %d.\n", ret);
                }
            }
            else
            {
                ret = adapter->doCalibraion(param.time, param.noquery);

                if(ret == SiSTouchAdapter::SUCCESS)
                {
                    exitValue = EXIT_OK;
                    printf("Calibration successfully.\n");
                }
                else
                {
                    exitValue = EXIT_ERR;
                    printf("some error occurs, please check the output. Err = %d.\n", ret);
                }
            }
        }
        else
        {
            if(typeid(*adapter) == typeid(AegisSlaveSiSTouchAdapter))
            {
                if(param.conParameter.slave_Addr < AegisMultiSiSTouchAdapter::SLAVE_DEVICE0_ADDR || param.conParameter.slave_Addr > AegisMultiSiSTouchAdapter::getStaticSlaveAddr(multiNum-1))
                {
                    printf("invalid slave addr \"0x%02x\" \n", param.conParameter.slave_Addr);

                    delete adapter;
                    return EXIT_ERR;
                }
                else
                {
                    ((AegisSlaveSiSTouchAdapter*)adapter)->setSlaveAddr(param.conParameter.slave_Addr);
                }
            }

            if(param.checkFlag)
            {
                ret = adapter->doCheckCalibrationFlag();

                if(ret == SiSTouchAdapter::RESULT_SAME)
                {
                    exitValue = EXIT_OK;
                    printf("calibration flag has been set\n");
                }
                else if(ret == SiSTouchAdapter::RESULT_DIFFERENT)
                {
                    exitValue = EXIT_FAIL;
                    printf("calibration flag has not been set yet\n");
                }
                else
                {
                    exitValue = EXIT_ERR;
                    printf("some error occurs, please check the output. Err = %d.\n", ret);
                }

            }
            else if(param.eraseFlag)
            {
                ret = adapter->doEraseCalibrationFlag();

                if(ret == SiSTouchAdapter::SUCCESS)
                {
                    exitValue = EXIT_OK;
                    printf("Erase calibration flag successfully.\n");
                }
                else
                {
                    exitValue = EXIT_ERR;
                    printf("some error occurs, please check the output. Err = %d.\n", ret);
                }
            }
            else
            {
                ret = adapter->doCalibraion(param.time, param.noquery);

                if(ret == SiSTouchAdapter::SUCCESS)
                {
                    exitValue = EXIT_OK;
                    printf("Calibration successfully.\n");
                }
                else
                {
                    exitValue = EXIT_ERR;
                    printf("some error occurs, please check the output. Err = %d.\n", ret);
                }
            }
        }
        delete adapter;
    }
    else
    {
        exitValue = EXIT_NODEV;
    }

    delete param_tmp;

    return exitValue;
}

void print_sep()
{
    printf( "-----\n" );
}

FILE* open_firmware_bin( const char* filename, int* size )
{
    printf( "opening firmware bin: %s\n", filename );

    FILE* input_file = fopen( filename, "r" );

    if ( !input_file )
    {
        printf( "ERROR: file not found\n" );
        return 0;
    }

    fseek( input_file, 0, SEEK_END );

    int file_size = ftell( input_file );
    *size = file_size;
    printf( "pattern file contains %i bytes\n", file_size );

    fseek( input_file, 0, SEEK_SET );
    return input_file;
}
