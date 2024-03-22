import os
import datetime
from flask import Flask, request, jsonify
import jwt

app = Flask(__name__)

# Secret key for encoding the JWT
app.config['SECRET_KEY'] = 'your_secret_key'

# Dummy database as a list of dictionaries
news_db = [
    {"id": 1, "title": "First news", "report": "This is the first news report."},
    {"id": 2, "title": "Second news", "report": "This is the second news report."}
]

@app.route('/login', methods=['POST'])
def login():
    """Endpoint to login and receive a token."""
    username = request.json.get("username")
    password = request.json.get("password")

    # Dummy user verification
    if username == "admin" and password == "password":
        token = jwt.encode(
            {
                "username": username,
                "exp": datetime.datetime.utcnow() + datetime.timedelta(hours=1)
            },
            app.config['SECRET_KEY'],
            algorithm="HS256"
        )
        return jsonify({"token": token}), 200

    return jsonify({"message": "Invalid credentials"}), 401

def token_required(f):
    """Decorator function to ensure token is present."""
    def decorated(*args, **kwargs):
        token = request.headers.get("X-API-Token")
        if not token:
            return jsonify({"message": "Token is missing!"}), 403
        try:
            jwt.decode(token, app.config['SECRET_KEY'], algorithms=["HS256"])
        except:
            return jsonify({"message": "Token is invalid!"}), 403
        return f(*args, **kwargs)
    return decorated

@app.route('/news', methods=['GET'])
@token_required
def get_news():
    """Endpoint to retrieve news reports."""
    return jsonify(news_db), 200

@app.route('/news', methods=['POST'])
@token_required
def add_news():
    """Endpoint to add a news report."""
    news = request.get_json()
    news_db.append(news)
    return jsonify(news), 201

@app.route('/news/<int:news_id>', methods=['PUT'])
@token_required
def update_news(news_id):
    """Endpoint to update a news report."""
    news = request.get_json()
    for item in news_db:
        if item['id'] == news_id:
            item.update(news)
            return jsonify(item), 200
    return jsonify({"error": "News not found"}), 404

@app.route('/news/<int:news_id>', methods=['DELETE'])
@token_required
def delete_news(news_id):
    """Endpoint to delete a news report."""
    for item in news_db:
        if item['id'] == news_id:
            news_db.remove(item)
            return jsonify({"message": "News deleted successfully"}), 200
    return jsonify({"error": "News not found"}), 404

if __name__ == '__main__':
    app.run(debug=True)
