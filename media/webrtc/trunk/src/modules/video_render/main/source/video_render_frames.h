/*
 *  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_MODULES_VIDEO_RENDER_MAIN_SOURCE_VIDEO_RENDER_FRAMES_H_
#define WEBRTC_MODULES_VIDEO_RENDER_MAIN_SOURCE_VIDEO_RENDER_FRAMES_H_

#include "list_wrapper.h"
#include "video_render.h"

namespace webrtc {

// Class definitions
class VideoRenderFrames
{
public:
    VideoRenderFrames();
    ~VideoRenderFrames();

    /*
     *   Add a frame to the render queue
     */
    WebRtc_Word32 AddFrame(VideoFrame* ptrNewFrame);

    /*
     *   Get a frame for rendering, if it's time to render.
     */
    VideoFrame* FrameToRender();

    /*
     *   Return an old frame
     */
    WebRtc_Word32 ReturnFrame(VideoFrame* ptrOldFrame);

    /*
     *   Releases all frames
     */
    WebRtc_Word32 ReleaseAllFrames();

    /*
     *   Returns the number of ms to next frame to render
     */
    WebRtc_UWord32 TimeToNextFrameRelease();

    /*
     *   Sets estimates delay in renderer
     */
    WebRtc_Word32 SetRenderDelay(const WebRtc_UWord32 renderDelay);

private:
    enum
    {
        KMaxNumberOfFrames = 300
    }; // 10 seconds for 30 fps.
    enum
    {
        KOldRenderTimestampMS = 500
    }; //Don't render frames with timestamp older than 500ms from now.
    enum
    {
        KFutureRenderTimestampMS = 10000
    }; //Don't render frames with timestamp more than 10s into the future.

    ListWrapper _incomingFrames; // Sorted oldest video frame first
    ListWrapper _emptyFrames; // Empty frames

    WebRtc_UWord32 _renderDelayMs; // Set render delay
};

} //namespace webrtc

#endif  // WEBRTC_MODULES_VIDEO_RENDER_MAIN_SOURCE_VIDEO_RENDER_FRAMES_H_
