#include "GGameDemoHeader.h"
#include "GAnimController.h"
#include "GTimer.h"


GAnimController::GAnimController ( void )
    : mD3DController ( nullptr )
{
    mSetDone = false;
    mCurSet = nullptr;
    mTargetSet = nullptr;
    mSetFrameLast = 0;
    mSetFrameCur = 0;
}


GAnimController::~GAnimController ( void )
{
    dSafeRelease ( mD3DController );
}

void GAnimController::setBaseController ( ID3DXAnimationController* controller )
{
    CXASSERT_RETURN ( controller != nullptr );
    CXASSERT_RESULT ( controller->CloneAnimationController
                            (
                                controller->GetMaxNumAnimationOutputs()
                                , controller->GetMaxNumAnimationSets()
                                , controller->GetMaxNumTracks()
                                , controller->GetMaxNumEvents()
                                , &mD3DController
                            )
                          );
	assert(controller->AddRef()==2);
	assert(controller->Release()==1);
    CXASSERT_RETURN ( mD3DController );
    playFirstAnim();
}

void GAnimController::update()
{
    mD3DController->AdvanceTime ( TheTimer->getFrameTimeSec(), NULL );

    if ( mCurSet != NULL )
    {
        D3DXTRACK_DESC trackDesc;

        mD3DController->GetTrackDesc ( 0, &trackDesc );

        double dbPassTime = mCurSet->GetPeriodicPosition ( trackDesc.Position );

        mSetFrameCur = dbPassTime * 300000;

        if ( mSetFrameCur < mSetFrameLast )
        {
            mSetDone = true;
            mSetFrameLast = 0;
            mSetFrameCur = 0;
            if ( mTargetSet )
            {
                mCurSet = mTargetSet;
                mTargetSet = nullptr;
            }
        }
        else
        {
            mSetFrameLast = mSetFrameCur;
        }
    }
}

void GAnimController::play ( const char* animSetName )
{
    assert ( mD3DController );
    ID3DXAnimationSet* pset = nullptr;
    mD3DController->GetAnimationSetByName ( animSetName , &pset );
    if ( mSetDone )
        mCurSet = pset;
    else
        mTargetSet = pset;
}

void GAnimController::playFirstAnim()
{
    if ( mCurSet != nullptr )
    {
        mD3DController->GetAnimationSet ( 0, &mTargetSet );
    }
    else
    {
        mD3DController->GetAnimationSet ( 0, &mCurSet );
    }
}
