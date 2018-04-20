RC Circuit Transient Response
*resistor connected between nodes 1 and 2
r1 1 2 1k
*capacitor connected between nodes 2 and 0
c1 2 0 1u
*piecewise linear input voltage
vin 1 0 pwl (0 0 10ms 0 11ms 5v 20ms 5v)
*transient analysis for 20ms, step size 0.02ms
.tran 0.02ms 20ms
*defining the run-time control functions
.control
run
*plotting input and output voltages
plot v(1) v(2)
.endc
.end
