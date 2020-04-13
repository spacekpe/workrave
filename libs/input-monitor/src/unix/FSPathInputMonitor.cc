// Copyright (C) 2020 Petr Spacek <workrave@petr.spackovi.net>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <boost/filesystem.hpp>

#include "FSPathInputMonitor.hh"

#include "debug.hh"

using namespace std;
using namespace boost::filesystem;

FSPathInputMonitor::FSPathInputMonitor()
{
}

FSPathInputMonitor::~FSPathInputMonitor()
{
  if (monitor_thread)
    {
      monitor_thread->join();
    }
}

bool
FSPathInputMonitor::init()
{
  TRACE_ENTER("FSPathInputMonitor::init");
  bool result = true;

  monitor_thread = std::shared_ptr<boost::thread>(new boost::thread(std::bind(&FSPathInputMonitor::run, this)));
  if (!monitor_thread)
    {
      TRACE_MSG("Error creating monitor thread ");
      result = false;
    }

  TRACE_EXIT();
  return result;
}

void
FSPathInputMonitor::terminate()
{
  mutex.lock();
  abort = true;
  cond.notify_all();
  mutex.unlock();

  monitor_thread->join();
}

void
FSPathInputMonitor::run()
{
  TRACE_ENTER("FSPathInputMonitor::run");

  {
    boost::mutex::scoped_lock lock(mutex);

    while (!abort)
      {
	      // TODO: per-user file name
        if (exists("/tmp/workrave.lock"))
          {
            /* Notify the activity monitor */
            fire_action();
          }

        boost::system_time timeout = boost::get_system_time()+ boost::posix_time::milliseconds(1000);
        cond.timed_wait(lock, timeout);
      }
  }

  TRACE_EXIT();
}
