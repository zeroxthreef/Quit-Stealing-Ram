# Quit Stealing Ram!
Are you absolutely tired of inefficient programs stealing all of your ram? I know I am, and I made this out of pure hatred of being stuck in an OOM state because I disabled swapfiles to preserve my SSD. Just modify -

```
INTERVAL, - seconds of delay
FREE_RAM_THRESHOLD, - the amount of free ram you have to have left for it to run
NO_MERCY, -if this is set to 1, it will kill the preoccess instead of terminate. Useful if the situation is dire

and

the "offenders" string array
```

## Instructions
```
git clone https://github.com/zeroxthreef/Quit-Stealing-Ram.git
cd Quit-Stealing-Ram
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


