import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""

        self.posList = []
        file = open(positives, "r")
        for line in file:
            if not line.startswith(";"):
                self.posList.append(line.strip("\n"))
        file.close()

        self.negList = []
        file = open(negatives, "r")
        for line in file:
            if not line.startswith(";"):
                self.negList.append(line.strip("\n"))
        file.close()

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        score = 0

        for word in tokens:
            if word.lower() in self.posList:
                score += 1
            elif word.lower() in self.negList:
                score -= 1
            else:
                continue
        return score