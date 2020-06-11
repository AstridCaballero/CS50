import cs50
import math


def main():
    card = get_card_number()
    l = get_length(card)
    luhns = luhns_alg(card, l)
    if (luhns == 0):
        print("INVALID\n")
    else:
        get_brand(card, l)


def get_card_number():
    while True:
        n = cs50.get_int("Number: ")
        if n > 1:
            break
    return n


def get_length(n):
    l_count = 0
    while True:
        # card = card / 10 is the same as card/= 10. to get an integer in python3 i need to use //
        n //= 10
        l_count += 1
        if n == 0:
            break
    return l_count

# Perform luhns algorithm


def luhns_alg(n, l):
    sum_digit = 0
    # iterate over each digit from right to left
    for i in range(l):
        # Get digit from right to left
        digit = n % 10
        # multiply by 2 every other digit starting from second-to-last digit
        # all the even ith values such 0,2,4,etc will have i % 2 == 0, these positions won't change their digit
        if i % 2 != 0:
            digit_mul = digit * 2
            len_digit = get_length(digit_mul)
            sum_digit_mul = 0
            for j in range(len_digit):
                # get last digit of the multiplied number
                mod_digit = digit_mul % 10
                # addition of each digit of the multiplied number
                sum_digit_mul += mod_digit
                digit_mul //= 10
            digit = sum_digit_mul
        # some of the digits were multiplied by 2
        sum_digit += digit
        n //= 10
    if sum_digit % 10 != 0:
        return 0
    return 1

# card's brand


def get_brand(n, l):
    # get first two digits of the card
    duo = n // (math.pow(10, l - 2))
    # indentifies the brand
    if (l == 15 and (duo == 34 or duo == 37)):
        print("AMEX\n")
    elif (l == 13 or l == 16) and (duo // 10 == 4):
        print("VISA\n")
    elif (l == 16 and (duo >= 51 and duo <= 55)):
        print("MASTERCARD\n")
    else:
        print("INVALID\n")


main()

