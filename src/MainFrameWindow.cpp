#include "MainFrameWindow.hpp"

#include "Button.hpp"
#include "Checkbox.hpp"
#include "Client.hpp"
#include "FileTraceFunction.hpp"
#include "LaserDistanceSensor.hpp"
#include "LogTextCtrl.hpp"
#include "Logger.hpp"
#include "MainApplication.hpp"
#include "MathUtils.hpp"
#include "Message.hpp"
#include "MessageTypes.hpp"
#include "Radiobox.hpp"
#include "Robot.hpp"
#include "RobotWorld.hpp"
#include "RobotWorldCanvas.hpp"
#include "Shape2DUtils.hpp"
#include "StdOutTraceFunction.hpp"
#include "Trace.hpp"
#include "WidgetTraceFunction.hpp"

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <iostream>

namespace Application
{
  /**
   * IDs for the controls and the menu commands
   * If there are (default) wxWidget ID's: try to maintain
   * compatibility, especially wxID_ABOUT because on a Mac it is special
   */
  enum
  {
    ID_QUIT = wxID_EXIT,          //!< ID_QUIT
    ID_OPTIONS = wxID_PROPERTIES, //!< ID_OPTIONS
    ID_ABOUT = wxID_ABOUT,        //!< ID_ABOUT
    ID_WIDGET_TRACE_FUNCTION,     //!< ID_WIDGET_TRACE_FUNCTION
    ID_STDCOUT_TRACE_FUNCTION,    //!< ID_STDCOUT_TRACE_FUNCTION
    ID_FILE_TRACE_FUNCTION        //!< ID_FILE_TRACE_FUNCTION
  };
  /**
   *
   */
  MainFrameWindow::MainFrameWindow(const std::string &aTitle)
      : wxFrame(nullptr, wxID_ANY, aTitle, wxDefaultPosition, wxSize(1200, 600)),
        clientPanel(nullptr), menuBar(nullptr), splitterWindow(nullptr),
        lhsPanel(nullptr), robotWorldCanvas(nullptr), rhsPanel(nullptr),
        logTextCtrl(nullptr), logDestination(nullptr), buttonPanel(nullptr)
  {
    initialise();
  }
  /**
   *
   */
  void MainFrameWindow::initialise()
  {
    SetMenuBar(initialiseMenuBar());

    wxGridBagSizer *sizer = new wxGridBagSizer(5, 5);

    sizer->Add(initialiseClientPanel(), wxGBPosition(0, 0), // row ,col
               wxGBSpan(1, 1),                              // row ,col
               wxGROW);

    sizer->AddGrowableCol(0);
    sizer->AddGrowableRow(0);

    sizer->SetSizeHints(this);

    Bind(
        wxEVT_COMMAND_MENU_SELECTED,
        [this](wxCommandEvent &anEvent)
        { this->OnQuit(anEvent); },
        ID_QUIT);
    Bind(
        wxEVT_COMMAND_MENU_SELECTED,
        [this](wxCommandEvent &anEvent)
        { this->OnWidgetTraceFunction(anEvent); },
        ID_WIDGET_TRACE_FUNCTION);
    Bind(
        wxEVT_COMMAND_MENU_SELECTED,
        [this](wxCommandEvent &anEvent)
        { this->OnStdOutTraceFunction(anEvent); },
        ID_STDCOUT_TRACE_FUNCTION);
    Bind(
        wxEVT_COMMAND_MENU_SELECTED,
        [this](wxCommandEvent &anEvent)
        { this->OnFileTraceFunction(anEvent); },
        ID_FILE_TRACE_FUNCTION);
    Bind(
        wxEVT_COMMAND_MENU_SELECTED,
        [this](wxCommandEvent &anEvent)
        { this->OnAbout(anEvent); },
        ID_ABOUT);

    // By default we use the WidgettraceFunction as we expect that this is what
    // the user wants....
    Base::Trace::setTraceFunction(
        std::make_unique<Application::WidgetTraceFunction>(logTextCtrl));
  }
  /**
   *
   */
  wxMenuBar *MainFrameWindow::initialiseMenuBar()
  {
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(ID_QUIT, "E&xit\tAlt-X", "Exit the application");

    wxMenu *debugMenu = new wxMenu;
    debugMenu->AppendRadioItem(ID_WIDGET_TRACE_FUNCTION, "Widget", "Widget");
    debugMenu->AppendRadioItem(ID_STDCOUT_TRACE_FUNCTION, "StdOut", "StdOut");
    debugMenu->AppendRadioItem(ID_FILE_TRACE_FUNCTION, "File", "File");

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(ID_ABOUT, "&About...\tF1", "Show about dialog");

    menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(debugMenu, "&Debug");
    menuBar->Append(helpMenu, "&Help");

    return menuBar;
  }
  /**
   *
   */
  wxPanel *MainFrameWindow::initialiseClientPanel()
  {
    if (!clientPanel)
    {
      clientPanel = new wxPanel(this);

      wxGridBagSizer *sizer = new wxGridBagSizer();

      sizer->Add(5, 5, wxGBPosition(0, 0));
      sizer->AddGrowableRow(0);
      sizer->AddGrowableCol(0);

      sizer->Add(initialiseSplitterWindow(), wxGBPosition(1, 1), wxGBSpan(1, 1),
                 wxGROW);

      sizer->Add(5, 5, wxGBPosition(2, 2));
      sizer->AddGrowableRow(2);
      sizer->AddGrowableCol(2);

      clientPanel->SetSizer(sizer);
    }
    return clientPanel;
  }
  /**
   *
   */
  wxSplitterWindow *MainFrameWindow::initialiseSplitterWindow()
  {
    if (!splitterWindow)
    {
      splitterWindow = new wxSplitterWindow(clientPanel);
      splitterWindow->SplitVertically(initialiseLhsPanel(), initialiseRhsPanel());
    }
    return splitterWindow;
  }
  /**
   *
   */
  wxPanel *MainFrameWindow::initialiseLhsPanel()
  {
    if (!lhsPanel)
    {
      lhsPanel = new wxPanel(splitterWindow, wxID_ANY, wxDefaultPosition,
                             wxDefaultSize, wxBORDER_SUNKEN);

      wxGridBagSizer *sizer = new wxGridBagSizer();
      sizer->Add(5, 5, wxGBPosition(0, 0), wxGBSpan(1, 1), wxGROW);

      sizer->Add(robotWorldCanvas = new View::RobotWorldCanvas(lhsPanel),
                 wxGBPosition(1, 1), wxGBSpan(1, 1), wxGROW);
      robotWorldCanvas->SetMinSize(wxSize(1000, 1000));

      sizer->Add(5, 5, wxGBPosition(2, 2), wxGBSpan(1, 1), wxGROW);

      lhsPanel->SetSizer(sizer);
      sizer->SetSizeHints(lhsPanel);
    }
    return lhsPanel;
  }
  /**
   *
   */
  wxPanel *MainFrameWindow::initialiseRhsPanel()
  {
    if (!rhsPanel)
    {
      rhsPanel = new wxPanel(splitterWindow);

      wxGridBagSizer *sizer = new wxGridBagSizer();

      sizer->Add(5, 5, wxGBPosition(0, 0), wxGBSpan(1, 1), wxGROW);

      sizer->Add(logPanel = initialiseLogPanel(), wxGBPosition(1, 1),
                 wxGBSpan(1, 1), wxGROW);
      sizer->AddGrowableCol(1);
      sizer->AddGrowableRow(1);

      sizer->Add(buttonPanel = initialiseButtonPanel(), wxGBPosition(2, 1),
                 wxGBSpan(1, 1), wxSHRINK);

      sizer->Add(5, 5, wxGBPosition(3, 2), wxGBSpan(1, 1), wxGROW);

      rhsPanel->SetSizer(sizer);
      sizer->SetSizeHints(rhsPanel);
    }
    return rhsPanel;
  }
  /**
   *
   */
  wxPanel *MainFrameWindow::initialiseLogPanel()
  {
    wxPanel *panel = new wxPanel(rhsPanel);

    wxGridBagSizer *sizer = new wxGridBagSizer();

    sizer->Add(5, 5, wxGBPosition(0, 0), wxGBSpan(1, 1), wxGROW);

    std::array<std::string, 3> choicesArray{"Window", "StdOut", "File"};

    sizer->Add(logDestination = makeRadiobox(
                   panel, choicesArray,
                   [this](wxCommandEvent &event)
                   {
                     wxRadioBox *radiobox =
                         dynamic_cast<wxRadioBox *>(event.GetEventObject());
                     if (radiobox)
                     {
                       switch (radiobox->GetSelection())
                       {
                       case 0:
                       {
                         OnWidgetTraceFunction(event);

                         break;
                       }
                       case 1:
                       {
                         OnStdOutTraceFunction(event);
                         break;
                       }
                       case 2:
                       {
                         OnFileTraceFunction(event);
                         break;
                       }
                       default:
                       {
                         TRACE_DEVELOP("Unknown trace destination");
                       }
                       }
                     }
                   },
                   "Log destination", wxRA_SPECIFY_COLS),
               wxGBPosition(1, 1), wxGBSpan(1, 1), wxALIGN_CENTER);

    sizer->Add(logTextCtrl = new LogTextCtrl(panel, wxID_ANY,
                                             wxTE_MULTILINE | wxTE_DONTWRAP),
               wxGBPosition(2, 1), wxGBSpan(1, 1), wxGROW);
    sizer->AddGrowableCol(1);
    sizer->AddGrowableRow(2);
    logTextCtrl->SetMinSize(wxSize(500, 300));

    sizer->Add(makeButton(panel, "Clear log window",
                          [this](wxCommandEvent & /*anEvent*/)
                          {
                            logTextCtrl->Clear();
                          }),
               wxGBPosition(3, 1), wxGBSpan(1, 1), wxALIGN_CENTER);

    sizer->Add(5, 5, wxGBPosition(4, 2), wxGBSpan(1, 1), wxGROW);

    panel->SetSizerAndFit(sizer);

    return panel;
  }
  /**
   *
   */
  wxPanel *MainFrameWindow::initialiseButtonPanel()
  {
    wxPanel *panel = new wxPanel(rhsPanel);

    wxGridBagSizer *sizer = new wxGridBagSizer();

    sizer->Add(5, 5, wxGBPosition(0, 0), wxGBSpan(1, 1), wxGROW);

    sizer->Add(makeButton(panel, "Populate",
                          [this](wxCommandEvent &anEvent)
                          {
                            this->OnPopulate(anEvent);
                          }),
               wxGBPosition(1, 1), wxGBSpan(1, 1), wxGROW);
    sizer->Add(makeButton(panel, "Unpopulate",
                          [this](wxCommandEvent &anEvent)
                          {
                            this->OnUnpopulate(anEvent);
                          }),
               wxGBPosition(1, 2), wxGBSpan(1, 1), wxGROW);

    sizer->Add(makeButton(panel, "Start robot",
                          [this](wxCommandEvent &anEvent)
                          {
                            this->OnStartRobot(anEvent);
                          }),
               wxGBPosition(1, 5), wxGBSpan(1, 1), wxGROW);
    sizer->Add(makeButton(panel, "Stop robot",
                          [this](wxCommandEvent &anEvent)
                          {
                            this->OnStopRobot(anEvent);
                          }),
               wxGBPosition(1, 6), wxGBSpan(1, 1), wxGROW);

    sizer->Add(makeButton(panel, "Particle filter",
                          [this](wxCommandEvent &anEvent)
                          {
                            this->OnParticleFilterSelection(anEvent);
                          }),
               wxGBPosition(1, 3), wxGBSpan(1, 1), wxGROW);

    sizer->Add(makeButton(panel, "Kalman filter",
                          [this](wxCommandEvent &anEvent)
                          {
                            this->OnKalmanFilterSelection(anEvent);
                          }),
               wxGBPosition(1, 4), wxGBSpan(1, 1), wxGROW);

    // add slider for compass deviation
    wxSlider *compassSlider = new wxSlider(panel, wxID_ANY, Model::CompassSensor::getStdDev(), 0, 20, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);

    Bind(
        wxEVT_SCROLL_THUMBTRACK,
        [this](wxScrollEvent &anEvent)
        {
          this->OnCompassDeviationSlider(anEvent);
        },
        compassSlider->GetId());

    sizer->Add(compassSlider, wxGBPosition(4, 1), wxGBSpan(1, 2), wxGROW);

    compassDeviationLabel = new wxStaticText(panel, wxID_ANY, "Compass deviation: " + std::to_string(Model::CompassSensor::getStdDev()));
    sizer->Add(compassDeviationLabel, wxGBPosition(3, 1), wxGBSpan(1, 2), wxGROW);

    // add slider for Lidar deviation
    wxSlider *lidarSlider = new wxSlider(panel, wxID_ANY, Model::LidarDistanceSensor::getStdDev(), 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);

    Bind(
        wxEVT_SCROLL_THUMBTRACK,
        [this](wxScrollEvent &anEvent)
        {
          this->OnLidarDeviationSlider(anEvent);
        },
        lidarSlider->GetId());

    sizer->Add(lidarSlider, wxGBPosition(4, 3), wxGBSpan(1, 2), wxGROW);

    lidarDeviationLabel = new wxStaticText(panel, wxID_ANY, "Lidar deviation: " + std::to_string(Model::LidarDistanceSensor::getStdDev()));
    sizer->Add(lidarDeviationLabel, wxGBPosition(3, 3), wxGBSpan(1, 2), wxGROW);

    wxSlider *odomSlider = new wxSlider(panel, wxID_ANY, Model::OdometerSensor::getStdDev() * 10.0, 0, 50, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);

    Bind(
        wxEVT_SCROLL_THUMBTRACK,
        [this](wxScrollEvent &anEvent)
        {
          this->OnOdomDeviationSlider(anEvent);
        },
        odomSlider->GetId());

    sizer->Add(odomSlider, wxGBPosition(4, 5), wxGBSpan(1, 2), wxGROW);

    odomDeviationLabel = new wxStaticText(panel, wxID_ANY, "Odometer deviation: " + std::to_string(Model::OdometerSensor::getStdDev()));
    sizer->Add(odomDeviationLabel, wxGBPosition(3, 5), wxGBSpan(1, 2), wxGROW);

    sizer->Add(5, 5, wxGBPosition(6, 4), wxGBSpan(1, 1), wxGROW);

    panel->SetSizerAndFit(sizer);

    return panel;
  }
  /**
   *
   */
  void MainFrameWindow::OnQuit(wxCommandEvent &UNUSEDPARAM(anEvent))
  {
    Base::Trace::setTraceFunction(std::make_unique<Base::StdOutTraceFunction>());
    Close(true);
  }
  /**
   *
   */
  void MainFrameWindow::OnWidgetTraceFunction(
      wxCommandEvent &UNUSEDPARAM(anEvent))
  {
    Base::Trace::setTraceFunction(
        std::make_unique<Application::WidgetTraceFunction>(logTextCtrl));

    wxMenuItem *item = menuBar->FindItem(ID_WIDGET_TRACE_FUNCTION);
    if (item && item->IsRadio() && !item->IsCheck())
    {
      item->Check();
    }
    logDestination->SetSelection(0);
  }
  /**
   *
   */
  void MainFrameWindow::OnStdOutTraceFunction(
      wxCommandEvent &UNUSEDPARAM(anEvent))
  {
    Base::Trace::setTraceFunction(std::make_unique<Base::StdOutTraceFunction>());

    wxMenuItem *item = menuBar->FindItem(ID_STDCOUT_TRACE_FUNCTION);
    if (item && item->IsRadio() && !item->IsCheck())
    {
      item->Check();
    }
    logDestination->SetSelection(1);
  }
  /**
   *
   */
  void MainFrameWindow::OnFileTraceFunction(
      wxCommandEvent &UNUSEDPARAM(anEvent))
  {
    Base::Trace::setTraceFunction(
        std::make_unique<Base::FileTraceFunction>("trace", "log", true));

    wxMenuItem *item = menuBar->FindItem(ID_FILE_TRACE_FUNCTION);
    if (item && item->IsRadio() && !item->IsCheck())
    {
      item->Check();
    }
    logDestination->SetSelection(2);
  }
  /**
   *
   */
  void MainFrameWindow::OnAbout(wxCommandEvent &UNUSEDPARAM(anEvent))
  {
    wxMessageBox("ESD 2012-present RobotWorld.\n", "About RobotWorld",
                 wxOK | wxICON_INFORMATION, this);
  }
  /**
   *
   */
  void MainFrameWindow::OnStartRobot(wxCommandEvent &UNUSEDPARAM(anEvent))
  {
    Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot("Robot");
    if (robot && !robot->isActing())
    {
      robot->startActing();
    }
  }
  /**
   *
   */
  void MainFrameWindow::OnStopRobot(wxCommandEvent &UNUSEDPARAM(anEvent))
  {
    Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot("Robot");
    if (robot && robot->isActing())
    {
      robot->stopActing();
    }
  }
  /**
   *
   */
  void MainFrameWindow::OnPopulate(wxCommandEvent &UNUSEDPARAM(anEvent))
  {
    robotWorldCanvas->populate(4);
  }
  /**
   *
   */
  void MainFrameWindow::OnUnpopulate(wxCommandEvent &UNUSEDPARAM(anEvent))
  {
    robotWorldCanvas->unpopulate();

    logTextCtrl->Clear();
  }
  /**
   *
   */
  void MainFrameWindow::OnStartListening(wxCommandEvent &UNUSEDPARAM(anEvent))
  {
    Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot("Robot");
    if (robot)
    {
      robot->startCommunicating();
    }
  }
  /**
   *
   */
  void MainFrameWindow::OnSendMessage(wxCommandEvent &UNUSEDPARAM(anEvent))
  {
    Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot("Robot");
    if (robot)
    {
      std::string remoteIpAdres = "localhost";
      std::string remotePort = "12345";

      if (MainApplication::isArgGiven("-remote_ip"))
      {
        remoteIpAdres = MainApplication::getArg("-remote_ip").value;
      }
      if (MainApplication::isArgGiven("-remote_port"))
      {
        remotePort = MainApplication::getArg("-remote_port").value;
      }

      // We will request an echo message. The response will be "Hello World", if
      // all goes OK, "Goodbye cruel world!" if something went wrong.
      Messaging::Client c1ient(remoteIpAdres,
                               static_cast<unsigned short>(std::stoi(remotePort)),
                               robot);
      Messaging::Message message(Messaging::EchoRequest, "Hello world!");
      c1ient.dispatchMessage(message);
    }
  }
  /**
   *
   */
  void MainFrameWindow::OnStopListening(wxCommandEvent &UNUSEDPARAM(anEvent))
  {
    Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot("Robot");
    if (robot)
    {
      robot->stopCommunicating();
    }
  }

  void MainFrameWindow::OnParticleFilterSelection(
      wxCommandEvent &UNUSEDPARAM(anEvent))
  {
    Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot("Robot");
    if (robot)
    {
      robot->setParticleFilterActive(!robot->isParticleFilterActive());
      if (robot->isParticleFilterActive())
      {
        Logger::log("Particle filter enabled");
      }
      else
      {
        Logger::log("Particle filter disabled");
      }
    }
  }

  void MainFrameWindow::OnKalmanFilterSelection(
      wxCommandEvent &UNUSEDPARAM(anEvent))
  {
    Model::RobotPtr robot = Model::RobotWorld::getRobotWorld().getRobot("Robot");
    if (robot)
    {
      robot->setKalmanFilterActive(!robot->isKalmanFilterActive());
      if (robot->isKalmanFilterActive())
      {
        Logger::log("Kalman filter enabled");
      }
      else
      {
        Logger::log("Kalman filter disabled");
      }
    }
  }

  void MainFrameWindow::OnCompassDeviationSlider(wxScrollEvent &anEvent)
  {
    // get value from anEvent
    double value = anEvent.GetPosition();
    Model::CompassSensor::setStdDev(value);

    // update label
    compassDeviationLabel->SetLabel("Compass deviation: " + std::to_string(Model::CompassSensor::getStdDev()));
  }
  void MainFrameWindow::OnLidarDeviationSlider(wxScrollEvent &anEvent)
  {
    // get value from anEvent
    double value = anEvent.GetPosition();
    Model::LidarDistanceSensor::setStdDev(value);

    // update label
    lidarDeviationLabel->SetLabel("Lidar deviation: " + std::to_string(Model::LidarDistanceSensor::getStdDev()));

  }

  void MainFrameWindow::OnOdomDeviationSlider(wxScrollEvent &anEvent)
  {
    // get value from anEvent
    double value = anEvent.GetPosition() / 10.0;
    Model::OdometerSensor::setStdDev(value);

    // update label
    odomDeviationLabel->SetLabel("Odometer deviation: " + std::to_string(Model::OdometerSensor::getStdDev()));

  }

} // namespace Application
