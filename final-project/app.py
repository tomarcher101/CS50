from jinja2 import Template
from flask import Flask, render_template
app = Flask(__name__, static_url_path='/static')

@app.route('/')
def index():
    return render_template("index.html")

@app.route('/login')
def login():
    return render_template("login.html")
