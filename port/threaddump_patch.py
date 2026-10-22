#!/usr/bin/env python3
# Scratch-tree-only: (a) per-thread dump after each [run:tick] line; (b) bounded alarm set/fire traces.
import sys

R = "/tmp/ps2recomp_build/PS2Recomp/ps2xRuntime"


def patch(path, old, new):
    s = open(path).read()
    n = s.count(old)
    if n != 1:
        print(f"FAIL {path}: expected 1 match, found {n} for:\n{old[:160]}")
        sys.exit(1)
    open(path, "w").write(s.replace(old, new))
    print("patched", path)


patch(R + "/src/lib/ps2_runtime.cpp",
      """                                               << " gsw=" << curGs
                                               << " vif=" << curVif
                                               << std::endl);
""", """                                               << " gsw=" << curGs
                                               << " vif=" << curVif
                                               << std::endl);
                for (const auto &thread : eeSnapshot.threads)
                {
                    RUNTIME_LOG("[run:thread] id=" << thread.id
                                                   << " status=" << static_cast<int>(thread.status)
                                                   << " pc=0x" << std::hex << thread.pc
                                                   << " ra=0x" << thread.ra
                                                   << std::dec
                                                   << " wait=" << static_cast<int>(thread.waitReason)
                                                   << "/" << thread.waitId
                                                   << " prio=" << thread.currentPriority
                                                   << std::endl);
                }
""")

patch(R + "/src/lib/Kernel/EeScheduler.cpp",
      """    m_alarms.emplace(id, EeAlarm{id, ticks, handler, argument, gp, sp});
""", """    m_alarms.emplace(id, EeAlarm{id, ticks, handler, argument, gp, sp});
    {
        static uint32_t s_alarmSetLogs = 0u;
        if (s_alarmSetLogs < 24u)
        {
            ++s_alarmSetLogs;
            std::cerr << "[ee-alarm] set id=" << id << " ticks=" << ticks << " handler=0x" << std::hex << handler
                      << " arg=0x" << argument << std::dec << " eeCycle=" << m_eeCycle << std::endl;
        }
    }
""")
patch(R + "/src/lib/Kernel/EeScheduler.cpp",
      """        const EeAlarm alarm = it->second;
        m_alarms.erase(it);
""", """        const EeAlarm alarm = it->second;
        m_alarms.erase(it);
        {
            static uint32_t s_alarmFireLogs = 0u;
            if (s_alarmFireLogs < 24u)
            {
                ++s_alarmFireLogs;
                std::cerr << "[ee-alarm] fire id=" << alarm.id << " handler=0x" << std::hex << alarm.handler << std::dec
                          << " eeCycle=" << m_eeCycle << std::endl;
            }
        }
""")
