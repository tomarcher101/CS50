import cs50
import math

chg_amt = 0

while chg_amt <= 0:
    chg_amt = cs50.get_float("Change owed: ")

org_change = chg_amt * 100
change = int(round(org_change))

quarters = math.floor(change / 25)
change = change % 25
dimes = math.floor(change / 10)
change = change % 10
fivecents = math.floor(change / 5)
change = change % 5
cents = change

no_of_coins = quarters + dimes + fivecents + cents

print('{}'.format(no_of_coins))
