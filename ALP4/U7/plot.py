from math import pow
from matplotlib import pyplot as plt

def plotGraphic():
	# Aufgabe 4 a)
	plt.plot([1, 1, 2, 3, 4, 5, 6, 7, 8, 16, 32], [2.119 * pow(10, -4), 3.147 * pow(10, -4), 5.210 * pow(10, -4), 2.805 * pow(10, -3), 2.881 * pow(10, -3), 1.0852 * pow(10, -3), 1.258 * pow(10, -3), 1.5128 * pow(10, -3), 1.357 * pow(10, -3), 3.695 * pow(10, -3), 4.464 * pow(10, -3)], 'gv:')
	plt.title("Beziehung zwischen der Laufzeit und eingesetzter Anzahl von Threads")
	plt.xlabel("Anzahl der Threads")
	plt.ylabel("Laufzeit")
	plt.show()
    
def main():
	plotGraphic()

if(__name__ == '__main__'):
	main()
