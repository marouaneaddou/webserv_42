#!/usr/bin/env python3

import cgi

# Output HTTP header
# print("Content-Type: text/html")
# print()  # Blank line required between headers and content

print("<html><body>")

# Get form data
form = cgi.FieldStorage()

# Extract the values from the URL
fname = form.getvalue('username', 'Guest')

# Display the received information
print(f"""
<html>
<head>
    <style>
        body {{
            font-family: Arial, sans-serif;
            background-color: #f0f0f0;
            color: #333;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
        }}
        .welcome-container {{
            background-color: #fff;
            padding: 20px;
            border-radius: 10px;
            box-shadow: 0 0 15px rgba(0,0,0,0.1);
            text-align: center;
        }}
        h1 {{
            font-size: 36px;
            color: #4CAF50;
        }}
        p {{
            font-size: 20px;
            margin: 10px 0;
        }}
    </style>
</head>
<body>
    <div class="welcome-container">
        <h1>Welcome, {fname}!</h1>
        <p>We're glad to have you here.</p>
    </div>
</body>
</html>
""")

