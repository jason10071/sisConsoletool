#ifndef MULTIOPENSHORTRESULT_H
#define MULTIOPENSHORTRESULT_H

#define MULTI_CHIP_MASK 0x000000ff
#define MULTI_CHIP_MASK_LEN 8

#include "OpenShortResult.h"
#include "MultiOpenShortData.h"
#include "OpenShortConfig.h"


class MultiOpenShortResult
{
private:
    int mChipNum;
    OpenShortResult* mResults;
    int* mChipOrderIndexs; 

    // MutualShortExtraCheckByTP, begin
    int mWidthTP;
    int mHeightTP;
    unsigned int * mPointTpResult;
    unsigned int * mLineTpResult;
    //int mPointTpNG;
    int mLineTpNG;
    // MutualShortExtraCheckByTP, end

    // MutualNearFarCheckByTP, begin
    float * mLineTpNearFarValue;
    unsigned int * mLineTpNearFarResult;
public:
    int lostSide;
    // MutualNearFarCheckByTP, end

protected:
    void queryMappingSingle(int &devIdx, int &x, int &y);

public:
    MultiOpenShortResult();
    MultiOpenShortResult(int chipNum);

    ~MultiOpenShortResult();


    void setChipNum(int chipNum);
    int getChipNum();

    bool setOrderInfo(OpenShortConfig* config);

    void setChipOrderIndex(int index, int orderIndex);
    int getChipOrderIndex(int index);

    OpenShortResult* getResult(int index);

    int getWidth();
    int getWidth(int index);
    int getHeight();
    int getHeight(int index);
    int getBtnChannel();
	int getSecondNG();
    int getPointNG();
    int getLineNG();
    // MutualShortExtraCheck, begin
    virtual float getCycleTxDiff(int x, int y);
    virtual unsigned int getTXPointShortResult(int x, int y);
    virtual float getCycleRxDiff(int x, int y);
    virtual unsigned int getRXPointShortResult(int x, int y);
    // MutualShortExtraCheck, end

    // MutualShortExtraCheckByTP, begin
    void init(MultiOpenShortData* deviceData); // do after init each single-chip (mResults)
    //void setPointTpNG(int ngNum);
    void setLineTpNG(int ngNum);
    //int getPointTpNG();
    int getLineTpNG();
    void setPointTpResult(int x, int y, unsigned int result);
    void setXLineTpResult(int x, unsigned int result);
    void setYLineTpResult(int y, unsigned int result);
    unsigned int getPointTpResult(int x, int y);
    unsigned int getXLineTpResult(int x);
    unsigned int getYLineTpResult(int y);
    // MutualShortExtraCheckByTP, end

    // MutualNearFarCheckByTP, begin
    void setXLineTpNearFarValue(int x, float result);
    void setYLineTpNearFarValue(int y, float result);
    float getXLineTpNearFarValue(int x);
    float getYLineTpNearFarValue(int y);
    void setXLineTpNearFarResult(int x, unsigned int result);
    void setYLineTpNearFarResult(int y, unsigned int result);
    unsigned int getXLineTpNearFarResult(int x);
    unsigned int getYLineTpNearFarResult(int y);
    // MutualNearFarCheckByTP, end

    unsigned int getPointResult(int x, int y);
    void setPointResult(int x, int y, unsigned int result);
    void appendPointResult(int x, int y, unsigned int result);
    int getXChipCount();
    int getYChipCount();
    unsigned int getXLineResult(int x);
    unsigned int getYLineResult(int y);
    unsigned int getYLineResult(int devIdx, int y);

    void dumpResult();

	static void fprintMultiData(FILE *fp2D, MultiOpenShortData* data, MultiOpenShortResult *result, OpenShortConfig *config, int tableID);

    static bool saveMultiOpenShortResult(FILE *file, MultiOpenShortData* multiData, MultiOpenShortResult *multiResult, OpenShortConfig* config);
private:
    // copy constructor
    MultiOpenShortResult(const MultiOpenShortResult & rhs):
        mChipNum(0)
        ,mResults(0)
        ,mChipOrderIndexs(0) 
    {}
    // assignment operator
    MultiOpenShortResult & operator = (const MultiOpenShortResult & rhs) { return *this; }
};

#endif
