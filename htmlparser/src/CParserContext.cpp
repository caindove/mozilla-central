/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL. 
 *  
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation.  All Rights
 * Reserved.
 */


#include "CParserContext.h"
#include "nsToken.h"

MOZ_DECL_CTOR_COUNTER(CParserContext);
/**
 * Your friendly little constructor. Ok, it's not the friendly, but the only guy
 * using it is the parser.
 * @update	gess7/23/98
 * @param   aScanner
 * @param   aKey
 * @param   aListener
 */
CParserContext::CParserContext(nsScanner* aScanner,void* aKey,nsIStreamObserver* aListener) :
  mSourceType()
  //,mTokenDeque(gTokenDeallocator)
{
  MOZ_COUNT_CTOR(CParserContext);

  mScanner=aScanner;
  mKey=aKey;
  mPrevContext=0;
  mListener=aListener;
  NS_IF_ADDREF(mListener);
  mParseMode=eParseMode_unknown;
  mAutoDetectStatus=eUnknownDetect;
  mTransferBuffer=0;
  mDTD=0;
  mTransferBufferSize=eTransferBufferSize;
  mParserEnabled=PR_TRUE;
  mStreamListenerState=eNone;
  mMultipart=PR_TRUE;
  mContextType=eCTNone;
}


/**
 * Destructor for parser context
 * NOTE: DO NOT destroy the dtd here.
 * @update	gess7/11/98
 */
CParserContext::~CParserContext(){

  MOZ_COUNT_DTOR(CParserContext);

  if(mScanner)
    delete mScanner;

  if(mTransferBuffer)
    delete [] mTransferBuffer;

  NS_IF_RELEASE(mDTD);

  //Remember that it's ok to simply ingore the PrevContext.

}

