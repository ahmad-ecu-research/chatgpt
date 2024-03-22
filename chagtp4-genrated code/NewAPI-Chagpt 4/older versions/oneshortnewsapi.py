import os
from flask import Flask, request, jsonify

app = Flask(__name__)

# Dummy database as a list of dictionaries
news_db = [
    {"id": 1, "title": "First news", "report": "This is the first news report."},
    {"id": 2, "title": "Second news", "report": "This is the second news report."}
]

@app.before_request
def verify_api_key():
    """Verify the API key."""
    api_key = request.headers.get('X-API-Key')
    if api_key != os.environ.get('API_KEY'):
        return jsonify({"error": "Invalid API Key"}), 401

@app.after_request
def add_api_key_to_response(response):
    """Add the API key to the response header."""
    api_key = request.headers.get('X-API-Key')
    response.headers['Authorization'] = f'Bearer {api_key}'
    return response

@app.route('/news', methods=['GET'])
def get_news():
    """Endpoint to retrieve news reports."""
    return jsonify(news_db), 200

@app.route('/news', methods=['POST'])
def add_news():
    """Endpoint to add a news report."""
    news = request.get_json()
    news_db.append(news)
    return jsonify(news), 201

@app.route('/news/<int:news_id>', methods=['PUT'])
def update_news(news_id):
    """Endpoint to update a news report."""
    news = request.get_json()
    for item in news_db:
        if item['id'] == news_id:
            item.update(news)
            return jsonify(item), 200
    return jsonify({"error": "News not found"}), 404

@app.route('/news/<int:news_id>', methods=['DELETE'])
def delete_news(news_id):
    """Endpoint to delete a news report."""
    for item in news_db:
        if item['id'] == news_id:
            news_db.remove(item)
            return jsonify({"message": "News deleted successfully"}), 200
    return jsonify({"error": "News not found"}), 404

if __name__ == '__main__':
    app.run(debug=True)
