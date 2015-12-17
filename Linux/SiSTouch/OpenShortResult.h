#ifndef OPENSHORTRESULT_H
#define OPENSHORTRESULT_H

#include <stdio.h>
#include "OpenShortData.h"
#include "OpenShortConfig.h"

#define POINT_PASS 0x00000000
#define POINT_SHORT 0x00000001
#define POINT_OVEROPEN_VOLTAGE_ON_SAMPLE 0x00000002
#define POINT_OPEN_VOLTAGE_ON_SAMPLE 0x00000004
#define POINT_OVEROPEN_TIME_ON_SAMPLE 0x00000008
#define POINT_OPEN_TIME_ON_SAMPLE 0x00000010
#define POINT_OVEROPEN_VOLTAGE_ON_TARGET 0x00000020
#define POINT_OPEN_VOLTAGE_ON_TARGET 0x00000040

#define LINE_PASS 0x00000000
#define LINE_SHORT 0x00000001
#define LINE_SHORT_LOOP 0x00000002
#define LINE_EXTREME_DIF 0x00000003
#define LINE_NEARFAR_SHORT 0x00000004

#define LOST_NONE  0x0
#define LOST_LEFT 0x1
#define LOST_RIGHT 0x2
#define LOST_TOP 0x4
#define LOST_DOWN 0x8


class OpenShortResult
{
private:
    int mWidth;
    int mHeight;
    unsigned int * mPointReulst;
    unsigned int * mLineResult;
    int mPointNG;
	int mSecondNG;
    int mLineNG;

    // MutualShortExtraCheck, begin
    float * mCycleTxDiff;
    unsigned int * mTXPointShortResult;
    float * mCycleRxDiff;
    unsigned int * mRXPointShortResult;
    // MutualShortExtraCheck, end

    // MutualNearFarCheck, begin
    float * mLineNearFarValue;
    unsigned int * mLineNearFarResult;
public:
    int lostSide;
    // MutualNearFarCheck, end


public:
    OpenShortResult();
    OpenShortResult(int w, int h);

    virtual ~OpenShortResult();

    // MutualShortExtraCheck, begin
    virtual void setCycleTxDiff(int x, int y, float result);
    virtual void setTXPointShortResult(int x, int y, unsigned int result);
    virtual void setCycleRxDiff(int x, int y, float result);
    virtual void setRXPointShortResult(int x, int y, unsigned int result);
    virtual float getCycleTxDiff(int x, int y);
    virtual unsigned int getTXPointShortResult(int x, int y);
    virtual float getCycleRxDiff(int x, int y);
    virtual unsigned int getRXPointShortResult(int x, int y);
    // MutualShortExtraCheck, end

    // MutualNearFarCheck, begin
    void setXLineNearFarValue(int x, float result);
    void setYLineNearFarValue(int y, float result);
    float getXLineNearFarValue(int x);
    float getYLineNearFarValue(int y);
    void setXLineNearFarResult(int x, unsigned int result);
    void setYLineNearFarResult(int y, unsigned int result);
    unsigned int getXLineNearFarResult(int x);
    unsigned int getYLineNearFarResult(int y);
    // MutualNearFarCheck, end

    void setWH(int w, int h);

    virtual void setPointResult(int x, int y, unsigned int result);

    virtual void setXLineResult(int x, unsigned int result);

    virtual void setYLineResult(int y, unsigned int result);

    virtual void appendPointResult(int x, int y, unsigned int result);

    virtual void appendXLineResult(int x, unsigned int result);

    virtual void appendYLineResult(int y, unsigned int result);

	void setSecondNG(int secondNgNum);
    void setPointNG(int ngNum);
    void setLineNG(int ngNum);


    int getWidth();

    int getHeight();

    virtual unsigned int getPointResult(int x, int y);

    virtual unsigned int getXLineResult(int x);

    virtual unsigned int getYLineResult(int y);

	int getSecondNG();
    int getPointNG();
    int getLineNG();

    virtual void dumpResult();

	static void fprintData(FILE *fp2D, OpenShortData* data, OpenShortResult *result, OpenShortConfig *config, int tableID);
	
    static int transferPointResultToWindowsResult(unsigned int result);
    static void transferPointResultToErrorReason(unsigned int result, char* buf);
	static void transferPointResultToErrorReasonSimple(unsigned int result, char* buf);
    static bool saveOpenShortResult(FILE *file, OpenShortData* data, OpenShortResult *result, OpenShortConfig *config);

private:
        // copy constructor
        OpenShortResult(const OpenShortResult & rhs);
        // assignment operator
        OpenShortResult & operator = (const OpenShortResult & rhs);
};

#endif
