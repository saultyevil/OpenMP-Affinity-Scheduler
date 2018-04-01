import numpy as np
from matplotlib import pyplot as plt

"""
PLOTS FOR CHANGING THE NUMBER OF CHUNKS PER THREAD
"""

data_n_chunks = np.loadtxt('run_times_chunks.csv', skiprows=1)
static = data_n_chunks[data_n_chunks[:, 0] == 1, :]
dynamic = data_n_chunks[data_n_chunks[:, 0] == 2, :]
guided = data_n_chunks[data_n_chunks[:, 0] == 3, :]

fig = plt.figure(figsize=(20, 10))
ax1 = fig.add_subplot(121)
ax1.plot(static[:, 1], static[:, 2], 'x-', label='Static')
ax1.plot(dynamic[:, 1], dynamic[:, 2], 'x-', label='Dynamic')
ax1.plot(guided[:, 1], guided[:, 2], 'x-', label='Guided')
ax1.set_xlabel('Chunksize')
ax1.set_ylabel('Run time (s)')
ax1.set_xlim(min(static[:, 1]), max(static[:, 1]))
ax1.set_title('Loop 1')
ax1.legend(fontsize='x-large')
ax1.grid()

ax2 = fig.add_subplot(122)
ax2.plot(static[:, 1], static[:, 3], 'x-', label='Static')
ax2.plot(dynamic[:, 1], dynamic[:, 3], 'x-', label='Dynamic')
ax2.plot(guided[:, 1], guided[:, 3], 'x-', label='Guided')
ax2.set_xlabel('Chunksize')
ax2.set_ylabel('Run time (s)')
ax2.set_xlim(min(static[:, 1]), max(static[:, 1]))
ax2.set_title('Loop 2')
ax2.legend(fontsize='x-large')
ax2.grid()

fig.tight_layout()
plt.savefig('chunksize.pdf')
plt.close()


"""
DATA FOR CHANGING THE NUMBER OF THREADS FOR THE BEST SCHEDULE
"""

data_n_threads = np.loadtxt('time_output_n_threads.txt')
guided_n_threads = data_n_threads[data_n_threads[:, 0] == 3, :]
dynamic_n_threads = data_n_threads[data_n_threads[:, 0] == 2, :]

fig = plt.figure(figsize=(20, 10))
ax1 = fig.add_subplot(121)
ax1.plot(guided_n_threads[:, 2], guided_n_threads[:, 3], 'x-', label='Loop 1')
ax1.plot(guided_n_threads[:, 2], guided_n_threads[:, 4], 'x-', label='Loop 2')
ax1.set_xlabel('Number of Threads')
ax1.set_ylabel('Run time (s)')
ax1.set_xlim(min(guided_n_threads[:, 2]), max(guided_n_threads[:, 2]))
ax1.set_title('Guided, 4')
ax1.legend(fontsize='x-large')
ax1.grid()

ax2 = fig.add_subplot(122)
ax2.plot(dynamic_n_threads[:, 2], dynamic_n_threads[:, 3], 'x-',
         label='Loop 1')
ax2.plot(dynamic_n_threads[:, 2], dynamic_n_threads[:, 4], 'x-',
         label='Loop 2')
ax2.set_xlabel('Number of Threads')
ax2.set_ylabel('Run time (s)')
ax2.set_xlim(min(dynamic_n_threads[:, 2]), max(dynamic_n_threads[:, 2]))
ax2.set_title('Dynamic, 8')
ax2.legend(fontsize='x-large')
ax2.grid()

fig.tight_layout()
plt.savefig('best_schedules.pdf')
plt.close()

"""
T1/TA graphs
"""

loops_serial = np.loadtxt('loops_output.txt')
loop1_avg = np.average(loops_serial[:, 0])
loop2_avg = np.average(loops_serial[:, 1])

loop1_speedup_guided = loop1_avg/guided_n_threads[:, 3]
loop2_speedup_guided = loop2_avg/guided_n_threads[:, 4]

loop1_speedup_dynamic = loop1_avg/dynamic_n_threads[:, 3]
loop2_speedup_dynamic = loop2_avg/dynamic_n_threads[:, 4]

fig = plt.figure(figsize=(20, 8))
ax1 = fig.add_subplot(121)
ax1.plot(guided_n_threads[:, 2], loop1_speedup_guided, 'x-',
         label='Loop 1')
ax1.plot(guided_n_threads[:, 2], loop2_speedup_guided, 'x-',
         label='Loop 2')
ax1.hlines(1, min(dynamic_n_threads[:, 2]), max(dynamic_n_threads[:, 2]),
           colors='r', label=r'$T_{1}$')
ax1.set_xlabel(r'Threads, $p$')
ax1.set_ylabel(r'Speedup, $T_{1}/T_{p}$')
ax1.set_xlim(min(dynamic_n_threads[:, 2]), max(dynamic_n_threads[:, 2]))
ax1.set_ylim(0)
ax1.set_title('Guided, 4')
ax1.legend(fontsize='x-large')
ax1.grid()

ax2 = fig.add_subplot(122)
ax2.plot(dynamic_n_threads[:, 2], loop1_speedup_dynamic, 'x-',
         label='Loop 1')
ax2.plot(dynamic_n_threads[:, 2], loop2_speedup_dynamic, 'x-',
         label='Loop 2')
ax2.hlines(1, min(dynamic_n_threads[:, 2]), max(dynamic_n_threads[:, 2]),
           colors='r', label=r'$T_{1}$')
ax2.set_xlabel(r'Threads, $p$')
ax2.set_ylabel(r'Speedup, $T_{1}/T_{p}$')
ax2.set_xlim(min(dynamic_n_threads[:, 2]), max(dynamic_n_threads[:, 2]))
ax2.set_ylim(0)
ax2.set_title('Dynamic, 8')
ax2.legend(loc="upper left", fontsize='x-large')
ax2.grid()

fig.tight_layout()
plt.savefig('speedup.pdf')
plt.close()

"""
static and auto bar chart
"""

no_chunks_data = np.loadtxt('run_times_no_chunks.csv', skiprows=1)
static_equal = no_chunks_data[0, :]
auto = no_chunks_data[1, :]

tick_locations = np.array([1, 2])
tick_labels = ['Loop 1', 'Loop 2']
bar_width = 0.35

fig = plt.figure(figsize=(15, 8))
ax1 = fig.add_subplot(121)
ax1.bar(tick_locations, [static_equal[2], static_equal[3]], bar_width,
        label='Static')
ax1.bar(tick_locations+bar_width, [loop1_avg, loop2_avg], bar_width,
        label='Serial')
ax1.set_ylabel('Run time (s)')
ax1.set_title('Static')
ax1.set_xticks(tick_locations + bar_width/2)
ax1.set_xticklabels(tick_labels)
ax1.legend(fontsize='x-large')

ax2 = fig.add_subplot(122)
ax2.bar(tick_locations, [auto[2], auto[3]], bar_width, label='Auto')
ax2.bar(tick_locations+bar_width, [loop1_avg, loop2_avg], bar_width,
        label='Serial')
ax2.set_ylabel('Run time (s)')
ax2.set_title('Auto')
ax2.set_xticks(tick_locations + bar_width/2)
ax2.set_xticklabels(tick_labels)
ax2.legend(fontsize='x-large')

fig.tight_layout()
plt.savefig('static_auto.pdf')
plt.close()

"""
loop balance
"""

loop1_balance = np.loadtxt('balance1.txt')
loop2_balance = np.loadtxt('balance2.txt')

fig = plt.figure(figsize=(15, 8))
ax1 = fig.add_subplot(121)
ax1.bar(loop1_balance[:, 0], loop1_balance[:, 1])
ax1.set_xlabel('i')
ax1.set_ylabel('Iterations')
ax1.set_xlim(min(loop1_balance[:, 0]), max(loop1_balance[:, 0]))
ax1.set_ylim(min(loop1_balance[:, 1]), max(loop1_balance[:, 1]))
ax1.set_title('Loop 1 balance')

ax2 = fig.add_subplot(122)
ax2.bar(loop2_balance[:, 0], loop2_balance[:, 1])
ax2.set_xlabel('i')
ax2.set_ylabel('Iterations')
ax2.set_xlim(min(loop2_balance[:, 0]), max(loop2_balance[:, 0]))
ax2.set_ylim(min(loop2_balance[:, 1]), max(loop2_balance[:, 1]))
ax2.set_title('Loop 2 balance')

fig.tight_layout()
plt.savefig('loop_balance.pdf')
plt.close()

"""
plots for the affininty scheduler
"""

affinity_data = np.loadtxt('affinity_time_output.txt')

fig = plt.figure(figsize=(15, 8))
ax1 = fig.add_subplot(121)
ax1.plot(affinity_data[:, 0], affinity_data[:, 1])
ax1.set_xlabel(r'Threads, $p$')
ax1.set_ylabel('Run time (s)')
ax1.set_xlim(min(affinity_data[:, 0]), max(affinity_data[:, 0]))
ax1.set_title('Loop 1')

ax2 = fig.add_subplot(122)
ax2.plot(affinity_data[:, 0], affinity_data[:, 2])
ax2.set_xlabel(r'Threads, $p$')
ax2.set_ylabel('Run time (s)')
ax2.set_xlim(min(affinity_data[:, 0]), max(affinity_data[:, 0]))
ax2.set_title('Loop 2')

fig.tight_layout()
plt.savefig('affinity_runtime.pdf')
plt.close()

"""
affinity comparison
"""

fig = plt.figure(figsize=(15, 8))
ax1 = fig.add_subplot(121)
ax1.plot(affinity_data[:, 0], affinity_data[:, 1], 'x-', label='affinity')
ax1.plot(dynamic_n_threads[:, 2], dynamic_n_threads[:, 3], 'x-',
         label='dynamic, 8')
ax1.plot(guided_n_threads[:, 2], guided_n_threads[:, 3], 'x-',
         label='guided, 4')
ax1.set_xlabel(r'Threads, $p$')
ax1.set_ylabel('Run time (s)')
ax1.set_xlim(min(affinity_data[:, 0]), max(affinity_data[:, 0]))
ax1.set_title('Loop 1')
ax1.legend(fontsize='x-large')

ax2 = fig.add_subplot(122)
ax2.plot(affinity_data[:, 0], affinity_data[:, 2], 'x-', label='affininty')
ax2.plot(dynamic_n_threads[:, 2], dynamic_n_threads[:, 4], 'x-',
         label='dynamic, 8')
ax2.plot(guided_n_threads[:, 2], guided_n_threads[:, 4], 'x-',
         label='guided, 4')
ax2.set_xlabel(r'Threads, $p$')
ax2.set_ylabel('Run time (s)')
ax2.set_xlim(min(affinity_data[:, 0]), max(affinity_data[:, 0]))
ax2.set_title('Loop 2')
ax2.legend(fontsize='x-large')

fig.tight_layout()
plt.savefig('affinity_comparison.pdf')
plt.close()
