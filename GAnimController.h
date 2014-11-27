#ifndef GAnimController_h__
#define GAnimController_h__

//-------------------------------------------------------------------------
/**
	@brief ����������
	@author nnboy,21/11/2014  14:40
*/
class GAnimController
{
public:
    GAnimController ( void );
    ~GAnimController ( void );
    void setBaseController ( ID3DXAnimationController* controller );
    void update();
    void play ( const char* animSetName );
    void playFirstAnim();
private:
    ID3DXAnimationController* mD3DController;
    LPD3DXANIMATIONSET mCurSet;						
    LPD3DXANIMATIONSET mTargetSet;

	/** @brief ��ǰ��������һ�β��ŵ���֡�� **/
    u32 mSetFrameLast;

	/** @brief ��ǰ�������˴β��ŵ���֡�� **/
    u32 mSetFrameCur;

	/** @brief ��һ���������Ƿ񲥷���� **/
    bool mSetDone;
};

#endif // GAnimController_h__
