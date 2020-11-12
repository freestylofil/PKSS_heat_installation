//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: Subsystem.h
//
// Code generated for Simulink model 'Subsystem'.
//
// Model version                  : 2.6
// Simulink Coder version         : 9.4 (R2020b) 29-Jul-2020
// C/C++ source code generated on : Wed Nov 11 21:04:10 2020
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#ifndef RTW_HEADER_Subsystem_h_
#define RTW_HEADER_Subsystem_h_
#include "rtwtypes.h"
#include "Subsystem_types.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

//
//  Exported Global Parameters
//
//  Note: Exported global parameters are tunable parameters with an exported
//  global storage class designation.  Code generation will declare the memory for
//  these parameters and exports their symbols.
//

extern real_T cb;                      // Variable: cb
                                          //  Referenced by: '<S1>/Gain1'

extern real_T ch;                      // Variable: ch
                                          //  Referenced by: '<S1>/Gain'

extern real_T cw;                      // Variable: cw
                                          //  Referenced by: '<S1>/Constant1'

extern real_T dzeta;                   // Variable: dzeta
                                          //  Referenced by: '<S1>/Constant'

extern real_T kext;                    // Variable: kext
                                          //  Referenced by: '<S1>/Gain3'

extern real_T kh;                      // Variable: kh
                                          //  Referenced by:
                                          //    '<S1>/Gain2'
                                          //    '<S1>/Gain5'

extern real_T mb;                      // Variable: mb
                                          //  Referenced by: '<S1>/Gain1'

extern real_T mh;                      // Variable: mh
                                          //  Referenced by: '<S1>/Gain'


// Class declaration for model Subsystem
class SubsystemModelClass {
  // public data and function members
 public:
  // Block states (default storage) for system '<Root>'
  typedef struct {
    real_T Integrator_DSTATE;          // '<S34>/Integrator'
    real_T DiscreteTimeIntegrator1_DSTATE;// '<S1>/Discrete-Time Integrator1'
    real_T Filter_DSTATE;              // '<S29>/Filter'
    real_T DiscreteTimeIntegrator_DSTATE;// '<S1>/Discrete-Time Integrator'
  } DW_Subsystem_T;

  // External inputs (root inport signals with default storage)
  typedef struct {
    real_T Trref;                      // '<Root>/Trref'
    real_T Tzco;                       // '<Root>/Tzco'
    real_T To;                         // '<Root>/To'
  } ExtU_Subsystem_T;

  // External outputs (root outports fed by signals with default storage)
  typedef struct {
    real_T Tpco;                       // '<Root>/Tpco'
    real_T Tr;                         // '<Root>/Tr'
    real_T Fcob;                       // '<Root>/Fcob'
  } ExtY_Subsystem_T;

  // Real-time Model Data Structure
  struct RT_MODEL_Subsystem_T {
    const char_T * volatile errorStatus;
  };

  // model initialize function
  void initialize();

  // model step function
  void step();

  // model terminate function
  void terminate();

  // Constructor
  SubsystemModelClass();

  // Destructor
  ~SubsystemModelClass();

  // Root-level structure-based inputs set method

  // Root inports set method
  void setExternalInputs(const ExtU_Subsystem_T* pExtU_Subsystem_T)
  {
    Subsystem_U = *pExtU_Subsystem_T;
  }
  // My methods
  void set_T0(const real_T x)
  {
      Subsystem_U.To = x;
  }

  void set_Trref(const real_T x)
  {
      Subsystem_U.Trref = x;
  }

  void set_Tzco(const real_T x)
  {
      Subsystem_U.Tzco = x;
  }


  // Root-level structure-based outputs get method

  // Root outports get method
  const SubsystemModelClass::ExtY_Subsystem_T & getExternalOutputs() const
  {
    return Subsystem_Y;
  }
  // My methods
  const real_T get_Tpco()const
  {
      return Subsystem_Y.Tpco;
  }
  const real_T get_Tro()const
  {
      return Subsystem_Y.Tr;
  }
  const real_T get_Fcob()const
  {
      return Subsystem_Y.Fcob;
  }


  // Real-Time Model get method
  SubsystemModelClass::RT_MODEL_Subsystem_T * getRTM();

  // private data and function members
 private:
  // Block states
  DW_Subsystem_T Subsystem_DW;

  // External inputs
  ExtU_Subsystem_T Subsystem_U;

  // External outputs
  ExtY_Subsystem_T Subsystem_Y;

  // Real-Time Model
  RT_MODEL_Subsystem_T Subsystem_M;
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S1>/Scope2' : Unused code path elimination


//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Note that this particular code originates from a subsystem build,
//  and has its own system numbers different from the parent model.
//  Refer to the system hierarchy for this subsystem below, and use the
//  MATLAB hilite_system command to trace the generated code back
//  to the parent model.  For example,
//
//  hilite_system('budynek_model/Subsystem')    - opens subsystem budynek_model/Subsystem
//  hilite_system('budynek_model/Subsystem/Kp') - opens and selects block Kp
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'budynek_model'
//  '<S1>'   : 'budynek_model/Subsystem'
//  '<S2>'   : 'budynek_model/Subsystem/Discrete PID Controller'
//  '<S3>'   : 'budynek_model/Subsystem/Discrete PID Controller/Anti-windup'
//  '<S4>'   : 'budynek_model/Subsystem/Discrete PID Controller/D Gain'
//  '<S5>'   : 'budynek_model/Subsystem/Discrete PID Controller/Filter'
//  '<S6>'   : 'budynek_model/Subsystem/Discrete PID Controller/Filter ICs'
//  '<S7>'   : 'budynek_model/Subsystem/Discrete PID Controller/I Gain'
//  '<S8>'   : 'budynek_model/Subsystem/Discrete PID Controller/Ideal P Gain'
//  '<S9>'   : 'budynek_model/Subsystem/Discrete PID Controller/Ideal P Gain Fdbk'
//  '<S10>'  : 'budynek_model/Subsystem/Discrete PID Controller/Integrator'
//  '<S11>'  : 'budynek_model/Subsystem/Discrete PID Controller/Integrator ICs'
//  '<S12>'  : 'budynek_model/Subsystem/Discrete PID Controller/N Copy'
//  '<S13>'  : 'budynek_model/Subsystem/Discrete PID Controller/N Gain'
//  '<S14>'  : 'budynek_model/Subsystem/Discrete PID Controller/P Copy'
//  '<S15>'  : 'budynek_model/Subsystem/Discrete PID Controller/Parallel P Gain'
//  '<S16>'  : 'budynek_model/Subsystem/Discrete PID Controller/Reset Signal'
//  '<S17>'  : 'budynek_model/Subsystem/Discrete PID Controller/Saturation'
//  '<S18>'  : 'budynek_model/Subsystem/Discrete PID Controller/Saturation Fdbk'
//  '<S19>'  : 'budynek_model/Subsystem/Discrete PID Controller/Sum'
//  '<S20>'  : 'budynek_model/Subsystem/Discrete PID Controller/Sum Fdbk'
//  '<S21>'  : 'budynek_model/Subsystem/Discrete PID Controller/Tracking Mode'
//  '<S22>'  : 'budynek_model/Subsystem/Discrete PID Controller/Tracking Mode Sum'
//  '<S23>'  : 'budynek_model/Subsystem/Discrete PID Controller/Tsamp - Integral'
//  '<S24>'  : 'budynek_model/Subsystem/Discrete PID Controller/Tsamp - Ngain'
//  '<S25>'  : 'budynek_model/Subsystem/Discrete PID Controller/postSat Signal'
//  '<S26>'  : 'budynek_model/Subsystem/Discrete PID Controller/preSat Signal'
//  '<S27>'  : 'budynek_model/Subsystem/Discrete PID Controller/Anti-windup/Passthrough'
//  '<S28>'  : 'budynek_model/Subsystem/Discrete PID Controller/D Gain/Internal Parameters'
//  '<S29>'  : 'budynek_model/Subsystem/Discrete PID Controller/Filter/Disc. Forward Euler Filter'
//  '<S30>'  : 'budynek_model/Subsystem/Discrete PID Controller/Filter ICs/Internal IC - Filter'
//  '<S31>'  : 'budynek_model/Subsystem/Discrete PID Controller/I Gain/Internal Parameters'
//  '<S32>'  : 'budynek_model/Subsystem/Discrete PID Controller/Ideal P Gain/Passthrough'
//  '<S33>'  : 'budynek_model/Subsystem/Discrete PID Controller/Ideal P Gain Fdbk/Disabled'
//  '<S34>'  : 'budynek_model/Subsystem/Discrete PID Controller/Integrator/Discrete'
//  '<S35>'  : 'budynek_model/Subsystem/Discrete PID Controller/Integrator ICs/Internal IC'
//  '<S36>'  : 'budynek_model/Subsystem/Discrete PID Controller/N Copy/Disabled'
//  '<S37>'  : 'budynek_model/Subsystem/Discrete PID Controller/N Gain/Internal Parameters'
//  '<S38>'  : 'budynek_model/Subsystem/Discrete PID Controller/P Copy/Disabled'
//  '<S39>'  : 'budynek_model/Subsystem/Discrete PID Controller/Parallel P Gain/Internal Parameters'
//  '<S40>'  : 'budynek_model/Subsystem/Discrete PID Controller/Reset Signal/Disabled'
//  '<S41>'  : 'budynek_model/Subsystem/Discrete PID Controller/Saturation/Passthrough'
//  '<S42>'  : 'budynek_model/Subsystem/Discrete PID Controller/Saturation Fdbk/Disabled'
//  '<S43>'  : 'budynek_model/Subsystem/Discrete PID Controller/Sum/Sum_PID'
//  '<S44>'  : 'budynek_model/Subsystem/Discrete PID Controller/Sum Fdbk/Disabled'
//  '<S45>'  : 'budynek_model/Subsystem/Discrete PID Controller/Tracking Mode/Disabled'
//  '<S46>'  : 'budynek_model/Subsystem/Discrete PID Controller/Tracking Mode Sum/Passthrough'
//  '<S47>'  : 'budynek_model/Subsystem/Discrete PID Controller/Tsamp - Integral/Passthrough'
//  '<S48>'  : 'budynek_model/Subsystem/Discrete PID Controller/Tsamp - Ngain/Passthrough'
//  '<S49>'  : 'budynek_model/Subsystem/Discrete PID Controller/postSat Signal/Forward_Path'
//  '<S50>'  : 'budynek_model/Subsystem/Discrete PID Controller/preSat Signal/Forward_Path'

#endif                                 // RTW_HEADER_Subsystem_h_

//
// File trailer for generated code.
//
// [EOF]
//
