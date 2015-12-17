#include "GetFirmwareIdParameter.h"
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
    ApplicationParameter *param_tmp= ApplicationParameter::create(ApplicationParameter::GET_FW_ID);
    GetFirmwareIdParameter &param = dynamic_cast<GetFirmwareIdParameter &>(*param_tmp);
    int exitValue = EXIT_OK;

    printVersion();

    if ( !param.parse( argc, argv ) )
    {
        param.print_usage();
        return EXIT_BADARGU;
    }

    print_sep();

    param.conParameter.setActiveDeviceName();

    SiSTouchAdapter* adapter = SiSTouchAdapter::prepareAdapter(param.conParameter);

    if(adapter != 0)
    {
        int multiNum = SiSTouchAdapter::DEFALUT_SINGLE_NUM;

        multiNum = adapter->doDetectSlaveNum() + 1;

        if (multiNum==SiSTouchAdapter::DEFALUT_SINGLE_NUM) {
            printf("isMulti=0, SlaveNum=0\n");
        }
        else {
            printf("isMulti=1, SlaveNum=%d\n", multiNum-1);
        }

        if(typeid(*adapter) == typeid(AegisMultiSiSTouchAdapter))
        {
            int ret = 0;

            if(param.fromFile)
            {
                if(multiNum != param.conParameter.multi_Num)
                {
                    printf("input data count \"%d\" do not match chip count \"%d\"\n", param.conParameter.multi_Num, multiNum);
                    delete adapter;
                    return EXIT_ERR;
                }
                else
                {
                    FILE* multi_input_files[MAX_SLAVE_NUM] = {0};
                    int multi_files_size[MAX_SLAVE_NUM] = {0};

                    if(multiNum >= 1)
                    {
                        multi_input_files[0] = open_firmware_bin( param.filename, &multi_files_size[0] );

                        if ( !multi_input_files[0] )
                        {
                            delete adapter;
                            return EXIT_ERR;
                        }
                    }

                    for(int i = 1; i < multiNum; i++)
                    {
                        multi_input_files[i] = open_firmware_bin( param.slave_filenames[i-1], &(multi_files_size[i]) );
                        if ( !multi_input_files[i] )
                        {
                            for(int j=0;j<i;j++)
                            {
                                if(multi_input_files[j])
                                {
                                    fclose(multi_input_files[j]);
                                }
                            }
                            delete adapter;
                            return EXIT_ERR;
                        }
                    }

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
                        ret = adapter->doCompareId(multi_input_files[i]);

                        if(ret == SiSTouchAdapter::RESULT_SAME)
                        {
                            exitValue |= EXIT_OK;
                            printf("the firmware id is the same\n");
                        }
                        else if(ret == SiSTouchAdapter::RESULT_DIFFERENT)
                        {
                            exitValue = EXIT_FAIL;
                            printf("the firmware id is different\n");
                        }
                        else
                        {
                            exitValue = EXIT_ERR;
                            printf("some error occurs, please check the output. Err = %d.\n", ret);
                            break;
                        }
                    }
                    for(int i = 0; i < multiNum; i++)
                    {
                        if(multi_input_files[i])
                        {
                            fclose(multi_input_files[i]);
                        }
                    }
                }

            }
            else if(param.fromInput)
            {
                if(multiNum != param.conParameter.multi_Num)
                {
                    printf("input data count \"%d\" do not match chip count \"%d\"\n", param.conParameter.multi_Num, multiNum);
                    delete adapter;
                    return EXIT_ERR;
                }
                else
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
                        if(i == 0)
                        {
                            ret = adapter->doCompareId(param.input);
                        }
                        else
                        {
                            ret = adapter->doCompareId(param.slave_input[i-1]);
                        }

                        if(ret == SiSTouchAdapter::RESULT_SAME)
                        {
                            exitValue |= EXIT_OK;
                            printf("the firmware id is the same\n");
                        }
                        else if(ret == SiSTouchAdapter::RESULT_DIFFERENT)
                        {
                            exitValue = EXIT_FAIL;
                            printf("the firmware id is different\n");
                        }
                        else
                        {
                            exitValue = EXIT_ERR;
                            printf("some error occurs, please check the output. Err = %d.\n", ret);
                            break;
                        }
                    }
                }
            }
            else
            {
                exitValue = EXIT_OK;
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
                    ret = adapter->showDeviceId();
                    if(ret != SiSTouchAdapter::SUCCESS)
                    {
                        exitValue = EXIT_ERR;
                        printf("some error occurs, please check the output. Err = %d.\n", ret);
                        break;
                    }
                }
            }
        }
        else
        {
            if(param.conParameter.multi_Num != 1)
            {
                printf("input data count \"%d\" incorrect\n", param.conParameter.multi_Num);
                delete adapter;
                return EXIT_ERR;
            }

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

            int ret = 0;

            if(param.fromFile)
            {
                int file_size;

                FILE* input_file = 0;

                input_file = open_firmware_bin( param.filename, &file_size );
                if ( !input_file )
                {
                    delete adapter;
                    return EXIT_ERR;
                }

                ret = adapter->doCompareId(input_file);

                if(ret == SiSTouchAdapter::RESULT_SAME)
                {
                    exitValue = EXIT_OK;
                    printf("the firmware id is the same\n");
                }
                else if(ret == SiSTouchAdapter::RESULT_DIFFERENT)
                {
                    exitValue = EXIT_FAIL;
                    printf("the firmware id is different\n");
                }
                else
                {
                    exitValue = EXIT_ERR;
                    printf("some error occurs, please check the output. Err = %d.\n", ret);
                }

                fclose(input_file);
            }
            else if(param.fromInput)
            {
                ret = adapter->doCompareId(param.input);
                if(ret == SiSTouchAdapter::RESULT_SAME)
                {
                    exitValue = EXIT_OK;
                    printf("the firmware id is the same\n");
                }
                else if(ret == SiSTouchAdapter::RESULT_DIFFERENT)
                {
                    exitValue = EXIT_FAIL;
                    printf("the firmware id is different\n");
                }
                else
                {
                    exitValue = EXIT_ERR;
                    printf("some error occurs, please check the output. Err = %d.\n", ret);
                }
            }
            else
            {
                ret = adapter->showDeviceId();
                if(ret != SiSTouchAdapter::SUCCESS)
                {
                    exitValue = EXIT_ERR;
                    printf("some error occurs, please check the output. Err = %d.\n", ret);
                }
                else
                {
                    exitValue = EXIT_OK;
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
    printf( "opening firmwave bin: %s\n", filename );

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
