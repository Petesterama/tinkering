    # def isPretty(name):
    #     pretty_people = ["AYLA", "mingming", "Willow", "Jorgi", "Lettie", "ur mum"]
    #     if name.upper() in pretty_people:
    #         return True
    #     else:
    #         return False





    def isPretty(name):
        pretty_people = ["AYLA", "mingming", "Willow", "Jorgi", "Lettie", "ur mum"]

        for person in pretty_people:
            if name.lower() == person.lower():
                return True

        return False


    def encouragement(name, age):
        # return name + " is really pretty, and " + name + " is so young, at only " + str(age) + "and also, " + name + " smells really good."
        return "{0} is really pretty, and {0} is so young, at only {1} and also, {0} smells really good".format(name, age)



    encouragement("ayla", 25)


def IsOld(name, age, gender):
    if gender.lower() == "male":
        pronoun = "He"
    else:
        pronoun = "She"
    if age >= 40:
        print("{} is old.".format(name))
    else:
        print("{} is young. {}'s only {}.".format(name, pronoun, age))


IsOld("Ayla", 25, "feeeemale")



class Node:
    self.name = 
    self.colour

    def delete():
        asdas
