from cs50 import get_int

cardNumber = get_int("Number: ")
startNumber = 0
digit = 0
digitCount = 0
luhn = 0

while digitCount < 16 and cardNumber != 0:
    digit = int(cardNumber % 10)

    if (int)(digitCount % 2) == 0:
        luhn += digit
    else:
        if digit * 2 >= 10:
            luhn += (int)(digit * 2 % 10) + (int)(digit * 2 / 10)
        else:
            luhn += (int)(digit * 2)

    cardNumber = int(cardNumber / 10)

    if cardNumber < 100 and cardNumber >= 10:
        startNumber = cardNumber

    digitCount += 1

luhn = int(luhn % 10)

if digitCount == 15 and luhn == 0 and (startNumber == 34 or startNumber == 37):
    print("AMEX")
elif digitCount == 16 and luhn == 0 and (startNumber >= 51 and startNumber <= 55):
    print("MASTERCARD")
elif (digitCount == 13 or digitCount == 16) and luhn == 0 and (startNumber >= 40 and startNumber < 50):
    print("VISA")
else:
    print("INVALID")