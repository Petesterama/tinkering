class Human:

	def __init__(self, age, weight, sex, height):
		self.age = age
		self.weight = weight
		self.sex = sex
		self.height = height


	def calcBmi(self):
		h = self.height/100
		return self.weight / (h*h)

	def riskWeight(self):
		h = self.height/100
		b = 24
		return h*h*b

	def lifeExpectancy(self):
		bmi = self.calcBmi()

		if bmi>=19 and bmi<=24:
			life_expectancy = 80
		elif bmi<19:
			bmi_fac = (19-bmi)/19
			ry = 80-self.age
			life_expectancy = ry * pow(1-bmi_fac,6) + self.age
		elif bmi>24:
			bmi_fac = min((bmi-24)/40,1)
			ry = 80-self.age
			life_expectancy = ry * pow(1-bmi_fac,1) + self.age

		if self.sex == "Male":
			sex_mult = 0.95
		else:
			sex_mult = 1.0

		return life_expectancy * sex_mult


peter = Human(25,74.6,"Male", 177)
ayla = Human(25,50,"Female", 164.5)

cat = 5
dog = "tree"


#print("BMI is {}, life expectancy is {}".format(peter.calcBmi(), peter.lifeExpectancy()))
print(peter.calcBmi())