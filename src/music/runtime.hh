/*
 *  This file is part of MUSIC.
 *  Copyright (C) 2007, 2008, 2009 INCF
 *
 *  MUSIC is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  MUSIC is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MUSIC_RUNTIME_HH
//#define MUSIC_DEBUG
#include "music/music-config.hh"
#if MUSIC_USE_MPI
#include <mpi.h>
#include <vector>

#include "music/misc.hh"
/* #include "music/setup.hh" */
#include "music/port.hh"
#include "music/clock.hh"
#include "music/connector.hh"
#include "music/scheduler.hh"
#include "music/scheduler_agent.hh"

namespace MUSIC
{

  /*
   * This is the Runtime object in the MUSIC API
   *
   * It is documented in section 4.4 of the MUSIC manual
   */

  using Connections = std::vector<Connection*>;

  class Application;
  class PortConnectivityManager;

  class Runtime
  {
  public:
    Runtime (Application& app, PortConnectivityManager& portMgr, double h);
	Runtime (const Runtime& other);

    ~Runtime ();

    MPI::Intracomm
    communicator ();

    void
    finalize ();
    void
    tick ();
    double
    time ();

  private:
	Application& app_;
	PortConnectivityManager& portMgr_;
    Clock localTime;
    //Clock nextComm;
    /* std::string app_name; */
    TemporalNegotiator temporalNegotiator_;
    int leader_;
    std::vector<std::pair<double, Connector *> > schedule;
    MPI::Intracomm comm;
    SPVec<Port> ports;
	SPVec<TickingPort> tickingPorts;
    std::vector<Connector*> connectors;
    std::vector<PostCommunicationConnector*> postCommunication;
    std::vector<PreCommunicationConnector*> preCommunication;
    Scheduler *scheduler;
    std::vector<SchedulerAgent*> sAgents;
    MulticommAgent* mAgent;

	void setup ();

    bool needsMultiCommunication ();
    void
    takeTickingPorts ();
    void
    connectToPeers (Connections* connections);
    void
    specializeConnectors (Connections* connections);
    void
    spatialNegotiation ();
    void
    takePreCommunicators ();
    void
    takePostCommunicators ();
    void
    buildTables ();
    void
    temporalNegotiation (Connections* connections);
    void
    initialize ();
  };

}
#endif
#define MUSIC_RUNTIME_HH
#endif
