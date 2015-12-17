#ifndef OPENSHORTCONFIG_H
#define OPENSHORTCONFIG_H


class PointOpenShortConfig
{
    public:
        float targetMutl;
        float negTargetMutl;
        float mutl;
        float negMutl;
        float calib;
        float negCalib;
    public:
        PointOpenShortConfig() :
            targetMutl(50.0f)
            ,negTargetMutl(-50.0f)
            ,mutl(30.0f)
            ,negMutl(-30.0f)
            ,calib(50.0f)
            ,negCalib(-50.0f)
    {}
};

class PointSecondConfig
{
    public:
        int enableSecondThreshold;
        int maxAllowNg;
        int maxContinueNg;
        int targetPosVol;
        int targetNegVol;
        int goldenPosCalib;
        int goldenNegCalib;
        int goldenPosVol;
        int goldenNegVol;
    public:
        PointSecondConfig() :
            enableSecondThreshold(0)
            ,maxAllowNg(0)
            ,maxContinueNg(0)
            ,targetPosVol(0)
            ,targetNegVol(0)
            ,goldenPosCalib(0)
            ,goldenNegCalib(0)
            ,goldenPosVol(0)
            ,goldenNegVol(0)
    {}
};


class LineOpenShortConfig
{
    public:
        float outOfSpecNode;
        float minLoop;
        float maxLoop;
        float calibTime;
        float voltage;
    public:
        LineOpenShortConfig () :
            outOfSpecNode(70)
            ,minLoop(0)
            ,maxLoop(10)
            ,calibTime(40)
            ,voltage(40)
    {}
};

class OrderOpenShortConfig
{
    public:
        bool reArrange;

        int InvXorder;
        int InvYorder;

        int arrangeIndexs[7];

        enum chipIdx
        {
            INDEX_MASTER = 0,
            INDEX_SLAVE0 = 1,
            INDEX_SLAVE1 = 2,
            INDEX_SLAVE2 = 3,
            INDEX_SLAVE3 = 4,
            INDEX_SLAVE4 = 5,
            INDEX_SLAVE5 = 6,
            MAX_CHIP = 7,
        };
    public:
        OrderOpenShortConfig() :
         reArrange(false)
        ,InvXorder(0)
        ,InvYorder(0)
        {}
};

// MutualShortExtraCheck
class LineExtraOpenShortConfig
{
    public:
        float enableTXExtraCheck;
        float enableRXExtraCheck;
        float enableAutoNgCheck;
        float MutualShortExtraCheckByTP;
        float TXCycleDiffHighPercent;
        float TXCycleDiffLowPercent;
        float RXCycleDiffHighPercent;
        float RXCycleDiffLowPercent;
        float cycleOutOfSpecNodePercent;
        float detectShortOnly;
    public:
        LineExtraOpenShortConfig () :
            enableTXExtraCheck(0)
            ,enableRXExtraCheck(0)
            ,enableAutoNgCheck(0)
            ,MutualShortExtraCheckByTP(0)
            ,TXCycleDiffHighPercent(0)
            ,TXCycleDiffLowPercent(0)
            ,RXCycleDiffHighPercent(0)
            ,RXCycleDiffLowPercent(0)
            ,cycleOutOfSpecNodePercent(0)
            ,detectShortOnly(0)
    {}
};

// MutualNearFarCheck
class LineNearFarOpenShortConfig
{
    public:
        float enableCOBConnectorYType;
        float enableAutoNearFarCheck;
        float cycleDifferencePercentTx;
        float cycleDifferencePercentRx;
        float cycleOutOfChannels;
        float cycleAvgCountNumberTx;
        float cycleAvgCountNumberRx;
    public:
        LineNearFarOpenShortConfig () :
            enableCOBConnectorYType(0)
            ,enableAutoNearFarCheck(0)
            ,cycleDifferencePercentTx(0)
            ,cycleDifferencePercentRx(0)
            ,cycleOutOfChannels(0)
            ,cycleAvgCountNumberTx(0)
            ,cycleAvgCountNumberRx(0)
    {}
};

class OpenShortConfig
{
public:
	int mBaseVoltage;
    PointOpenShortConfig pointConfig;
	PointSecondConfig secondConfig;
    LineOpenShortConfig lineConfig;
    OrderOpenShortConfig orderConfig;

    LineExtraOpenShortConfig lineExtraConfig;
    LineNearFarOpenShortConfig lineNearFarConfig;

    OpenShortConfig();
    static bool loadOpenShortConfig(char *filename, OpenShortConfig* config, bool isMuti);

public:
    int m_chipNum;

};



#endif
