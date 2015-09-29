/*------------------------------------------------------------------------------
 *
 *    Copyright (C) 1998 : Space Systems Finland Ltd.
 *
 * Space Systems Finland Ltd (SSF) allows you to use this version of
 * the DEBIE-I DPU software for the specific purpose and under the
 * specific conditions set forth in the Terms Of Use document enclosed
 * with or attached to this software. In particular, the software
 * remains the property of SSF and you must not distribute the software
 * to third parties without written and signed authorization from SSF.
 *
 *    System Name:   DEBIE DPU SW
 *    Subsystem  :   DAS
 *    Module     :   debie.c
 *
 * Main function.
 *
 * Based on the SSF file debie.c, revision 1.4, Tue Jun 01 13:37:20 1999.
 *
 *- * --------------------------------------------------------------------------
 */


#include "reg52.h"
#include "kernobj.h"
#include "keyword.h"
#include "health.h"
#include "taskctrl.h"


int main(void)
{
   TARGET_INIT;
   /* Initialize the benchmark target system. */

   EA=0;
   /* Disable all interrupts */

   Boot();
   /* Execute boot sequence */

   EA=1;
   /* Enable 'enabled' interrupts */

   StartSystem(HEALTH_MONITORING_TASK);
   /* Start RTX kernel and first task */

   // do not run simulation for ever, just exit here
   return 0;
   _Pragma("loopbound min 0 max 0")
   while(1)
   {
      /* StartSystem has failed, so we just      */
      /* wait here for a while for the Watch Dog */
      /* to wake up.                             */
   }
}

