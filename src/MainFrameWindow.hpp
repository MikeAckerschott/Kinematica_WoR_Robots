#ifndef MAINFRAMEWINDOW_HPP_
#define MAINFRAMEWINDOW_HPP_

#include "Config.hpp"

#include "Widgets.hpp"
#include <wx/wx.h>

#include "CompassSensor.hpp"
#include "LidarDistanceSensor.hpp"
#include "OdometerSensor.hpp"

namespace Base
{
	class ITraceFunction;
} // namespace BaseBase

namespace View
{
	class RobotWorldCanvas;
}

namespace Application
{
	class LogTextCtrl;

	/**
	 *
	 */
	class MainFrameWindow : public wxFrame
	{
	public:
		/**
		 *
		 * @param aTitle The title which is shown in the title bar
		 */
		explicit MainFrameWindow(const std::string &aTitle);

	protected:
		/**
		 *
		 */
		void initialise();
		/**
		 *
		 */
		wxMenuBar *initialiseMenuBar();
		/**
		 *
		 */
		wxPanel *initialiseClientPanel();
		/**
		 *
		 */
		wxSplitterWindow *initialiseSplitterWindow();
		/**
		 *
		 */
		wxPanel *initialiseLhsPanel();
		/**
		 *
		 */
		wxPanel *initialiseRhsPanel();
		/**
		 *
		 */
		wxPanel *initialiseLogPanel();
		/**
		 *
		 */
		wxPanel *initialiseButtonPanel();

	protected:
	private:
		wxPanel *clientPanel;
		wxMenuBar *menuBar;
		wxSplitterWindow *splitterWindow;

		wxStaticText *compassDeviationLabel;
		wxStaticText *lidarDeviationLabel;
		wxStaticText *odomDeviationLabel;

		wxPanel *lhsPanel;
		View::RobotWorldCanvas *robotWorldCanvas;

		wxPanel *rhsPanel;

		wxPanel *logPanel;
		LogTextCtrl *logTextCtrl;
		wxRadioBox *logDestination;

		wxPanel *buttonPanel;

		void OnQuit(wxCommandEvent &anEvent);
		void OnWidgetTraceFunction(wxCommandEvent &anEvent);
		void OnStdOutTraceFunction(wxCommandEvent &anEvent);
		void OnFileTraceFunction(wxCommandEvent &anEvent);
		void OnAbout(wxCommandEvent &anEvent);

		void OnStartRobot(wxCommandEvent &anEvent);
		void OnStopRobot(wxCommandEvent &anEvent);
		void OnPopulate(wxCommandEvent &anEvent);
		void OnUnpopulate(wxCommandEvent &anEvent);
		void OnStartListening(wxCommandEvent &anEvent);
		void OnSendMessage(wxCommandEvent &anEvent);
		void OnStopListening(wxCommandEvent &anEvent);

		void OnParticleFilterSelection(wxCommandEvent &anEvent);
		void OnKalmanFilterSelection(wxCommandEvent &anEvent);

		void OnCompassDeviationSlider(wxScrollEvent &anEvent);
		void OnLidarDeviationSlider(wxScrollEvent &anEvent);
		void OnOdomDeviationSlider(wxScrollEvent &anEvent);
	};
	//	class MainFrameWindow
} // namespace Application

#endif // MAINFRAMEWINDOW_HPP_
