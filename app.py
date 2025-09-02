import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from datetime import datetime
from datetime import date
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///justbefore.db")

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
    """Show inserted products"""
    # just to declare information so the method will be get
    user_id = session["user_id"]

    # get the information to display
    products = db.execute("SELECT name, expiring_date, quantity, unit FROM products WHERE user_id = ? ORDER BY expiring_date", user_id)
    for product in products:
        product["expiring_date"] = product["expiring_date"].split(" ")[0]

    return render_template("index.html", products=products)

@app.route("/add", methods=["GET", "POST"])
@login_required
def add():
    """Adding new product"""
    if request.method == "POST":
        # getting the name of the product
        productName = request.form.get("product")
        expiringDate = request.form.get("expiringDate")
        quantity = request.form.get("quantity")
        quantity = int(quantity)
        unit = request.form.get("unit")

        #checking the input
        if not productName:
            return apology("Must insert a name")
        elif not expiringDate:
            return apology("Expiring date required")
        elif not quantity:
            return apology("Quantity required")
        elif not unit:
            return apology("Please insert a measurement unit")
        else:
            user_id = session["user_id"]
            dateObject = datetime.strptime(expiringDate, "%Y-%m-%d")
            today = datetime.today()
            if today > dateObject:
                return apology("Must insert a valid date")
            else:
                db.execute("INSERT INTO products (user_id, name, expiring_date, quantity, unit) VALUES (?, ?, ?, ?, ?)",
                            user_id, productName, dateObject, quantity, unit)
                return render_template("add.html")
    else:
        return render_template("add.html")

@app.route("/search", methods=["GET", "POST"])
@login_required
def search():
    if request.method == "POST":
        searchedProduct = request.form.get("searched")
        if not searchedProduct:
            return apology("Must provide a name")
        #list of the products names
        user_id = session["user_id"]
        compatibles = []
        productsList = db.execute("SELECT name FROM products WHERE user_id = ? ORDER BY expiring_date", user_id)
        for product in productsList:
            if searchedProduct in product["name"]:
                productInfo = db.execute("SELECT name, expiring_date, quantity, unit FROM products WHERE name = ? ORDER BY expiring_date", product["name"])
                for eachProduct in productInfo:
                    eachProduct["expiring_date"] = eachProduct["expiring_date"].split(" ")[0]
                compatibles.append(productInfo)

        return render_template("search.html", compatibles=compatibles)
    else:
        return render_template("search.html")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("Must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("Must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("Invalid username and/or password", 403)

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

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if (request.method == "POST"):
        # to obtain the values of the input fields
        username = request.form.get("username")
        email = request.form.get("email")
        password = request.form.get("password")
        confirmPassword = request.form.get("confirmation")

        # checking if the input is blank
        if not username:
            return apology("Must insert a username")
        elif not email:
            return apology("Must insert an email")
        elif not password:
            return apology("Must insert a password")
        elif not confirmPassword:
            return apology("Password confirmation is required")
        elif password != confirmPassword:
            return apology("Password and its confirmation must match")

        # adding the datas in the database while checking if the username and the email are unique
        alreadyRegistered = db.execute("SELECT username FROM users")
        emailAlreadyUsed = db.execute("SELECT email FROM users")
        for registeredUsername in alreadyRegistered:
            if registeredUsername["username"] == username:
                return apology("Username already in use, please choose another one")
        for registeredEmail in emailAlreadyUsed:
            if registeredEmail["email"] == email:
                return apology("This email has already been used")

        # hashing the password and adding the data in the database
        hash = generate_password_hash(password)
        db.execute("INSERT INTO users(username, email, hash) VALUES(?, ?, ?)", username, email, hash)
        return render_template("login.html")
    else:  # if the method is GET instead
        return render_template("register.html")

@app.route("/account", methods=["GET", "POST"])
@login_required
def account():
    user_id = session["user_id"]
    if request.method == "POST":
        newUsername = request.form.get("newUsername")
        newEmail = request.form.get("newEmail")
        newPassword = request.form.get("newPassword")
        confirmation = request.form.get("confirmation")
        # possibly update username
        if newUsername:
            alreadyRegistered = db.execute("SELECT username FROM users")
            for registeredUsername in alreadyRegistered:
                if registeredUsername["username"] == newUsername:
                    return apology("Username already in use, please choose another one")
            db.execute("UPDATE users SET username = ? WHERE id = ?", newUsername, user_id)
        # possibly update email
        if newEmail:
            emailAlreadyUsed = db.execute("SELECT email FROM users")
            for registeredEmail in emailAlreadyUsed:
                if registeredEmail["email"] == newEmail:
                    return apology("This email has already been used")
            db.execute("UPDATE users SET email = ? WHERE id = ?", newEmail, user_id)
        # possibly update password
        if newPassword:
            if newPassword != confirmation:
                return apology("New password and its confirmation must match")
            newPassword = hash(newPassword)
            db.execute("UPDATE users SET hash = ? WHERE id = ?", newPassword, user_id)
        userInfo = db.execute("SELECT username, email, hash FROM users WHERE id = ?", user_id)
        displayedInfo = userInfo[0]
        displayedInfo["hash"] = "*********"
        return render_template("account.html", info=displayedInfo)
    else:
        userInfo = db.execute("SELECT username, email, hash FROM users WHERE id = ?", user_id)
        displayedInfo = userInfo[0]
        displayedInfo["hash"] = "*********"
        return render_template("account.html", info=displayedInfo)

@app.route("/delete", methods=["GET", "POST"])
@login_required
def delete():
    user_id = session["user_id"]
    if request.method == "POST":
        selected = request.form.get("productName")
        if selected == "Select a product":
            return apology("Must select a product")
        db.execute("DELETE FROM products WHERE name = ?", selected)
        productsNames = db.execute("SELECT name FROM products WHERE user_id = ? ORDER BY name", user_id)
        return render_template("delete.html", productsNames=productsNames, selected=selected)
    else:
        productsNames = db.execute("SELECT name FROM products WHERE user_id = ? ORDER BY name", user_id)
        return render_template("delete.html", productsNames=productsNames, selected=None)

@app.route("/change", methods=["GET", "POST"])
@login_required
def change():
    user_id = session["user_id"]
    productsNames = db.execute("SELECT name FROM products WHERE user_id = ? ORDER BY name", user_id)
    if request.method == "POST":
        selected = request.form.get("productName")
        quantity = request.form.get("quantity")
        unit = request.form.get("unit")
        expiring_date = request.form.get("expiringDate")
        if selected == "Select a product":
            return apology("Must select a product")
        if quantity:
            db.execute("UPDATE products SET quantity = ? WHERE name = ? AND user_id = ? ", quantity, selected, user_id)
        if unit:
            db.execute("UPDATE products SET unit = ? WHERE name = ? AND user_id = ? ", unit, selected, user_id)
        if expiring_date:
            dateObject = datetime.strptime(expiringDate, "%Y-%m-%d")
            today = datetime.today()
            if today > dateObject:
                return apology("Must insert a valid date")
            else:
                db.execute("UPDATE products SET expiring_date = ? WHERE name = ? AND user_id = ? ", expiring_date, selected, user_id)
        return render_template("change.html", productsNames=productsNames, selected=selected)
    else:
        return render_template("change.html", productsNames=productsNames, selected=None)
