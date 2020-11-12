//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: ert_main.cpp
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
#include <stddef.h>
#include <stdio.h>                // This ert_main.c example uses printf/fflush
#include <iostream>
#include "Subsystem.h"                 // Model's header file
#include "rtwtypes.h"

static SubsystemModelClass Subsystem_Obj;// Instance of model class

//
// Associating rt_OneStep with a real-time clock or interrupt service routine
// is what makes the generated code "real-time".  The function rt_OneStep is
// always associated with the base rate of the model.  Subrates are managed
// by the base rate from inside the generated code.  Enabling/disabling
// interrupts and floating point context switches are target specific.  This
// example code indicates where these should take place relative to executing
// the generated code step function.  Overrun behavior should be tailored to
// your application needs.  This example simply sets an error status in the
// real-time model and returns from rt_OneStep.
//
void rt_OneStep(void);
void rt_OneStep(void)
{
  static boolean_T OverrunFlag = false;

  // Disable interrupts here

  // Check for overrun
  if (OverrunFlag) {
    rtmSetErrorStatus(Subsystem_Obj.getRTM(), "Overrun");
    return;
  }

  OverrunFlag = true;

  // Save FPU context here (if necessary)
  // Re-enable timer or interrupt here
  // Set model inputs here
  
 


  // Step the model
  Subsystem_Obj.step(); // sample time = 0.1s

  // Get model outputs here

 

  // Indicate task complete
  OverrunFlag = false;

  // Disable interrupts here
  // Restore FPU context here (if necessary)
  // Enable interrupts here
}

//
// The example "main" function illustrates what is required by your
// application code to initialize, execute, and terminate the generated code.
// Attaching rt_OneStep to a real-time clock is target specific.  This example
// illustrates how you do this relative to initializing the model.
//
int_T main(int_T argc, const char *argv[])
{
  // Unused arguments
  (void)(argc);
  (void)(argv);

  // Initialize model
  Subsystem_Obj.initialize();
 
  // Attach rt_OneStep to a timer or interrupt service routine with
  //  period 0.1 seconds (the model's base sample time) here.  The
  //  call syntax for rt_OneStep is
  //
  int time_period = 3600 * 20; // [sek]
  real_T T0_in = 12; // Temperatura otoczenia stopnie Celcjusza
  real_T Tzco_in = 70; // Temperatura zasilania
  real_T Trref_in = 19; // Temperatura zadana

  real_T Tpco_out; // Temperatura powrotu
  real_T Tro_out;  // Temperatura pomieszczenia
  real_T Fcob_out; // Przep³yw wody

  //while (true)
  //{
      
  bool start_count = 1;
  if (start_count)
  {
      // przeczytaj na jaki okres czasu liczyc
      // time_period

      Subsystem_Obj.set_T0(T0_in);
      Subsystem_Obj.set_Tzco(Tzco_in);
      Subsystem_Obj.set_Trref(Trref_in);
      for (int i = 0; i < time_period; i++)
      {
          rt_OneStep();
      }
      
      Tpco_out = Subsystem_Obj.get_Tpco();
      Tro_out = Subsystem_Obj.get_Tro();
      Fcob_out = Subsystem_Obj.get_Fcob();
  }
  std::cout << "Temperatura powrotu: " << Tpco_out << std::endl;
  std::cout << "Temperatura pomieszczenia: " << Tro_out << std::endl;
  std::cout << "Przeplyw wody: " << Fcob_out  << std::endl;

 // }
   
  printf("Warning: The simulation will run forever. "
         "Generated ERT main won't simulate model step behavior. "
         "To change this behavior select the 'MAT-file logging' option.\n");
  fflush((NULL));
  while (rtmGetErrorStatus(Subsystem_Obj.getRTM()) == (NULL)) {
    //  Perform other application tasks here
  }

  // Disable rt_OneStep() here

  // Terminate model
  Subsystem_Obj.terminate();
  return 0;
}

//
// File trailer for generated code.
//
// [EOF]
//
