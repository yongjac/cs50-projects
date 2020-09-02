import os
import re

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

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

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # get stock symbols and names
    stocks = db.execute("SELECT symbol, name FROM purchases WHERE user_id = ? GROUP BY symbol", session["user_id"])
    total = 0

    # loop through stocks
    for i in range(len(stocks)):

        # get stock shares
        price = db.execute("SELECT SUM(shares) FROM purchases WHERE user_id = ? AND symbol = ?", session["user_id"], stocks[i]["symbol"])
        stocks[i]["shares"] = price[0]["SUM(shares)"]

        # get stock price
        quote = lookup(stocks[i]["symbol"])
        stocks[i]["price"] = usd(quote["price"])

        # get stock total price
        stocks[i]["TOTAL"] = usd(quote["price"] * stocks[i]["shares"])

        total += round(quote["price"] * stocks[i]["shares"], 2)

    # get user's cash
    cash_table = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = usd(cash_table[0]["cash"])

    # get total value of user's cash and stocks
    total += cash_table[0]["cash"]
    total = usd(total)

    # show portfoilio
    return render_template("index.html", stocks = stocks, cash = cash, total = total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # When requested via GET, ask user for a stock's symbol
    if request.method == "GET":
        return render_template("buy.html")

    # User reached route via POST
    else:

        # Ensure username was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 403)

        # Ensure password was submitted
        if not request.form.get("shares").isdigit():
            return apology("must provide positive integer for shares", 403)

        # Ensure shares not 0
        shares = int(request.form.get("shares"))
        if shares == 0:
            return apology("must provide positvie integer for shares", 403)

        # look up quote
        quote_found = lookup(request.form.get("symbol"))

        # Ensure quote found
        if quote_found == None:
            return apology("Symbol not found", 403)

        # get cash
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

        # Calculate total price
        total_price = round(quote_found["price"] * shares, 2)

        # Ensure enough cash
        if float(cash[0]["cash"]) < total_price:
            return apology("Not enough cash", 403)

        # Insert purchase
        db.execute("INSERT INTO purchases (user_id, symbol, name, shares, price) VALUES (?, ?, ?, ?, ?)", session["user_id"], quote_found["symbol"], quote_found["name"], shares, quote_found["price"])

        # update cash
        cash[0]["cash"] -= total_price
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash[0]["cash"], session["user_id"])

        # Redirect user to homepage
        flash("Brought")
        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # get stocks
    stocks = db.execute("SELECT * FROM purchases WHERE user_id = ?", session["user_id"])

    # change prices from float to usd
    for i in range(len(stocks)):
        stocks[i]["price"] = usd(stocks[i]["price"])

    return render_template("history.html", stocks=stocks)


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


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # When requested via GET, ask user for a stock's symbol
    if request.method == "GET":
        return render_template("quote.html")

    # User reached route via POST
    else:
        # look up quote
        quote_found = lookup(request.form.get("symbol"))

        # if found show name, symbol and price of quote
        if quote_found != None:
            return render_template("quoted.html", name = quote_found["name"], price = quote_found["price"], symbol = quote_found["symbol"])

        # if not found show error message
        else:
            return apology("Symbol not found", 403)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # When requested via GET, display registration form
    if request.method == "GET":
        return render_template("register.html")

    # User reached route via POST
    else:

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure password (again) was submitted
        elif not request.form.get("password (again)"):
            return apology("must provide password", 403)

        # Ensure password contain at least one letter, number, and symbol
        if not any(not c.isalnum() for c in request.form.get("password")) or not re.search('[a-zA-Z]', request.form.get("password")) or not re.search('[0-9]', request.form.get("password")):
            return apology("password must contain at least one letter, number, and symbol")

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username don't already exist
        if len(rows) == 1:
            return apology("username already exists", 403)

        # Ensure passwords match
        if request.form.get("password") != request.form.get("password (again)"):
            return apology("passwords must match", 403)

        # Insert new user
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", request.form.get("username"), generate_password_hash(request.form.get("password")))

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to homepage
        flash("Registered")
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via GET (as by clicking a link or via redirect)
    if request.method == "GET":

        # get stock symbols
        stocks = db.execute("SELECT symbol FROM purchases WHERE user_id = ? GROUP BY symbol", session["user_id"])

        return render_template("sell.html", stocks=stocks)

    # User reached route via POST
    else:

        # Render an apology if user fail to select a stock
        symbol = request.form.get("symbol")
        if symbol == "Symbol":
            return apology("must select a stock", 403)

        # Render an apology if user does not own any shares of that stock
        share = db.execute("SELECT SUM(shares) FROM purchases WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)
        share = share[0]["SUM(shares)"]
        if share <= 0:
            return apology("User does not own any shares of that stock", 403)

        # Render an apology if shares not an integer
        shares = request.form.get("shares")
        if not shares.isdigit():
            return apology("shares must be a positive integer", 403)

        # Render an apology if shares is 0
        shares = int(shares)
        if shares == 0:
            return apology("shares must be a positive integer", 403)

        # Render an apology if trying to sell more shares than user has
        if shares > share:
            return apology("Trying to sell more shares than a user has")

        # look up quote
        quote_found = lookup(symbol)

        # Calculate total price
        total_price = round(quote_found["price"] * shares, 2)

        # Insert purchase
        db.execute("INSERT INTO purchases (user_id, symbol, name, shares, price) VALUES (?, ?, ?, ?, ?)", session["user_id"], quote_found["symbol"], quote_found["name"], -shares, quote_found["price"])

        # update cash
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash[0]["cash"] += total_price
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash[0]["cash"], session["user_id"])

        # redirect to homepage with message
        flash("Sold")
        return redirect("/")

@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    # When requested via GET, display registration form
    if request.method == "GET":
        return render_template("change_password.html")

    # User reached route via POST
    else:

        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure password (again) was submitted
        elif not request.form.get("password (again)"):
            return apology("must provide password", 403)

        # Ensure new password was submitted
        elif not request.form.get("new password"):
            return apology("must provide new password", 403)

        # Ensure password contain at least one letter, number, and symbol
        if not any(not c.isalnum() for c in request.form.get("new password")) or not re.search('[a-zA-Z]', request.form.get("new password")) or not re.search('[0-9]', request.form.get("new password")):
            return apology("password must contain at least one letter, number, and symbol")

        # Ensure passwords match
        if request.form.get("password") != request.form.get("password (again)"):
            return apology("passwords must match", 403)

        # Query database for password
        rows = db.execute("SELECT * FROM users WHERE id = :id",
                          id=session["user_id"])

        # Ensure password is correct
        if not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid password", 403)

        #update password
        db.execute("UPDATE users SET hash = ? WHERE id = ?", generate_password_hash(request.form.get("new password")), session["user_id"])

        # redirect to homepage with message
        flash("Password changed")
        return redirect("/")

@app.route("/add_cash", methods=["GET", "POST"])
@login_required
def add_cash():
    # When requested via GET, display registration form
    if request.method == "GET":
        return render_template("add_cash.html")

    # User reached route via POST
    else:

        # Ensure input is float
        input = request.form.get("cash")
        try:
            float(input)
        except ValueError:
            return apology("cash must be a number")

        # Ensure input more than 0
        input = float(input)
        if input <= 0:
            return apology("cash must be more than 0")

        # get cash
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = round(cash[0]["cash"] + input, 2)

        # update user cash
        db.execute("UPDATE users SET cash = ? where id = ?", cash, session["user_id"])

        # redirect to homepage with message
        flash("Cash added")
        return redirect("/")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
