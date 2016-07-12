#include <uwsgi.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

static int isfresh(char *path, int maximum_age) {
  // Get a struct of the file's attributes.
  struct stat attr;
  stat(path, &attr);

  // Get the current time and
  // calculate the age of the file's last
  // modified time (mtime) in seconds.
  time_t now = time(0);
  int age = now - attr.st_mtime;

  // If the last modified time is younger than the
  // defined age, consider the file to be fresh.
  if (age < maximum_age) {
    return 1;
  }

  // Still here? The file is not fresh.
  return 0;
}

static int router_isfreshfile(struct wsgi_request *wsgi_req, struct uwsgi_route *ur) {
  struct uwsgi_buffer *ub = uwsgi_routing_translate(wsgi_req, ur, NULL, 0, ur->subject_str, ur->subject_str_len);

  if(!ub) return -1;

  // Check if the file actually exists.
  // If not, assume it still needs to be made and allow
  // the router to continue.
  if(!uwsgi_file_exists(ub->buf)) return UWSGI_ROUTE_NEXT;

  int age = 10; // Should be replaced by a modifier.

  // If the is younger than the set age, all is good.
  if(isfresh(ub->buf, age)) {
    uwsgi_buffer_destroy(ub);
    return 1;
  }

  // Not good, continue on.
  uwsgi_buffer_destroy(ub);
  return UWSGI_ROUTE_NEXT;
}

void uwsgi_register_isfreshfile(){
  uwsgi_register_route_condition("isfreshfile", router_isfreshfile);
}

struct uwsgi_plugin router_isfreshfile_plugin = {
  .name = "router_isfreshfile",
  .on_load = uwsgi_register_isfreshfile,
};
