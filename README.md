# Quit Stealing Ram!
Are you absolutely tired of inefficient programs stealing all of your ram? I know I am, and I made this out of pure hatred of being stuck in an OOM state because I disabled swapfiles to preserve my SSD. Just modify -

```
INTERVAL, - seconds of delay
FREE\_RAM\_THRESHOLD, - the amount of free ram you have to have left for it to run
NO\_MERCY, -if this is set to 1, it will kill the preoccess instead of terminate. Useful if the situation is dire

and

the "offenders" string array
```

## Instructions
```
git clone {this repository url}
cd {this repository url, I dont wanna edit this file after this one and only push}
mkdir build && cd build && cmake ..
make
sudo make install
```

## Adding to the "offenders" array
any string within a proccess executable name that is in the list will be killed if needed

## Dependencies
* libprocps-dev

## License
* GPL (no choice, libprocps is GPL)


