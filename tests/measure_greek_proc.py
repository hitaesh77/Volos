import subprocess
import time
import statistics
from pathlib import Path

exe = "greeks.exe" 
warmup = 5
iterations = 1000

# Warmup runs
for _ in range(warmup):
    subprocess.run([exe], check=True)

times = []
for i in range(iterations):
    t0 = time.perf_counter_ns()
    subprocess.run([exe], check=True)
    t1 = time.perf_counter_ns()
    times.append((t1 - t0) / 1e3)  # microseconds

results = f"""runs: {iterations}
mean (us): {statistics.mean(times)}
median (us): {statistics.median(times)}
stdev (us): {statistics.stdev(times) if iterations > 1 else 0}
min (us): {min(times)}
max (us): {max(times)}
"""

print(results)

output_file = Path(__file__).parent / "test_results.txt"
with open(output_file, "w") as f:
    f.write(results)