from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/history")
@login_required
def history():

    # get all transactions
    transDB = db.execute("SELECT * FROM trans WHERE user_id=:uid ORDER BY trans_id",
                         uid=session["user_id"])
    print("archerboi + \n" + str(transDB))

    for trans in transDB:
        # find if transaction is a buy or sell
        if trans["quantity"] < 0:
            trans["action"] = "Sell"
            trans["quantity"] = -(trans["quantity"])
            trans["total_cost"] = -(trans["total_cost"])

        else:
            trans["action"] = "Buy"

        # make all usd
        trans["price"] = "$" + str(trans["price"])
        trans["total_cost"] = usd(trans["total_cost"])

    return render_template("history.html", transDB=transDB)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")

@app.route("/")
@login_required
def index():

    # make stockDB for jinja to iterate over
    stockDB = db.execute("SELECT symbol, SUM(quantity), price, SUM(total_cost) FROM trans WHERE user_id=:uid GROUP BY symbol ORDER BY SUM(total_cost);",
                         uid=session["user_id"])

    totalPortfolioValue = 0

    for x in stockDB:
        # get updated price
        quote = lookup(x['symbol'])
        x['price'] = float(quote['price'])

        x['quantity'] = x['SUM(quantity)']
        x['total_cost'] = float(x['quantity']) * float(x['price'])
        totalPortfolioValue += x['total_cost']

        # usd everything
        x['price'] = "$" + str(x['price'])
        x['total_cost'] = usd(x['total_cost'])

    print("\n wagwanmyg" + str(stockDB) + "\ntotalportfoliovalue = " + str(totalPortfolioValue))

    # get available cash amount
    cash = db.execute("SELECT cash FROM users WHERE id=:uid",
                        uid=session["user_id"])

    print("\n bruv" + str(cash) + "\ncashtings")

    cash = cash[0]['cash']
    totalPortfolioValue += cash

    # get everything in usd form
    cash = usd(cash)
    totalPortfolioValue = usd(totalPortfolioValue)

    return render_template("index.html", stockDB=stockDB, cash=cash, totalPortfolioValue=totalPortfolioValue)




@app.route("/sell/<sellSym>", methods=["GET"])
@login_required
def sell(sellSym):
    """Sell shares of stock"""
    if request.method == "POST":

        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")

        if request.form.get("symbol") == "":
            return apology("Missing stock symbol!")

        elif lookup(symbol) == None:
            return apology("Not a valid stock symbol")

        elif not request.form.get("shares").isnumeric() or int(shares) < 0:
            return apology("Share number invalid")

        else:
            # get share symbol and no. of shares
            # get price of shares
            # minus those shares from transaction table
            # add cash to users table
            quote = lookup(symbol)
            price = float(quote["price"])
            name = quote["name"]
            symbol = quote["symbol"]
            sell_quantity = int(shares)

            # check if user has enough to sell
            currentShares = db.execute("SELECT SUM(quantity) FROM trans WHERE user_id=:uid AND symbol=:symbol",
                                        uid=session["user_id"], symbol=symbol)
            currentShares = int(currentShares[0]["SUM(quantity)"])
            if currentShares < sell_quantity:
                return apology("You don't have that many shares to sell!")

            else:
                # sell shares

                quantity = -sell_quantity
                total_cost = price * quantity

                db.execute("INSERT INTO trans (user_id, symbol, price, quantity, total_cost) VALUES (:user_id, :symbol, :price, :quantity, :total_cost)"
                           ,user_id=session["user_id"], symbol=symbol, price=price, quantity=quantity, total_cost=total_cost)

                # return money to user
                current_balance = db.execute("SELECT cash FROM users WHERE id = :user_id",
                                         user_id=session["user_id"])
                current_balance = current_balance[0]['cash']

                new_balance = current_balance - total_cost

                db.execute("UPDATE users SET cash=:new_balance WHERE id=:user_id",
                           new_balance=new_balance, user_id=session["user_id"])

                return redirect("/")

    else:

        # if from index page sell button make autofocus on quantity
        if sellSym == "none":
            autosymbol = "autofocus"
            autoquantity = ""
            return render_template("sell.html", sellSym="", autosymbol=autosymbol, autoquantity=autoquantity)

        # if from nav make autofocus on quantity
        else:
            autoquantity = "autofocus"
            autosymbol = ""

        return render_template("sell.html", sellSym=sellSym, autosymbol=autosymbol, autoquantity=autoquantity)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():

    if request.method == "POST":

        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")

        if request.form.get("symbol") == "":
            return apology("Missing stock symbol!")

        elif lookup(symbol) == None:
            return apology("Not a valid stock symbol")

        elif not request.form.get("shares").isnumeric() or int(shares) < 0:
            return apology("Share number invalid")

        else:
            quote = lookup(symbol)
            price = float(quote["price"])
            name = quote["name"]
            symbol = quote["symbol"]

            # check if user can afford stock
            quantity = int(shares)
            current_balance = db.execute("SELECT cash FROM users WHERE id = :user_id",
                                         user_id=session["user_id"])
            current_balance = current_balance[0]['cash']
            total_cost = float(quantity) * price
            print("total cost = " + str(total_cost))

            if current_balance < total_cost:
                return apology("You can't afford that!")

            else:
                # create transaction
                db.execute("INSERT INTO trans (user_id, symbol, price, quantity, total_cost) VALUES (:user_id, :symbol, :price, :quantity, :total_cost)"
                           ,user_id=session["user_id"], symbol=symbol, price=price, quantity=quantity, total_cost=total_cost)

                # take money from user
                new_balance = current_balance - total_cost

                db.execute("UPDATE users SET cash=:new_balance WHERE id=:user_id",
                           new_balance=new_balance, user_id=session["user_id"])

                return redirect("/")


    else:
        return render_template("buy.html")



@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():

    if request.method == "POST":

        symbol = request.form.get("symbol")

        if request.form.get("symbol") == "":
            return apology("Missing stock symbol!")

        elif lookup(request.form.get("symbol")) == None:
            return apology("Invalid stock symbol!")

        else:
            quote = lookup(symbol)

            print("bitchwaddap" + str(quote))

            name = quote["name"]
            price = quote["price"]
            symbol = quote["symbol"]
            price = usd(price)

            return render_template("quoted.html", symbol=symbol, name=name, price=price)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():

    session.clear()

    if request.method == "POST":    # make sure page is reached via POST NOT GET

        if request.form.get("username") == "":
            return apology("Missing username!")

        elif request.form.get("password") == "":
            return apology("Missing password!")

        elif request.form.get("confirmation") == "":
            return apology("Missing password confirmation!")

        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("Passwords must match!")

        result = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                            username=request.form.get("username"), hash=generate_password_hash(request.form.get("password")))

        if not result:
            return apology("Username is already in use.")

        # store userID in session, and keep user logged in
        user_id = db.execute("SELECT id FROM users WHERE username IS :username",
                             username=request.form.get("username"))

        session['user_id'] = user_id[0]['id']
        return redirect("/login")

    else:   # make sure not GET request (details could be seen in the url)
        return render_template("register.html") # reload page if not POST




def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
