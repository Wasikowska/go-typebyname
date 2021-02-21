性能调优，需要建立在测量的基础上

测量的时候，要保证CPU频率固定

sudo apt install cpufrequtils

通过intel_pstate，可以将cpu设置为两个governor: performance和power save

https://www.kernel.org/doc/Documentation/cpu-freq/governors.txt
sudo cpufreq-set -g performance：调整frequency governer为performance，永远使用最大频率
sudo cpufreq-set -g powersave：调整governer为powersave，永远使用最小频率

intel pstate driver会忽略governor固定频率，一直变频，但performance的性能还是要好一点
https://www.cult-of-tech.net/2018/08/linux-ubuntu-cpu-power-frequency-scaling/
https://www.kernel.org/doc/html/v4.12/admin-guide/pm/intel_pstate.html

CPU变频真是个神奇的东西
https://www.kernel.org/doc/html/v4.12/admin-guide/pm/intel_pstate.html

测量CSAPP中说的cycle per element是困难的，因为cpu频率一直在变
