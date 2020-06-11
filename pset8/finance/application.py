import os

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
    # Use a place holder ':curr_id' to call the session id which is the user's id
    rows = db.execute("SELECT stocks.symbol, stocks.name, portfolio.shares FROM portfolio JOIN users ON users.id = portfolio.user_id JOIN stocks ON portfolio.stock_id = stocks.id WHERE users.id==:curr_id", curr_id=session["user_id"])
    # Make a select query only on cash to be able to display it in portfolio's table
    row_cash = db.execute("SELECT cash FROM users WHERE id==:curr_id", curr_id=session["user_id"])

    # gets the current price of each stock queried
    if rows:
        for r in rows:
            r_shares = r["shares"]
            r_symbol = r["symbol"]
            # run lookup function to get current price
            dict_2 = lookup(r_symbol)
            # Adds the key "price" and its value to the dictionary "rows"
            r["price"] = dict_2["price"]
    # Calculates the grand total (stocks’ total value plus cash)
    total = sum([r["price"]*r["shares"] for r in rows]) + row_cash[0]["cash"]
    return render_template("portfolio.html", rows=rows, row_cash=row_cash, total=total)



@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        # run lookup function
        dict_3 = lookup(symbol)
        if not dict_3:
            return apology("invalid symbol")
        else:
            symbol = dict_3["symbol"].upper()
            name = dict_3["name"]
            price = dict_3["price"]
            row_stock = db.execute("SELECT id FROM stocks WHERE symbol==:symbol", symbol=symbol)
            row_cash = db.execute("SELECT cash FROM users WHERE id==:user_id", user_id=session["user_id"])
            new_cash = row_cash[0]["cash"] - (price*shares)
            if new_cash < 0:
                return apology("Not enough cash")
            # Add new stock symbol to table stocks
            if not row_stock:
                db.execute("INSERT INTO stocks(symbol, name) VALUES(:symbol, :name)", symbol=symbol, name=name )
                # Get id from new inserted stock
                row_stock = db.execute("SELECT id FROM stocks WHERE symbol==:symbol", symbol=symbol)
            # Insert new transaction in 'history' table
            db.execute("INSERT INTO history(user_id, stock_id, price, shares, buy) VALUES(:user_id, :stock_id, :price, :shares, :buy)", user_id=session["user_id"], stock_id=row_stock[0]["id"], price=price, shares=shares, buy=1)
            # INSERT information in 'portfolio' table
            row_portfolio = db.execute("SELECT stock_id FROM portfolio WHERE user_id==:user_id and stock_id=:stock_id", user_id=session["user_id"], stock_id=row_stock[0]["id"])
            if not row_portfolio:
                db.execute("INSERT INTO portfolio(user_id, stock_id, shares) VALUES(:user_id, :stock_id, :shares)", user_id=session["user_id"], stock_id=row_stock[0]["id"], shares=shares)
            else:
                # UPDATE shares in 'portfolio' table
                shares_db = db.execute("SELECT shares FROM portfolio WHERE user_id==:user_id and stock_id==:stock_id", user_id=session["user_id"], stock_id=row_stock[0]["id"])
                if shares_db:
                    new_shares = shares_db[0]["shares"]+shares
                    db.execute("UPDATE portfolio SET shares==:shares WHERE user_id==:user_id and stock_id==:stock_id", user_id=session["user_id"], stock_id=row_stock[0]["id"], shares=new_shares)
            # Update cash in 'users' table
            db.execute("UPDATE users SET cash==:cash WHERE id==:user_id", user_id=session["user_id"], cash=new_cash)
            # message to be retrieved in portfolio.html when user buys stock
            flash('Bought!')
            return redirect("/")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    rows = db.execute("SELECT stock_id, stocks.symbol, price, shares, date FROM history JOIN stocks ON history.stock_id=stocks.id WHERE user_id=:user_id", user_id=session["user_id"])
    return render_template("history.html", rows=rows)


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
    if request.method == "GET":
        return render_template("quote.html")
    else:
        symbol = request.form.get("symbol")
        # run lookup function
        dict_1 = lookup(symbol)
        if not dict_1:
            return apology("invalid symbol")
        # render 'quoted.html'
        return render_template("quoted.html", name=dict_1["name"], symbol=dict_1["symbol"],price=usd(dict_1["price"]))


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    else:
        # gets list of usernames to compare against the new username that has been assigned
        rows_username = db.execute("SELECT username FROM users WHERE username = :username",
                          username=request.form.get("username"))
        if len(rows_username) == 1:
            return apology("username already exist")
        else:
            username = request.form.get("username")
            first_name = request.form.get("first_name")
            last_name = request.form.get("last_name")
            email = request.form.get("email")
            # hash the password
            user_hash = generate_password_hash(request.form.get("password"), method='pbkdf2:sha256', salt_length=8)

            # insert user registration data into tables 'users' and 'contacts'
            db.execute("INSERT INTO users(username, hash) VALUES(:username, :user_hash)", username=username, user_hash=user_hash)

            # gets the id the new user has been assigned
            rows = db.execute("SELECT id FROM users WHERE username = :username",
                              username=request.form.get("username"))
            db.execute("INSERT INTO contacts(user_id, first_name, last_name, email) VALUES(:user_id, :first_name, :last_name, :email)", user_id=rows[0]["id"], first_name=first_name, last_name=last_name, email=email)

            # Remember which user has logged in
            session["user_id"] = rows[0]["id"]

            # message to be retrieved in portfolio.html when user registers
            flash('Registered!')
            return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    rows = db.execute("SELECT stock_id, shares, stocks.symbol FROM portfolio JOIN stocks ON portfolio.stock_id = stocks.id WHERE user_id==:user_id", user_id=session["user_id"])
    if request.method=="GET":
        return render_template("sell.html", rows=rows)
    else:
        symbol = request.form.get("symbol")
        if symbol=="None":
            return apology("You must select a symbol")
        # shares sold will be stored in history table with negative value
        shares = int(request.form.get("shares"))*(-1)
        if abs(shares) > rows[0]["shares"]:
            return apology("You don't own enough shares")
        # run lookup function
        dict_4 = lookup(symbol)
        price = dict_4["price"]
        # Insert new transaction in 'history' table
        db.execute("INSERT INTO history(user_id, stock_id, price, shares, buy) VALUES(:user_id, :stock_id, :price, :shares, :buy)", user_id=session["user_id"], stock_id=rows[0]["stock_id"], price=price, shares=shares, buy=0)
        # UPDATE shares in 'portfolio' table
        new_shares = (rows[0]["shares"])+shares
        db.execute("UPDATE portfolio SET shares==:shares WHERE user_id==:user_id and stock_id==:stock_id", user_id=session["user_id"], stock_id=rows[0]["stock_id"], shares=new_shares)
        # Update cash in 'users' table
        row_cash = db.execute("SELECT cash FROM users WHERE id==:user_id", user_id=session["user_id"])
        new_cash = row_cash[0]["cash"]-(price*shares)
        db.execute("UPDATE users SET cash==:cash WHERE id==:user_id", user_id=session["user_id"], cash=new_cash)
        # message to be retrieved in portfolio.html when user sells stock
        flash('Sold!')
        return redirect("/")

@app.route("/cashier", methods=["GET", "POST"])
@login_required
def cashier():
    """Deposits and withdraws money"""
    rows = db.execute("SELECT cash FROM users WHERE id==:user_id", user_id=session["user_id"])
    if request.method=="GET":
        return render_template("cashier.html", cash=rows[0]["cash"])
    else:
        # multiple forms in same html that send post request to same handler in flask see https://stackoverflow.com/questions/19794695/flask-python-buttons
        if request.form['submit_button'] == 'Value-1':
            deposit = float(request.form.get("deposit"))
            # Update cash in 'users' table
            new_cash = rows[0]["cash"]+deposit
            db.execute("UPDATE users SET cash==:cash WHERE id==:user_id", user_id=session["user_id"], cash=new_cash)
            # Insert transaction into table cashier_history
            db.execute("INSERT INTO cashier_history(user_id, amount, deposit) VALUES(:user_id, :amount, :deposit)", user_id=session["user_id"], amount=deposit, deposit=1)
            # message to be retrieved in portfolio.html when user deposits cash
            flash('Deposit!')
            return redirect("/")
        else:
            withdraw = float(request.form.get("withdraw"))
            withdraw = withdraw*(-1)
            # Update cash in 'users' table
            new_cash = rows[0]["cash"]+withdraw
            db.execute("UPDATE users SET cash==:cash WHERE id==:user_id", user_id=session["user_id"], cash=new_cash)
            # Insert transaction into table cashier_history
            db.execute("INSERT INTO cashier_history(user_id, amount, deposit) VALUES(:user_id, :amount, :deposit)", user_id=session["user_id"], amount=withdraw, deposit=0)
            # message to be retrieved in portfolio.html when user withdraws cash
            flash('Withdraw!')
            return redirect("/")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)