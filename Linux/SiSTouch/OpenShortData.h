#ifndef OPENSHORTDATA_H
#define OPENSHORTDATA_H

#include <stdio.h>


#define DEFAULT_W 54
#define DEFAULT_H 36

enum TableID
{
    LOOPID=1,
    CYCLEID,
    VoltageID,
    ResultID,
    CycleTxDiffID, // MutualShortExtraCheck
    CycleRxDiffID, // MutualShortExtraCheck
    TXPointShortResultID, // MutualShortExtraCheck
    RXPointShortResultID, // MutualShortExtraCheck
};


class OpenShortData
{
private:
    int mWidth;
    int mHeight;
    float mTargetVoltage;
    float * mBaseVoltage;
    float * mRawVoltage;
    float * mDiffVoltage;
    float * mCycle;
    float * mLoop;

    int mReadBaseVoltage;

	void fprintSampleData(FILE *fp2D, int tableID, int x_inverse, int y_inverse);
    OpenShortData(const OpenShortData & rhs) :
    mWidth(0)
    ,mHeight(0)
    ,mTargetVoltage(0.0f)
    ,mBaseVoltage(0)
    ,mRawVoltage(0)
    ,mDiffVoltage(0)
    ,mCycle(0)
    ,mLoop(0)
    ,mReadBaseVoltage(1)
    {}
    OpenShortData & operator = (const OpenShortData & rhs) { return *this; }
public:
    OpenShortData();
    OpenShortData(int w, int h);

    void setWH(int w, int h);

    void setTargetVoltage(float targetVoltage);

    void setBaseVoltage(int x, int y, float baseVoltage);

    void setRawVoltage(int x, int y, float rawVoltage);

    void setDiffVoltage(int x, int y, float diffVoltage);

    void setCycle(int x, int y, float cycle);

    void setLoop(int x, int y, float loop);

    int getReadData();
	void setReadData(int baseVoltage);


    int getWidth();

    int getHeight();

    inline int getBtnChannel()
    {
        return 0;
    }

    float getTargetVoltage();

    float getBaseVoltage(int x, int y);

    float getRawVoltage(int x, int y);

    float getDiffVoltage(int x, int y);

    float getCycle(int x, int y);

    float getLoop(int x, int y);


    bool saveSampleFile(FILE* file, int x_inverse, int y_inverse);

    bool loadSampleFile(FILE* file);

    bool loadSampleSeedResultFile(FILE* file, int &x_inverse, int &y_inverse, bool bFilter = false);

    bool combineData(OpenShortData* resultDatas, int size);

	bool combineDataWithFilter(OpenShortData* resultDatas, int size);

	void writeCalibrationToBuf(char *buf);
	bool loopCycleToBinFile(FILE *file);



    ~OpenShortData();


};

#endif
