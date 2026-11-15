# philo

A 42 Amman project on threads and mutexes: the dining philosophers problem.

Philosophers sit around a table with one fork between each pair. To eat, a philosopher needs both forks next to them. Each philosopher is a thread and each fork is a mutex. A philosopher who doesn't start eating within `time_to_die` milliseconds of their last meal dies, and the simulation stops.

## Usage

```sh
make
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [meals_per_philosopher]
```

```sh
./philo 5 800 200 200      # runs until stopped, nobody should die
./philo 5 800 200 200 7    # stops once everyone has eaten 7 times
./philo 4 310 200 100      # a philosopher dies
```

Each state change is printed as `<ms since start> <philosopher id> <action>`, for example `200 3 is eating`. Nothing is printed after a death.

All arguments must be positive integers that fit in an `int`; anything else is rejected.

## How it works

- **Forks:** each philosopher locks the lower-numbered fork first, so the threads can never wait on each other in a circle (no deadlock). Odd-numbered philosophers start slightly later to spread out the first meals.
- **Monitor:** the main thread checks every philosopher's last meal time and meal count, sets the shared `stop` flag on a death or once everyone has eaten enough, then joins the threads.
- **Shared data:** meal times, meal counts and the `stop` flag are read and written under `dead_lock`; printing goes through `write_lock`.

## Files

| File | Contents |
| --- | --- |
| `src/main.c` | argument checks, `main` |
| `src/initilayzation.c` | setting up data, mutexes and philosophers |
| `src/routine.c` | starting threads, the eat / sleep / think loop |
| `src/monitor.c` | death and meal-count checks, printing |
| `src/utils.c` | `ft_atoi`, time helpers, cleanup |

## Author

[thaqer](https://github.com/thaqer), 42 Amman.
