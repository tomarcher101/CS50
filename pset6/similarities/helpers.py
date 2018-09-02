from nltk.tokenize import sent_tokenize

def lines(a, b):
    """Return lines in both a and b"""

    aLineList = a.splitlines()
    bLineList = b.splitlines()
    similarLines = set()

    for aLine in aLineList:
        for bLine in bLineList:
            if aLine == bLine:
                similarLines.add(aLine)

    return similarLines

def sentences(a, b):
    """Return sentences in both a and b"""

    aSentenceList = sent_tokenize(a)
    bSentenceList = sent_tokenize(b)
    similarSentences = set()

    for aSentence in aSentenceList:
        for bSentence in bSentenceList:
            if aSentence == bSentence:
                similarSentences.add(aSentence)

    return similarSentences


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    aSubList = set()
    bSubList = set()
    similarSubs = set()

    for i in range(len(a)):
        aSubList.add(str(a[i:i+n]))

    for j in range(len(b)):
        bSubList.add(str(b[j:j+n]))

    for aSub in aSubList:
        for bSub in bSubList:
            if aSub == bSub:
                similarSubs.add(aSub)

    return similarSubs
