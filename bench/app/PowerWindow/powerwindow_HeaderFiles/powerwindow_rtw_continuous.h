/*

 This program is part of the TACLeBench benchmark suite.
 Version V 1.x

 Name: powerwindow_rtw_continuous.h

 Author: University of Antwerp

 Function: headerfile

 Source: https://github.com/tacle/tacle-bench/blob/master/bench/app/PowerWindow/powerwindow_HeaderFiles/powerwindow_rtw_continuous.h

 Changes: a brief summary of major functional changes and formatting)

 License: GNU General Public License

 */

#ifndef powerwindow___RTW_CONTINUOUS_H__
#define powerwindow___RTW_CONTINUOUS_H__

//#include "../powerwindow_HeaderFiles/powerwindow_tmwtypes.h"

/* For models registering MassMatrix */
typedef enum {
    SS_MATRIX_NONE,
    SS_MATRIX_CONSTANT,
    SS_MATRIX_TIMEDEP,
    SS_MATRIX_STATEDEP
} powerwindow_ssMatrixType;

typedef enum {
    SOLVER_MODE_AUTO,          /* only occurs in
                                  mdlInitializeSizes/mdlInitializeSampleTimes */
    SOLVER_MODE_SINGLETASKING,
    SOLVER_MODE_MULTITASKING
} powerwindow_SolverMode;

typedef enum {
    MINOR_TIME_STEP,
    MAJOR_TIME_STEP
} powerwindow_SimTimeStep;

/* =============================================================================
 * Model methods object
 * =============================================================================
 */
typedef void (*powerwindow_rtMdlInitializeSizesFcn)(void *rtModel);
typedef void (*powerwindow_rtMdlInitializeSampleTimesFcn)(void *rtModel);
typedef void (*powerwindow_rtMdlStartFcn)(void *rtModel);
typedef void (*powerwindow_rtMdlOutputsFcn)(void *rtModel, powerwindow_int_T tid);
typedef void (*powerwindow_rtMdlUpdateFcn)(void *rtModel, powerwindow_int_T tid);
typedef void (*powerwindow_rtMdlDerivativesFcn)(void *rtModel);
typedef void (*powerwindow_rtMdlProjectionFcn)(void *rtModel);
typedef void (*powerwindow_rtMdlMassMatrixFcn)(void *rtModel);
typedef void (*powerwindow_rtMdlForcingFunctionFcn)(void *rtModel);
typedef void (*powerwindow_rtMdlTerminateFcn)(void *rtModel);
#ifdef RT_MALLOC
typedef real_T (*rtMdlDiscreteEventsFcn)(void  *pModel,
        powerwindow_int_T  rtmNumSampTimes,
        void  *rtmTimingData,
        powerwindow_int_T  *rtmSampleHitPtr,
        powerwindow_int_T  *rtmPerTaskSampleHits);
#endif

typedef struct powerwindow__RTWRTModelMethodsInfo_tag {
    void                          *rtModelPtr;
    powerwindow_rtMdlInitializeSizesFcn       rtmInitSizesFcn;
    powerwindow_rtMdlInitializeSampleTimesFcn rtmInitSampTimesFcn;
    powerwindow_rtMdlStartFcn                 rtmStartFcn;
    powerwindow_rtMdlOutputsFcn               rtmOutputsFcn;
    powerwindow_rtMdlUpdateFcn                rtmUpdateFcn;
    powerwindow_rtMdlDerivativesFcn           rtmDervisFcn;
    powerwindow_rtMdlProjectionFcn            rtmProjectionFcn;
    powerwindow_rtMdlMassMatrixFcn            rtmMassMatrixFcn;
    powerwindow_rtMdlForcingFunctionFcn       rtmForcingFunctionFcn;
    powerwindow_rtMdlTerminateFcn             rtmTerminateFcn;
#ifdef  RT_MALLOC
    rtMdlDiscreteEventsFcn        rtmDiscreteEventsFcn;
#endif
} powerwindow_RTWRTModelMethodsInfo;

#define rtmiSetRTModelPtr(M,rtmp) ((M).rtModelPtr = (rtmp))
#define rtmiGetRTModelPtr(M)      (M).rtModelPtr

#define rtmiSetInitSizesFcn(M,fp) \
  ((M).rtmInitSizesFcn = ((powerwindow_rtMdlInitializeSizesFcn)(fp)))
#define rtmiSetInitSampTimesFcn(M,fp) \
  ((M).rtmInitSampTimesFcn = ((powerwindow_rtMdlInitializeSampleTimesFcn)(fp)))
#define rtmiSetStartFcn(M,fp) \
  ((M).rtmStartFcn = ((powerwindow_rtMdlStartFcn)(fp)))
#define rtmiSetOutputsFcn(M,fp) \
  ((M).rtmOutputsFcn = ((powerwindow_rtMdlOutputsFcn)(fp)))
#define rtmiSetUpdateFcn(M,fp) \
  ((M).rtmUpdateFcn = ((powerwindow_rtMdlUpdateFcn)(fp)))
#define rtmiSetDervisFcn(M,fp) \
  ((M).rtmDervisFcn = ((powerwindow_rtMdlDerivativesFcn)(fp)))
#define rtmiSetProjectionFcn(M,fp) \
  ((M).rtmProjectionFcn = ((powerwindow_rtMdlProjectionFcn)(fp)))
#define rtmiSetMassMatrixFcn(M,fp) \
  ((M).rtmMassMatrixFcn = ((powerwindow_rtMdlMassMatrixFcn)(fp)))
#define rtmiSetForcingFunctionFcn(M,fp) \
  ((M).rtmForcingFunctionFcn = ((powerwindow_rtMdlForcingFunctionFcn)(fp)))
#define rtmiSetTerminateFcn(M,fp) \
  ((M).rtmTerminateFcn = ((powerwindow_rtMdlTerminateFcn)(fp)))
#ifdef  RT_MALLOC
#define rtmiSetDiscreteEventsFcn(M,fp) \
  ((M).rtmDiscreteEventsFcn = ((rtMdlDiscreteEventsFcn)(fp)))
#endif

#define rtmiInitializeSizes(M)                  \
    ((*(M).rtmInitSizesFcn)((M).rtModelPtr))
#define rtmiInitializeSampleTimes(M)                    \
    ((*(M).rtmInitSampTimesFcn)((M).rtModelPtr))
#define rtmiStart(M) \
    ((*(M).rtmStartFcn)((M).rtModelPtr))
#define rtmiOutputs(M, tid) \
    ((*(M).rtmOutputsFcn)((M).rtModelPtr,tid))
#define rtmiUpdate(M, tid) \
    ((*(M).rtmUpdateFcn)((M).rtModelPtr,tid))
#define rtmiDerivatives(M) \
    ((*(M).rtmDervisFcn)((M).rtModelPtr))
#define rtmiProjection(M) \
    ((*(M).rtmProjectionFcn)((M).rtModelPtr))
#define rtmiMassMatrix(M) \
    ((*(M).rtmMassMatrixFcn)((M).rtModelPtr))
#define rtmiForcingFunction(M) \
    ((*(M).rtmForcingFunctionFcn)((M).rtModelPtr))
#define rtmiTerminate(M) \
    ((*(M).rtmTerminateFcn)((M).rtModelPtr))
#ifdef  RT_MALLOC
#define rtmiDiscreteEvents(M,x1,x2,x3,x4)                               \
    ((*(M).rtmDiscreteEventsFcn)((M).rtModelPtr,(x1),(x2),(x3),(x4)))
#endif
#endif /* __RTW_CONTINUOUS_H__ */
