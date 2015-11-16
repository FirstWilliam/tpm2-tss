/***********************************************************************;
 * Copyright (c) 2015, Intel Corporation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, 
 * this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
 * THE POSSIBILITY OF SUCH DAMAGE.
 ***********************************************************************/

#include <tpm20.h>   
#include <tss2_sysapi_util.h>   

TPM_RC Tss2_Sys_PolicyTicket_Prepare(
    TSS2_SYS_CONTEXT *sysContext,
    TPMI_SH_POLICY	policySession,
    TPM2B_TIMEOUT	*timeout,
    TPM2B_DIGEST	*cpHashA,
    TPM2B_NONCE	*policyRef,
    TPM2B_NAME	*authName,
    TPMT_TK_AUTH	*ticket
    )
{
    if( sysContext == NULL )
    {
        return( TSS2_SYS_RC_BAD_REFERENCE );
    }

    SYS_CONTEXT->rval = TSS2_RC_SUCCESS;
    
    CommonPreparePrologue( sysContext, TPM_CC_PolicyTicket );

    Marshal_UINT32( SYS_CONTEXT->tpmInBuffPtr, SYS_CONTEXT->maxCommandSize, &(SYS_CONTEXT->nextData), policySession, &(SYS_CONTEXT->rval) );

    if( timeout == 0 )
	{
		SYS_CONTEXT->decryptNull = 1;
	}
            
    MARSHAL_SIMPLE_TPM2B( sysContext, &( timeout->b ) );

    MARSHAL_SIMPLE_TPM2B( sysContext, &( cpHashA->b ) );

    MARSHAL_SIMPLE_TPM2B( sysContext, &( policyRef->b ) );

    MARSHAL_SIMPLE_TPM2B( sysContext, &( authName->b ) );

    Marshal_TPMT_TK_AUTH( sysContext, ticket );

    SYS_CONTEXT->decryptAllowed = 1;
    SYS_CONTEXT->encryptAllowed = 0;
    SYS_CONTEXT->authAllowed = 1;

    CommonPrepareEpilogue( sysContext );

    return SYS_CONTEXT->rval;
}


TPM_RC Tss2_Sys_PolicyTicket(
    TSS2_SYS_CONTEXT *sysContext,
    TPMI_SH_POLICY	policySession,
    TSS2_SYS_CMD_AUTHS const *cmdAuthsArray,
    TPM2B_TIMEOUT	*timeout,
    TPM2B_DIGEST	*cpHashA,
    TPM2B_NONCE	*policyRef,
    TPM2B_NAME	*authName,
    TPMT_TK_AUTH	*ticket,
    TSS2_SYS_RSP_AUTHS *rspAuthsArray
    )
{
    TSS2_RC     rval = TPM_RC_SUCCESS;

    if( sysContext == NULL || timeout == NULL || cpHashA == NULL || policyRef == NULL || authName == NULL || ticket == NULL  )
    {
        return( TSS2_SYS_RC_BAD_REFERENCE );
    }

    rval = Tss2_Sys_PolicyTicket_Prepare( sysContext, policySession, timeout, cpHashA, policyRef, authName, ticket );
    
    if( rval == TSS2_RC_SUCCESS )
    {
        rval = CommonOneCallForNoResponseCmds( sysContext, cmdAuthsArray, rspAuthsArray );
    }
    
    return rval;
}

