import cs50
import math

print("O hai! How much change is owed?")

while True:
    change = cs50.get_float()
    if change > 0.00:
        break

change = change * 100
change = int(round(change, 0))

quarters = math.floor(change / 25)
change = change % 25

dimes = math.floor(change / 10)
change = change % 10

fives = math.floor(change / 5)
change = change % 5

ones = math.floor(change)

coins = quarters + dimes + fives + ones

print(coins)