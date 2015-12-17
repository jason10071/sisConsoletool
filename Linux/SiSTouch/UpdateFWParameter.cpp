#include "UpdateFWParameter.h"
#include <cstdio>
#include <cstdlib>

/*===========================================================================*/
enum MultiChipSelectiveID {
    ID_INVALID = 0xff,
    ID_NONE    = 0x0,
    ID_SINGLE  = 0x0,
    ID_MASTER  = 0x1,
    ID_SLAVE1  = 0x5,
    ID_SLAVE2  = 0x6,
    ID_SLAVE3  = 0x7,
    ID_SLAVE4  = 0x8,
    ID_BRIDGE  = 0xB,
    ID_DUMMY   = 0xD,
};
/*===========================================================================*/
static bool sortFwBinfileOrder(std::vector<std::string> & filenames)
{
    int bin_files_num = filenames.size();
    int slave_num = 0;
    int multichip_selective_id = 0;
    int num_master_cnt = 0;

    std::vector<int> chip_order(0);

    chip_order.clear();

    for (int idx = 0; idx < bin_files_num; idx++) {
        FILE *fp = NULL;

        if ((fp = fopen(filenames[idx].c_str(), "rb")) != NULL) {

#           define MULTICHIP_SELECTIVE_ID_ADDR (0xc027)
            if (0!=fseek(fp, MULTICHIP_SELECTIVE_ID_ADDR, SEEK_SET)) {
                printf( "file %s is not correct sis firmware file\n", filenames[idx].c_str() );
                return false;
            }
#           undef MULTICHIP_SELECTIVE_ID_ADDR
            multichip_selective_id = fgetc(fp);

            switch (multichip_selective_id) {
                case ID_INVALID:
                    printf( "%s: old firmware, cannot check multi-chip relation\n", filenames[idx].c_str() );
                    return false;
                    break;

                case ID_NONE:
                    printf( "%s: single chip firmware\n", filenames[idx].c_str() );
                    chip_order.push_back(multichip_selective_id);
                    break;

                case ID_MASTER:
                case ID_BRIDGE:
                    chip_order.push_back(0);
                    slave_num = fgetc(fp);
                    num_master_cnt++;
                    break;

                case 0x5:
                case 0x6:
                case 0x7:
                case 0x8:
                case 0x9:
                case 0xa:
                    chip_order.push_back(multichip_selective_id-4);
                    break;

                default:
                    printf( "%s: FW format error, slave id of FW = %d\n", filenames[idx].c_str(), multichip_selective_id);
                    return false;
                    break;
            }
        } else {
            fprintf(stderr, "can't open file ");
            perror(filenames[idx].c_str());
            exit(-1);
        }

        fclose(fp);
    }

    if (multichip_selective_id==ID_SINGLE) {
        return true;
    }

    // false if more than one master fw bin file
    if (num_master_cnt>1) {
        printf( "number of master FW = %d > 1\n", num_master_cnt);
        return false;
    }

    // only update master

    if ((num_master_cnt==1)&&(chip_order.size()==1)) {
        return true;
    }


    std::vector<std::string> tmp_filename(filenames);

    if ((chip_order.size()!=0)&&(slave_num==0)) {
        printf( "number of FW file doesn't not match\n");
        return false;
    }

    if (slave_num!=(bin_files_num-1)) {
	    printf( "number of FW file doesn't not match\n");
	    return false;
    }
    // sort chip order to Master->Slave0->...->SlaveN
    for (int idx = 0; idx < bin_files_num; idx++) {
        filenames[chip_order[idx]] = tmp_filename[idx];
    }

    return true;
}
/*===========================================================================*/
static std::vector<std::string> filenames_ini(0);
UpdateFWParameter::UpdateFWParameter() :
    filenames(filenames_ini),
    update_bootloader(false),
    update_bootloader_auto(false),
    reserve_RODATA(false),
    update_parameter(false),
    force_update(false),
    jump_check(false),
    wait_time(10)
{
    this->filenames.clear();
}
/*===========================================================================*/
UpdateFWParameter::~UpdateFWParameter()
{

}
/*===========================================================================*/
int UpdateFWParameter::parse( int argc, char** argv )
{
    if ( argc <= 1 )
    {
        return 0;
    }

    printf( "parsing command parameter\n" );

    for( int i = 1; i < argc; i++ )
    {
        printf( "%d: [%s] \n", i, argv[i] );

        if(!parseArgument(argv[i]))
        {
            printf( "unknown parameter: %s\n", argv[i] );
            return 0;
        }
    }

    return check();
}
/*===========================================================================*/
bool UpdateFWParameter::parseArgument(char *arg)
{

    if(conParameter.parseArgument(arg))
    {
        return true;
    }

    if ( strcmp( arg, "-b" ) == 0 )
    {
        update_bootloader = true;
        printf( "update bootloader\n");
    }
    else if ( strcmp( arg, "-ba" ) == 0 )
    {
        update_bootloader_auto = true;
        printf( "update bootloader automatically\n");
    }
    else if ( strcmp( arg, "-g" ) == 0 )
    {
        reserve_RODATA = true;
        printf( "reserve RO data\n");
    }
    else if ( strcmp( arg, "-r" ) == 0 )
    {
        update_parameter = true;
        printf( "only update parameter\n");
    }
    else if ( strcmp( arg, "--force" ) == 0 )
    {
        force_update = true;
        printf( "force update\n");
    }
    else if ( strcmp( arg, "--jump" ) == 0 )
    {
        jump_check = true;
        printf( "jump parameter validation\n");
    }
    else if ( strstr( arg, "-w=" ) == arg )
    {
        wait_time = atoi(arg + 3);
        printf( "wait time set: %d\n", wait_time);
    }
    else
    {
        filenames.push_back(arg);
    }

    return true;
}
/*===========================================================================*/
int UpdateFWParameter::check()
{
    if(update_parameter && update_bootloader) {
        printf( "-r and -b is conflict\n" );
        return 0;
    }

    if(update_parameter && reserve_RODATA) {
        printf( "-r and -g is conflict\n" );
        return 0;
    }

    if(force_update && jump_check) {
        printf( "--force and --jump is conflict\n" );
        return 0;
    }

    if(conParameter.check() == 0) {
        return 0;
    }

    int bin_files_num = this->filenames.size();

    if (bin_files_num==0) {
        printf( "no fw bin file specified\n" );
        return 0;
    }

    if (sortFwBinfileOrder(filenames)==false) {
        exit(-1);
    }


    this->conParameter.multi_Num = filenames.size();

    return 1;
}
/*===========================================================================*/
void UpdateFWParameter::print_usage()
{
    printf( "\n" );
    printf( "Usage: updateFW [-i=CONNECT] [-n=DEVICE_NAME] [-w=WAIT_TIME] [--slave=SLAVEADDR] [-t=TIMEOUT] [-d=DELAY] [-io-delay=IO_DELAY] [-l] [-b] [-ba] [-g] [-r] [--force] [--jump] [--no-diag] [--no-power]\n" );
    printf( "Update firmware from specified file.\n" );
    printf( "    CONNECT:     'auto'|'zeus_i2c'|'zeus_usbbridge'|'zeus_internalusb'|'aegis_i2c'|'aegis_usbbridge'|'aegis_internalusb'|'aegis_slave'|'aegis_multi', default 'auto'\n" );
    printf( "    DEVICE_NAME: the device name to be operate, ex. sis_aegis_hid_bridge_touch_device\n");
    printf( "    WAIT_TIME:   when FW is broken, update master first and wait for WAIT_TIME secs.\n");
    printf( "    SLAVEADDR:   specify slave address used in aegis_slave, default is 5\n" );
    printf( "    TIMEOUT:     integer, retry times for NACK, default 100\n" );
    printf( "    DELAY:       integer, delay time between every each query retry in millisecond, default 1\n" );
    printf( "    IO_DELAY:    integer, delay time after each query in millisecond, default 0\n" );
    printf( "    -l:          verbose mode\n" );
    printf( "    -b:          update bootloader\n" );
    printf( "    -ba:         update bootloader automatically\n" );
    printf( "    -g:          reserve RO data\n" );
    printf( "    -r:          only update parameter\n" );
    printf( "    --force:          force update\n" );
    printf( "    --jump:          jump parameter validation\n" );
    printf( "    --no-diag:   disable diagnosis mode changing automatically *** only for experiment, don't use in normal flow ***\n" );
    printf( "    --no-power:  disable power mode changing automatically *** only for experiment, don't use in normal flow ***\n" );
    printf( "    ps. don't use space around '='\n" );
    printf( "    \n" );
    printf( "        -r and -b is conflict\n" );
    printf( "        -r and -g is conflict\n" );
    printf( "        --force and --jump is conflict\n" );
    printf( "    'aegis_slave' won't be detected in aegis_auto, only can be specified manually\n" );
}
/*===========================================================================*/
