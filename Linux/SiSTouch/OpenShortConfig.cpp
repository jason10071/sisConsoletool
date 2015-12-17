#include "OpenShortConfig.h"
#include "config.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


OpenShortConfig::OpenShortConfig() :
    m_chipNum(1),
    mBaseVoltage(1)
    ,pointConfig(PointOpenShortConfig())
    ,secondConfig(PointSecondConfig())
    ,lineConfig(LineOpenShortConfig())
    ,orderConfig(OrderOpenShortConfig())
    ,lineExtraConfig(LineExtraOpenShortConfig())
    ,lineNearFarConfig(LineNearFarOpenShortConfig())
{
    pointConfig.targetMutl = 50;
    pointConfig.negTargetMutl = -50;
    pointConfig.mutl = 30;
    pointConfig.negMutl = -30;
    pointConfig.calib = 50;
    pointConfig.negCalib = -50;

    lineConfig.outOfSpecNode = 70;
    lineConfig.minLoop = 0;
    lineConfig.maxLoop = 10;
    lineConfig.calibTime = 40;
    lineConfig.voltage = 40;

	secondConfig.enableSecondThreshold = 0;
	secondConfig.goldenNegCalib = 0;
	secondConfig.goldenNegVol = 0;
	secondConfig.goldenPosCalib = 0;
	secondConfig.goldenPosVol = 0;
	secondConfig.maxAllowNg = 0;
	secondConfig.maxContinueNg = 0;
	secondConfig.targetNegVol = 0;
	secondConfig.targetPosVol = 0;

    orderConfig.InvXorder = 0;
	orderConfig.InvYorder = 0;

    orderConfig.reArrange = false;

    orderConfig.arrangeIndexs[0] = OrderOpenShortConfig::INDEX_MASTER;
    orderConfig.arrangeIndexs[1] = OrderOpenShortConfig::INDEX_SLAVE0;
    orderConfig.arrangeIndexs[2] = OrderOpenShortConfig::INDEX_SLAVE1;
    orderConfig.arrangeIndexs[3] = OrderOpenShortConfig::INDEX_SLAVE2;
    orderConfig.arrangeIndexs[4] = OrderOpenShortConfig::INDEX_SLAVE3;
    orderConfig.arrangeIndexs[5] = OrderOpenShortConfig::INDEX_SLAVE4;
    orderConfig.arrangeIndexs[6] = OrderOpenShortConfig::INDEX_SLAVE5;

	mBaseVoltage = 1;
}


bool
OpenShortConfig::loadOpenShortConfig(char *filename, OpenShortConfig* config, bool isMulti)
{
    configFile* cfgFile = readConfigFile(filename);
    if(cfgFile)
    {
        printf("loading config file\n");

        char * str;

        str = getConfigString(cfgFile, (char *)"MutThreshold", (char *)"NegTargetMutl");
        if(str)
        {
            config->pointConfig.negTargetMutl = atof(str);
        }

        str = getConfigString(cfgFile, (char *)"MutThreshold", (char *)"TargetMutl");
        if(str)
        {
            config->pointConfig.targetMutl = atof(str);
        }

        str = getConfigString(cfgFile, (char *)"MutThreshold", (char *)"NegCalib");
        if(str)
        {
            config->pointConfig.negCalib = atof(str);
        }

        str = getConfigString(cfgFile, (char *)"MutThreshold", (char *)"NegMutl");
        if(str)
        {
            config->pointConfig.negMutl = atof(str);
        }

        str = getConfigString(cfgFile, (char *)"MutThreshold", (char *)"Mutl");
        if(str)
        {
            config->pointConfig.mutl = atof(str);
        }

        str = getConfigString(cfgFile, (char *)"MutThreshold", (char *)"Calib");
        if(str)
        {
            config->pointConfig.calib = atof(str);
        }

        str = getConfigString(cfgFile, (char *)"MutualShortThreshold", (char *)"threshold.outofspecnode");
        if(str)
        {
            config->lineConfig.outOfSpecNode = atof(str);
        }

        str = getConfigString(cfgFile, (char *)"MutualShortThreshold", (char *)"threshold.min_loop");
        if(str)
        {
            config->lineConfig.minLoop = atof(str);
        }

        str = getConfigString(cfgFile, (char *)"MutualShortThreshold", (char *)"threshold.calibtime");
        if(str)
        {
            config->lineConfig.calibTime = atof(str);
        }

        str = getConfigString(cfgFile, (char *)"MutualShortThreshold", (char *)"threshold.voltage");
        if(str)
        {
            config->lineConfig.voltage = atof(str);
        }

        str = getConfigString(cfgFile, (char *)"MutualShortThreshold", (char *)"threshold.max_loop");
        if(str)
        {
            config->lineConfig.maxLoop = atof(str);
        }

        str = getConfigString(cfgFile, (char *)"Arrangement", (char *)"ReArrange");
        if(str)
        {
            config->orderConfig.reArrange = atoi(str);
        }

        str = getConfigString(cfgFile, (char *)"Arrangement", (char *)"Arrange0");
        if(str)
        {
            config->orderConfig.arrangeIndexs[0] = atoi(str);
        }

        str = getConfigString(cfgFile, (char *)"Arrangement", (char *)"Arrange1");
        if(str)
        {
            config->orderConfig.arrangeIndexs[1] = atoi(str);
        }

        str = getConfigString(cfgFile, (char *)"Arrangement", (char *)"Arrange2");
        if(str)
        {
            config->orderConfig.arrangeIndexs[2] = atoi(str);
        }

        str = getConfigString(cfgFile, (char *)"Arrangement", (char *)"Arrange3");
        if(str)
        {
            config->orderConfig.arrangeIndexs[3] = atoi(str);
        }

        str = getConfigString(cfgFile, (char *)"Arrangement", (char *)"Arrange4");
        if(str)
        {
            config->orderConfig.arrangeIndexs[4] = atoi(str);
        }

        str = getConfigString(cfgFile, (char *)"Arrangement", (char *)"Arrange5");
        if(str)
        {
            config->orderConfig.arrangeIndexs[5] = atoi(str);
        }

        str = getConfigString(cfgFile, (char *)"Arrangement", (char *)"Arrange6");
        if(str)
        {
            config->orderConfig.arrangeIndexs[6] = atoi(str);
        }

		str = getConfigString(cfgFile, (char *)"MutSecondThreshold", (char *)"enableSecondThreshold");
        if(str)
        {
            config->secondConfig.enableSecondThreshold = atoi(str);
        }

		str = getConfigString(cfgFile, (char *)"MutSecondThreshold", (char *)"maxAllowNg");
        if(str)
        {
            config->secondConfig.maxAllowNg = atoi(str);
        }

		str = getConfigString(cfgFile, (char *)"MutSecondThreshold", (char *)"maxContinueNg");
        if(str)
        {
            config->secondConfig.maxContinueNg = atoi(str);
        }

		str = getConfigString(cfgFile, (char *)"MutSecondThreshold", (char *)"targetPosVol");
        if(str)
        {
            config->secondConfig.targetPosVol = atoi(str);
        }

		str = getConfigString(cfgFile, (char *)"MutSecondThreshold", (char *)"targetNegVol");
        if(str)
        {
            config->secondConfig.targetNegVol = atoi(str);
        }

		str = getConfigString(cfgFile, (char *)"MutSecondThreshold", (char *)"goldenPosCalib");
        if(str)
        {
            config->secondConfig.goldenPosCalib = atoi(str);
        }

		str = getConfigString(cfgFile, (char *)"MutSecondThreshold", (char *)"goldenNegCalib");
        if(str)
        {
            config->secondConfig.goldenNegCalib = atoi(str);
        }

		str = getConfigString(cfgFile, (char *)"MutSecondThreshold", (char *)"goldenPosVol");
        if(str)
        {
            config->secondConfig.goldenPosVol = atoi(str);
        }

		str = getConfigString(cfgFile, (char *)"MutSecondThreshold", (char *)"goldenNegVol");
        if(str)
        {
            config->secondConfig.goldenNegVol= atoi(str);
        }

		str = getConfigString(cfgFile, (char *)"General", (char *)"BaseVoltage");
        if(str)
        {
            config->mBaseVoltage = atoi(str);
        }

		str = getConfigString(cfgFile, (char *)"General", (char *)"Inv_X_order");
        if(str)
        {
            config->orderConfig.InvXorder = atoi(str);
        }

		str = getConfigString(cfgFile, (char *)"General", (char *)"Inv_Y_order");
        if(str)
        {
            config->orderConfig.InvYorder= atoi(str);
        }

        // MutualShortExtraCheck
        str = getConfigString(cfgFile, (char *)"MutualShortExtraCheck", (char *)"enable_TX_extra_check");
        if(str)
        {
            // Multi: x/y switch
            if(isMulti == false)
            {
                config->lineExtraConfig.enableTXExtraCheck = atof(str);
            } else
            {
                config->lineExtraConfig.enableRXExtraCheck = atof(str);
            }
        }
        str = getConfigString(cfgFile, (char *)"MutualShortExtraCheck", (char *)"enable_RX_extra_check");
        if(str)
        {
            // Multi: x/y switch
            if(isMulti == false)
            {
                config->lineExtraConfig.enableRXExtraCheck = atof(str);
            } else
            {
                config->lineExtraConfig.enableTXExtraCheck = atof(str);
            }
        }
        str = getConfigString(cfgFile, (char *)"MutualShortExtraCheck", (char *)"enable_auto_ng_check");
        if(str)
        {
            config->lineExtraConfig.enableAutoNgCheck = atof(str);
        }
        str = getConfigString(cfgFile, (char *)"MutualShortExtraCheck", (char *)"MutualShort_extra_check_by_TP");
        if(str)
        {
            config->lineExtraConfig.MutualShortExtraCheckByTP = atof(str);
        }
        str = getConfigString(cfgFile, (char *)"MutualShortExtraCheck", (char *)"TX_cycle_diff_high_percent");
        if(str)
        {
            // Multi: x/y switch
            if(isMulti == false)
            {
                config->lineExtraConfig.TXCycleDiffHighPercent = atof(str);
            } else
            {
                config->lineExtraConfig.RXCycleDiffHighPercent = atof(str);
            }
        }
        str = getConfigString(cfgFile, (char *)"MutualShortExtraCheck", (char *)"TX_cycle_diff_low_percent");
        if(str)
        {
            // Multi: x/y switch
            if(isMulti == false)
            {
                config->lineExtraConfig.TXCycleDiffLowPercent = atof(str);
            } else
            {
                config->lineExtraConfig.RXCycleDiffLowPercent = atof(str);
            }
        }
        str = getConfigString(cfgFile, (char *)"MutualShortExtraCheck", (char *)"RX_cycle_diff_high_percent");
        if(str)
        {
            // Multi: x/y switch
            if(isMulti == false)
            {
                config->lineExtraConfig.RXCycleDiffHighPercent = atof(str);
            } else
            {
                config->lineExtraConfig.TXCycleDiffHighPercent = atof(str);
            }
        }
        str = getConfigString(cfgFile, (char *)"MutualShortExtraCheck", (char *)"RX_cycle_diff_low_percent");
        if(str)
        {
            // Multi: x/y switch
            if(isMulti == false)
            {
                config->lineExtraConfig.RXCycleDiffLowPercent = atof(str);
            } else
            {
                config->lineExtraConfig.TXCycleDiffLowPercent = atof(str);
            }
        }
        str = getConfigString(cfgFile, (char *)"MutualShortExtraCheck", (char *)"cycle_out_of_spec_node_percent");
        if(str)
        {
            config->lineExtraConfig.cycleOutOfSpecNodePercent = atof(str);
        }
        str = getConfigString(cfgFile, (char *)"MutualShortExtraCheck", (char *)"detect_short_only");
        if(str)
        {
            config->lineExtraConfig.detectShortOnly = atof(str);
        }

        // MutualNearFarCheck
        str = getConfigString(cfgFile, (char *)"MutualNearFarCheck", (char *)"enable_COB_connector_y_type");
        if(str)
        {
            config->lineNearFarConfig.enableCOBConnectorYType = atof(str);
        }
        str = getConfigString(cfgFile, (char *)"MutualNearFarCheck", (char *)"enable_auto_near_far_check");
        if(str)
        {
            config->lineNearFarConfig.enableAutoNearFarCheck = atof(str);
        }
        str = getConfigString(cfgFile, (char *)"MutualNearFarCheck", (char *)"cycle_difference_percent_tx");
        if(str)
        {
            config->lineNearFarConfig.cycleDifferencePercentTx = atof(str);
        }
        str = getConfigString(cfgFile, (char *)"MutualNearFarCheck", (char *)"cycle_difference_percent_rx");
        if(str)
        {
            config->lineNearFarConfig.cycleDifferencePercentRx = atof(str);
        }
        str = getConfigString(cfgFile, (char *)"MutualNearFarCheck", (char *)"cycle_out_of_channels");
        if(str)
        {
            config->lineNearFarConfig.cycleOutOfChannels = atof(str);
        }
        str = getConfigString(cfgFile, (char *)"MutualNearFarCheck", (char *)"cycle_avg_count_number_tx");
        if(str)
        {
            config->lineNearFarConfig.cycleAvgCountNumberTx = atof(str);
        }
        str = getConfigString(cfgFile, (char *)"MutualNearFarCheck", (char *)"cycle_avg_count_number_rx");
        if(str)
        {
            config->lineNearFarConfig.cycleAvgCountNumberRx = atof(str);
        }


        unloadConfigFile(cfgFile);

        return true;
    }
    else
    {
        return false;
    }
}


