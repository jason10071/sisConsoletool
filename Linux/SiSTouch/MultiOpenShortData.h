#ifndef MULTIOPENSHORTDATA_H
#define MULTIOPENSHORTDATA_H
#include "OpenShortData.h"
#include "OpenShortConfig.h"

class MultiOpenShortData
{
private:
    int mChipNum;
    OpenShortData* mDatas;
    int* mChipOrderIndexs;

    int mReadBaseVoltage;

protected:
    void queryMappingSingle(int &devIdx, int &x, int &y);

public:
    MultiOpenShortData();
    MultiOpenShortData(int chipNum);

    ~MultiOpenShortData();

    void setChipNum(int chipNum);
    int getChipNum();

    bool setOrderInfo(OpenShortConfig* config);

    void setChipOrderIndex(int index, int orderIndex);
    int getChipOrderIndex(int index);

    OpenShortData* getData(int index);

    void divideToLike(MultiOpenShortData* likeObject);

    int getWidth();
    int getWidth(int index);
    int getHeight();
    int getHeight(int index);
    int getBtnChannel();

    float getCycle(int x, int y);
    float getLoop(int x, int y);
    float getBaseVoltage(int x, int y);
    float getRawVoltage(int x, int y);
    float getDiffVoltage(int x, int y);
    float getTargetVoltage(int x, int y);

    void setCycle(int x, int y, float cycle);
    void setLoop(int x, int y, float loop);
    void setBaseVoltage(int x, int y, float baseVoltage);
    void setRawVoltage(int x, int y, float rawVoltage);
    void setDiffVoltage(int x, int y, float diffVoltage);
    void setTargetVoltage(int x, int y, float targetVoltage);

    int getReadData();
	void setReadData(int BaseVoltage);

    bool saveSampleFile(FILE* file);
    bool loadSampleFile(FILE* file);
    bool loadSampleSeedResultFile(FILE* file);
private:
    MultiOpenShortData(const MultiOpenShortData & rhs) : 
    mChipNum(1)
    ,mDatas(0)
    ,mChipOrderIndexs(0)
    ,mReadBaseVoltage(1)
    {}
    // assignment operator
    MultiOpenShortData & operator = (const MultiOpenShortData & rhs) { return *this; }
};
#endif
