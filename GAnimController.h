#ifndef GAnimController_h__
#define GAnimController_h__

//-------------------------------------------------------------------------
/**
	@brief 动画控制器
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

	/** @brief 当前动画集上一次播放到的帧数 **/
    u32 mSetFrameLast;

	/** @brief 当前动画集此次播放到的帧数 **/
    u32 mSetFrameCur;

	/** @brief 上一个动作集是否播放完成 **/
    bool mSetDone;
};

#endif // GAnimController_h__
