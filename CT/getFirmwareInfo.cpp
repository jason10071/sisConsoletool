#include <iostream>

#include "ctexitcode/ctexitcode.h"
#include "sislog.h"
#include "ctexception/ctexception.h"
#include "spexception/spexception.h"
#include "getfirmwareinfocore/getfirmwareinfocore.h"

using namespace std;
using namespace CT;
using namespace CT::CTBP;
using namespace CT::GFINFOP;
using namespace SiS;
using namespace SiS::Procedure;
using namespace SE;

int main(int argc, char *argv[])
{
    USING_EXIT_CODE;

    GetFirmwareInfoParameter* getFirmwareInfoParameter = new GetFirmwareInfoParameter();
    PARSE_ARGS(getFirmwareInfoParameter, argc, argv);
    DEBUG_SETTING_IF_HAS_ARG_DEBUG_LOG(getFirmwareInfoParameter);
    SHOW_CTCORE_VERSION;
    SHOW_HELP_IF_HAS_ARG_HELP(getFirmwareInfoParameter);
    PARSE_ARGUMENT(getFirmwareInfoParameter);

    GetFirmwareInfoCore getFirmwareInfoCore(getFirmwareInfoParameter);
    CTCORE_INIT(getFirmwareInfoCore);
    CTCORE_EXEC(getFirmwareInfoCore);
    CTCORE_DESTROY(getFirmwareInfoCore, true);

    delete getFirmwareInfoParameter;
    RETURN_EXIT_CODE;
}

