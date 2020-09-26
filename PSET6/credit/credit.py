from cs50 import get_int


def main():
    card = get_int("Number: ")

    # Checks for each of the different card types
    if (int(card / 10000000000000) == 34 or int(card / 10000000000000) == 37):
        # Amex
        if int(credit(card, 8, 8)) % 10 == 0:
            print("AMEX")
        else:
            print("INVALID")
    elif (int(card / 100000000000000) == 51 or int(card / 100000000000000) == 52 or int(card / 100000000000000) == 53 or int(card / 100000000000000) == 54 or int(card / 100000000000000) == 55):
        # Mastercard
        if int(credit(card, 8, 8) % 10) == 0:
            print("MASTERCARD")
        else:
            print("INVALID")
    elif (int(card / 1000000000000) == 4):
        # 13 Digit Visa
        if int(credit(card, 7, 6) % 10) == 0:
            print("VISA")
        else:
            print("MASTERCARD")
    elif (int(card / 1000000000000000) == 4):
        # 16 Digit Visa
        if int(credit(card, 8, 8) % 10) == 0:
            print("VISA")
        else:
            print("INVALID")
    else:
        print("INVALID")


# Determines the total that can determine the what card
def credit(card, i1, i2):
    creditcard = card
    total = 0
    for i in range(i1):
        total = total + int(creditcard % 10)
        creditcard = int(creditcard / 100)
    creditcard = int(card / 10)
    for i in range(i2):
        tmp = int(creditcard % 10)
        tmp *= 2
        if tmp < 9:
            total = int(total + tmp)
        else:
            total = int(total + (tmp % 10))
            tmp = int(tmp / 10)
            total = int(total + (tmp % 10))
        creditcard = int(creditcard / 100)
    #print(f"TOTAL: {total}")
    return total


main()