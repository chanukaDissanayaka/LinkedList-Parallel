import statistics

f = open("results-case3.txt", "r")

results = []

for x in f:
	s = x.rstrip()
	results.append(float(s))

average = statistics.mean(results)
std = statistics.stdev(results)

print(average, std)