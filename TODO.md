Performance testing on side
	- compare C/C++

Think of concrete testbenches
	- priority vs order of interrupts
	- If interrupt is done, sometime (in timeout) it should be triggered

(nice to have: standard assertions (no crash, memory leak, ..))

Hashmap ersetzen durch normale map
UART, Sensor2 mit register range

klee/angr comparison?
	Kosten / Nutzen?

Hierachical waits in called functions?

Systemc basierte Metriken, automatisch herausfinden?
    Coverage?

Laufzeit messen, Sinvolle Tests schreiben.
    Eigenschaften definieren und dagegen testen (Beispieleigenschaften aus 2016 Datepaper)
    
    
Erste Erkenntnis: Boost und Fremdlibs verzögern _extrem_ die Ausführung.

       | PLIC | SENSOR_bare | SENSOR_TLM_MAP
--------------------------------------------
Faults  |   X   |            6  |          5
Instr.  |   X   |      172.131  |    338.552
Paths   |   X   |           70  |         80
Time    |   X   |         4,23s |       5,54s
I/s     |   X   |       40.692  |     61.105

--> Complete Path exploration time!