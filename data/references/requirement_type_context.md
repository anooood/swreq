## REQUIREMENT TYPE REFERENCE

Assign exactly one of: Functional Req. | Performance Req. | Interface Req. | Constraint Req.

**Functional Req.** — Describes WHAT the system does: a computation, logic branch, state
transition, mapping, clamping, fault response, or conditional action.
  → Keywords: compute, calculate, set, map, convert, clamp, detect, enable, disable, trigger
  → Example: "The system shall scale the ADC value by 0.00488 to obtain voltage."

**Performance Req.** — Describes HOW WELL or HOW OFTEN: a timing bound, rate, frequency,
accuracy, latency, or throughput. Always contains a numeric value with a unit.
  → Keywords: within N ms, at a rate of, every N ms, accuracy of, no more than, maximum
  → Example: "The system shall execute the control loop within 1 ms."

**Interface Req.** — Describes interaction with an EXTERNAL ENTITY: CAN, SPI, I²C, UART,
GPIO, ADC, DMA, register, memory-mapped address, OS API, or another module.
  → Keywords: transmit, receive, read from, write to, signal, frame, message, pin, register
  → Example: "The system shall transmit CAN message 0x3A2 containing the torque setpoint."

**Constraint Req.** — Imposes a DESIGN RESTRICTION: a coding standard, architectural rule,
memory policy, or safety mandate — restricts the solution regardless of behavior.
  → Keywords: shall comply with, shall not use, MISRA-C, ISO 26262, stack depth, watchdog policy
  → Example: "The system shall not use dynamic memory allocation after initialization."

Decision rule: timing/accuracy → Performance | external bus/signal → Interface |
standard/policy → Constraint | everything else → Functional
