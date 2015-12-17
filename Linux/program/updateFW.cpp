#include "UpdateFWParameter.h"
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
    ApplicationParameter *param_tmp= ApplicationParameter::create(ApplicationParameter::UPDATE_FW);
    UpdateFWParameter &param = dynamic_cast<UpdateFWParameter &>(*param_tmp);
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
        multiNum = (!param.jump_check) ? (adapter->doDetectSlaveNum() + 1) : (param.conParameter.multi_Num);

        if(typeid(*adapter) == typeid(AegisMultiSiSTouchAdapter))
        {
            printf("multi\n");
            if (param.filenames.size()==1) {
                multiNum = 1;
            }

            if(!param.jump_check)
            {
                if(multiNum != param.conParameter.multi_Num)
                {
                    printf("input data count \"%d\" do not match chip count \"%d\"\n", param.conParameter.multi_Num, multiNum);
                    delete adapter;
                    return EXIT_ERR;
                }
            }

#define NUM_OF_FILES (7)
            FILE* multi_input_files[NUM_OF_FILES] = {0};
            int multi_files_size[NUM_OF_FILES] = {0};
#undef NUM_OF_FILES

            if(multiNum >= 1)
            {
                multi_input_files[0] = open_firmware_bin( param.filenames[0].c_str(), &multi_files_size[0] );

                if ( !multi_input_files[0] )
                {
                    return EXIT_ERR;
                }
            }

            for(int i = 1; i < multiNum; i++)
            {
                multi_input_files[i] = open_firmware_bin( param.filenames[i].c_str(), &(multi_files_size[i]) );
                if ( !multi_input_files[i] )
                {
                    for(int j=0;j<i;j++)
                    {
                        if(multi_input_files[j])
                        {
                            fclose(multi_input_files[j]);
                        }
                    }

                    return EXIT_ERR;
                }
            }

            int ret = (dynamic_cast<AegisMultiSiSTouchAdapter*>(adapter))->doUpdate( multi_input_files, multiNum, param);

            if(ret != SiSTouchAdapter::SUCCESS)
            {
                exitValue = EXIT_ERR;
                printf("some error occurs, please check the output. Err = %d.\n", ret);
            }
            else
            {
                exitValue = EXIT_OK;
                printf( "update firmwares finished.\n" );
            }

            for(int i = 0; i < multiNum; i++)
            {
                if(multi_input_files[i])
                {
                    fclose(multi_input_files[i]);
                }
            }
        }
        else
        {
            printf("single\n");
            if(param.conParameter.multi_Num != 1)
            {
                printf("input data count \"%d\" incorrect\n", param.conParameter.multi_Num);
                delete adapter;
                return EXIT_ERR;
            }

            if(typeid(*adapter) == typeid(AegisSlaveSiSTouchAdapter))
            {
                if(!param.jump_check)
                {
                    if(param.conParameter.slave_Addr < AegisMultiSiSTouchAdapter::SLAVE_DEVICE0_ADDR || param.conParameter.slave_Addr > AegisMultiSiSTouchAdapter::getStaticSlaveAddr(multiNum-1))
                    {
                        printf("invalid slave addr \"0x%02x\" \n", param.conParameter.slave_Addr);

                        delete adapter;
                        return EXIT_ERR;
                    }
                }
                ((AegisSlaveSiSTouchAdapter*)adapter)->setSlaveAddr(param.conParameter.slave_Addr);
            }

            int file_size;
            FILE* input_file = 0;

            input_file = open_firmware_bin( param.filenames[0].c_str(), &file_size );
            if ( !input_file )
            {
                delete adapter;
                return EXIT_ERR;
            }

            int ret = adapter->doUpdate( input_file, param.update_bootloader, param.update_bootloader_auto, param.reserve_RODATA, param.update_parameter, param.force_update, param.jump_check);

            if(ret != SiSTouchAdapter::SUCCESS)
            {
                exitValue = EXIT_ERR;
                printf("some error occurs, please check the output. Err = %d.\n", ret);
            }
            else
            {
                exitValue = EXIT_OK;
                printf( "update firmware finished.\n" );
            }

            fclose(input_file);
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
