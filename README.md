# WARNING: File age is currently hard coded at 10 seconds, not ready for production just yet.

# uwsgi-router-isfreshfile
Gives you an extra router condition by which you can check if a given file is to be considered "fresh" or if it is outdated.
The check is done against the file's last modified timestamp.

# Flat file caching with expiration

This plugin is part of a set of tiny plugins which aims to give the user flat file caching with sane directory structure and file expiration.

Other plugins in this set:

- [uWSGI mkdir](https://github.com/Timusan/uwsgi-router-mkdir)

With the above plugin you can create a directory structure based on any available variable.

Complete usage example:

```ini
# Check if the cached file is fresh (86400 = 24 hours), if so serve it directly.
route-if = isfreshfile:/tmp/uwsgi_cache${PATH_INFO}file.html,86400 static:/tmp/uwsgi_cache${PATH_INFO}file.html
# Not fresh or does not exist? Create the full directory structure first.
route = .* mkdir:/tmp/uwsgi_cache${PATH_INFO}
# And (re)create the cached response file.
route-run = tofile:filename=/tmp/uwsgi_cache${PATH_INFO}file.html
```

Big thanks to [Roberto](https://gist.github.com/rdeioris) for his support!
