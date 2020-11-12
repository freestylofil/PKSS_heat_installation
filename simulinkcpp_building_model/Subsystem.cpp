//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: Subsystem.cpp
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
#include "Subsystem.h"
#include "Subsystem_private.h"

// Exported block parameters
real_T cb = 1000.0;                    // Variable: cb
                                          //  Referenced by: '<S1>/Gain1'

real_T ch = 2700.0;                    // Variable: ch
                                          //  Referenced by: '<S1>/Gain'

real_T cw = 4200.0;                    // Variable: cw
                                          //  Referenced by: '<S1>/Constant1'

real_T dzeta = 1000.0;                 // Variable: dzeta
                                          //  Referenced by: '<S1>/Constant'

real_T kext = 15000.0;                 // Variable: kext
                                          //  Referenced by: '<S1>/Gain3'

real_T kh = 12000.0;                   // Variable: kh
                                          //  Referenced by:
                                          //    '<S1>/Gain2'
                                          //    '<S1>/Gain5'

real_T mb = 20000.0;                   // Variable: mb
                                          //  Referenced by: '<S1>/Gain1'

real_T mh = 3000.0;                    // Variable: mh
                                          //  Referenced by: '<S1>/Gain'


// Model step function
void SubsystemModelClass::step()
{
  real_T rtb_FilterCoefficient;
  real_T rtb_Subtract1_tmp;
  real_T rtb_Sum;

  // Outputs for Atomic SubSystem: '<Root>/Subsystem'
  // Outport: '<Root>/Tr' incorporates:
  //   DiscreteIntegrator: '<S1>/Discrete-Time Integrator1'

  Subsystem_Y.Tr = Subsystem_DW.DiscreteTimeIntegrator1_DSTATE;

  // Sum: '<S1>/Sum' incorporates:
  //   DiscreteIntegrator: '<S1>/Discrete-Time Integrator1'
  //   Inport: '<Root>/Trref'

  rtb_Sum = Subsystem_U.Trref - Subsystem_DW.DiscreteTimeIntegrator1_DSTATE;

  // Gain: '<S37>/Filter Coefficient' incorporates:
  //   DiscreteIntegrator: '<S29>/Filter'
  //   Gain: '<S28>/Derivative Gain'
  //   Sum: '<S29>/SumD'

  rtb_FilterCoefficient = (-0.00168554182386 * rtb_Sum -
    Subsystem_DW.Filter_DSTATE) * 0.171582409720465;

  // Saturate: '<S1>/Saturation1' incorporates:
  //   DiscreteIntegrator: '<S34>/Integrator'
  //   Gain: '<S39>/Proportional Gain'
  //   Sum: '<S43>/Sum'

  Subsystem_Y.Fcob = (0.000316420719124277 * rtb_Sum +
                      Subsystem_DW.Integrator_DSTATE) + rtb_FilterCoefficient;

  // Saturate: '<S1>/Saturation1'
  if (Subsystem_Y.Fcob > 0.0083333333333333332) {
    // Saturate: '<S1>/Saturation1'
    Subsystem_Y.Fcob = 0.0083333333333333332;
  } else {
    if (Subsystem_Y.Fcob < 0.0) {
      // Saturate: '<S1>/Saturation1'
      Subsystem_Y.Fcob = 0.0;
    }
  }

  // End of Saturate: '<S1>/Saturation1'

  // Outport: '<Root>/Tpco' incorporates:
  //   DiscreteIntegrator: '<S1>/Discrete-Time Integrator'

  Subsystem_Y.Tpco = Subsystem_DW.DiscreteTimeIntegrator_DSTATE;

  // Sum: '<S1>/Subtract1' incorporates:
  //   DiscreteIntegrator: '<S1>/Discrete-Time Integrator'
  //   DiscreteIntegrator: '<S1>/Discrete-Time Integrator1'
  //   Sum: '<S1>/Subtract4'

  rtb_Subtract1_tmp = Subsystem_DW.DiscreteTimeIntegrator_DSTATE -
    Subsystem_DW.DiscreteTimeIntegrator1_DSTATE;

  // Update for DiscreteIntegrator: '<S34>/Integrator' incorporates:
  //   Gain: '<S31>/Integral Gain'

  Subsystem_DW.Integrator_DSTATE += 1.42804500861683E-7 * rtb_Sum * 0.1;

  // Update for DiscreteIntegrator: '<S1>/Discrete-Time Integrator1' incorporates:
  //   Gain: '<S1>/Gain1'
  //   Gain: '<S1>/Gain2'
  //   Gain: '<S1>/Gain3'
  //   Inport: '<Root>/To'
  //   Sum: '<S1>/Subtract3'
  //   Sum: '<S1>/Subtract5'

  Subsystem_DW.DiscreteTimeIntegrator1_DSTATE += (rtb_Subtract1_tmp * kh -
    (Subsystem_DW.DiscreteTimeIntegrator1_DSTATE - Subsystem_U.To) * kext) *
    (1.0 / (mb * cb)) * 0.1;

  // Update for DiscreteIntegrator: '<S29>/Filter'
  Subsystem_DW.Filter_DSTATE += 0.1 * rtb_FilterCoefficient;

  // Update for DiscreteIntegrator: '<S1>/Discrete-Time Integrator' incorporates:
  //   Constant: '<S1>/Constant'
  //   Constant: '<S1>/Constant1'
  //   Gain: '<S1>/Gain'
  //   Gain: '<S1>/Gain5'
  //   Inport: '<Root>/Tzco'
  //   Product: '<S1>/Product'
  //   Sum: '<S1>/Subtract'
  //   Sum: '<S1>/Subtract1'
  //   Sum: '<S1>/Subtract2'

  Subsystem_DW.DiscreteTimeIntegrator_DSTATE += ((Subsystem_U.Tzco -
    Subsystem_DW.DiscreteTimeIntegrator_DSTATE) * Subsystem_Y.Fcob * dzeta * cw
    - kh * rtb_Subtract1_tmp) * (1.0 / (mh * ch)) * 0.1;

  // End of Outputs for SubSystem: '<Root>/Subsystem'
}

// Model initialize function
void SubsystemModelClass::initialize()
{
  // SystemInitialize for Atomic SubSystem: '<Root>/Subsystem'
  // InitializeConditions for DiscreteIntegrator: '<S1>/Discrete-Time Integrator1' 
  Subsystem_DW.DiscreteTimeIntegrator1_DSTATE = 15.0;

  // InitializeConditions for DiscreteIntegrator: '<S1>/Discrete-Time Integrator' 
  Subsystem_DW.DiscreteTimeIntegrator_DSTATE = 22.0;

  // End of SystemInitialize for SubSystem: '<Root>/Subsystem'
}

// Model terminate function
void SubsystemModelClass::terminate()
{
  // (no terminate code required)
}

// Constructor
SubsystemModelClass::SubsystemModelClass() :
  Subsystem_DW(),
  Subsystem_U(),
  Subsystem_Y(),
  Subsystem_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
SubsystemModelClass::~SubsystemModelClass()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
SubsystemModelClass::RT_MODEL_Subsystem_T * SubsystemModelClass::getRTM()
{
  return (&Subsystem_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
