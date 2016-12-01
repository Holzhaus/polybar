#pragma once

#include <poll.h>
#include <sys/inotify.h>
#include <cstdio>

#include "common.hpp"

POLYBAR_NS

struct inotify_event {
  string filename;
  bool is_dir;
  int wd = 0;
  int cookie = 0;
  int mask = 0;
};

namespace inotify_util {
  using event_t = inotify_event;

  class inotify_watch {
   public:
    explicit inotify_watch(string path) : m_path(path) {}
    ~inotify_watch() noexcept;

    void attach(int mask = IN_MODIFY);
    void remove(bool force = false);
    bool poll(int wait_ms = 1000);
    unique_ptr<event_t> get_event();
    bool await_match();
    const string path() const;

   protected:
    string m_path;
    int m_fd = -1;
    int m_wd = -1;
    int m_mask = 0;
  };

  using watch_t = unique_ptr<inotify_watch>;

  watch_t make_watch(string path);

  bool match(const event_t* evt, int mask);
}

POLYBAR_NS_END
