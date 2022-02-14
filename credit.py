from cs50 import get_string

cardnum = get_string("Number: ")
count = len(cardnum)

checksum = 0
i = count - 2
while i >= 0:
    cur = ord(cardnum[i]) - ord('0')
    product = cur * 2
    while product != 0:
        checksum += product % 10
        product = product // 10
    i -= 2

i = count - 1
while i >= 0:
    cur = ord(cardnum[i]) - ord('0')
    checksum += cur
    i -= 2

ris = checksum % 10
if ris == 0:
    start = (ord(cardnum[0]) - ord('0')) * 10 + ord(cardnum[1]) - ord('0')
    if ((start == 34 or start == 37) and count == 15):
        print("AMEX")
    elif ((start in range(51, 56)) and count == 16):
        print("MASTERCARD")
    elif (ord(cardnum[0]) - ord('0') == 4 and (count == 16 or count == 13)):
        print("VISA")
    else:
        print("INVALID")
else:
    print ("INVALID")


