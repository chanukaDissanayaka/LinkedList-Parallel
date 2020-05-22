import statistics

cases = [1,2,3]
threadCount = [1,2,4,8]


for i  in cases:
	for j in threadCount:
		fileName = "results-case"+str(i)+"-thread_count_"+str(j)+".txt"


		f = open(fileName, "r")

		results = []

		for x in f:
			s = x.rstrip()
			results.append(float(s))

		average = statistics.mean(results)
		std = statistics.stdev(results)

		print(fileName, average, std)
		print(" ")