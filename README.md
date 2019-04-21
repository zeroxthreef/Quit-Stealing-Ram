# Quit Stealing Ram!
Are you absolutely tired of inefficient programs stealing all of your RAM? I know I am, and I made this out of pure hatred of being stuck in an OOM state because I disabled swapfiles to preserve my SSD.

## Editing QSR config
Open `/usr/share/qsr/qsr.conf` and edit the values. Restart QSR to apply the changes
```
qsr_interval  -> Interval in seconds to check system info
qsr_freeramtsh 300 -> Amount of RAM in MB which, if free RAM is less than or equal to this amount, will initate QSR
qsr_mercy -> Mercy. If 0, then processes will be SIGKILL'd
qsr_offenders -> A comma seperated list of offenders to search for and kill when threshold is reached
```

## Instructions
```
git clone https://github.com/zeroxthreef/Quit-Stealing-Ram.git
cd Quit-Stealing-Ram
[[ ! -d "build" ]] && mkdir build && cd build && cmake ..
make
sudo make install
```

## Dependencies
* libprocps-dev

## Contributors
* modeco80

## License
* GPL (no choice, libprocps is GPL)
