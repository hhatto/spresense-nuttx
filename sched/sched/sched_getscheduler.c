/****************************************************************************
 * sched/sched/sched_getscheduler.c
 *
 *   Copyright (C) 2007, 2009, 2015 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <sched.h>
#include <errno.h>

#include <nuttx/arch.h>

#include "sched/sched.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sched_getscheduler
 *
 * Description:
 *   sched_getscheduler() returns the scheduling policy currently
 *   applied to the task identified by pid. If pid equals zero, the
 *   policy of the calling task will be retrieved.
 *
 * Inputs:
 *   pid - the task ID of the task to query.  If pid is zero, the
 *     calling task is queried.
 *
 * Return Value:
 *    On success, sched_getscheduler() returns the policy for the task
 *    (either SCHED_FIFO or SCHED_RR).  On error,  ERROR (-1) is
 *    returned, and errno is set appropriately:
 *
 *      ESRCH  The task whose ID is pid could not be found.
 *
 * Assumptions:
 *
 ****************************************************************************/

int sched_getscheduler(pid_t pid)
{
  FAR struct tcb_s *tcb;
  int policy;

  /* Verify that the PID corresponds to a real task */

  if (!pid)
    {
      tcb = this_task();
    }
  else
    {
      tcb = sched_gettcb(pid);
    }

  if (!tcb)
    {
      set_errno(ESRCH);
      return ERROR;
    }

  /* Return the scheduling policy from the TCB.  NOTE that the user-
   * interpretable values are 1 based; the TCB values are zero-based.
   */

  policy = (tcb->flags & TCB_FLAG_POLICY_MASK) >> TCB_FLAG_POLICY_SHIFT;
  return policy + 1;
}
