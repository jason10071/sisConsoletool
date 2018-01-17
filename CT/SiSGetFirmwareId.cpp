#include <iostream>

#include "ctexitcode/ctexitcode.h"
#include "sislog.h"
#include "ctexception/ctexception.h"
#include "spexception/spexception.h"
#include "getfirmwareidcore/getfirmwareidcore.h"

using namespace std;
using namespace CT;
using namespace CT::CTBP;
using namespace CT::BFP;
using namespace CT::GFIDP;
using namespace SiS;
using namespace SiS::Procedure;
using namespace SE;

int main(int argc, char *argv[])
{
    USING_EXIT_CODE;

    GetFirmwareIdParameter* getFirmwareIdParameter = new GetFirmwareIdParameter();
    PARSE_ARGS(getFirmwareIdParameter, argc, argv);
    DEBUG_SETTING_IF_HAS_ARG_DEBUG_LOG(getFirmwareIdParameter);
    SHOW_CTCORE_VERSION;
    SHOW_HELP_IF_HAS_ARG_HELP(getFirmwareIdParameter);
    PARSE_ARGUMENT(getFirmwareIdParameter);

    GetFirmwareIdCore getFirmwareIdCore(getFirmwareIdParameter);
    CTCORE_INIT(getFirmwareIdCore);
    CTCORE_EXEC(getFirmwareIdCore);
    CTCORE_DESTROY(getFirmwareIdCore);

    delete getFirmwareIdParameter;
    RETURN_EXIT_CODE;
}

