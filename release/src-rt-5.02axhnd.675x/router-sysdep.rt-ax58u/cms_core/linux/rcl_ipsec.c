/***********************************************************************
 *
 *  Copyright (c) 2006-2008  Broadcom Corporation
 *  All Rights Reserved
 *
<:label-BRCM:2012:proprietary:standard

 This program is the proprietary software of Broadcom and/or its
 licensors, and may only be used, duplicated, modified or distributed pursuant
 to the terms and conditions of a separate, written license agreement executed
 between you and Broadcom (an "Authorized License").  Except as set forth in
 an Authorized License, Broadcom grants no license (express or implied), right
 to use, or waiver of any kind with respect to the Software, and Broadcom
 expressly reserves all rights in and to the Software and all intellectual
 property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE
 NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY
 BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.

 Except as expressly set forth in the Authorized License,

 1. This program, including its structure, sequence and organization,
    constitutes the valuable trade secrets of Broadcom, and you shall use
    all reasonable efforts to protect the confidentiality thereof, and to
    use this information only in connection with your use of Broadcom
    integrated circuit products.

 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
    RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND
    ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT,
    FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR
    COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE
    TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE OR
    PERFORMANCE OF THE SOFTWARE.

 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR
    ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
    INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY
    WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN
    IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES;
    OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
    SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
    SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY
    LIMITED REMEDY.
:>
 *
 ************************************************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <net/if.h>
#include <net/route.h>
#include <fcntl.h>
#include <unistd.h>

#include "cms_core.h"
#include "cms_util.h"
#include "rcl.h"
#include "rut_util.h"
#include "rut_ipsec.h"

CmsRet rcl_ipSecCfgObject( _IPSecCfgObject *newObj __attribute__((unused)),
                const _IPSecCfgObject *currObj __attribute__((unused)),
                const InstanceIdStack *iidStack __attribute__((unused)),
                char **errorParam __attribute__((unused)),
                CmsRet *errorCode __attribute__((unused)))
{

#ifdef DMP_X_BROADCOM_COM_IPSEC_1  /* aka SUPPORT_PMAP */
   CmsRet ret = CMSRET_SUCCESS;

   if ((ret = rut_validateObjects(newObj, currObj)) != CMSRET_SUCCESS)
   {
      printf("rcl_ipSecCfgObject: rut_validateObjects returns error. ret=%d", ret);
      cmsLog_error("rut_validateObjects returns error. ret=%d", ret);
      return ret;
   }

   if (newObj != NULL && currObj == NULL)
   {
      /*
       * We are booting up so start strongSwan and run setkey.sh script
       */
      if (newObj->connName) {
         if ((ret = rutIPSec_config()) != CMSRET_SUCCESS) {
            printf("rcl_ipSecCfgObject: rutIPSec_config failed, ret=%d", ret);
            cmsLog_error("rutIPSec_config failed, ret=%d", ret);
            return ret;
         }
         if ((ret = rutIPSec_restart()) != CMSRET_SUCCESS) {
            printf("rcl_ipSecCfgObject: rutIPSec_restart failed, ret=%d", ret);
            cmsLog_error("rutIPSec_restart failed, ret=%d", ret);
            return ret;
         }
      }
   }
   else if (newObj != NULL && currObj != NULL)
   {
      // Edit the current instance.  No edit of this table is allowed.
      ;
   }
   else
   {
      /*
       * We deleted a tunnel, restart strongSwan and rerun setkey.sh script
       */
      if ((ret = rutIPSec_config()) != CMSRET_SUCCESS) {
         printf("rcl_ipSecCfgObject: Delete rutIPSec_config failed, ret=%d", ret);
         cmsLog_error("rutIPSec_config failed, ret=%d", ret);
         return ret;
      }
      if ((ret = rutIPSec_restart()) != CMSRET_SUCCESS) {
         printf("rcl_ipSecCfgObject: Delete rutIPSec_restart failed, ret=%d", ret);
         cmsLog_error("rutIPSec_restart failed, ret=%d", ret);
         return ret;
      }
   }

#endif /* DMP_X_BROADCOM_COM_IPSEC_1 */
   return CMSRET_SUCCESS;
}

