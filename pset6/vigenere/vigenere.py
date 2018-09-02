import cs50
import sys

if not len(sys.argv) == 2:
    print("You must have one keyword!")
    exit(1)
if not (sys.argv[1]).isalpha():
    print("You must only use letters in your keyword!")
    exit(1)
else:
    keyword = sys.argv[1]
    print("Your keyword is {}".format(keyword))

print("plaintext: ", end="")
plaintext = cs50.get_string()
print("ciphertext: ", end="")


plainLen = len(plaintext)
keyLen = len(keyword)
cipher = []

z = 0

for i in range(plainLen):
    plainLet = plaintext[i]
    h = i - 1 - z

    if not plainLet.isalpha():
        print("{}".format(plainLet), end="")
        z += 1

    if plainLet.isalpha():
        h += 1
        j = h % keyLen
        keyLet = keyword[j]

        if keyLet.isupper():
            keyLet = keyLet.lower()

        keyShift = ord(keyLet)
        keyShift = keyShift - 97

        plainInt = ord(plainLet)
        ciphInt = plainInt + keyShift

        if plainLet.isupper():
            if ciphInt > 90:
                ciphInt = ciphInt - 26
            ciphLet = chr(ciphInt)
            print(ciphLet, end="")

        if plainLet.islower():
            if ciphInt > 122:
                ciphInt = ciphInt - 26
            ciphLet = chr(ciphInt)
            print(ciphLet, end="")

print("")