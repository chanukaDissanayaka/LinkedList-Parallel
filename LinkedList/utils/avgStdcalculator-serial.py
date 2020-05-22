import statistics

cases = [1,2,3]


for i  in cases:
	fileName = "results-case"+str(i)+".txt"


	f = open(fileName, "r")

	results = []

	for x in f:
		s = x.rstrip()
		results.append(float(s))

	average = statistics.mean(results)
	std = statistics.stdev(results)

	print(fileName, average, std)
	print(" ")



