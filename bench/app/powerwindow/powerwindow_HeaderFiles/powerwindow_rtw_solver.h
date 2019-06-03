/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: powerwindow_rtw_solver.h

  Author: CoSys-Lab, University of Antwerp

  Function: headerfile

  Source: https://github.com/tacle/tacle-bench/blob/master/bench/app/PowerWindow/powerwindow_HeaderFiles/powerwindow_rtw_solver.h

  Changes: a brief summary of major functional changes and formatting)

  License: GNU General Public License

*/

#ifndef powerwindow___RTW_SOLVER_H__
#define powerwindow___RTW_SOLVER_H__

/* =============================================================================
   Solver object
   =============================================================================
*/
#ifndef NO_FLOATS /* ERT integer-only */
/*
   Enum for solver tolerance
*/
typedef enum {
  SL_SOLVER_TOLERANCE_AUTO  = 0,  /* Set Automatically by Solver */
  SL_SOLVER_TOLERANCE_LOCAL = 1,  /* Set Locally, e.g., by Blocks */
  SL_SOLVER_TOLERANCE_GLOBAL = 2, /* Set Globally, e.g., by Block Diagram */
  SL_SOLVER_TOLERANCE_UNDEFINED = 255 /* Signal uninitialized */
} powerwindow_SL_SolverToleranceControlFlag_T;


/*
   Enum for jacobian method control
*/
typedef enum {
  SL_JM_BD_AUTO = 0,
  SL_JM_BD_SPARSE_PERTURBATION,
  SL_JM_BD_FULL_PERTURBATION,
  SL_JM_BD_SPARSE_ANALYTICAL,
  SL_JM_BD_FULL_ANALYTICAL
} powerwindow_slJmBdControl;


typedef struct _ssSolverInfo_tag {
  void        *rtModelPtr;


  const char  *solverName;
  powerwindow_boolean_T   isVariableStepSolver;
  powerwindow_boolean_T   solverNeedsReset;

  powerwindow_time_T      solverStopTime;
  powerwindow_time_T      *stepSizePtr;
  powerwindow_time_T      minStepSize;
  powerwindow_time_T      maxStepSize;
  powerwindow_time_T      fixedStepSize;

  powerwindow_int_T       solverShapePreserveControl;
  powerwindow_int_T       solverMaxConsecutiveMinStep;
  powerwindow_int_T       maxNumMinSteps;
  powerwindow_int_T       solverMaxOrder;
  powerwindow_real_T      solverConsecutiveZCsStepRelTol;
  powerwindow_int_T       solverMaxConsecutiveZCs;

  powerwindow_int_T       solverExtrapolationOrder;
  powerwindow_int_T       solverNumberNewtonIterations;

  powerwindow_int_T       solverRefineFactor;
  powerwindow_real_T      solverRelTol;
  powerwindow_real_T      unused_real_T_1;

  powerwindow_real_T      **dXPtr;
  powerwindow_time_T      **tPtr;

  powerwindow_int_T       *numContStatesPtr;
  powerwindow_real_T      **contStatesPtr;

  powerwindow_real_T     *zcSignalVector;
  powerwindow_uint8_T    *zcEventsVector;
  powerwindow_uint8_T    *zcSignalAttrib;
  powerwindow_int_T       zcSignalVectorLength;
  powerwindow_uint8_T    *reserved;

  powerwindow_boolean_T   foundContZcEvents;
  powerwindow_boolean_T   isAtLeftPostOfContZcEvent;
  powerwindow_boolean_T   isAtRightPostOfContZcEvent;
  powerwindow_boolean_T   adaptiveZcDetection;

  powerwindow_int_T       numZcSignals;

  powerwindow_boolean_T   stateProjection;
  powerwindow_boolean_T   robustResetMethod;  /* user's preference */
  powerwindow_boolean_T   updateJacobianAtReset; /* S-Fcn request (sticky) */
  powerwindow_boolean_T   consistencyChecking;

  powerwindow_int_T         massMatrixNzMax;
  powerwindow_int_T        *massMatrixIr;
  powerwindow_int_T        *massMatrixJc;
  powerwindow_real_T       *massMatrixPr;

  const powerwindow_char_T **errStatusPtr;

  powerwindow_real_T      zcThreshold;
  powerwindow_int_T       zeroCrossAlgorithm;
  powerwindow_int_T       consecutiveZCsError;

  powerwindow_boolean_T   isComputingJacobian;
  powerwindow_slJmBdControl solverJacobianMethodControl;
  powerwindow_int_T       ignoredZcDiagnostic;
  powerwindow_int_T       maskedZcDiagnostic;
  powerwindow_boolean_T   isOutputMethodComputed;
} ssSolverInfo;

/* Support old name RTWSolverInfo */
typedef ssSolverInfo RTWSolverInfo;

#define rtsiSetRTModelPtr(S,rtmp) ((S)->rtModelPtr = (rtmp))
#define rtsiGetRTModelPtr(S)      (S)->rtModelPtr

#define rtsiSetSimTimeStepPtr(S,stp) ((S)->simTimeStepPtr = (stp))
#define rtsiGetSimTimeStepPtr(S) ((S)->simTimeStepPtr)
#define rtsiGetSimTimeStep(S)        *((S)->simTimeStepPtr)
#define rtsiSetSimTimeStep(S,st)     (*((S)->simTimeStepPtr) = (st))

#define rtsiSetSolverData(S,sd) ((S)->solverData = (sd))
#define rtsiGetSolverData(S)    (S)->solverData

#define rtsiSetSolverName(S,sn) ((S)->solverName = (sn))
#define rtsiGetSolverName(S)    (S)->solverName

#define rtsiSetVariableStepSolver(S,vs) ((S)->isVariableStepSolver = (vs))
#define rtsiIsVariableStepSolver(S)     (S)->isVariableStepSolver

#define rtsiSetSolverNeedsReset(S,sn) ((S)->solverNeedsReset = (sn))
#define rtsiGetSolverNeedsReset(S)    (S)->solverNeedsReset

#define rtsiSetBlkStateChange(S,sn) ((S)->blkStateChange = (sn))
#define rtsiGetBlkStateChange(S)    (S)->blkStateChange

#define rtsiSetSolverMode(S,sm) ((S)->solverMode = (sm))
#define rtsiGetSolverMode(S)    (S)->solverMode

#define rtsiSetSolverStopTime(S,st) ((S)->solverStopTime = (st))
#define rtsiGetSolverStopTime(S)    (S)->solverStopTime

#define rtsiSetStepSizePtr(S,ssp) ((S)->stepSizePtr = (ssp))
#define rtsiSetStepSize(S,ss)     (*((S)->stepSizePtr) = (ss))
#define rtsiGetStepSize(S)        *((S)->stepSizePtr)

#define rtsiSetMinStepSize(S,ss) (((S)->minStepSize = (ss)))
#define rtsiGetMinStepSize(S)    (S)->minStepSize

#define rtsiSetMaxStepSize(S,ss) ((S)->maxStepSize = (ss))
#define rtsiGetMaxStepSize(S)    (S)->maxStepSize

#define rtsiSetFixedStepSize(S,ss) ((S)->fixedStepSize = (ss))
#define rtsiGetFixedStepSize(S)    (S)->fixedStepSize

#define rtsiSetMaxNumMinSteps(S,mns) ((S)->maxNumMinSteps = (mns))
#define rtsiGetMaxNumMinSteps(S)     (S)->maxNumMinSteps

#define rtsiSetSolverMaxOrder(S,smo) ((S)->solverMaxOrder = (smo))
#define rtsiGetSolverMaxOrder(S)     (S)->solverMaxOrder

#define rtsiSetSolverJacobianMethodControl(S,smcm)   (ssGetSolverInfo(S)->solverJacobianMethodControl = (smcm))
#define rtsiGetSolverJacobianMethodControl(S)        ssGetSolverInfo(S)->solverJacobianMethodControl

#define rtsiSetSolverShapePreserveControl(S,smcm)   (ssGetSolverInfo(S)->solverShapePreserveControl = (smcm))
#define rtsiGetSolverShapePreserveControl(S)        ssGetSolverInfo(S)->solverShapePreserveControl

#define rtsiSetSolverConsecutiveZCsStepRelTol(S,scr) (ssGetSolverInfo(S)->solverConsecutiveZCsStepRelTol = (scr))
#define rtsiGetSolverConsecutiveZCsStepRelTol(S)     ssGetSolverInfo(S)->solverConsecutiveZCsStepRelTol

#define rtsiSetSolverMaxConsecutiveZCs(S,smcz)       (ssGetSolverInfo(S)->solverMaxConsecutiveZCs = (smcz))
#define rtsiGetSolverMaxConsecutiveZCs(S)            ssGetSolverInfo(S)->solverMaxConsecutiveZCs

#define rtsiSetSolverMaxConsecutiveMinStep(S,smcm)   (ssGetSolverInfo(S)->solverMaxConsecutiveMinStep = (smcm))
#define rtsiGetSolverMaxConsecutiveMinStep(S)        ssGetSolverInfo(S)->solverMaxConsecutiveMinStep

#define rtsiSetSolverExtrapolationOrder(S,seo) ((S)->solverExtrapolationOrder = (seo))
#define rtsiGetSolverExtrapolationOrder(S)      (S)->solverExtrapolationOrder

#define rtsiSetSolverNumberNewtonIterations(S,nni) ((S)->solverNumberNewtonIterations = (nni))
#define rtsiGetSolverNumberNewtonIterations(S)      (S)->solverNumberNewtonIterations

#define rtsiSetSolverRefineFactor(S,smo) ((S)->solverRefineFactor = (smo))
#define rtsiGetSolverRefineFactor(S)     (S)->solverRefineFactor

#define rtsiSetSolverRelTol(S,smo) ((S)->solverRelTol = (smo))
#define rtsiGetSolverRelTol(S)     (S)->solverRelTol

#define rtsiSetSolverMassMatrixType(S,type)  ((S)->massMatrixType = (type))
#define rtsiGetSolverMassMatrixType(S)  (S)->massMatrixType

#define rtsiSetSolverMassMatrixNzMax(S,nzMax)  ((S)->massMatrixNzMax = (nzMax))
#define rtsiGetSolverMassMatrixNzMax(S)  (S)->massMatrixNzMax

#define rtsiSetSolverMassMatrixIr(S,ir)  ((S)->massMatrixIr = (ir))
#define rtsiGetSolverMassMatrixIr(S)  (S)->massMatrixIr

#define rtsiSetSolverMassMatrixJc(S,jc)  ((S)->massMatrixJc = (jc))
#define rtsiGetSolverMassMatrixJc(S)  (S)->massMatrixJc

#define rtsiSetSolverMassMatrixPr(S,pr)  ((S)->massMatrixPr = (pr))
#define rtsiGetSolverMassMatrixPr(S)  (S)->massMatrixPr

#define rtsiSetdXPtr(S,dxp) ((S)->dXPtr = (dxp))
#define rtsiSetdX(S,dx)     (*((S)->dXPtr) = (dx))
#define rtsiGetdX(S)        *((S)->dXPtr)

#define rtsiSetTPtr(S,tp) ((S)->tPtr = (tp))
#define rtsiSetT(S,t)     ((*((S)->tPtr))[ 0 ] = (t))
#define rtsiGetT(S)       (*((S)->tPtr))[ 0 ]

#define rtsiSetContStatesPtr(S,cp) ((S)->contStatesPtr = (cp))
#define rtsiGetContStates(S)       *((S)->contStatesPtr)

#define rtsiSetNumContStatesPtr(S,cp) ((S)->numContStatesPtr = (cp))
#define rtsiGetNumContStates(S)       *((S)->numContStatesPtr)

#define rtsiSetErrorStatusPtr(S,esp) ((S)->errStatusPtr = (esp))
#define rtsiSetErrorStatus(S,es) (*((S)->errStatusPtr) = (es))
#define rtsiGetErrorStatus(S)    *((S)->errStatusPtr)

#define rtsiSetModelMethodsPtr(S,mmp) ((S)->modelMethodsPtr = (mmp))
#define rtsiGetModelMethodsPtr(S)     (S)->modelMethodsPtr

#define rtsiSetSolverComputingJacobian(S,val) ((S)->isComputingJacobian = (val))
#define rtsiIsSolverComputingJacobian(S)    (S)->isComputingJacobian

#define rtsiSetSolverOutputComputed(S,val) ((S)->isOutputMethodComputed = (val))
#define rtsiIsSolverOutputComputed(S) (S)->isOutputMethodComputed

#endif /* !NO_FLOATS */

#endif /* powerwindow___RTW_SOLVER_H__ */
