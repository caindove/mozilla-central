/* -*- Mode: java; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
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
 * Copyright (C) 1997-1999 Netscape Communications Corporation.  All Rights
 * Reserved.
 */

// DEBUG API class

package org.mozilla.javascript.debug;

/**
 * This interface supports immediately interrupting execution.
 * <p>
 * This interface can be implemented and used to stop execution
 * of compiled JavaScript code immediately at any program location.
 * 
 * @see org.mozilla.javascript.debug.IDebugManager#setInterruptHook
 * @author John Bandhauer
 */

public interface IInterruptHook
{
    /**
    * Method called when interrupt hit.
    *
    * @param debug thread state of the stopped target thread
    * @param pc current program location (somewhat redundant)
    */
    public void aboutToExecute(IThreadState state, IPC pc);
}    
