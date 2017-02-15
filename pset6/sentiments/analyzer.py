import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.positives = []
        self.negatives = []
        
        ##initiate tokenizer first from nltk to enable analysis later
        self.tokenizer = nltk.tokenize.TweetTokenizer()
        
        ##open positives.txt and run through each line in the file
        with open(positives) as pos:
            for pos_line in pos:
                if not(pos_line.startswith(';') or pos_line.startswith(' ')):
                    self.positives.append(pos_line.strip('\n')) #don't forget that each line has a newline character!
                    
        ##similar to opening positive files            
        with open(negatives) as neg:
            for neg_line in neg:
                if not(neg_line.startswith(';') or neg_line.startswith(' ')):
                    self.negatives.append(neg_line.strip('\n')) #don't forget that each line has a newline character! 

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        score = 0
        
        ##initialize list of tokens (a.k.a. splitted texts from the text variable, that are called tokens by nltk)
        tokens = self.tokenizer.tokenize(text)
        for token in tokens:
            if token.lower() in self.negatives: ##remember that the negative and positive libraries are all in lowercases!
                score-=1
            elif token.lower() in self.positives:
                score+=1
            else:
                score+=0
        return score
