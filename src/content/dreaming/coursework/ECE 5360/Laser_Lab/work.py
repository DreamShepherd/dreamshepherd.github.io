import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
# Read CSV file
data = pd.read_csv('Group1_PowerVsTime.csv')
data2 = pd.read_csv('IV curve 0 to 5 V half a sec delay.csv')

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt


data['Time (s)'] = data['Time (ms)'] / 1000

print(max(data['Power (W)']))

# Merge dataframes on closest time (you may use interpolation here)
merged_data = pd.merge_asof(data2.sort_values('SMU-1 Time (s)'),
                            data.sort_values('Time (s)'),
                            left_on='SMU-1 Time (s)', right_on='Time (s)',
                            direction='nearest')


# Plot Power vs Current
plt.plot(merged_data['SMU-1 Current (A)'], merged_data['Power (W)'])
plt.xlabel('Current (A)')
plt.ylabel('Power (W)')
plt.title('Power vs Current')
plt.grid(True)
plt.show()
