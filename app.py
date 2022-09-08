import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    #just to declare information so the method will be get
    user_id = session["user_id"]

    #get the information to display
    stocksList = db.execute("SELECT symbol, name, price, SUM(shares) as totalShares FROM transactions WHERE user_id = ? GROUP BY symbol", user_id)
    #cash of the user
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

    total = cash
    for stock in stocksList:
        total += stock["price"] * stock["totalShares"]
    return render_template("index.html", stocks=stocksList, cash=cash, usd=usd, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        stockInfo = lookup(symbol)

        #checking th eipnut
        if not symbol:
            return apology("Please enter a symbol")
        elif not stockInfo:
            return apology("Insert a valid symbol")

        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("The number of shares must be an integer")

        if shares <= 0:
            return apology("Shares must be a positive integer")

        user_id = session["user_id"] #the foreign key
        cashDict = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        #cashDict is a dictionary so to get the cash
        cash = cashDict[0]["cash"]
        stockInfoName = stockInfo["name"]
        stockInfoPrice = stockInfo["price"]
        totalPrice = stockInfoPrice * shares

        if cash < totalPrice:
            return apology("sorry you don't have enough cash")
        else:
            #if the user can afford to buy those stocks
            remainedCash = cash - totalPrice #money the user still has after buying the stocks
            db.execute("UPDATE users SET cash = ? WHERE id = ?", remainedCash, user_id)
            db.execute("INSERT INTO transactions (user_id, name, shares, price, type, symbol) VALUES(?, ?, ?, ?, ?, ?)",
                        user_id, stockInfoName, shares, stockInfoPrice, 'buy', symbol)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    transactions = db.execute("SELECT type, symbol, price, shares, time FROM transactions WHERE user_id = ?", user_id)
    return render_template("history.html", transactions=transactions, usd=usd)

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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

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


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    else:
        #get the symbol entered by the user
        symbol = request.form.get("symbol").upper()
        #checking symbol is not blank
        if not symbol:
            return apology("Must enter a symbol")
    print(symbol)
    stockInfo = lookup(symbol)
    print(stockInfo)
    if not stockInfo:
        return apology("Symbol not valid")

    return render_template("quoted.html", stockInfo = stockInfo, todollars = usd)

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if (request.method == "POST"):
        #to obtain the values of the input fields
        username = request.form.get("username")
        password = request.form.get("password")
        confirmPassword = request.form.get("confirmation")
        #checking if the input is blank
        if not username:
            return apology("Must insert a username")
        elif not password:
            return apology("Must insert a password")
        elif not confirmPassword:
            return apology("Password confirmation is required")
        elif password != confirmPassword:
            return apology("Password and its confirmation must match")

        #hashing the password
        hash = generate_password_hash(password)
        #adding the datas in the database while checking if the username is unique
        try:
            db.execute("INSERT INTO users(username, hash) VALUES(:username, :hash)", username, hash)
            return redirect("/login")
        except:
            return apology("Username already in use, please choose another one")

    else: # if the method is GET instead
        return render_template("registration.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]
    if request.method == "POST":
        symbol = request.form.get("symbol")
        sharesToSell = int(request.form.get("shares"))

        if sharesToSell <= 0:
            return apology("Must provide a valid number of shares")

        stockPrice = lookup(symbol)["price"]
        stockName = lookup(symbol)["name"]
        moneyEarned = sharesToSell * stockPrice

        #check if the number of shares is not superior to the ones owned
        sharesOwned = db.execute("SELECT shares FROM transactions WHERE user_id = ? AND symbol = ? GROUP BY symbol", user_id, symbol)[0]["shares"]
        if sharesToSell > sharesOwned:
            return apology ("You don't have that many shares to sell")

        #update the current cash after selling
        currentCash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
        newCash = currentCash + moneyEarned
        db.execute("UPDATE users SET cash = ? WHERE id = ?", newCash, user_id)
        db.execute("INSERT INTO transactions (user_id, name, shares, price, type, symbol) VALUES (?, ?, ?, ?, ?, ?)",
                    user_id, stockName, -sharesToSell, stockPrice, "sell", symbol)
        return redirect('/')
    else:
        symbols = db.execute("SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol", user_id)
        return render_template("sell.html", symbols=symbols)

