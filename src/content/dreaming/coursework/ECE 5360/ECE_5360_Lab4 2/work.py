import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

zeta = 0.026 
def exp_func(x, a, b):
    return a * (np.exp(x / (zeta * b)) - 1)

# Function to read data from a text file
def read_data(file_path):
    with open(file_path, 'r') as file:
        next(file)
        next(file)
        data = []
        for line in file:
            values = list(map(float, line.split()))
            data.append(values)
    data = data[:-1]
    return np.array(data)

file_path = 'VBE_IBE_Diode_Data.txt'
file_path2 = 'VBE_IBE_Diode_Data2.txt'
data = read_data(file_path)
data2 = read_data(file_path2)
base_voltage = data[:, 0]

idx = 0
endidx = 0
for i, v in enumerate(base_voltage):
    if idx == 0 and v > 0:
        idx = i
        break
base_voltage = base_voltage[idx:]
base_current = data[idx:, 1]


params, _ = curve_fit(exp_func, base_voltage, base_current)
print(params)


x1 = np.linspace(1.2, 2, 1000)
y1 = exp_func(x1, params[0], params[1])


# plt.plot(x1, y1, 'o')
plt.plot(base_voltage, base_current, 'o')
plt.yscale('log')
plt.show()



