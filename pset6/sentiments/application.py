from flask import Flask, redirect, render_template, request, url_for

import helpers
import os
import sys
import nltk
from analyzer import Analyzer

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "").lstrip("@")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets
    tweets = helpers.get_user_timeline(screen_name,100)

    if tweets is None:
        return redirect(url_for("index"))
    else:
        ######### copied straight from 'tweets' file, but with some minor tweaking for applicaiton.py ##########
        tokenizer = nltk.tokenize.TweetTokenizer()
        
        # tweets now contain a list of the user's last 50 tweets, thanks to get_user_timeline from helper.
        # all we need to do now is to iterate into each tweet, tokenize it into smaller texts, and input it into analyzer
        
        # initialize positive and negative dictionaries first
        positives = os.path.join(sys.path[0], "positive-words.txt")
        negatives = os.path.join(sys.path[0], "negative-words.txt")
    
        # instantiate analyzer and score
        analyzer = Analyzer(positives, negatives)
        
        ###changed to be using 3 values/percentage of positive, negative or neutral instead
        positive, negative, neutral = 0.0, 0.0, 100.0

    
        # analyze and iterate for each tweets tweets
        for tweet in tweets:
            score = 0.0 # score for each tweet initialized as 0
            texts = tokenizer.tokenize(tweet) # this tokenize each tweet into smaller text tokens, which are then passed on to analyzer (which then
                                             # tokenizes the texts further to be analyzed more easily)
            for text in texts:
                score += analyzer.analyze(text)
            if score > 0.0:
                #tweet is positive
                positive+=1
                neutral-=1
            elif score < 0.0:
                #tweet is negative
                negative+=1
                neutral-=1
                
            score = 0 #reset for analysis on next tweets (otherwise, it will be cumulated lol)
        ###################################################################################################################################
    
    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
