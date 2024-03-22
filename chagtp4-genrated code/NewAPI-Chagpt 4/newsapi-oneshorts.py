from functools import wraps
import mysql.connector
from flask import Flask, jsonify, request, abort

# Import the necessary module for generating API keys
import secrets

# Import the necessary module for environment variables
import os

# Import the necessary module for datetime
from datetime import datetime

from flask_httpauth import HTTPTokenAuth

# Create a Flask app
app = Flask(__name__)

# Create an HTTP token auth object
auth = HTTPTokenAuth()

# Load API keys from a file or environment variable
# Replace 'YOUR_SECRET_API_KEY' with your actual API key
API_KEY = os.environ.get("API_KEY", "YOUR_SECRET_API_KEY")

# Decorator to authenticate requests
def authenticate(fn):
    @wraps(fn)
    def decorated(*args, **kwargs):
        api_key = request.headers.get("X-API-Key")
        if api_key != API_KEY:
            abort(401, "Invalid API key")
        return fn(*args, **kwargs)
    return decorated

# Connect to the MySQL database
db = mysql.connector.connect(host="localhost", user="root", password="", database="news_api")

# Get the cursor object
cursor = db.cursor()

# Endpoint to search for news articles
@app.route("/news/search", methods=["GET"])
@authenticate
def news_search():
    # Get the query string and other parameters from the request
    query = request.args.get("query")
    category = request.args.get("category")
    country = request.args.get("country")
    language = request.args.get("language")

    # Basic input validation
    if not query:
        abort(400, "Missing query parameter")

    # Construct the SQL query
    sql = """
        SELECT *
        FROM news_articles
        WHERE title LIKE %s OR description LIKE %s
    """
    params = (f"%{query}%", f"%{query}%")

    # Execute the SQL query
    cursor.execute(sql, params)

    # Get the results
    results = cursor.fetchall()

    # Convert the results to JSON format
    articles = []
    for row in results:
        article = {
            "id": row[0],
            "title": row[1],
            "description": row[2],
            "url": row[3]
        }
        articles.append(article)

    # Return the results in JSON format
    return jsonify({"articles": articles})

# Endpoint to get the top headlines
@app.route("/news/top-headlines", methods=["GET"])
@authenticate
def news_top_headlines():
    # Get the country and language from the request
    country = request.args.get("country")
    language = request.args.get("language")

    # Basic input validation
    if not country or not language:
        abort(400, "Missing country or language parameters")

    # Construct the SQL query
    sql = """
        SELECT *
        FROM news_articles
        WHERE country = %s AND language = %s
        ORDER BY published_at DESC
        LIMIT 10
    """
    params = (country, language)

    # Execute the SQL query
    cursor.execute(sql, params)

    # Get the results
    results = cursor.fetchall()

    # Convert the results to JSON format
    articles = []
    for row in results:
        article = {
            "id": row[0],
            "title": row[1],
            "description": row[2],
            "url": row[3]
        }
        articles.append(article)

    # Return the results in JSON format
    return jsonify({"articles": articles})

# Create an endpoint to read breaking news from top news sources
@app.route("/news/breaking", methods=["GET"])
@authenticate
def news_breaking():
    # Get the country and language from the request
    country = request.args.get("country")
    language = request.args.get("language")

    # Basic input validation
    if not country or not language:
        abort(400, "Missing country or language parameters")

    # Construct the SQL query
    sql = """
        SELECT *
        FROM news_articles
        WHERE country = %s AND language = %s
        ORDER BY published_at DESC
        LIMIT 10
    """
    params = (country, language)

    # Execute the SQL query
    cursor.execute(sql, params)

    # Get the results
    results = cursor.fetchall()

    # Convert the results to JSON format
    articles = []
    for row in results:
        article = {
            "id": row[0],
            "title": row[1],
            "description": row[2],
            "url": row[3]
        }
        articles.append(article)

    # Return the results in JSON format
    return jsonify({"articles": articles})

# Endpoint to get a list of all available countries
@app.route("/news/countries", methods=["GET"])
@authenticate
def news_countries():
    # Construct the SQL query
    sql = """
        SELECT DISTINCT country
        FROM news_articles
    """

    # Execute the SQL query
    cursor.execute(sql)

    # Get the results
    results = cursor.fetchall()

    # Convert the results to a list
    countries = [row[0] for row in results]

    # Return the results in JSON format
    return jsonify(countries)

# Endpoint to get a list of all available news categories
@app.route("/news/categories", methods=["GET"])
@authenticate
def news_categories():
    # Construct the SQL query
    sql = """
        SELECT DISTINCT category
        FROM news_articles
    """

    # Execute the SQL query
    cursor.execute(sql)

    # Get the results
    results = cursor.fetchall()

    # Convert the results to a list
    categories = [row[0] for row in results]

    # Return the results in JSON format
    return jsonify(categories)

# Endpoint to add a new news article
@app.route("/news/add", methods=["POST"])
@authenticate
def news_add():
    # Get the title, description, and URL of the new news article from the request
    title = request.json.get("title")
    description = request.json.get("description")
    url = request.json.get("url")

    # Basic input validation
    if not title or not description or not url:
        abort(400, "Missing required parameters")

    # Construct the SQL query
    sql = """
        INSERT INTO news_articles (title, description, url)
        VALUES (%s, %s, %s)
    """
    params = (title, description, url)

    # Execute the SQL query
    cursor.execute(sql, params)

    # Commit the changes to the database
    db.commit()

    # Return a success message
    return jsonify({"message": "The article was added successfully."})

# Endpoint to delete a news article
@app.route("/news/delete", methods=["DELETE"])
@authenticate
def news_delete():
    # Get the ID of the news article to delete from the request
    id = request.json.get("id")

    # Basic input validation
    if not id:
        abort(400, "Missing ID parameter")

    # Construct the SQL query
    sql = """
        DELETE FROM news_articles
        WHERE
