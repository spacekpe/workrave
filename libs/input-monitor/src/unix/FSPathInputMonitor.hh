// Copyright (C) 2016 Rob Caelers <robc@krandor.nl>
// All rights reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef FSPATHINPUTMONITOR_HH
#define FSPATHINPUTMONITOR_HH

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

#include "InputMonitor.hh"

class FSPathInputMonitor :
  public InputMonitor
{
public:
  FSPathInputMonitor();

  //! Destructor.
  virtual ~FSPathInputMonitor();

  //! Initialize
  virtual bool init();

  //! Terminate the monitor.
  virtual void terminate();

private:
  //! The monitor's execution thread.
  virtual void run();

private:
  bool abort = false;
  std::shared_ptr<boost::thread> monitor_thread;

  boost::mutex mutex;
  boost::condition_variable cond;
};

#endif // FSPATHINPUTMONITOR_HH
