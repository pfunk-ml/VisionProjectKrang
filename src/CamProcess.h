/**
 * @file CamProcess.h
 * @author: Nehchal J. (nehchal@gatech.edu)
 * Created On: Dec 28, 2015
 *
 * Dependencies:
    . basicFilter2
 */

#include <platform/CaptureFactory.h>
#include <MarkerDetector.h>

#include <ach.h>

class CamProcess {

  public:

    CamProcess();
    ~CamProcess();

    /* Repeat at every captured frame */
    static void videocallback(IplImage *_img);

    /* Initialize required parameters
     *  _devIndex: [IN] TODO: Write description
     *  _camIndex: [IN] TODO: Write description
     *  _cap: [IN]
     *  verbosity: [IN] Prints verbose message if non-zero.
     *  quit_signal_ptr: [IN] Reference to quit_signal. The application exits 
     *    whenever quit_signal becomes zero.
     *  vis: [IN] If true, visualization is On. Off, if false.
     *  channel_name: [IN] The name of channel to publish data to. */
    bool init( int _devIndex,
           int _camIndex,
           alvar::Capture **_cap,
           int verbosity,
           volatile int *quit_signal_ptr,
           bool vis,
           std::string channel_name);

  private:
    ach_channel_t initAchChannel(const char* channelName);
    int getIndex(int markerID);
};