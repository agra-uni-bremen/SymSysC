somehow implement notifies on sc_events?
	- Minikernel.run() -> Check next time waiting
	
Performance testing on side
	- compare C/C++

Think of concrete testbenches
	- priority vs order of interrupts
	- If interrupt is done, sometime (in timeout) it should be triggered

(nice to have: standard assersions (no crash, memory leak, ..))

klee/angr comparison?