class Employee:
	def __init__(self, first, last, rate):
		self.first = first
		self.last = last
		self.rate = rate
		self.email = "{}@causefx.co.nz".format(first, last)
		self.tax = rate*.2

	def raterise(self, percent):
		self.rate *= 1+(percent/100)
		self.tax = self.rate*.2

	def payday(self, hours):
		i = self.rate * hours
		t = i * .2
		s = i * .8
		invoice = {"gross":i, "tax":t, "sub total":s}
		return invoice



peter = Employee("Peter", "Jansen", 33)


# print(peter.payday(216.5)["gross"])
print(peter.email)

