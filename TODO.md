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
    

PLIC Ausführungsgeschwindigkeit wo Probleme?

Erste Erkenntnis: Boost und Fremdlibs verzögern _extrem_ die Ausführung.

Getrennte Tabellen für jede Testbench.
    -> Fehlergruppen for jede TB
        -> Robustness (interface)
        -> Memory-Fehler
        -> Off-by-one
        -> Whitebox: Erwartete Werte
            -> Prio richtig?
            -> Interrupt auch claim-bar?


Error handling (generic values as input, watch behaviour),
Functional testing
Fault injection vs Correctness, how?


max prio und min id, ist das richtig?