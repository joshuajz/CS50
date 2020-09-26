# TODO:
# Formating all cash as cash (2 decimal places, dollar sign)
# TOTAL line at the bottom of the index page
# CS50's solution: https://finance.cs50.net/history

import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

from datetime import datetime

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
    # Determines all of our holdings
    x = db.execute(
        "SELECT * FROM holdings WHERE userid=:usrid", usrid=session["user_id"]
    )
    # Determines amount of cash
    cash = db.execute(
        "SELECT cash FROM users WHERE id=:sessionid", sessionid=int(session["user_id"])
    )[0]["cash"]
    # List of transactions
    transactions = []
    # Loop through all of the holdings
    total = cash
    for stock in x:
        look = lookup(stock["symbol"])
        transactions.append(
            {
                "symbol": stock["symbol"],
                "name": look["name"],
                "shares": stock["shares"],
                "price": "{:0,.2f}".format(look["price"]),
                "total": "{:0,.2f}".format(look["price"] * stock["shares"]),
            }
        )
        total += look["price"]
    # Render
    return render_template(
        "portfolio.html",
        index=transactions,
        cash="{:0,.2f}".format(cash),
        total="{:0,.2f}".format(total),
    )


@app.route("/cash_injection", methods=["GET", "POST"])
@login_required
def cash_injection():
    if request.method == "GET":
        # Display the webpagae
        return render_template("cash_injection.html")
    else:
        cash = db.execute(
            "SELECT cash FROM users WHERE id=:sessionid",
            sessionid=int(session["user_id"]),
        )[0]["cash"]
        injection = int(request.form.get("cash_injection"))
        print(injection)
        if not injection or injection < 0:
            return apology("Invalid cash amount", 404)
        newcash = cash + injection
        db.execute(
            "UPDATE users SET cash = :newcash WHERE id = :usrid",
            newcash=newcash,
            usrid=session["user_id"],
        )
        return redirect("/")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        # Display the webpage
        return render_template("buy.html")
    else:
        # Provides information about the symbol
        symbol = lookup(request.form.get("symbol"))
        # If that symbol isn't valid, return an apology
        if not symbol:
            return apology("Invalid Symbol", 404)
        # Determines amount of shares + checks if valid
        shares = request.form.get("shares")
        if int(shares) < 1:
            return apology("Must be 1+ Shares", 404)
        # Determines amount of cash the user has
        cash = db.execute(
            "SELECT cash FROM users WHERE id=:sessionid",
            sessionid=int(session["user_id"]),
        )[0]["cash"]
        # Determines how much this purchase costs
        loss = float(symbol["price"]) * float(shares)
        # If the user can't afford it, render an apology
        if loss > cash:
            return apology("Not enough cash", 404)
        # Updates the cash value to the new value in the database
        cash = cash - loss
        db.execute(
            """
            UPDATE users
            SET cash = :cashamnt
            WHERE id = :sessionid
        """,
            cashamnt=cash,
            sessionid=int(session["user_id"]),
        )
        # Provides the current datetime
        date = (str(datetime.now())).split(".")[0]
        # Inserts the information into the transactions tab
        db.execute(
            """
        INSERT INTO transactions (userid, symbol, shares, price, datetime)
        VALUES (:userid, :symbol, :shares, :price, :time)
        """,
            userid=int(session["user_id"]),
            symbol=symbol["symbol"],
            shares=shares,
            price=symbol["price"],
            time=date,
        )

        # Checks the database to see if the current stock already has a holding
        amnt_owned = db.execute(
            "SELECT shares FROM holdings WHERE userid = :usrid AND symbol = :symbol",
            usrid=session["user_id"],
            symbol=symbol["symbol"],
        )
        if amnt_owned:
            # Update the amount of stock that is owned by the user
            amnt_owned = amnt_owned[0]["shares"] + int(shares)
            print(amnt_owned)
            db.execute(
                "UPDATE holdings SET shares = :shares WHERE symbol = :symbol AND userid = :usrid",
                shares=amnt_owned,
                symbol=symbol["symbol"],
                usrid=session["user_id"],
            )
        # If we don't already have a holding, create one
        else:
            db.execute(
                "INSERT INTO holdings (userid, symbol, shares) VALUES (:usrid, :symbol, :shares)",
                usrid=session["user_id"],
                symbol=symbol["symbol"],
                shares=int(shares),
            )

        # Return to transactions page + I want to render a bar that says "bought"
        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    x = db.execute(
        "SELECT * FROM transactions WHERE userid=:usrid", usrid=session["user_id"]
    )
    return render_template("history.html", transactions=x)


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
        rows = db.execute(
            "SELECT * FROM users WHERE username = :username",
            username=request.form.get("username"),
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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
        # Displays the site
        return render_template("quote.html")
    else:
        # The symbol provided
        symbol = request.form.get("symbol")
        # Looks up the info
        info = lookup(symbol)
        if not info:
            # Invalid stock, render apology
            return apology("Invalid Symbol", 404)
        else:
            # Correct stock, provide information to page
            return render_template(
                "quoted.html",
                name=info["name"],
                symbol=info["symbol"],
                price=info["price"],
            )


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # List of all users
        userlist = db.execute("SELECT username FROM users")
        # Determines the username provided
        username = request.form.get("username")
        # If it's an invalid username, render apology
        if not username:
            return apology("must provide username", 403)
        # If the username is already in the database, render apology
        if username in userlist:
            return apology("username already exists", 403)
        """
        for user in userlist:
            if user['username'] == username:
                return apology("username already exists", 403)
        """
        # Determins the password + confirmation provided
        password = request.form.get("password")
        confirmpassword = request.form.get("confirmation")
        # Invalid password, render apology
        if not password:
            return apology("must provide password", 403)
        # Password does not equal confirmation, render apology
        if password != confirmpassword:
            return apology("passwords do not match", 403)
        # Insert the new user into the database
        db.execute(
            "INSERT INTO users (username, hash) VALUES (:user, :hashed)",
            user=username,
            hashed=generate_password_hash(password),
        )
        # Redirect to index page
        return redirect("/")
    else:
        # Render the page
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # Stock that is being sold
        stock = request.form.get("stocks")
        # Determine information about the stock
        stock = lookup(stock)
        if not stock:
            # Invalid stock, render apology
            return apology("Invalid stock", 404)
        # Determines the amount of shares
        shares = int(request.form.get("shares"))

        if shares <= 0:
            # Invalid amount of shares, render apology
            return apology("Invalid amount of shares", 404)

        # Determines the current share amount
        current_share_amnt = db.execute(
            "SELECT shares FROM holdings WHERE userid=:usrid AND symbol=:sym",
            usrid=session["user_id"],
            sym=stock["symbol"],
        )[0]["shares"]
        # Determines how much cash the user has
        cash = db.execute(
            "SELECT cash FROM users WHERE id=:sessionid",
            sessionid=int(session["user_id"]),
        )[0]["cash"]

        # Can the user sell this amount?
        if current_share_amnt < shares:
            return apology("Not enough stocks to sell", 404)

        # Updated stock amount
        final_shares = current_share_amnt - shares

        # Update Holdings
        if final_shares == 0:
            # We have 0 now, so delete this row
            db.execute(
                "DELETE FROM holdings WHERE userid=:usrid AND symbol=:symbol",
                usrid=session["user_id"],
                symbol=stock["symbol"],
            )
        else:
            # Updated stock amount
            db.execute(
                "UPDATE holdings SET shares=:finalshares WHERE userid=:usrid AND symbol=:symbol",
                finalshares=final_shares,
                usrid=session["user_id"],
                symbol=stock["symbol"],
            )

        # Updated cash amount with sale
        cash += shares * stock["price"]
        # Update cash in database
        db.execute(
            "UPDATE users SET cash = :newcash WHERE id = :usrid",
            newcash=cash,
            usrid=session["user_id"],
        )

        # Add the transaction to the transaction database
        # Datetime
        date = (str(datetime.now())).split(".")[0]
        # Inserts the information into the transactions tab
        db.execute(
            """
        INSERT INTO transactions (userid, symbol, shares, price, datetime)
        VALUES (:userid, :symbol, :shares, :price, :time)
        """,
            userid=int(session["user_id"]),
            symbol=stock["symbol"],
            shares=shares * -1,
            price=stock["price"],
            time=date,
        )

        # Redirect to the index page
        return redirect("/")

    else:
        # ALl of the stocks that the user owns
        x = db.execute(
            "SELECT symbol FROM holdings WHERE userid=:usrid", usrid=session["user_id"]
        )
        # Fills a list
        stocks = []
        for stock in x:
            stocks.append(stock["symbol"])
        # Renders
        return render_template("sell.html", stocks=stocks)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
