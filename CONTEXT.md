# Performance Measurement Harness

This context defines the language for the course-style repetition tester used to benchmark code paths. It exists to keep measurement semantics stable while implementation details evolve.

## Language

**Repetition Tester**:
A harness that repeatedly executes one measured workload and reports min, max, and average results.
_Avoid_: benchmark loop, timing helper

**Standalone Harness**:
An implementation boundary where the repetition tester owns its state and behavior and does not depend on profiler internals.
_Avoid_: profiler extension, profiler wrapper

**Measurement Wave**:
A bounded measurement run where repeated samples are collected until a trial duration is exceeded.
_Avoid_: pass, phase

**Polling Loop**:
The execution style where callers iterate while tester.is_testing() and submit one sample per iteration.
_Avoid_: callback-owned loop

**Sample Finalization**:
An explicit API step that validates and commits the current sample into wave statistics.
_Avoid_: implicit commit on next poll

**Trial Duration**:
The configured CPU-timer interval that ends a measurement wave once exceeded.
_Avoid_: sample budget, run length

**Minimum Reset Rule**:
When a new minimum sample is observed, the wave start time is reset so the full trial duration runs again from that point.
_Avoid_: fixed uninterrupted window

**Wave Config**:
An immutable configuration object passed when starting a wave, including target bytes, trial duration, validation policy, and metric toggles.
_Avoid_: mutable tester settings, global benchmark options

**Required CPU Frequency**:
The CPU timer frequency must be provided by the caller in each wave config for time and bandwidth conversion.
_Avoid_: implicit calibration, hidden frequency cache

**Warm-up Policy**:
A per-wave rule that allows initial samples to run without contributing to recorded statistics.
_Avoid_: always-count samples, global warm-up switch

**Default Warm-up Off**:
Warm-up is disabled unless explicitly enabled in wave configuration.
_Avoid_: implicit warm-up behavior

**Sample**:
One fully balanced BeginTime/EndTime measurement with a validated byte count.
_Avoid_: run, attempt

**Measurement Vector**:
The set of counters tracked per sample: CPU timer ticks, memory page faults, and processed bytes.
_Avoid_: stats blob, metrics bag

**Metric Toggle**:
A per-wave option that enables or disables selected counters, such as memory page faults.
_Avoid_: compile-time-only metric switch

**Result Snapshot**:
An immutable structured result object returned by the tester for min, max, and average statistics.
_Avoid_: printed output, log line

**Aggregate Totals**:
Wave-wide accumulated counters and sample count retained alongside min/max/avg summaries.
_Avoid_: min/max/avg-only summary, per-sample log

**Status Result**:
An explicit non-exception outcome that reports success or failure with diagnostic detail.
_Avoid_: thrown exception, implicit failure

**Start Validation Gate**:
Wave configuration is validated before testing begins, and invalid configuration is rejected immediately.
_Avoid_: deferred config failure, silent defaulting

**Reusable Tester**:
A long-lived tester object that can execute multiple waves sequentially using new wave configs.
_Avoid_: one-shot tester instance

**Single-Thread Contract**:
One tester instance is used by one thread at a time with no internal synchronization.
_Avoid_: shared-instance locking, concurrent mutation

**Validation Policy**:
A runtime policy that controls whether invariant violations hard-stop a wave or are reported and skipped.
_Avoid_: error mode, fail flag

**Timing Scope**:
An RAII object that starts timing on construction and closes timing on destruction for one measured section.
_Avoid_: auto timer, scope guard

**Non-Nested Timing**:
A sample may contain multiple timing scopes only when they do not overlap in time.
_Avoid_: nested scopes, hierarchical timing

**Scope Factory**:
The tester API that creates a **Timing Scope** object, typically via tester.time_scope().
_Avoid_: timing macro, direct constructor call

**Modern Naming**:
The public API uses contemporary C++ method names and does not expose course-era alias names.
_Avoid_: compatibility aliases, legacy naming shim

**Target Byte Count**:
The required processed-byte total for each sample; mismatch invalidates the sample.
_Avoid_: expected size, throughput target

**Byte Accumulation**:
Explicit byte counting where one or more count_bytes calls are summed across a sample and validated against the target.
_Avoid_: implicit byte estimate, single-shot byte entry

## Relationships

- A **Repetition Tester** executes one or more **Measurement Waves**
- A **Measurement Wave** consists of many **Samples**
- Each **Sample** produces one **Measurement Vector**
- Each **Measurement Wave** is created from an immutable **Wave Config**
- Each **Wave Config** includes a **Required CPU Frequency** value
- A **Sample** is valid only when its processed bytes equal the **Target Byte Count**
- A **Sample** may use one or more **Timing Scope** blocks, and all blocks must be balanced
- A **Sample** enforces **Non-Nested Timing** to avoid overlapping scope intervals
- Each **Timing Scope** is created through the tester **Scope Factory** API
- Public calls use **Modern Naming** for wave, scope, byte, and finalize operations
- A **Sample** uses explicit **Byte Accumulation** and validates the final sum against **Target Byte Count**
- A **Measurement Wave** is driven by caller-controlled **Polling Loop** execution
- Each loop iteration commits results through explicit **Sample Finalization**
- A **Standalone Harness** may use shared low-level timer/page-fault primitives but not profiler state
- A **Measurement Wave** completion yields a **Result Snapshot** that callers may format as needed
- A **Result Snapshot** contains min/max/avg and **Aggregate Totals**
- A **Validation Policy** governs how invalid **Samples** affect wave progression
- A **Measurement Wave** ends when elapsed CPU timer exceeds the configured **Trial Duration**
- A **Metric Toggle** can disable page-fault collection for a specific wave without changing API shape
- The **Minimum Reset Rule** extends the wave window whenever a better minimum sample is found
- A **Warm-up Policy** can exclude early samples from min/max/avg aggregation
- **Default Warm-up Off** applies unless the wave config explicitly enables warm-up
- API operations return **Status Result** values instead of throwing exceptions
- The **Start Validation Gate** rejects invalid wave configs before entering testing mode
- A **Reusable Tester** persists across waves and resets only per-wave runtime state
- A **Reusable Tester** follows a **Single-Thread Contract** per instance

## Example dialogue

> **Dev:** "Should this **Sample** be accepted if CPU ticks are valid but byte count is short?"
> **Domain expert:** "No, that violates **Target Byte Count**; the **Sample** must be rejected as invalid."

## Flagged ambiguities

- "repetition tester" could mean either a minimal timer-only loop or the richer page-fault-aware version — resolved: in this repo it means the page-fault-aware **Repetition Tester** baseline unless explicitly stated otherwise.
- "C++ style" could mean all-RAII accounting — resolved: we use hybrid style with RAII for **Timing Scope** and explicit byte counting for **Target Byte Count**.
- "integrate with profiler" could mean sharing anchor/state structures — resolved: we keep a **Standalone Harness** and only share low-level metric primitives.
- "reporting" could mean immediate console printing — resolved: core measurement returns a **Result Snapshot** and formatting is a caller concern.
- "invariant failure" could imply immediate stop in all cases — resolved: default hard-fail with an optional soft-fail mode under configurable **Validation Policy**.
- "RAII API" could mean direct public scope constructors — resolved: call sites use a tester **Scope Factory** (tester.time_scope()) to obtain a **Timing Scope**.
- "wave limit" could mean a fixed sample-count cap — resolved: waves are duration-driven using **Trial Duration**.
- "byte counting" could imply one call per sample — resolved: **Byte Accumulation** allows multiple explicit calls and validates the summed total.
- "page-fault-aware baseline" could imply always-on collection — resolved: page-fault counter is controlled by a per-wave **Metric Toggle**.
- "user loop" could mean callback-owned execution — resolved: wave progression uses a caller-owned **Polling Loop**.
- "error handling" could imply exceptions in C++ style APIs — resolved: this harness uses explicit **Status Result** outcomes.
- "configuration" could imply mutable pre-run setters — resolved: waves start from immutable **Wave Config** objects.
- "sample commit" could imply hidden lifecycle transitions — resolved: samples are recorded via explicit **Sample Finalization**.
- "trial duration" could imply a single uninterrupted deadline — resolved: the **Minimum Reset Rule** restarts the window on new minima.
- "warm-up" could imply always-on or global behavior — resolved: warm-up is controlled by per-wave **Warm-up Policy**.
- "tester lifetime" could imply reconstructing for every run — resolved: we use a **Reusable Tester** that runs multiple waves.
- "final stats" could imply only min/max/avg — resolved: snapshots include **Aggregate Totals** and sample count too.
- "CPU frequency source" could imply automatic calibration — resolved: frequency is a **Required CPU Frequency** field in wave configuration.
- "invalid config handling" could imply first-sample failure or silent coercion — resolved: fail fast at the **Start Validation Gate**.
- "multiple scopes" could imply profiler-like hierarchical timing — resolved: **Non-Nested Timing** forbids nested scope overlap.
- "API style" could imply keeping course-era function names — resolved: expose only **Modern Naming**.
- "thread safety" could imply shared concurrent use — resolved: each tester instance follows a **Single-Thread Contract**.
- "warm-up default" could imply hidden skipped samples — resolved: **Default Warm-up Off** unless opted in per wave.
