from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():

    userID = session['user_id']
    # get the stocks for this particular users
    stocksymbols = db.execute("SELECT symbol, quantity FROM transactions WHERE userID = :userID GROUP BY symbol;", \
                                userID=session['user_id'])

    stocktotal = 0
    cash = db.execute("SELECT cash FROM users WHERE id = :userID;", userID = userID)
    cash = float(cash[0]['cash'])

    for stock in stocksymbols:
        symbol = stock["symbol"]
        quantity = stock["quantity"]

        try:
            stockData = lookup(stock["symbol"])
        except:
            return apology('Cannot connect to the stock database')

        name = stockData["name"]
        price = float(stockData['price'])
        totalcost = quantity * price
        stocktotal += totalcost

        db.execute("UPDATE transactions SET price = :price, totalcost = :totalcost WHERE userID = :userID AND symbol = :symbol", \
                    price=usd(price), totalcost=usd(totalcost), userID=session["user_id"], symbol=symbol)

    # update their grand total
    grandtotal = stocktotal + cash

    # get their updated portfolio for display on index.html
    portfolio = db.execute("SELECT * from transactions WHERE userID = :userID", userID = session["user_id"])

    return render_template("index.html", stocks=portfolio, cash=usd(cash), \
                            stocktotal=usd(stocktotal), grandtotal=usd(grandtotal))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""

    if request.method == 'POST':

        userID = session['user_id']
        shareNo = int(request.form.get("shareNo"))
        cash = db.execute("SELECT cash FROM users WHERE id = :userID;", userID = userID)
        cash = float(cash[0]['cash'])

        try:
            stock = request.form.get("stock")
            quote = lookup(stock)
            name = quote['name']
            price = float(quote['price'])
            symbol = quote['symbol']
        except:
            return apology("That stock could not be found.")

        if not symbol:  # make sure symbol is valid
            return apology("Stock could not be found.")

        elif shareNo < 0:
            return apology("Invalid number of shares.")

        elif cash < price * shareNo:  # if user can afford
            return apology('Insufficient funds.')

        else:

            totalcost = price * shareNo

            # insert new stocks into users portfolio
            db.execute("INSERT INTO transactions (userID, symbol, name, quantity, price, totalcost) \
            VALUES (:userID, :symbol, :name, :quantity, :price, :totalcost);", \
            name=name, symbol=symbol, quantity=shareNo, price=price, userID=userID, totalcost=totalcost)

            # update users balance
            db.execute("UPDATE users SET cash = cash - :totalcost WHERE id = :userID;", \
            totalcost = totalcost, userID = userID)

            return redirect(url_for("index"))


    else:
        return render_template("buy.html")

    return apology("TODO")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    return apology("TODO")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == 'POST':
        try:
            stock = request.form.get("stock")
            quote = lookup(stock)
            name = quote['name']
            price = quote['price']
            symbol = quote['symbol']
        except:
            return apology("That stock could not be found.")

        if not quote:
            return apology("That stock could not be found.")

        else:
            quote['price'] = usd(quote['price'])    # convert price to usd
            return render_template("quoted.html", name=name, price=price, symbol=symbol)  # return the quoted page

    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""

    if request.method == "POST":    # make sure page is reached via POST NOT GET

        if request.form.get("username") == "":
            return apology("Missing username!")

        elif request.form.get("password") == "":
            return apology("Missing password!")

        elif request.form.get("confirmpass") == "":
            return apology("Missing password confirmation!")

        elif request.form.get("password") != request.form.get("confirmpass"):
            return apology("Passwords must match!")

        result = db.execute("INSERT INTO users (username, hash) \
                            VALUES (:username, :hash)", \
                            username=request.form.get("username"), \
                            hash=pwd_context.hash(request.form.get("password")))

        if not result:  # user is primary key so cannot be duplicates
            return apology("Username is already in use.")

        # store userID in session, and keep user logged in
        user_id = db.execute("SELECT id FROM users WHERE username IS :username",\
        username=request.form.get("username"))
        session['user_id'] = user_id[0]['id']
        return redirect(url_for("index"))

    else:   # make sure not GET request (details could be seen in the url)
        return render_template("register.html") # reload page if not POST


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""

    if request.method == 'POST':

        userID = session['user_id']
        sharestosell = int(request.form.get("shareNo"))
        cash = db.execute("SELECT cash FROM users WHERE id = :userID;", userID = userID)
        cash = float(cash[0]['cash'])

        try:
            stock = request.form.get("stock")
            quote = lookup(stock)
            name = quote['name']
            price = float(quote['price'])
            symbol = quote['symbol']
        except:
            return apology("That stock could not be found.")

        if shareNo < 0:
            return apology("You can't sell negative shares!")

        currentshares = db.execute("SELECT quantity FROM transactions WHERE userID = :userID \
                                AND symbol = :symbol", userID = userID, symbol = symbol)

        print(str(currentshares))

        # check user has enough shares to make this sell
        if currentshares < sharestosell:
            return apology("You do not have this many shares to sell!")

        else:
            print('you can do it')

    else:
        return render_template("sell.html")

    return apology("TODO")
